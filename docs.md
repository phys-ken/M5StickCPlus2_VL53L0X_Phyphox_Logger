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
アップロード速度: 921600
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
| 測定範囲 | 30mm - 2000mm |
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

## コード構造解析

### 主要な処理フロー
```cpp
setup() {
    M5.begin()                    // M5システム初期化
    Wire.begin(0, 26)            // I2C初期化（HAT用ピン）
    sensor.init()                // VL53L0Xセンサー初期化
    PhyphoxBLE::start()          // BLE通信開始
    // Phyphox実験設定
}

loop() {
    M5.update()                  // ボタン状態更新
    // ボタンA押下時の処理
    sensor.readRangeSingleMillimeters()  // 距離測定
    // 移動平均フィルタ処理
    // 速度計算
    PhyphoxBLE::write()          // BLEデータ送信
    // LCD表示更新
    // シリアル出力
    delay(10)                    // 10ms待機
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
const int filterSize = 10;
float filterBuffer[filterSize];
int filterIndex = 0;

// フィルタ更新
filterBuffer[filterIndex] = newValue;
filterIndex = (filterIndex + 1) % filterSize;

// 平均値計算
float filteredValue = 0.0;
for (int i = 0; i < filterSize; i++) {
    filteredValue += filterBuffer[i];
}
filteredValue /= filterSize;
```

#### 速度計算
```cpp
float speed = 0.0;
if (deltaTime > 0) {
    speed = (currentDistance - lastDistance) / deltaTime;
}
```

## Phyphox BLE通信仕様

### 実験構成
```cpp
PhyphoxBleExperiment experiment;
experiment.setTitle("phys_ken_001");
experiment.setCategory("phys_ken_001");

// 距離グラフ (Channel 0→1)
PhyphoxBleExperiment::Graph distanceGraph;
distanceGraph.setChannel(0, 1);  // time, distance

// 速度グラフ (Channel 0→2)  
PhyphoxBleExperiment::Graph speedGraph;
speedGraph.setChannel(0, 2);     // time, speed
```

### データ送信
```cpp
PhyphoxBLE::write(distanceInMeters, speedInMps);
// Channel 0: 時刻（自動）
// Channel 1: 距離（メートル）
// Channel 2: 速度（m/s）
```

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

## カスタマイズ例

### サンプリング周波数変更
```cpp
// loop()最後のdelay値を変更
delay(100);  // 100ms → 10Hz
delay(50);   // 50ms → 20Hz  
delay(10);   // 10ms → 100Hz（デフォルト）
```

### センサー名変更
```cpp
const char* sensorName = "MyDevice_001";  // 任意の名前
```

### フィルタサイズ調整
```cpp
const int filterSize = 5;   // より応答性重視
const int filterSize = 20;  // より安定性重視
```

## 性能特性

### 処理性能
- **測定周波数**: 約100Hz（10ms周期）
- **BLE送信レート**: 約100Hz（フィルタなし）
- **表示更新レート**: 約100Hz

### 電力消費
- **測定中**: 約80-120mA
- **待機中**: 約20-40mA  
- **連続動作時間**: 約2-3時間

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

---
⚙️ **開発者へ**: このプロジェクトの改良やフォークを歓迎します。Issue報告やPull Requestをお待ちしています。
