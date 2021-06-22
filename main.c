#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "instrumental.h"

// 実行方法
// gcc -c main.c
// gcc -c instrumental.c
// gcc main.o instrumental.o -lm
// ./a.out | play -t raw -b 16 -c 1 -e s -r 44100 -

// ↑キー入力の後、a,bなどを入力するとオクターブ上が出る
// ↓キー入力の後、a,bなどを入力するとオクターブ下が出る
// 0(zero)入力の後、サイン波が鳴る(デフォルト)
// 1入力の後、オルガンが鳴る

void die(char *s){
    perror(s);
    exit(1);
}

int main(int argc, char **argv){

    unsigned short A = 10000;
    const int n = 13;

    const int fs = 44100; // 標本化周波数
    double freq[n];
    scale_freq(n, freq);
  
    unsigned char key;
    unsigned char mode = '0';
    system("/bin/stty raw onlcr");  // enterを押さなくてもキー入力を受け付けるようになる

    while(1){
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
        if ('0' <= key && key <= '1'){
            if (key == '0') mode = 0; // sin
            else if (key == '1') mode = 1; // オルガン
            continue;
        }

        double f = key_to_freq(key, n, freq, flag);
        if (f == 0) continue;
        
        int duration = (int)fs * 0.3; // 0.3秒
        signed short data;
        for (int i = 0; i < duration; ++i){
            switch(mode){
                // case 0: data = sin_wave(A, f, fs, i); break;
                case 1: data = orugan_sound(A, f, fs, i); break;
                default: data = sin_wave(A, f, fs, i); // mode = 0
            }
            // if (mode == 0) data = sin_wave(A, f, fs, i);
            // else if (mode == 1) data = orugan_sound(A, f, fs, i);

            int m = write(1, &data, sizeof(data)); // 標準出力に出す
            if (m == -1) die("write");
        }
    }

    system("/bin/stty cooked");  // 後始末

    return 0;
}