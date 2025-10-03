# M5StickC Plus2 距離測定ロガー

M5StickC Plus2とToF HATを使って距離を測定し、Phyphoxアプリでリアルタイムグラフ表示できるプロジェクトです。

## 📖 詳細なドキュメント

**完全なドキュメントサイトを公開しています：**

### 🌐 [📚 公式ドキュメントサイト](https://phys-ken.github.io/M5StickCPlus2_VL53L0X_Phyphox_Logger/)

対象者別の詳しいガイドは以下のページをご覧ください：

- **🎓 [学習者向けガイド](https://phys-ken.github.io/M5StickCPlus2_VL53L0X_Phyphox_Logger/student/)** - 基本的な使い方と実験手順
- **👨‍🏫 [教師向けガイド](https://phys-ken.github.io/M5StickCPlus2_VL53L0X_Phyphox_Logger/teacher/)** - 指導のポイントと技術的背景  
- **🔧 [技術仕様書](https://phys-ken.github.io/M5StickCPlus2_VL53L0X_Phyphox_Logger/docs/)** - 開発環境とライブラリの詳細

## 🚀 概要

- **距離測定**: ToF HAT（VL53L0Xセンサ）で0〜2mの距離を測定
- **リアルタイム表示**: 測定値を本体画面とPhyphoxアプリに同時表示
- **データログ**: シリアル通信でCSV形式のデータ出力
- **教育活用**: 物理・数学・情報の実験教材として最適

## 🛠️ 必要な機材

| 機材 | 説明 |
|------|------|
| **M5StickC Plus2** | メインコントローラー |
| **ToF HAT** | 距離センサーモジュール（本体上部装着） |
| **スマートフォン** | Phyphoxアプリ用 |

## ⚠️ 重要な注意事項

- **ToF HAT使用**: ToF UNIT（ケーブル接続）ではなく、ToF HAT（本体上部装着）を使用
- **Arduino IDE必須**: PlatformIOは未対応のため、Arduino IDEでの開発が必要

## 📁 リポジトリ構成

```
M5StickCPlus2_VL53L0X_Phyphox_Logger/
├── M5StickCPlus2_VL53L0X_Phyphox_Logger.ino  # メインのArduinoコード
├── docs/                                     # ドキュメントファイル
├── mkdocs.yml                               # サイト設定
└── README.md                                # このファイル
```

## 🎯 クイックスタート

1. **[公式ドキュメントサイト](https://phys-ken.github.io/M5StickCPlus2_VL53L0X_Phyphox_Logger/)** で詳細な手順を確認
2. **Arduino IDE** で必要ライブラリをインストール  
3. **M5StickCPlus2_VL53L0X_Phyphox_Logger.ino** を書き込み
4. **スマホにPhyphoxアプリ** をインストールして測定開始！

## 📄 ライセンス

MIT License

---

**🔗 リンク**
- **📖 [ドキュメントサイト](https://phys-ken.github.io/M5StickCPlus2_VL53L0X_Phyphox_Logger/)** - 使い方・指導法・技術仕様
- **🐛 [Issues](https://github.com/phys-ken/M5StickCPlus2_VL53L0X_Phyphox_Logger/issues)** - バグ報告・機能提案
- **🏠 [Phyphox公式サイト](https://phyphox.org/)** - 物理実験アプリ
