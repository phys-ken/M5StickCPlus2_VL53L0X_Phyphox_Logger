/*
 * M5StickCPlus2 + VL53L0X ToF センサによる距離測定システム
 * 機能：距離測定、速度計算、Phyphox BLE連携、シリアル出力
 * 作成：物理実験用データロガー
 */

#include <M5Unified.h>
#include <Wire.h>
#include <VL53L0X.h>
#include <phyphoxBle.h>

// ===========================================
// デバイス設定（複数デバイスでの書き込み時に変更）
// ===========================================
const char* SENSOR_NAME = "phys_ken_001";

// ===========================================
// ハードウェア設定
// ===========================================
const int LED_PIN = 19;                    // M5StickCPlus2内蔵LED（LOW=点灯、HIGH=消灯）
const int I2C_SDA_PIN = 0;                 // TOF HAT専用I2Cピン（Grove端子とは異なる）
const int I2C_SCL_PIN = 26;                // TOF HAT専用I2Cピン（Grove端子とは異なる）

// ===========================================
// センサ設定
// ===========================================
const int SENSOR_TIMEOUT = 500;            // VL53L0X応答タイムアウト時間
const int MAX_VALID_DISTANCE = 3000;       // 測定可能距離上限（3m）

// ===========================================
// フィルタ設定
// ===========================================
const int FILTER_SIZE = 2;                 // 移動平均によるノイズ除去（サイズ小=応答性重視）

// ===========================================
// 表示設定
// ===========================================
const int LCD_TEXT_SIZE_LARGE = 3;         // センサ名表示用
const int LCD_TEXT_SIZE_NORMAL = 2;        // 測定値表示用
const int SENSOR_NAME_Y_POS = 0;           // 画面上部配置
const int DATA_Y_POS = 40;                 // センサ名の下に配置

// ===========================================
// タイミング設定
// ===========================================
const int LOOP_DELAY = 50;                 // 20Hz測定レート（BLE安定性とのバランス）
const int SERIAL_BAUD_RATE = 115200;       // Arduino IDE標準レート
const int DECIMAL_PLACES = 3;              // 測定精度に応じた表示桁数

// ===========================================
// グローバル変数
// ===========================================
VL53L0X sensor;
M5Canvas sprite(&M5.Lcd);

// フィルタ関連
float filterBuffer[FILTER_SIZE];
int filterIndex = 0;
float preDist = 0;

// 速度計算用
float lastDist = 0.0;
unsigned long lastTime = 0;

// 測定制御用
bool ledState = true;                       // true=停止中、false=測定中
unsigned long startTime = 0;               // 測定開始時刻

/*
 * 初期化処理
 * ハードウェア、センサ、BLE、画面の設定を行う
 */
void setup() {
  M5.begin();

  // LED初期化（測定停止状態）
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  // 画面初期化
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setRotation(3);                    // 横向き表示
  
  // シリアル通信初期化
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.println("VLX53LOX started.");

  // I2C通信初期化（TOF HAT専用ピン使用）
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  sensor.setTimeout(SENSOR_TIMEOUT);
  if (!sensor.init()) {
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.println("Failed to detect and initialize sensor!");
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}                            // 初期化失敗時は停止
  }
  sensor.startContinuous();

  // Phyphox BLE実験設定
  PhyphoxBLE::start(SENSOR_NAME);
  
  PhyphoxBleExperiment experiment;
  experiment.setTitle(SENSOR_NAME);
  experiment.setCategory(SENSOR_NAME);
  experiment.setDescription("Plot the distance from a time-of-flight sensor over time.");

  // 距離グラフ設定
  PhyphoxBleExperiment::View view;
  PhyphoxBleExperiment::Graph graph;
  graph.setLabel("x-tグラフ");
  graph.setUnitX("s");
  graph.setUnitY("m");
  graph.setLabelX("時刻");
  graph.setLabelY("距離");
  graph.setChannel(0, 1);

  // 速度グラフ設定
  PhyphoxBleExperiment::Graph speedGraph;
  speedGraph.setLabel("v-tグラフ");
  speedGraph.setUnitX("s");
  speedGraph.setUnitY("m/s");
  speedGraph.setLabelX("時刻");
  speedGraph.setLabelY("速度");
  speedGraph.setChannel(0, 2);

  view.addElement(graph);
  view.addElement(speedGraph);
  experiment.addView(view);
  PhyphoxBLE::addExperiment(experiment);

  // スプライト（オフスクリーンバッファ）初期化
  sprite.setColorDepth(8);
  sprite.setTextSize(LCD_TEXT_SIZE_NORMAL);
  sprite.createSprite(M5.Lcd.width(), M5.Lcd.height());
}

