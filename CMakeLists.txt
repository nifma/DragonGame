cmake_minimum_required(VERSION 3.10)
project(DragonGame)

set(CMAKE_CXX_STANDARD 17)

# Указываем путь к установленной библиотеке jsoncpp
find_package(jsoncpp REQUIRED)

# Указываем исходные файлы
add_executable(game.cgi game.cpp)

# Указываем путь к jsoncpp, если CMake не может найти его автоматически
include_directories(/usr/include)

# Связываем с jsoncpp
target_link_libraries(game.cgi jsoncpp)
