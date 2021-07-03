#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "../include/die.h"

#include <math.h>

// 1回の通信で送るデータバイト数
#define PACKET_N 1024

// 周波数のリスト
void scale_freq(const int n, double freq[n]){

    double f = 261.63; //ドの周波数
    for (int i = 0; i < n; ++i){
        freq[i] = f;
        f *= pow(2, 1.0 / 12);
    }
}

// キーに対応した音の周波数を返す(基本となる音階)
double key_to_freq(const unsigned char key, const int n, const double freq[n], const int flag){
    
    const unsigned char notes[] = 
        {'a', 'w', 's', 'e', 'd', 'f', 't', 'g', 'y', 'h', 'u', 'j', 'k'};
    
    for (int i = 0; i < n; i++) {
        if (key == notes[i]) return freq[i] * pow(2, flag);
    }
    return 0;
}

signed short sin_wave(const signed short A, const double f, const int fs, const int n){

    signed short res = (signed short)A * sin(2.0 * M_PI * f * n / fs);
    return res;
}

signed short orugan_sound(const signed short A, const double f, const int fs, const int n) {
  
  double res = 0; 

  double a[5] = {};
 
  a[0] = 1.0 * exp(-5.0 * n / (fs * 4.0));
  a[1] = 0.8 * exp(-5.0 * n / (fs * 2.0));
  a[2] = 0.6 * exp(-5.0 * n / (fs * 1.0));
  a[3] = 0.5 * exp(-5.0 * n / (fs * 0.5));
  a[4] = 0.4 * exp(-5.0 * n / (fs * 0.2));

  for (int i = 0; i < 5; i++) {
    res += a[i] * sin(2.0 * M_PI * f * (i+1) * n / fs);
  }
  
  res *= A;

  return (signed short)res;
  
}

signed short mokkin_sound(const signed short A, const double f, const int fs, const int n) {
  
  double res = 0; 

  double a[5] = {};
 
  a[0] = 0.01 * exp(-5.0 * n / (fs * 2.0));
  a[1] = 0.01 * exp(-5.0 * n / (fs * 1.0));
  a[2] = 0.01 * exp(-5.0 * n / (fs * 0.5));
  a[3] = 1.0 * exp(-5.0 * n / (fs * 0.25));
  a[4] = 0.8 * exp(-5.0 * n / (fs * 0.125));

  for (int i = 0; i < 5; i++) {
    res += a[i] * sin(2.0 * M_PI * f * (i+1) * n / fs);
  }
  
  res *= A;

  return (signed short)res;
  
}

void ADSR(double e[], const int A, const int D, const double S, const int R, const int gate, const int duration)
{

  if (A != 0){
    for (int n = 0; n < A; n++){
      e[n] = 1.0 - exp(-5.0 * n / A);
    }
  }
  
  if (D != 0){
    for (int n = A; n < gate; n++){
      e[n] = S + (1 - S) * exp(-5.0 * (n - A) / D);
    }
  } else{
    for (int n = A; n < gate; n++){
      e[n] = S;
    }
  }
  
  if (R != 0){
    for (int n = gate; n < duration; n++){
      e[n] = e[gate - 1] * exp(-5.0 * (n - gate + 1) / R);
    }
  }
}

void bell_sound(const signed short A, const double f, const int fs, const int duration, double res[duration]){
  
  double *ac = calloc(duration, sizeof(double));
  double *am = calloc(duration, sizeof(double));
  ADSR(ac, 0, duration, 0.0, duration, duration, duration);
  ADSR(am, 0, duration / 2, 0.0, duration / 2, duration, duration);
  double fm = f * 5.5;
  for (int i = 0; i < duration; ++i){
    res[i] = A * ac[i] * sin(2.0 * M_PI * f * i / fs) + A * am[i] * sin(2.0 * M_PI * fm * i / fs);
    res[i] = (signed short)res[i];
  }
  free(ac);
  free(am);
}