/*
 * メインループ
 * ボタン監視、センサ読み取り、データ処理、表示更新を実行
 */
void loop() {
  M5.update();

  // ボタンA押下で測定開始/停止切り替え
  if (M5.BtnA.wasPressed()) {
    ledState = !ledState;
    if (!ledState) {
      // 測定開始
      startTime = millis();
      digitalWrite(LED_PIN, LOW);           // LED点灯で測定中を表示
      Serial.println("<---!!start!!--->");
      Serial.println("---sec , m---");
    } else {
      // 測定停止
      digitalWrite(LED_PIN, HIGH);          // LED消灯で停止中を表示
      Serial.println("<---!!stop!!--->");
    }
  }

  // ToFセンサから距離取得
  int distance = sensor.readRangeSingleMillimeters();

  // 異常値の処理（センサ範囲外またはタイムアウト）
  if (distance > MAX_VALID_DISTANCE || sensor.timeoutOccurred()) {
    distance = preDist;                     // 前回値を使用してスパイク除去
  }

  // 移動平均フィルタによるノイズ除去
  filterBuffer[filterIndex] = distance;
  filterIndex = (filterIndex + 1) % FILTER_SIZE;
  float filteredDist = 0.0;
  for (int i = 0; i < FILTER_SIZE; i++) {
    filteredDist += filterBuffer[i];
  }
  filteredDist /= FILTER_SIZE;
  preDist = filteredDist;

  // 速度計算（数値微分）
  unsigned long currentTime = millis();
  float deltaTime = (currentTime - lastTime) / 1000.0;
  lastTime = currentTime;

  float speed = 0.0;
  if (deltaTime > 0) {
      speed = (filteredDist - lastDist) / deltaTime;
  }
  lastDist = filteredDist;

  // 単位変換（mm → m）
  float distanceInMeters = filteredDist / 1000.0;
  float speedInMps = speed / 1000.0;

  // Phyphoxアプリへデータ送信（BLE経由）
  PhyphoxBLE::write(distanceInMeters, speedInMps);

  // LCD画面更新
  sprite.fillRect(0, 0, sprite.width(), sprite.height(), BLACK);
  
  // センサ名表示（黄色、大きめ）
  sprite.setCursor(0, SENSOR_NAME_Y_POS);
  sprite.setTextSize(LCD_TEXT_SIZE_LARGE);
  sprite.setTextColor(TFT_YELLOW);
  sprite.printf("%s\n", SENSOR_NAME);

  // 測定値表示（白色、標準サイズ）
  sprite.setCursor(0, DATA_Y_POS);
  sprite.setTextSize(LCD_TEXT_SIZE_NORMAL);
  sprite.setTextColor(TFT_WHITE);
  sprite.printf("x = %.3f m\n", distanceInMeters);
  sprite.printf("v = %.3f m/s\n", speedInMps);

  sprite.pushSprite(0, 0);

  // 測定中のみCSV形式でシリアル出力
  if (!ledState) {
    float timeDifference = (currentTime - startTime) / 1000.0;
    Serial.print(timeDifference, DECIMAL_PLACES);
    Serial.print(",");
    Serial.println(distanceInMeters, DECIMAL_PLACES);
  }

  delay(LOOP_DELAY);
}
