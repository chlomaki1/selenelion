cmake_minimum_required(VERSION 3.21)

project(discord-sdk)

file(GLOB_RECURSE DISCORD "src/*.cpp", "/src/*.h")

add_library(${PROJECT_NAME} ${DISCORD})
target_include_directories(${PROJECT_NAME} INTERFACE "src")
target_link_libraries(discord-sdk "${CMAKE_CURRENT_SOURCE_DIR}/lib/discord_game_sdk.dll.lib")