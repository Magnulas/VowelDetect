#include "alsa/asoundlib.h"


void write_wav(char * filename, unsigned long num_samples, int * data, int s_rate);
snd_pcm_t* init_audio(unsigned int targetrate);
int maxDB(int* soundArray, signed long framesRecorded);
