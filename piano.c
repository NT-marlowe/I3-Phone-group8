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
void scale_freq(const int n, float freq[n]){
    float f = 261.63; //ドの周波数
    for (int i = 0; i < n; ++i){
        freq[i] = f;
        f *= pow(2, 1.0 / 12);
    }
}

// キーに対応した音の周波数を返す
float key_to_freq(const unsigned char key, const int n, const float freq[n]){
    if (key == 'a'){
        return freq[0];
    } else if (key == 'w'){
        return freq[1];
    } else if (key == 's'){
        return freq[2];
    } else if (key == 'e'){
        return freq[3];
    } else if (key == 'd'){
        return freq[4];
    } else if (key == 'f'){
        return freq[5];
    } else if (key == 't'){
        return freq[6];
    } else if (key == 'g'){
        return freq[7];
    } else if (key == 'y'){
        return freq[8];
    } else if (key == 'h'){
        return freq[9];
    } else if (key == 'u'){
        return freq[10];
    } else if (key == 'j'){
        return freq[11];
    } else if (key == 'k'){
        return freq[12];
    }
    else{
        return 0;
    }
}

int main(int argc, char **argv){
    int16_t A = 10000;
    int n = 13;

    float freq[n];
    scale_freq(n, freq);
    float dt = 1.0 / 44100;
    unsigned char key;

    system("/bin/stty raw onlcr");  // enterを押さなくてもキー入力を受け付けるようになる
    
    while(1){
        int n = read(0, &key, sizeof(key)); // 標準入力
        if (n == -1) die("read");
        if (n == 0) break;
        if (key == '.') break;
        float f = key_to_freq(key, n, freq);
        if (f == 0) continue;
        float t = 0;
        int count = 0;
        while(count < 13230){ // 0.3秒
            int16_t data = (signed short)A *sin(2*M_PI*f*t);
            int m = write(1, &data, 2); // 自分の標準出力に出せば自分で鳴らせる
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