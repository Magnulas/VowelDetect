all: classify_sound.cpp itpp_gmm.cpp recordData.c audioUtil.c
	g++ itpp_gmm.cpp -o itpp_gmm.out `pkg-config --libs itpp`
	g++ classify_sound.cpp audioUtil.c -o classify_sound.out `pkg-config --libs alsa itpp`
	gcc recordData.c audioUtil.c -o recordData.out `pkg-config --libs alsa`
	