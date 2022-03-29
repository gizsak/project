
g++ -O3 Final_Sparse_Lookup.cpp --std=c++11 -lbenchmark -lpthread -o FinalSparseLookupSmall
g++ -O3 Final_Dense_Lookup.cpp --std=c++11 -lbenchmark -lpthread -o FinalDenseLookupSmall

g++ -O3 Final_Sparse_Succes_Lookup.cpp --std=c++11 -lbenchmark -lpthread -o FinalSparseSuccesLookupSmall
g++ -O3 Final_Dense_Succes_Lookup.cpp --std=c++11 -lbenchmark -lpthread -o FinalDenseSuccesLookupSmall

g++ -O3 Final_Sparse_UnSucces_Lookup.cpp --std=c++11 -lbenchmark -lpthread -o FinalSparseUnSuccesLookupSmall
g++ -O3 Final_Dense_UnSucces_Lookup.cpp --std=c++11 -lbenchmark -lpthread -o FinalDenseUnSuccesLookupSmall


./FinalSparseLookupSmall --benchmark_out_format=csv --benchmark_out=FinalSparseLookup
./FinalDenseLookupSmall --benchmark_out_format=csv --benchmark_out=FinalDenseLookup

./FinalSparseSuccesLookupSmall --benchmark_out_format=csv --benchmark_out=FinalSparseSuccesLookup
./FinalDenseSuccesLookupSmall --benchmark_out_format=csv --benchmark_out=FinalDenseSuccesLookup

./FinalSparseUnSuccesLookupSmall --benchmark_out_format=csv --benchmark_out=FinalSparseUnSuccesLookup
./FinalDenseUnSuccesLookupSmall --benchmark_out_format=csv --benchmark_out=FinalDenseUnSuccesLookup


g++ -O3 Final_Insertion_Sparse.cpp --std=c++11 -lbenchmark -lpthread -o FinalInsertionSparseSmall
g++ -O3 Final_Insertion_Dense.cpp --std=c++11 -lbenchmark -lpthread -o FinalInsertionDenseSmall

./FinalInsertionSparseSmall --benchmark_out_format=csv --benchmark_out=FinalInsertionSparse24
./FinalInsertionDenseSmall --benchmark_out_format=csv --benchmark_out=FinalInsertionDense24
