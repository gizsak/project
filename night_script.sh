g++ -O3 Lookup_benchmark_sparse_shufle_test.cpp --std=c++11 -lbenchmark -lpthread -o Lookup_benchmark_dense
g++ -O3 Lookup_benchmark_dense_shufle_test.cpp --std=c++11 -lbenchmark -lpthread -o Lookup_benchmark_dense




./Lookup_benchmark_sparse --benchmark_out_format=csv --benchmark_out=LookupSparseBenchmark
./Lookup_benchmark_dense --benchmark_out_format=csv --benchmark_out=LookupSparseBenchmark

g++ -O3 Lookup_benchmark_dense_shufle_test.cpp --std=c++11 -lbenchmark -lpthread -o Lookup_benchmark_dense
./Lookup_benchmark_dense --benchmark_out_format=csv --benchmark_out=LookupSparseBenchmark
