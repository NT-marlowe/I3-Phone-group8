#include <stdio.h>
#include <stdlib.h>
#include <math.h>


signed short orugan_sound(const signed short A, const double f, const int fs, const int n) {
  
  double res = 0; 

  double a[5] = {};
 
  a[0] = 1.0 * exp(-5.0 * n / (sample_freq * 4.0));
  a[1] = 0.8 * exp(-5.0 * n / (sample_freq * 2.0));
  a[2] = 0.6 * exp(-5.0 * n / (sample_freq * 1.0));
  a[3] = 0.5 * exp(-5.0 * n / (sample_freq * 0.5));
  a[4] = 0.4 * exp(-5.0 * n / (sample_freq * 0.2));

  for (int i = 0; i < 5; i++) {
    res += a[i] * sin(2.0 * M_PI * sound_freq * (i+1) * n / sample_freq);
  }
  
  res *= A;
  
  return (signed short)res;
  
}