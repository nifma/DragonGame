cmake_minimum_required(VERSION 3.10)
project(DragonGame)

set(CMAKE_CXX_STANDARD 20)

# Убедитесь, что вы указали путь к заголовочному файлу jsoncpp

add_executable(game.cgi game.cpp)

# Указываем путь для генерации CGI скрипта
set_target_properties(game.cgi PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output
)