signed short guitar_sound(const signed short A, const double f, const int fs, const int n) {
  
  double res = 0; 

  double a[10] = {};
 
  a[0] = 1.0 * exp(-5.0 * n / (fs * 2.0));
  a[1] = 0.2 * exp(-5.0 * n / (fs * 1.0));
  a[2] = 0.8 * exp(-5.0 * n / (fs * 0.5));
  a[3] = 0.1 * exp(-5.0 * n / (fs * 0.25));
  a[4] = 0.6 * exp(-5.0 * n / (fs * 0.125));
  a[5] = 0 * exp(-5.0 * n / (fs * 1.0));
  a[6] = 0.4 * exp(-5.0 * n / (fs * 0.5));
  a[7] = 0.3 * exp(-5.0 * n / (fs * 0.25));
  a[8] = 0.2 * exp(-5.0 * n / (fs * 0.25));
  a[9] = 0 * exp(-5.0 * n / (fs * 0.125));

  for (int i = 0; i < 10; i++) {
    res += a[i] * sin(2.0 * M_PI * f * (i+1) * n / fs);
  }
  
  res *= A;

  return (signed short)res;
  
}

// sで指定した場所にキー入力から変換された波形を送る関数
// クライアントからサーバに波形を送る
void *send_music_to_server(void *arg){	
  int s = *((int*)arg);
  unsigned short A = 10000;
  const int n = 13;

  // const int PACKET = 1024;//一度に送るデータ量
  const int fs = 44100; // 標本化周波数
  double freq[n];
  scale_freq(n, freq);

  unsigned char key;
  unsigned char mode = '0';
  system("/bin/stty raw onlcr");  // enterを押さなくてもキー入力を受け付けるようになる

  while (1) {
    int r = read(0, &key, sizeof(key)); // 標準入力
    if (r == -1) die("read");
    if (r == 0) break;

    if (key == '.') break;
    
    int flag = 0;

    // ↑↓が押された時
    if ('A' <= key && key <= 'B'){
      if (key == 'A') flag = 1; // ↑の入力のときオクターブ上げる
      else if (key == 'B') flag = -1; // ↓の入力のとき1オクターブ下げる

      r = read(0, &key, sizeof(key)); // 標準入力を再度読み込み
      if (r == -1) die("read");
      if (r == 0) break;
      if (key == '.') break;
    }

      // 数字が押された時は以降の音色を変える
    if ('0' <= key && key <= '4'){
      if (key == '0') mode = 0; // sin
      else if (key == '1') mode = 1; // オルガン
      else if (key == '2') mode = 2; //木琴
      else if (key == '3') mode = 3; // ベル
      else if (key == '4') mode = 4; // guitar
      continue;
    }

    double f = key_to_freq(key, n, freq, flag);
    if (f == 0) continue;
    
    int duration = (int)fs * 0.3; // 0.3秒
    double bell[duration];

    if (mode == 3) {
      bell_sound(A, f, fs, duration, bell);
    }
    
    signed short data[PACKET_N];
    for (int i = 0; i < duration/PACKET_N; ++i){
      for (int j = 0; j < PACKET_N; ++j){
        switch(mode){
          case 1: data[j] = orugan_sound(A, f, fs, i*PACKET_N+j); break;
          case 2: data[j] = mokkin_sound(A, f, fs, i*PACKET_N+j); break;
          case 3: data[j] = bell[i*PACKET_N+j]; break;
          case 4: data[j] = guitar_sound(A, f/2, fs, i*PACKET_N+j); break; // guitarは低音が得意なのでデフォで低音に
          default: data[j] = sin_wave(A, f, fs, i*PACKET_N+j); // mode = 0
        }
      }
      int m = write(s, &data, PACKET_N*sizeof(data)); // sで指定した相手に送る
      if (m == -1) die("write");
    }
    for (int i = 0; i < duration / 2; ++i){ // 前の音が残らないように無音を書き込む
      data[i] = 0;
      if (write(s, &data, PACKET_N*sizeof(data)) == -1) die("write"); // sで指定した相手に送る
    }
  }

  system("/bin/stty cooked");  // 後始末
}

// ｓで指定した場所からデータを受け取って標準出力に出す関数
// クライアントがサーバから音楽を受け取る．
void *recv_music_from_server(void *arg){ 
  int s = *((int*)arg);
  while (1) {
    signed short data[PACKET_N];
    int m = recv(s, data, PACKET_N*sizeof(data[0]),0);
    if (m == -1) die("open");
    if (m == 0) break;
    if (write(1, data, PACKET_N*sizeof(data)) == -1) die("write");
  }
}
