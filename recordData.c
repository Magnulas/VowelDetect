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
#include "audioUtil.h"

int main(int argc, char **argv, char **envp)
{
	if(argc<4){
		fprintf(stderr,"Please specify the type of sound you are recording, which index to index from aswell as the number of recorings\n");
		return 0;
	}
	
	char* recordType = argv[1];
	
	int lowerbound = (int) strtol(argv[2], (char **)NULL, 10);
	int nsamples = (int) strtol(argv[3], (char **)NULL, 10);
	char wavFileName[50];

	int recordLengthMilliseconds = 100;
	int err;
	unsigned int targetrate = 16000;
	int buffsize = targetrate*recordLengthMilliseconds/1000;
	int* buffer;
	buffer = (int*)malloc(buffsize*sizeof(int));
	
	snd_pcm_t* capture_handle = init_audio(targetrate);

	int i;	
	
	//Record 100 ms

	for (i=lowerbound;i<lowerbound+nsamples;i++){
		signed long frames = snd_pcm_readi(capture_handle,buffer,buffsize);
		sprintf(wavFileName, "%s%d.wav", recordType, i);
		write_wav(wavFileName, buffsize, buffer, targetrate);
	}

	snd_pcm_close (capture_handle);

	return 0;
}

