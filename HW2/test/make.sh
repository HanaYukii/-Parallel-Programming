g++ -c test.cpp -o test.o -fopenmp
g++ test.o -o test -fopenmp -pthread
