rm -rf out/build
mkdir -p out/build
cmake -S . -B out/build -G"Visual Studio 16 2019"
cd out/build

"C:\Program Files\Microsoft Visual Studio\2022\Community\Msbuild\Current\Bin\MSBuild.exe" CMAKE.sln

"../../builds/windows-x64/MSBuild.exe" CMAKE.sln

# cp ./Debug/CMAKE.exe ../../builds/windows-x64/