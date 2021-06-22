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
// ↑キー入力の後、a,bなどを入力するとオクターブ上が出る


void die(char *s){
    perror(s);
    exit(1);
}

// 周波数のリスト
void scale_freq(const int n, double freq[n]){
    float f = 261.63; //ドの周波数
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
        if (key == notes[i]) return freq[i] * pow(2, flag); // 2 
    }
    return 0;
}

// // キーに対応した音の周波数を返す(1オクターブ上の音階)
// float key_to_freq2(const unsigned char key, const int n, const float freq[][13]){
    
//     const unsigned char notes[] = 
//         {'a', 'w', 's', 'e', 'd', 'f', 't', 'g', 'y', 'h', 'u', 'j', 'k'};
    
//     for (int i = 0; i < n; i++) {
//             if (key == notes[i]) return freq[1][i];
//     }
//     return 0;
// }

int main(int argc, char **argv){
    unsigned short A = 10000;
    const int n = 13;
    double freq[13];
    scale_freq(n, freq);
    const double dt = 1.0 / 44100;
    unsigned char key;

    system("/bin/stty raw onlcr");  // enterを押さなくてもキー入力を受け付けるようになる
    
    while(1){
        int r = read(0, &key, sizeof(key)); // 標準入力
        if (r == -1) die("read");
        if (r == 0) break;
        if (key == '.') break;
        double f;
        int flag = 0;
        if(key == 'A'){//↑の入力のとき
            flag = 1;
            r = read(0, &key, sizeof(key)); // 標準入力を再度読み込み、
            if (r == -1) die("read");
            if (r == 0) break;
            if (key == '.') break;
            // f = key_to_freq2(key, n, freq); // オクターブ上の周波数を取得する
        }

        f = key_to_freq(key, n, freq, flag);

        if (f == 0) continue;
        double t = 0;
        int count = 0;
        while(count < 44100 * 0.3 * 3){ // 0.1秒
            signed short data = (signed short)A *sin(2*M_PI*f*t);
            int m = write(1, &data, sizeof(data)); // 自分の標準出力に出せば自分で鳴らせる
            if (m == -1) die("write");
            // int m = write(s, &data, 2); // 相手に送る時
            // if (m == -1) die("send");
            t += dt;
            count += 1;
        }
    }

    system("/bin/stty cooked");  // 後始末

    return 0;
}