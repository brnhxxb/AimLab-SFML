cmake_minimum_required(VERSION 3.10)
project(AimTrainer)

# Включаем C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options("/utf-8")

#путь к vcpkg
set(VCPKG_ROOT_DIR "${CMAKE_SOURCE_DIR}/vcpkg/installed/x64-windows")

#исходники
file(GLOB_RECURSE HANDLERS_SOURCES 
    "handlers/*.cpp"
    "handlers/*.h"
)

add_executable(AimTrainer 
    main.cpp 
    ${HANDLERS_SOURCES}
)

#поиск всех файлов
target_include_directories(AimTrainer PRIVATE 
    "${CMAKE_SOURCE_DIR}"
    "${VCPKG_ROOT_DIR}/include"
)

#либы
target_link_directories(AimTrainer PRIVATE "${VCPKG_ROOT_DIR}/lib")

target_link_libraries(AimTrainer PRIVATE 
    sfml-graphics
    sfml-window
    sfml-system
)