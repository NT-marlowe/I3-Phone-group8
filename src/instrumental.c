#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
