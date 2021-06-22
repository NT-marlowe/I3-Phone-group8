#ifndef INCLUDE_orugan_h_
#define INCLUDE_orugan_h_

void scale_freq(const int n, double freq[n]);
double key_to_freq(const unsigned char key, const int n, const double freq[n], const int flag);

signed short sin_wave(const signed short A, const double f, const int fs, const int n);
signed short orugan_sound(const signed short A, const double f, const int fs, const int n);

#endif