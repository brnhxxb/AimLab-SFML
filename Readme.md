Как запустить:
#очистка билд папки
rm -r build
#конфигурация cmake
cmake -S . -B build
#сборка в режиме RELEASE
cmake --build build --config Release
#Копируем РЕЛИЗНЫЕ DLL и шрифт в папку Release
Copy-Item "vcpkg/installed/x64-windows/bin/*.dll" "build/Release/"
Copy-Item "font.ttf" "build/Release/"
сd build/Release
./AimTrainer.exe