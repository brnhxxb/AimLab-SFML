# AimLab Trainer — C++ / SFML

Тренажёр для прицеливания с псевдо-3D эффектом.

## Примечание по шрифту

Игра автоматически ищет системный шрифт.
Если не найдёт — положи `font.ttf` или любой `.ttf` рядом в проект.

### 1. Клонировать репозиторий
git clone https://github.com/твой/репо.git
cd aimlab_sfml

### 2. Инициализировать vcpkg и установить зависимости
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat
vcpkg install sfml:x64-windows
cd ..

### 3. Собрать проект
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE="../vcpkg/scripts/buildsystems/vcpkg.cmake"
cmake --build . --config Release

### 4. Скопировать ресурсы и DLL
copy ..\font.ttf .
copy ..\vcpkg\installed\x64-windows\bin*.dll .\Release\

### 5. Запустить
.\Release\AimTrainer.exe

## Что есть в игре

- **3 режима**: STATIC (мишени стоят) / MOVING (двигаются) / SHRINK (затухают)
- **HP бары** под каждой мишенью (нужно 3 попадания чтобы уничтожить)
- **Псевдо-3D**: дальние мишени меньше и темнее, ближние — большие и яркие
- **Перспективная сетка** на фоне
- **Партикли** при уничтожении мишени
- **Прицел** который сжимается при выстреле
- **Счёт** зависит от размера мишени и скорости реакции
- **Экран результатов**: очки, точность, среднее/лучшее время реакции, промахи
- Таймер 60 секунд

## Управление
 Выстрел / выбор режима     ЛКМ 
 Выход / пауза в игре       ESC 
 Играть снова (результаты)  Enter



