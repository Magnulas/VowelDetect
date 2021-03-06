all: classify_sound.cpp itpp_gmm.cpp recordData.c audioUtil.c evaluate_testdata.cpp datafile_util.c
	#g++ opencv_gmm.cpp datafile_util.c -o opencv_gmm.out `pkg-config --libs opencv --libs itpp`
	g++ itpp_gmm.cpp datafile_util.c -o itpp_gmm.out `pkg-config --libs itpp`
	g++ evaluate_testdata.cpp datafile_util.c -o evaluate_testdata.out `pkg-config --libs itpp`
	g++ -Wno-write-strings classify_sound.cpp audioUtil.c -o classify_sound.out `pkg-config --libs alsa itpp`
	gcc recordData.c audioUtil.c -o recordData.out `pkg-config --libs alsa`
	
