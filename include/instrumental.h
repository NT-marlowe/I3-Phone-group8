#ifndef INSTRUMENTAL_H
#define INSTRUMENTAL_H

void scale_freq(const int n, double freq[n]);

double key_to_freq(const unsigned char key, const int n, const double freq[n], const int flag);

signed short sin_wave(const signed short A, const double f, const int fs, const int n);

signed short orugan_sound(const signed short A, const double f, const int fs, const int n);

signed short mokkin_sound(const signed short A, const double f, const int fs, const int n);

void ADSR(double e[], const int A, const int D, const double S, const int R, const int gate, const int duration);

void bell_sound(const signed short A, const double f, const int fs, const int duration, double res[duration]);

signed short guitar_sound(const signed short A, const double f, const int fs, const int n);

void *send_music_to_server(void *arg);

void *recv_music_from_server(void *arg);

#endif