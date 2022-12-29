# HOW IT WORKS

### structure example
- J:\prj\work\prod\lig\s069\220\scenes\vd2_s069_220_lig_v000.ma

```
work though the directory seq, shot
read the .ma file and seperate by ';\n' into line
and copy the 
.ma
.ass
.abc
.exr
.vdo
into new directory
```

# Requirements
C:\Program Files\Microsoft Visual Studio\2022\Community\Msbuild\Current\Bin\MSBuild.exe

```
sh build_windows.sh
sh run_windows.sh
```

```
mkdir -p out/build
cmake -S . -B out/build
cd out/build
make
```

#DEBUG

--ggdb -g

```
sudo apt install gdb valgrind
cmake -DCMAKE_BUILD_TYPE=Debug .
```
