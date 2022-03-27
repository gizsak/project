g++ -O3 Insertion_benchmark_dense.cpp --std=c++11 -lbenchmark -lpthread -o Insertion_benchmark_dense
./Insertion_benchmark_dense --benchmark_out_format=csv --benchmark_out=InsertionDenseBenchmark
g++ -O3 Insertion_benchmark_sparse.cpp --std=c++11 -lbenchmark -lpthread -o Insertion_benchmark_sparse
./Insertion_benchmark_sparse --benchmark_out_format=csv --benchmark_out=InsertionSparseBenchmark


g++ -O3 Lookup_benchmark_dense.cpp --std=c++11 -lbenchmark -lpthread -o Lookup_benchmark_dense
./Lookup_benchmark_dense --benchmark_out_format=csv --benchmark_out=LookupDenseBenchmark
g++ -O3 Lookup_benchmark_sparse.cpp --std=c++11 -lbenchmark -lpthread -o Lookup_benchmark_sparse
./Lookup_benchmark_sparse --benchmark_out_format=csv --benchmark_out=LookupSparseBenchmark

g++ -O3 Lookup_benchmark_unsucces_dense.cpp --std=c++11 -lbenchmark -lpthread -o Lookup_benchmark_unsucces_dense
./Lookup_benchmark_unsucces_dense --benchmark_out_format=csv --benchmark_out=LookupUnsuccesDenseBenchmark
g++ -O3 Lookup_benchmark_unsucces_sparse.cpp --std=c++11 -lbenchmark -lpthread -o Lookup_benchmark_unsucces_sparse
./Lookup_benchmark_unsucces_sparse --benchmark_out_format=csv --benchmark_out=LookupUnsuccesSparseBenchmark

g++ -O3 Lookup_benchmark_succes_dense.cpp --std=c++11 -lbenchmark -lpthread -o Lookup_benchmark_succes_dense
./Lookup_benchmark_succes_dense --benchmark_out_format=csv --benchmark_out=LookupSuccesDenseBenchmark
g++ -O3 Lookup_benchmark_succes_sparse.cpp --std=c++11 -lbenchmark -lpthread -o Lookup_benchmark_succes_sparse
./Lookup_benchmark_succes_sparse --benchmark_out_format=csv --benchmark_out=LookupSuccesSparseBenchmark








