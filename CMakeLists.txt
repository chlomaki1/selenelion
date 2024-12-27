cmake_minimum_required(VERSION 3.30)
project(comedy)

set(CMAKE_CXX_STANDARD 20)
file(GLOB_RECURSE SOURCE_FILES src/*.cc)
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
target_compile_options(${PROJECT_NAME} PRIVATE /EHa)
target_compile_options(${PROJECT_NAME} PRIVATE /wd4530)

target_include_directories(${PROJECT_NAME} PRIVATE
        incl/minhook/include
        incl/discord/src
)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_subdirectory(incl/minhook)
add_subdirectory(incl/discord)
target_link_libraries(${PROJECT_NAME} minhook discord-sdk)