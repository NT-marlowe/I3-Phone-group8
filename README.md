# I3-Phone-group8

## 制作物
1. 多者間接続を実装する．
2. 電子ピアノ（およびその他電子楽器）を作成する．
3. 送受信するデータを圧縮し，低遅延化をはかる．

## 最終目標
班のメンバー4人がそれぞれ電子楽器を演奏し，セッションする．

## フォルダ構成
```
.
├── bin 
├── include 
│   ├── instrumental.h 
|   ├── communication.h 
|   ├── compression.h 
├── lib 
└── src 
    ├── main.c 
    ├── instrumental.c
    ├── communication.c
    ├── compression.c
```

## 実行方法
```
make 
./main.sh

make clean (余分なファイルを消したいとき)
```
- `make`はオブジェクトファイルの作成と実行ファイルのコンパイルを行う．
<br>  `src`ディレクトリに`*.o`ファイルが配置され，`bin`ディレクトリに実行ファイル`main`が置かれる．

- 実行はシェルスクリプトの`main.sh`を使う．実行ファイルの名前を変えた場合はシェルスクリプトも書き換える必要がある．
<br>  `Permission denied`というエラーが出る時は`chmod +x main.sh`によって実行権限を与える．

- `make clean`は`make`コマンドで作られたオブジェクトファイルと実行ファイルを強制的に削除する．