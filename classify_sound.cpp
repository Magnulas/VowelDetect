#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <errno.h>
#include <getopt.h>
#include "alsa/asoundlib.h"
#include "alsa/pcm.h"
#include <sys/time.h>
#include <math.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <math.h>
#include <itpp/itstat.h>
#include <float.h>

#include <itpp/itcomm.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <ios>

using namespace itpp;

#define CLOSE_PIPE(handle) if(close(handle)==-1) {fprintf(stderr,"failed to close pipe\n"); return -1;}

void write_wav(char * filename, unsigned long num_samples, int * data, int s_rate);
void write_little_endian(unsigned int word, int num_bytes, FILE *wav_file);
snd_pcm_t* init_audio(unsigned int targetrate);
int maxDB(int* soundArray, signed long framesRecorded);

int main(int argc, char **argv, char **envp)
{
	char* wavFileName = "tmp.wav";
	char* praatCommand = "praat";
	char* praatArgs[] = {"praat","formanter.praat", wavFileName, NULL};

	int nInput = 3;

	char* vowels[] = {"A", "AO", "I", "O" };
	double formants[nInput];
	char* vowel;
	char* lastVowel;
	const char* gmm_files[] = {"gmms/A.gmm","gmms/AO.gmm","gmms/I.gmm","gmms/O.gmm"}; 
	MOG_diag gmm[4];

	int j = 0;
	for(j = 0;j<4;j++){
		gmm[j].load(gmm_files[j]);
	}

	unsigned int pollSpeedMin = 100000000;
	unsigned int pollSpeedMax = 900000000;

	struct timespec remaningTime, pollSpeed;
	pollSpeed.tv_sec = 0;
	pollSpeed.tv_nsec = pollSpeedMax; 
	
	unsigned int poolSpeedDecreaseSpeed = 100000000;
	unsigned int targetrate = 16000;

	int recordLengthMilliseconds = 100;
	int err;
	signed long buffsize = targetrate*recordLengthMilliseconds/1000;
	int* buffer;
	buffer = (int*)malloc(buffsize*sizeof(int));
	
	int db = -1;
	int run = 1;
	int nBytes = -1;
	snd_pcm_t* capture_handle = init_audio(targetrate);
	int dbThreashold = 152;
	double loglikelihood = 0;	
	double probthreashold = 0.05;
	FILE* rec_file = NULL;
	snd_pcm_sframes_t available;
	snd_pcm_sframes_t frames; //signed long
	//Record 100 ms
	while(run){
		//available = snd_pcm_avail (capture_handle);
		available = snd_pcm_forwardable (capture_handle);
		while(available>2*buffsize){
			//signed long a = snd_pcm_forward(capture_handle, available-buffsize);
			frames = snd_pcm_readi(capture_handle,buffer,buffsize);
			available = available - frames;
		}
		
		//Would be better with a blocking read but I can't get it to work
		frames = snd_pcm_readi(capture_handle,buffer,buffsize);
		//fprintf(stderr, "%ld %ld\n", frames, available);
		if(frames>0){
			db = maxDB(buffer,frames);
			if(db>dbThreashold){
				write_wav(wavFileName, frames, buffer, targetrate);
		
				int pipeStream[2];
	
				if(pipe(pipeStream)!=-1){ /*create a new pipe*/
					int childId = fork();
					switch(childId){
					case -1: /*fork failed*/

						break;
					case 0: /*Child proccess code*/
						dup2(pipeStream[1],1); /*Make stdout write end of pipe*/			
						CLOSE_PIPE(pipeStream[0]); /*Close read end of pipe*/
						CLOSE_PIPE(pipeStream[1]); /*Close write end of pipe*/
						//free(pipeStream);
						execvp(praatCommand, praatArgs);
						break;
					default: /*Parent proccess code*/
						dup2(pipeStream[0],0); /*Make stdout write end of pipe*/			
						CLOSE_PIPE(pipeStream[0]); /*Close read end of pipe*/
						CLOSE_PIPE(pipeStream[1]); /*Close write end of pipe*/
						//free(pipeStream);
						scanf("%lf %lf %lf", &formants[0], &formants[1],&formants[2]);

						loglikelihood = DBL_MIN;
	
						for(j = 0;j<4;j++){
							double tmp = gmm[j].log_lhood(formants);
							if(loglikelihood<tmp){
								loglikelihood = tmp;
								vowel=vowels[j];
							}
						}
						
						if(log10(loglikelihood)>probthreashold){
							fprintf(stdout,"%s %lf %lf %lf\n",vowel, formants[0], formants[1],formants[2]);
						}
						/* Mean to controll the output speed by decreaseing time between polls when same vowel is sounded twice in a row but buffer audio buffer isn't cleared between each poll so it doesn't work*/

						if(vowel==lastVowel){
							pollSpeed.tv_nsec = pollSpeed.tv_nsec - poolSpeedDecreaseSpeed;
							if(pollSpeed.tv_nsec<pollSpeedMin){
								pollSpeed.tv_nsec = pollSpeedMin;
							}
						} else{
							pollSpeed.tv_nsec = pollSpeedMax;
						}

						lastVowel = vowel;
						nanosleep(&pollSpeed,&remaningTime);
						break;
					}
				}
			}
		}
	}

	snd_pcm_close (capture_handle);

	return 0;
}
