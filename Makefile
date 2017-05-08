LIBSRC = Error Fourier FourierFFTW FourierMKL Function InputParser \
IOSets jsoncpp Main Master MatlabVector NumericalMethod Partition \
SolveIterSym SolveIterSymOneKind Distribution1D

all: 
	g++ $(addprefix ./Numerical_Lib/, $(addsuffix .cpp, $(LIBSRC))) ./NumericalMethod/Source.cpp -Wall -std=c++11 -D LOMO -o ./bin/a.out

