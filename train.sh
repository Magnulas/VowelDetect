g++ itpp_gmm.cpp -o itpp_gmm.out `pkg-config --libs itpp`
#g++ test_gmm.cpp -o test_gmm.out `pkg-config --libs itpp`
./itpp_gmm.out "gmm_data/A.data" "gmms/A.gmm" > train.output
#./test_gmm.out "gmm_data/A_test.data" "gmms/A.gmm" >> train.output
./itpp_gmm.out "gmm_data/AO.data" "gmms/AO.gmm" >> train.output
#./test_gmm.out "gmm_data/AO_test.data" "gmms/AO.gmm" >> train.output
./itpp_gmm.out "gmm_data/I.data" "gmms/I.gmm" >> train.output
#./test_gmm.out "gmm_data/I_test.data" "gmms/I.gmm" >> train.output
./itpp_gmm.out "gmm_data/O.data" "gmms/O.gmm" >> train.output
#./test_gmm.out "gmm_data/O_test.data" "gmms/O.gmm" >> train.output

