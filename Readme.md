```
mkdir -p out/build
make -S . -B out/build
cd out/build
make
```

#DEBUG

--ggdb -g

```
sudo apt install gdb valgrind
cmake -DCMAKE_BUILD_TYPE=Debug .
```
