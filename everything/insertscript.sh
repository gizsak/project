g++ -O3 Insertion_benchmark_Sparse.cpp --std=c++11 -lbenchmark -lpthread -o Insertion_benchmark_sparse
g++ -O3 Insertion_benchmark_Dense.cpp --std=c++11 -lbenchmark -lpthread -o Insertion_benchmark_dense

./Insertion_benchmark_sparse --benchmark_out_format=csv --benchmark_out=InsertionSparseBenchmark
./Insertion_benchmark_dense --benchmark_out_format=csv --benchmark_out=InsertionDenseBenchmark
