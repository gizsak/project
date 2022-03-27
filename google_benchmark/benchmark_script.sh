g++ -O3 Insertion_benchmark_dense.cpp --std=c++11 -lbenchmark -lpthread -o Insertion_benchmark_dense
g++ -O3 Insertion_benchmark_sparse.cpp --std=c++11 -lbenchmark -lpthread -o Insertion_benchmark_sparse
g++ -O3 Lookup_benchmark_dense.cpp --std=c++11 -lbenchmark -lpthread -o Lookup_benchmark_dense
g++ -O3 Lookup_benchmark_sparse.cpp --std=c++11 -lbenchmark -lpthread -o Lookup_benchmark_sparse
g++ -O3 Lookup_benchmark_unsucces_dense.cpp --std=c++11 -lbenchmark -lpthread -o Lookup_benchmark_unsucces_dense
g++ -O3 Lookup_benchmark_unsucces_sparse.cpp --std=c++11 -lbenchmark -lpthread -o Lookup_benchmark_unsucces_sparse
g++ -O3 Lookup_benchmark_succes_dense.cpp --std=c++11 -lbenchmark -lpthread -o Lookup_benchmark_succes_dense
g++ -O3 Lookup_benchmark_succes_sparse.cpp --std=c++11 -lbenchmark -lpthread -o Lookup_benchmark_succes_sparse


./Insertion_benchmark_dense --benchmark_out_format=csv --benchmark_out=InsertionDenseBenchmark --benchmark_repetitions=3
./Insertion_benchmark_sparse --benchmark_out_format=csv --benchmark_out=InsertionSparseBenchmark --benchmark_repetitions=3
./Lookup_benchmark_dense --benchmark_out_format=csv --benchmark_out=LookupDenseBenchmark --benchmark_repetitions=3
./Lookup_benchmark_sparse --benchmark_out_format=csv --benchmark_out=LookupSparseBenchmark --benchmark_repetitions=3
./Lookup_benchmark_unsucces_dense --benchmark_out_format=csv --benchmark_out=LookupUnsuccesDenseBenchmark --benchmark_repetitions=3
./Lookup_benchmark_unsucces_sparse --benchmark_out_format=csv --benchmark_out=LookupUnsuccesSparseBenchmark --benchmark_repetitions=3
./Lookup_benchmark_succes_dense --benchmark_out_format=csv --benchmark_out=LookupSuccesDenseBenchmark --benchmark_repetitions=3
./Lookup_benchmark_succes_sparse --benchmark_out_format=csv --benchmark_out=LookupSuccesSparseBenchmark --benchmark_repetitions=3











