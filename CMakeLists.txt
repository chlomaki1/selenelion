cmake_minimum_required(VERSION 3.30)
project(comedy)

set(CMAKE_CXX_STANDARD 17)
file(GLOB_RECURSE SOURCE_FILES src/*.cc)
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)

target_include_directories(${PROJECT_NAME} PRIVATE
        incl/minhook/include
        incl/discord/src
)

add_subdirectory(incl/minhook)
add_subdirectory(incl/discord)
target_link_libraries(${PROJECT_NAME} minhook discord)