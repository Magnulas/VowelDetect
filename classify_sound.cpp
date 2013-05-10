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
#include "audioUtil.h"
#include <itpp/itcomm.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <ios>

using namespace itpp;

#define CLOSE_PIPE(handle) if(close(handle)==-1) {fprintf(stderr,"failed to close pipe\n"); return -1;}

int main(int argc, char **argv, char **envp)
{
	if(argc%2==0||argc<3){
		fprintf(stderr,"Please specify pairs of gmm and output symbol.\n");
		return 0;
	} 

	int nGmms = argc/2;
	char* vowels[nGmms];
	char* gmm_files[nGmms];
	for(int i = 0;i<nGmms;i++){
		vowels[i] = argv[2*i+2];
		gmm_files[i] = argv[2*i+1];
	}

	char* wavFileName = "tmp.wav";
	char* praatCommand = "praat";
	char* praatArgs[] = {"praat","formanter.praat", wavFileName, NULL};
	
	MOG_diag gmm[nGmms];

	for(int j = 0;j<4;j++){
		gmm[j].load(gmm_files[j]);
	}

	int nInput = gmm[0].get_means()(0).size();
	double formants[nInput];
	char* vowel;
	char* lastVowel;

	unsigned int targetrate = 16000;
	int recordLengthMilliseconds = 100;
	signed long buffsize = targetrate*recordLengthMilliseconds/1000;
	int* buffer = (int*)malloc(buffsize*sizeof(int));
	
	int run = 1;
	snd_pcm_t* capture_handle = init_audio(targetrate);
	int dbThreashold = 152;
	
	double probthreashold = 0.001;  //Probability density function value threashold, to eliminate tails
	FILE* rec_file = NULL;
	snd_pcm_sframes_t frames; //signed long
	int pipeStream[2];
	//Record 100 ms segments
	while(run){
		frames = snd_pcm_readi(capture_handle,buffer,buffsize);
		
		if(frames>0){
			int db = maxDB(buffer,frames);
			if(db>dbThreashold){
				write_wav(wavFileName, frames, buffer, targetrate);
	
				if(pipe(pipeStream)!=-1){ /*create a new pipe*/
					int childId = fork();
					switch(childId){
					case -1: /*fork failed*/

						break;
					case 0: /*Child proccess code*/
						dup2(pipeStream[1],1); /*Make stdout write end of pipe*/			
						CLOSE_PIPE(pipeStream[0]); /*Close read end of pipe*/
						CLOSE_PIPE(pipeStream[1]); /*Close write end of pipe*/
						execvp(praatCommand, praatArgs);
						break;
					default: /*Parent proccess code*/
						dup2(pipeStream[0],0); /*Make stdout write end of pipe*/			
						CLOSE_PIPE(pipeStream[0]); /*Close read end of pipe*/
						CLOSE_PIPE(pipeStream[1]); /*Close write end of pipe*/
						
						//scanf("%lf %lf %lf", &formants[0], &formants[1],&formants[2]);
						
						for(int i = 0 ;i<nInput;i++){
							scanf("%lf", &formants[i]);
						}


						double likelihood = DBL_MIN;	//Probability density function value
	
						for(int j = 0;j<4;j++){
							double tmp = gmm[j].lhood(formants);
							if(likelihood<tmp){
								likelihood = tmp;
								vowel=vowels[j];
							}
						}
						//fprintf(stderr, "%s %lf\n", vowel, likelihood);
						//fprintf(stderr,"%s %lf %lf %lf\n",vowel, formants[0], formants[1],formants[2]);
						if(likelihood>probthreashold){
							//fprintf(stdout,"%s %lf %lf %lf\n",vowel, formants[0], formants[1],formants[2]);
							fprintf(stdout,"%s\n",vowel);
						}
						
						break;
					}
				}
			}
		}
	}

	snd_pcm_close (capture_handle);

	return 0;
}

