cmake_minimum_required(VERSION 3.21)
project(discord LANGUAGES C CXX)

# 1) Create an IMPORTED library target for Discord
add_library(${PROJECT_NAME} SHARED IMPORTED)

# 2) Point to the actual .dll and .lib
set_target_properties(${PROJECT_NAME} PROPERTIES
        IMPORTED_LOCATION  "${CMAKE_CURRENT_SOURCE_DIR}/discord_game_sdk.dll"
        IMPORTED_IMPLIB    "${CMAKE_CURRENT_SOURCE_DIR}/discord_game_sdk.dll.lib"
)

# 3) Provide an include directory so anyone linking "discord" can see the headers
target_include_directories(${PROJECT_NAME} INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)