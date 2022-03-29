# hash-table-tuning
The repository contains unit_test and google_benchmark directories and the corresponding necessary files to run google unit tests and google benchmark test.
In order to run the test clone the repository to your local machine. The only prerequisite to be able to run the tests is having git installed.
## google unit test
Google unit tests experiments the correctnes of each hash table implementations populating them to size of 2^16 at 50% loadfactor and then deleting half of the elements(2^14). The correctnes check includes:
- hash table population succed with every not contained key
- hash table size equals with the number of elements inserted
- every contained key lookup in hash table succeed
- every contained key/value pair lookup in hash table succeed
- deletion of every contained key succeed
- after deletion every deleted key lookup fails
- after deletion every remaining key lookup succeed
- after deletion hash table size equals to size of remaining elements
- after deletion every remaining key/value pair lookup in hash table succeed
### How to run unit tests
- Navigate to unit_test directory<br /> 
$ cd unit_test
- Check out google test library <br />  
$ git clone https://github.com/google/googletest.git
- Make a build directory <br />
$ mkdir build
- Navigate to the directory <br />
$ cd build
- Build ,compile and run the test files <br />
$ cmake ..
$ make
$ ./mytests
## google benchmark test
### How to run benchmark

First, install google benchmark:

```
git clone https://github.com/google/benchmark.git
cd benchmark
cmake -E make_directory "build"
cmake -E chdir "build" cmake -DBENCHMARK_DOWNLOAD_DEPENDENCIES=on -DCMAKE_BUILD_TYPE=Release ../
cmake --build "build" --config Release
sudo cmake --build "build" --config Release --target install
```

If you have google benchmark install, run the benchmark test script:

```
sudo bash ./benchmark_script.sh
```

The scripts runs all insertion and lookup experiments producing the following csv files containing the run time measurements:
- InsertionDenseBenchmark.csv
- InsertionSparseBenchmark.csv
- LookupDenseBenchmark.csv
- LookupSparseBenchmark.csv
- LookupUnsuccesDenseBenchmark.csv
- LookupUnsuccesSparseBenchmark.csv
- LookupSuccesDenseBenchmark.csv
- LookupSuccesSparseBenchmark.csv
