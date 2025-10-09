# M5StickC Plus2 距離測定ロガー

M5StickC Plus2とToF HATを使って距離を測定し、Phyphoxアプリでリアルタイムグラフ表示できるプロジェクトです。

## 📋 対象者別ガイド

<div class="grid cards" markdown>

-   :material-school: **🎓 学習者向け**

    ---

    基本的な使い方と実験手順

    [:octicons-arrow-right-24: 使い方ガイド](student.md)

-   :material-human-male-board: **👨‍🏫 教師向け**

    ---

    指導のポイントと技術的背景

    [:octicons-arrow-right-24: 指導者ガイド](teacher.md)

-   :material-cog: **🔧 開発者向け**

    ---

    技術仕様とライブラリの詳細

    [:octicons-arrow-right-24: 技術仕様書](docs.md)

</div>

## 概要

- **距離測定**: ToF HAT（VL53L0Xセンサ）で0〜3mの距離を測定
- **リアルタイム表示**: 測定値を本体画面とPhyphoxアプリに同時表示  
- **データログ**: シリアル通信でCSV形式のデータ出力

## 必要な機材

| 機材 | 説明 |
|------|------|
| **M5StickC Plus2** | メインコントローラー |
| **ToF HAT** | 距離センサーモジュール（本体上部装着） |
| **スマートフォン** | Phyphoxアプリ用 |

!!! warning "重要"
    **ToF HAT**（本体上部装着タイプ）を使用してください。ToF UNIT（ケーブル接続タイプ）とは異なります。

---

## サポート

- 🐛 **バグ報告**: [GitHub Issues](https://github.com/phys-ken/M5StickCPlus2_VL53L0X_Phyphox_Logger/issues)
- � **プロジェクト**: [GitHub Repository](https://github.com/phys-ken/M5StickCPlus2_VL53L0X_Phyphox_Logger)