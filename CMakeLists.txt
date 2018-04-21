cmake_minimum_required(VERSION 3.8)
project(ld41)
#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)

include_directories(/home/loic/Programmation/libs/fluffy/include /home/loic/Programmation/libs/fluffy/extlibs/jsoncpp)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp main.cpp Application.cpp Application.hpp typedefs.hpp)
add_executable(ld41 ${SOURCE_FILES})
target_link_libraries(ld41 /home/loic/Programmation/libs/fluffy/cmake-build-debug/libfluffy.so sfml-system sfml-window sfml-graphics sfml-audio)
