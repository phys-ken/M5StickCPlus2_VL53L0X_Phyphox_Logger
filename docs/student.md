# 🎓 生徒向け使い方マニュアル

M5StickC Plus2を使った距離測定実験の手順を説明します。

## 準備

### 1. 機材の確認
- M5StickC Plus2（本体）
- ToF HAT（距離センサー）- 本体の上に帽子のように装着
- スマートフォン（Phyphoxアプリ用）

### 2. 組み立て
1. ToF HATをM5StickC Plus2の上部にしっかり装着
2. USBケーブルで充電（実験中は外してもOK）

## 🏷️ センサーの識別方法

### 自分の班のセンサーを確認
電源を入れると画面の一番上に表示される**センサー名**を確認してください：

```
phys_ken_001     ← これがセンサーの名前（例：1班用）
phys_ken_002     ← 2班用
phys_ken_003     ← 3班用
...
```

⚠️ **重要**: **必ず自分の班に割り当てられたセンサーを使用**してください
- 他の班のセンサーを使うと、データが混ざって正確な実験ができません


## 基本的な使い方

### 画面の見方
電源を入れると画面に3つの情報が表示されます：

```
phys_ken_001     ← センサーの名前（班番号を確認！）
x = 0.123 m      ← 測定した距離
v = 0.001 m/s    ← 移動の速さ
```

### 測定方法

**スマホアプリで測定**
1. スマホで「Phyphox」アプリを開く
2. 右上の「+」→「Bluetooth」を選択
3. **画面に表示されているセンサー名と同じもの**を見つけてタップ
   - 例：画面に「phys_ken_003」と表示されていたら、「phys_ken_003」を選択
   - ⚠️ 他の番号（他の班のセンサー）は選ばないこと！
4. グラフが表示されます
   1. x-tグラフは距離と時刻のグラフ
   2. v-tグラフは速度と時刻のグラフ
5. Phyphoxアプリの「開始」ボタンを押すとデータ記録が始まります
6. 測定が終了したら停止ボタンを教えてください。
7. Phyphoxアプリ上で、傾きを計算できます。データも保存できます。

<iframe width="280" height="160" src="https://www.youtube-nocookie.com/embed/6-5kAKFH8MQ?si=Tp9iTtyqkxi8RZDE" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" referrerpolicy="strict-origin-when-cross-origin" allowfullscreen></iframe>


## 実験のコツ

### 💡 測定のポイント
- センサーと対象物の距離：**3cm〜3m**が測定可能
- 手で本体を持って動かすと速度も測定できる
- 壁や大きな物体に向けると測定しやすい

### 📊 データの読み方
- **距離（x）**: センサーから物体までの直線距離
- **速度（v）**: プラス＝近づく、マイナス＝遠ざかる
- グラフの横軸は時間、縦軸は距離または速度


## より詳しく知りたい場合

- **先生向け資料**: [teacher.md](./teacher.md) - 技術的な仕組みの説明
- **開発者向け資料**: [docs.md](./docs.md) - プログラムとライブラリの詳細

