cmake_minimum_required(VERSION 3.10)
project(vienna_vulkan_engine)
set(CMAKE_CXX_STANDARD 20)

include_directories(
        ../external/stb
        ../external/threadpool
        ../external/irrKlang/include
        ../external/nuklear

        #brew headers
        /usr/local/include   
)

add_subdirectory(lib/ffmpeg)

link_directories(
        #brew lib
        /usr/local/lib
)

file(GLOB SOURCE_FILES CONFIGURE_DEPENDS "*.cpp")

find_library(IRRKLANG_LIBRARIES
    NAMES irrklang ikpMP3 ikpFLAC
    HINTS "${CMAKE_PREFIX_PATH}../external/irrklang/bin"
)

add_executable(
        VulkanEngine
        ${SOURCE_FILES}
        ../external/threadpool/ThreadPool.cpp
        )

target_link_libraries(
        VulkanEngine 
        vulkan glfw assimp ffmpeg ${IRRKLANG_LIBRARIES}
        )

