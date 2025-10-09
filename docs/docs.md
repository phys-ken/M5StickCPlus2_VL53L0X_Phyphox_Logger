# 🔧 技術仕様書

M5StickC Plus2 VL53L0X距離測定システムの開発環境、ライブラリ、技術仕様について詳細に説明します。

## 開発環境

### 対応プラットフォーム
- **Arduino IDE**: 2.0以降推奨
- **PlatformIO**: ❌ 未対応（M5StickC Plus2のボード定義なし）

### ボード設定
```
ボード: M5StickC Plus2
フラッシュサイズ: 4MB
パーティション: Default 4MB with spiffs
アップロード速度: 115200
```

## ハードウェア仕様

### M5StickC Plus2
| 項目 | 仕様 |
|------|------|
| MCU | ESP32-PICO-D4 |
| フラッシュ | 4MB |
| PSRAM | 8MB |
| ディスプレイ | 1.14インチ TFT (135x240) |
| バッテリー | 200mAh リチウムポリマー |
| 内蔵LED | GPIO19 |
| I2C | SDA:GPIO0, SCL:GPIO26 (HAT接続用) |

### ToF HAT (VL53L0X)
| 項目 | 仕様 |
|------|------|
| センサー | STMicroelectronics VL53L0X |
| 測定方式 | Time-of-Flight (ToF) |
| 測定範囲 | 30mm - 3000mm |
| 精度 | ±3% (理想条件) |
| 視野角 | 25° |
| I2Cアドレス | 0x29 |

## 必要ライブラリ

### Arduino IDEでのインストール方法
ライブラリマネージャーから以下を検索・インストール：

#### 1. M5Unified
```
作者: M5Stack
バージョン: 最新版
説明: M5Stack製品統合ライブラリ
GitHub: https://github.com/m5stack/M5Unified
```

#### 2. VL53L0X
```
作者: Pololu
バージョン: 1.3.1以降
説明: VL53L0X ToFセンサー制御ライブラリ
GitHub: https://github.com/pololu/vl53l0x-arduino
```

#### 3. phyphoxBle
```
作者: phyphox
バージョン: 1.2.3以降
説明: Phyphox BLE通信ライブラリ
GitHub: https://github.com/phyphox/phyphox-arduino
```

## 🏷️ 複数センサー対応のための設定

### センサー名のカスタマイズ方法

#### コード内での設定変更
```cpp
// センサー名の設定（17行目付近）
const char* SENSOR_NAME = "phys_ken_001";  // ←ここを変更

// 使用例:
const char* SENSOR_NAME = "phys_ken_001";  // 1班用
const char* SENSOR_NAME = "phys_ken_002";  // 2班用
const char* SENSOR_NAME = "phys_ken_003";  // 3班用
const char* SENSOR_NAME = "class2_A";      // 2組A班用
const char* SENSOR_NAME = "team_red";      // 赤チーム用
```

#### 推奨命名規則
```cpp
// 標準的な命名パターン
const char* SENSOR_NAME = "phys_ken_001";    // XXX = 001, 002, 003...
const char* SENSOR_NAME = "class_2_team_A";  // クラス番号_班番号
const char* SENSOR_NAME = "sensor_001";      // 通し番号
const char* SENSOR_NAME = "group_red";       // グループ名
```

#### 注意事項
- **最大20文字程度**まで推奨（Phyphox表示制限）
- **英数字とアンダースコア**のみ使用推奨
- **日本語は文字化けの可能性**があるため避ける
- **重複しない名前**にする（同一ネットワーク内）



## コード構造解析

### 主要な処理フロー
```cpp
setup() {
    M5.begin()                      // M5システム初期化
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN) // I2C初期化（0, 26）
    sensor.init()                   // VL53L0Xセンサー初期化
    sensor.startContinuous()        // 連続測定モード開始
    PhyphoxBLE::start(SENSOR_NAME)  // BLE通信開始（センサー名指定）
    // Phyphox実験設定（距離・速度グラフ）
}

loop() {
    M5.update()                     // ボタン状態更新
    // ボタンA押下時の測定開始/停止処理
    sensor.readRangeSingleMillimeters() // 距離測定
    // 移動平均フィルタ処理（FILTER_SIZE = 2）
    // 速度計算（数値微分）
    PhyphoxBLE::write(distance, speed)  // BLEデータ送信
    // LCD表示更新（センサー名・測定値表示）
    // シリアル出力（測定中のみCSV形式）
    delay(LOOP_DELAY)               // 50ms待機（20Hz測定）
}
```

### 重要な変更点（Plus → Plus2移行）

#### I2Cピン設定
```cpp
// M5StickC Plus (旧)
Wire.begin(21, 22);  // Grove端子

// M5StickC Plus2 (新)
Wire.begin(0, 26);   // HAT専用端子
```

#### LEDピン
```cpp
// M5StickC Plus (旧)
const int LED_PIN = 10;

// M5StickC Plus2 (新)  
const int LED_PIN = 19;
```

#### ライブラリ
```cpp
// 旧: M5StickCPlus ライブラリ
#include <M5StickCPlus.h>

// 新: M5Unified ライブラリ
#include <M5Unified.h>
```

### データ処理アルゴリズム

#### 移動平均フィルタ
```cpp
const int FILTER_SIZE = 2;
float filterBuffer[FILTER_SIZE];
int filterIndex = 0;

// フィルタ更新
filterBuffer[filterIndex] = distance;
filterIndex = (filterIndex + 1) % FILTER_SIZE;

// 平均値計算
float filteredDist = 0.0;
for (int i = 0; i < FILTER_SIZE; i++) {
    filteredDist += filterBuffer[i];
}
filteredDist /= FILTER_SIZE;
```

