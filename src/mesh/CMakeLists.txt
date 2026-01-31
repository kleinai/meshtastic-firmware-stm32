cmake_minimum_required(VERSION 3.13)
project(libmeshtastic)

# Plugin includes
include(ExternalProject)
include(FetchContent)

# Add Nanopb
# TODO: Check if Nanopb is already installed?
ExternalProject_Add(Nanopb
        GIT_REPOSITORY https://github.com/nanopb/nanopb.git
        SOURCE_DIR ${CMAKE_BINARY_DIR}/Nanopb-src
        BINARY_DIR ${CMAKE_BINARY_DIR}/Nanopb-build
        INSTALL_COMMAND ""
)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_BINARY_DIR}/Nanopb-src/extra)
find_package(Nanopb REQUIRED)

# Download meshtastic/crypto
FetchContent_Declare(Crypto GIT_REPOSITORY https://github.com/meshtastic/Crypto.git GIT_TAG main)
FetchContent_MakeAvailable(Crypto)

# Define the library
add_library(meshtastic MODULE)

target_include_directories(meshtastic PRIVATE ${NANOPB_INCLUDE_DIRS} ./generated)
target_link_libraries(meshtastic proto)

list(FILTER crypto_sources INCLUDE REGEX *.cpp)

target_include_directories(meshtastic PRIVATE ${crypto_SOURCE_DIR})

file(GLOB_RECURSE sources *.cpp)
#file(GLOB_RECURSE headers *.h  ../concurrency/*.h)

target_include_directories(meshtastic PRIVATE ../)
target_include_directories(meshtastic SYSTEM PRIVATE ${NANOPB_BINARY_DIR})


target_sources(meshtastic PRIVATE ${crypto_sources} ${sources})
