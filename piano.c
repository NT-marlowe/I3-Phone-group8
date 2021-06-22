#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// キー入力に対応する音を鳴らす
// 実行方法
// ./piano | play -t raw -b 16 -c 1 -e s -r 44100 -

void die(char *s){
    perror(s);
    exit(1);
}

// 周波数のリスト
void scale_freq(const int n, double freq[n]){
    double f = 261.63; //ドの周波数
    for (int i = 0; i < n; ++i){
        freq[i] = f;
        f *= pow(2, 1.0 / 12);
    }
}

// キーに対応した音の周波数を返す
double key_to_freq(const unsigned char key, const int n, const double freq[n]){
    
    const unsigned char notes[] = 
        {'a', 'w', 's', 'e', 'd', 'f', 't', 'g', 'y', 'h', 'u', 'j', 'k'};
    
    for (int i = 0; i < n; i++) {
        if (key == notes[i]) return freq[i];
    }
    
    return 0;
}

signed short sin_wave(const signed short A, const double f, const int fs, const int n){
    signed short data = (signed short)A * sin(2.0 * M_PI * f * n / fs);
    return data;
}

int main(int argc, char **argv){
    unsigned short A = 10000;
    const int n = 13;
    const int fs = 44100; // 標本化周波数
    double freq[n];
    scale_freq(n, freq);
    unsigned char key;
    system("/bin/stty raw onlcr");  // enterを押さなくてもキー入力を受け付けるようになる

    while(1){
        int r = read(0, &key, sizeof(key)); // 標準入力
        if (r == -1) die("read");
        if (r == 0) break;
        if (key == '.') break; //finish
        double f = key_to_freq(key, n, freq);
        if (f == 0) continue;
        
        int duration = (int)fs * 0.3; // 0.3秒
        signed short data;
        for (int i = 0; i < duration; ++i){
            data = sin_wave(A, f, fs, i);
            int m = write(1, &data, sizeof(data)); // 標準出力に出す
            if (m == -1) die("write");
        }
    }

    system("/bin/stty cooked");  // 後始末

    return 0;
}