#### 速度計算
```cpp
// 時刻取得・差分計算
unsigned long currentTime = millis();
float deltaTime = (currentTime - lastTime) / 1000.0;
lastTime = currentTime;

// 速度計算（数値微分）
float speed = 0.0;
if (deltaTime > 0) {
    speed = (filteredDist - lastDist) / deltaTime;
}
lastDist = filteredDist;
```

## Phyphox BLE通信仕様

### 実験構成
```cpp
PhyphoxBleExperiment experiment;
experiment.setTitle(SENSOR_NAME);        // センサー名を実験名に使用
experiment.setCategory(SENSOR_NAME);     // カテゴリにもセンサー名を使用

// 距離グラフ (Channel 0→1)
PhyphoxBleExperiment::Graph graph;
graph.setChannel(0, 1);  // time, distance

// 速度グラフ (Channel 0→2)  
PhyphoxBleExperiment::Graph speedGraph;
speedGraph.setChannel(0, 2);     // time, speed
```

### データ送信
```cpp
// 単位変換（mm → m）
float distanceInMeters = filteredDist / 1000.0;
float speedInMps = speed / 1000.0;

// Phyphoxアプリへデータ送信
PhyphoxBLE::write(distanceInMeters, speedInMps);
// Channel 0: 時刻（自動）
// Channel 1: 距離（メートル）
// Channel 2: 速度（m/s）
```

### BLE通信制限
- **同時接続数**: 理論上は無制限だが、実用的には**8-10台程度**
- **通信距離**: 約10m（障害物なし）
- **干渉**: 2.4GHz帯のWi-Fi等との干渉可能性

## トラブルシューティング

### よくある問題と解決方法

#### コンパイルエラー
```
Error: 'M5StickCPlus' was not declared
解決: M5Unifiedライブラリをインストール
```

```
Error: 'VL53L0X' was not declared  
解決: Pololu VL53L0Xライブラリをインストール
```

#### 実行時エラー
```
Failed to detect and initialize sensor!
原因: ToF HATの接続不良、I2Cピン設定ミス
解決: HAT装着確認、Wire.begin(0, 26)設定確認
```

#### BLE接続問題
```
Phyphoxで検出されない
原因: BLE名の重複、距離制限
解決: デバイス再起動、近距離での接続試行
```

#### 複数センサー特有の問題
```
同じ名前のデバイスが複数表示される
原因: センサー名の重複
解決: 各センサーに異なる名前を設定
```

```
他の班のセンサーに誤接続
原因: センサー名の取り違え
解決: 画面表示名を再確認、正しいデバイスに再接続
```

## カスタマイズ例

### サンプリング周波数変更
```cpp
// LOOP_DELAY値を変更（コード上部の定数定義）
const int LOOP_DELAY = 100;  // 100ms → 10Hz
const int LOOP_DELAY = 50;   // 50ms → 20Hz（デフォルト）  
const int LOOP_DELAY = 20;   // 20ms → 50Hz
```

### センサー名変更
```cpp
const char* SENSOR_NAME = "MyDevice_001";  // 任意の名前
```

### フィルタサイズ調整
```cpp
const int FILTER_SIZE = 1;   // フィルタなし（最高応答性）
const int FILTER_SIZE = 2;   // デフォルト（応答性重視）
const int FILTER_SIZE = 5;   // より安定性重視
const int FILTER_SIZE = 10;  // 最高安定性（応答遅延あり）
```

### 表示カスタマイズ
```cpp
// LCD表示内容の変更
sprite.printf("Team_%s\n", teamNumber);     // チーム番号表示
sprite.printf("Class_%d Group_%d\n", classNum, groupNum); // クラス・グループ表示
```

## 性能特性

### 処理性能
- **測定周波数**: 約20Hz（50ms周期）
- **BLE送信レート**: 約20Hz
- **表示更新レート**: 約20Hz

### 電力消費
- **測定中**: 約80-120mA
- **待機中**: 約20-40mA  
- **連続動作時間**: 約2-3時間

### 複数センサー運用時の制約
- **推奨同時接続数**: 8-10台
- **Bluetooth通信品質**: 台数増加に伴い低下
- **電波干渉**: 2.4GHz帯機器との競合

## 関連ドキュメント

### ユーザーマニュアル
- **基本操作**: [student.md](./student.md) - 生徒・学習者向け使い方
- **指導ガイド**: [teacher.md](./teacher.md) - 教師向け技術解説

### 外部リンク
- [M5Unified Documentation](https://github.com/m5stack/M5Unified)
- [VL53L0X Datasheet](https://www.st.com/resource/en/datasheet/vl53l0x.pdf)
- [Phyphox Documentation](https://phyphox.org/wiki/)

## 開発履歴

| バージョン | 日付 | 変更内容 |
|------------|------|----------|
| 1.0 | - | M5StickC Plus版（オリジナル） |
| 2.0 | 2024 | M5StickC Plus2対応版 |
| 2.1 | 2024 | 複数センサー対応、班別管理機能追加 |

---
⚙️ **開発者へ**: このプロジェクトの改良やフォークを歓迎します。Issue報告やPull Requestをお待ちしています。  
🏫 **教育関係者へ**: 複数センサーの同時運用により、効果的なグループ学習環境を構築できます。
