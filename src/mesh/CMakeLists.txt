cmake_minimum_required(VERSION 3.13)
project(meshtastic)

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

FetchContent_Declare(crc32 GIT_REPOSITORY git@github.com:Erriez/ErriezCRC32.git GIT_TAG master)
FetchContent_MakeAvailable(crc32)

# Define the library
add_library(mesh STATIC)

target_include_directories(mesh PRIVATE ${NANOPB_INCLUDE_DIRS} ./generated)
target_link_libraries(mesh nanopb)

list(FILTER crypto_sources INCLUDE REGEX *.cpp)
list(FILTER crc32_sources INCLUDE REGEX *.cpp)

target_include_directories(mesh PRIVATE ${crypto_SOURCE_DIR})
target_include_directories(mesh SYSTEM PRIVATE ${crc32_SOURCE_DIR}/src)

set(sources
        ../memGet.cpp
        Channels.cpp
        CryptoEngine.cpp
        Default.cpp
        FloodingRouter.cpp
#        InterfacesTemplates.cpp
        mesh-pb-constants.cpp
#        MeshService.cpp
        MeshPacketQueue.cpp
        NextHopRouter.cpp
        NodeDB.cpp
        PacketCache.cpp
        PacketHistory.cpp
        ProtobufModule.cpp
#        RadioInterface.cpp
        ReliableRouter.cpp
        Router.cpp
#        StreamAPI.cpp
        Throttle.cpp
        TypeConversions.cpp
        ../gps/RTC.cpp
)

target_include_directories(mesh PRIVATE ../)
target_include_directories(mesh PRIVATE ../gps/)
target_include_directories(mesh SYSTEM PRIVATE ./)
target_include_directories(mesh SYSTEM PRIVATE ${NANOPB_BINARY_DIR})

add_compile_definitions(mesh
        APP_VERSION=1.0.0
        HW_VENDOR=meshtastic_HardwareModel_UNSET
        THREAD_DRIVER=PosixThread
        PRINT_DRIVER=PosixPrint
        MESHTASTIC_EXCLUDE_TRACEROUTE
        MESHTASTIC_EXCLUDE_GPS
        MESHTASTIC_INCLUDE_NICHE_GRAPHICS
)

target_sources(mesh PRIVATE ${crypto_sources} ${crc32_sources} ${sources})
