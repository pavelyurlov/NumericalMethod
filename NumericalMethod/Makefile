all:
	mpicxx -o numerical.run -std=c++11 -lmkl_intel_ilp64 -lmkl_core -lmkl_sequential -static-libgcc -static-libstdc++ jsoncpp.cpp IOSets.cpp Main.cpp MatlabVector.cpp SolveIterSym.cpp Fourier.cpp
	rm -rf *.o

.PHONY: modules
modules: 
	module add slurm/2.5.6 mkl/11.2.0 gcc/5.2.0 openmpi/1.8.4-gcc