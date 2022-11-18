rm -rf out/build
mkdir -p out/build
cmake -S . -B out/build
cd out/build
make