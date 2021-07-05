# I3-Phone-group8

## 制作物
1. 多者間接続を実装する．
2. 電子ピアノ（およびその他電子楽器）を作成する.  
(時間的に厳しい)~~送受信するデータを圧縮し，低遅延化をはかる．~~

## 最終目標
班のメンバー4人がそれぞれ電子楽器を演奏し，セッションする．

## フォルダ構成
```
.
├── bin 
├── include 
│   ├── instrumental.h 
│   ├── communication.h 
│   └── die.h
|   └── mypthread.h
├── lib 
└── src 
    ├── main.c 
    ├── instrumental.c
    ├── communication.c
    ├── die.c
    └── mypthread.c

```
## 各ファイルに含まれる関数一覧
### instrumental.c

|  関数名  |  入力  |  出力 | 機能  |
|  ----  |  ----  |  ----  |  ----  |
|  scale_freq |  int n, double freq[n]  |　void(freq配列の操作)  |  n個(オクターブ分)の周波数の配列を返す |
|  key_to_freq  |  char key, int n, double freq[n], int flag  | double  freq | 各キーボード入力に対する音の周波数を返す。flagが0なら基本音、1ならオクターブ上、など  | 
|  sin_wave  |  short A, double f, int fs, int n  | signed short res | 振幅A、周波数f, 標本化周波数fsの正弦波resを返す  | 
|  楽器たち  |  short A, double f, int fs, int nなど  | signed short res |  フーリエ級数展開などを用いて各楽器に対応する波resを返す  | 
|  send_music_server  | 情報が詰まった構造体 arg | void |  pthreadに渡すようの関数。キーボード入力によりサーバへと波形を渡す  | 
|  recv_music_from_server  | 情報が詰まった構造体 arg | void |  pthreadに渡すようの関数。サーバから受け取った波形を標準出力に流す  | 

### communication.c 接続用
|  関数名  |  入力  |  出力 | 機能  |
|  ----  |  ----  |  ----  |  ----  |
|  server |  int port, int number_of_client, int *s |　void | サーバがnumber_of_client分のクライアントと接続する  |
|  client |  char* address, int port, int *s |　void | クライントがサーバと接続する  |

### die.c
|  関数名  |  入力  |  出力 | 機能  |
|  ----  |  ----  |  ----  |  ----  |
|  die |  char *s |　void | エラー出力用  |

### myptread.c サーバ側で使用する
|  関数名  |  入力  |  出力 | 機能  |
|  ----  |  ----  |  ----  |  ----  |
|  sum |  signed short **waves, signed short *result, int number_of_client |　void | resulutにwavesを人数分足した波を代入する |
|  send_data_to_client |  情報が詰まった構造体 arg |　void | pthreadに渡すようの関数。サーバが各クライアントにデータを送る |
|  recieve_data_from_client |  情報が詰まった構造体 arg |　void | pthreadに渡すようの関数。各クライアントから送られてきたデータをサーバが受け取る |



## 実行方法
### 多人数通信  
src/Mutitalk.cをコンパイル  
#### サーバ側
```
./server.sh -l PORT NUMBER_OF_CLIENTS

```
#### クライアント側
```
./client.sh IPADDRESS PORT
```

### make(現状は楽器のみ、今後通信機能をいれたい)
```
$ make 

$ make clean (余分なファイルを消したいとき)
```
- `make`はオブジェクトファイルの作成と実行ファイルのコンパイルを行う．
<br>  `src`ディレクトリに`*.o`ファイルが配置され，`bin`ディレクトリに実行ファイル`main`が置かれる．

- 実行はシェルスクリプトの`main.sh`を使う．実行ファイルの名前を変えた場合はシェルスクリプトも書き換える必要がある．
<br> `src`ディレクトリに`*.o`ファイルが配置され，`bin`ディレクトリに実行ファイル`main`が置かれる．

- 実行はシェルスクリプトでできる．実行ファイルの名前を変えた場合はシェルスクリプトも書き換える必要がある．
<br>  `Permission denied`というエラーが出る時は`chmod +x main.sh`によって実行権限を与える．

- `make clean`は`make`コマンドで作られたオブジェクトファイルと実行ファイルを強制的に削除する．

### 実行モード
キーボードのa-kが白鍵、w-uが黒鍵を表す.  
↑または↓を入力すると、直後に入力した音のみがオクターブ上、下の音に変更される.  
また、数字を入力するとその後の音が以下の楽器に変更される
|  キーボード  |  楽器  |
| ---- | ---- |
|  default (0) |  sin波  |
|  1  |  オルガン  |
|  2  |  木琴  |
|  3  |  ベル  |
|  4  |  ギター  |

