SET mypath=%~dp0
cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DBUILD_SHARED_LIBS=YES -DCMAKE_BUILD_TYPE=Debug -H%mypath%..\ -B%mypath%..\build-shared -G "Visual Studio 16 2019" -T host=x64 -A x64
cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DBUILD_SHARED_LIBS=NO  -DCMAKE_BUILD_TYPE=Debug -H%mypath%..\ -B%mypath%..\build-static -G "Visual Studio 16 2019" -T host=x64 -A x64
cmake --build %mypath%..\build-shared --config Debug -j 10
cmake --build %mypath%..\build-static --config Debug -j 10
cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DBUILD_SHARED_LIBS=YES -DCMAKE_BUILD_TYPE=Release -H%mypath%..\ -B%mypath%..\build-shared -G "Visual Studio 16 2019" -T host=x64 -A x64
cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DBUILD_SHARED_LIBS=NO  -DCMAKE_BUILD_TYPE=Release -H%mypath%..\ -B%mypath%..\build-static -G "Visual Studio 16 2019" -T host=x64 -A x64
cmake --build %mypath%..\build-shared --config Release -j 10
cmake --build %mypath%..\build-static --config Release -j 10