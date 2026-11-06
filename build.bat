@echo off
cmake -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel
echo [*] dotafloat.core.dll -> build/src/core/Release/dotafloat.core.dll
echo [*] dotafloat_loader.exe -> build/src/loader/Release/dotafloat_loader.exe
pause
