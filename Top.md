\image html header.png width=80%

![](https://img.shields.io/badge/csv_parser-green)
![](https://img.shields.io/badge/release-b1.0-green)
![](https://img.shields.io/badge/release-developing-red)
![](https://img.shields.io/badge/implementation-header_only-red)
![](https://img.shields.io/badge/c%2B%2B%20stable%20ver-14%20%7C%2017-blue)
<!-- ![](https://img.shields.io/badge/c%2B%2B%20stable%20ver-17-blue) -->
![](https://img.shields.io/badge/compiler-gcc-blue)
![](https://img.shields.io/badge/stable_OS-Linux(ubuntu)-yellow)
![](https://img.shields.io/badge/license-MIT-yellow)
![](https://img.shields.io/badge/developer-Keasha-purple)

# C++でもpandasみたいなことがしたい!

ということを目標に開発を始めたCSV Parserです.

- [C++でもpandasみたいなことがしたい!](#cでもpandasみたいなことがしたい)
- [基本情報](#基本情報)
      - [更新履歴](#更新履歴)
      - [更新予定の情報](#更新予定の情報)
- [使い方](#使い方)
  - [目次的なやつ](#目次的なやつ)
  - [配布物 \& ディレクトリ構造](#配布物--ディレクトリ構造)
  - [サンプルプログラムの使い方](#サンプルプログラムの使い方)
  - [簡単なAPIリファレンス](#簡単なapiリファレンス)
- [生まれた経緯](#生まれた経緯)

# 基本情報

- Author: Keasha 改め K. Kotera.
- 開発開始日: 2024/10/30-.
- Version: beta 1.0.1 (2024/11/01).
- Header only?: YES. 単一Headerで配布(他のライブラリとの兼ね合いで今後変わるかもしれないけど全部Header onlyにはする).
- 動作環境
  - コンパイラ: C++14以上
  - OS: Linux(Ubuntu, CentOS)は確認済み、Windowsでもwslを使えばいける.
    - ただしLinux使用を想定してるため、改行コードが違うWindowsやMacでは動作がうまくいかないと思われ(ref: [とほほ](https://www.tohoho-web.com/wwwxx011.htm))
    - 暇があれば対応したいわね.
- ライセンス: MIT

#### 更新履歴

- b 1.1.0 (2024/11/03): 
    - C++14対応(修正).
    - ドキュメントを追加.
- b 1.0.1 (2024/11/01): 
    - ドキュメントの修正: ~~C++14対応~~ →C++17以降対応, 記述内容を修正.
    - コードの方もC++14以降に対応できるように修正中.
- b 1.0.0 (2024/10/31):初リリース

#### 更新予定の情報

- 書き込みへの対応.
- CERN ROOT-likeな書き方ができるようにも準備中.

# 使い方

## 目次的なやつ

- [チュートリアルのソースコード](https://kyasya.github.io/Pandas-like-csv-parser/test_8c.html)
- [メインの制御クラス(KDataFrame Class)のリファレンス](https://kyasya.github.io/Pandas-like-csv-parser/classKDataFrame.html)

## 配布物 & ディレクトリ構造

- **配布物**

- `KDataFrame.h`: **これが本体**.
- `test.c`: サンプルプログラム.
- `test.csv`: 適当に作ったサンプル用csvファイル.

- **ディレクトリ構造**

```bash
. # 以下の作業ではここが基準になります
├─ header
|  └─ KDataFrame.h # これがメイン
├─ pics # (関係なし)
└─ sample
   ├─ test.c # テスト用のファイル
   └─ test.csv # テスト用のcsvファイル
```

## サンプルプログラムの使い方

サンプルプログラムを動かすには**配布物をすべて同じディレクトリに配置**してください。(メインのヘッダファイルはサンプルと混ざって理由がわからなくならないように配布時にはわざと分けています)
なので、まずカレントディレクトリ「配布物」で列挙したファイルをすべてコピーします。

```bash
cp header/KDataFrame.h .
cp sample/test* .
```

もちろん自分で`test.c`を書いてくれてもおｋ (関数の説明も兼ねて[次節](#簡単なapiリファレンス)で解説).とりまここまですればこうなってるはず

```bash
. 
├─ KDataFrame.h # コピーしたやつ
├─ test.c # コピーしたやつ
├─ test.csv # コピーしたやつ
├─ header
├─ pics
└─ sample
```

あとは普通にコンパイルするだけ(ヘッダオンリーだから楽だね!)

```bash
g++ test.c -o test.out
```

実行

```bash
./test.out
```

実行したら下の図の感じになっているはず。

\image html sample.png

## 簡単なAPIリファレンス

`sample/test.c`にあるコードが現状できることのすべてレベル.

```c++
#include <iostream>
#include <vector>
#include "KDataFrame.h"

int main (int argc, char **argv)
{
    KDataFrame df("./test.csv");
    
    // カラムの一覧を表示(名前、年齢、身長)
    std::cout<<"Key: "<<df.GetDFInfo().Keys()<<std::endl; 

    df.Scan(); // csv内のデータの情報を表で表示

    // データの取り出し
    // 文字列でいいとき
    auto Data0 =  df["Name"];
    for(auto &v : Data0) std::cout<<v<<", ";
    std::cout<<"\n";
    
    // 上と同じ意味
    // df["Name"] <=> df.GetcolumnStr("Name")
    auto Data1 = df.GetcolumnStr("Name");
    for(auto &v : Data1) std::cout<<v<<", ";
    std::cout<<"\n";

    std::cout<<"\n";

    // 数字として取り出したい(文字列をキャストして取り出したい)とき
    auto Data = df.Get<int>("Age");
    for(auto &v : Data) std::cout<<v<<", ";
    std::cout<<"\n";

    return 0;
}
```

解説としては以下の通り.

- ローカルに置いて使っているので`#include`は二重引用符("").
- `KDataFrame df("./test.csv")`: constructor. 読み込みたいファイル名を指定.
- `df.GetDFInfo().Keys()`: キーを取得.
    - `df.GetDFInfo()`: CSVの情報を取得している.
- `df.Scan()`: csvの内容を表形式で表示. **NOTE: 長すぎるとカットされる.**
- データの取得:
    - 文字列でいいとき(後で整数なり少数なりに変換するとき):
        - `df["カラム名"]`で取得できる. これは`df.GetcolumnStr("カラム名")`と同じ.
    - 数字などの別の型で取り出したいとき:
        - `df.Get<型>("カラム名")`
        - テスト段階であることもあって`int`, `double`のみ対応. これから増やしていく.

[**リファレンスガイドはこちら**](https://kyasya.github.io/Pandas-like-csv-parser/) 

# 生まれた経緯

上に書いてあることがすべてみたいなところがあるが…
数年前、別のCSV Parserを作って使っていたのだけど、それを改造中に本業が忙しくなり放置してたら壊れちゃった…しかも数千行くらいあるからなにがなんだかわからん!
ので一年くらい毎回毎回CSVのプログラムを書いていたのだけど、C++でCSVのプログラムを書いているとカラムが増えたときの対応が大変！例えば…

"Age"という名前のカラムは読み出したデータの配列のx番目だから配列(vector)のx番目を指定して…

っていうのを何度も考えないといけない。クソめんどい。Pythonの[pandas](https://pandas.pydata.org/)モジュールなら

```
df["Age"]
```

だけで全部のデータを一気に取ってこれる！。これがほしい。というか昔作ってたのはそれができたし、何なら書き出しもできた…
ということで「せめて読み出しだけはできるようにしよう…そのほうが結局時短になるわ…」ということで再開発を始めることに…

最終的には読み書きができることを目標です.

研究で使う環境がC++14以上なのでそれに対応できるように作っていきたい(と言いながら初日からミスが発覚したので別の方法を考え中だが…).
