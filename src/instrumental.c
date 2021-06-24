#include <stdio.h>
#include <stdlib.h>
#include <math.h>

<<<<<<< HEAD:src/instrumental.c
=======
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

>>>>>>> 3463f823aa0b6fa1dbdafed340d5277374068f19:instrumental.c
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

signed short violine_sound(const signed short A, const double f, const int fs, const int n) {
  
  double res = 0; 

  double a[5] = {};
 
  a[0] = 1.5 * exp(-5.0 * n / (fs * 4.0));
  a[1] = 0.1388 * exp(-5.0 * n / (fs * 2.0));
  a[2] = 0.486 * exp(-5.0 * n / (fs * 1.0));
  a[3] = 0.1388 * exp(-5.0 * n / (fs * 0.5));
  a[4] = 0.069 * exp(-5.0 * n / (fs * 0.2));

  for (int i = 0; i < 5; i++) {
    res += a[i] * sin(2.0 * M_PI * f * (i+1) * n / fs);
  }
  
  res *= A;

  return (signed short)res;
  
}


