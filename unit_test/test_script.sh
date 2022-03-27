
GOOGLETEST="/home/gabrielizsak6/test/googletest"
if [[ ! -d "$GOOGLETEST" ]]; then
  git clone https://github.com/google/googletest.git
fi

BUILD="/home/gabrielizsak6/test/build"
if [[ ! -d "$BUILD" ]]; then
  mkdir build
fi

cd build
cmake ..
make
./mytests








