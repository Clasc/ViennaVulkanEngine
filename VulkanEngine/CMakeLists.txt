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
        
        #ffmpeg from brew location
        /usr/local/Cellar/ffmpeg/4.4/include
)


link_directories(
        #brew lib
        /usr/local/lib
)

# add the executable
file(GLOB SOURCE_FILES CONFIGURE_DEPENDS  "*.h" "*.cpp")


find_library(AVCODEC_LIBRARY avcodec)

add_executable(
        VulkanEngine
        ${SOURCE_FILES}
        ../external/threadpool/ThreadPool.h
        ../external/threadpool/ThreadPool.cpp
)

target_link_libraries(VulkanEngine vulkan glfw assimp ${AVCODEC_LIBRARY})



set(CMAKE_CXX_STANDARD 14)
