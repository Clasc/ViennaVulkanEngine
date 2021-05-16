cmake_minimum_required(VERSION 3.10)
project(vienna_vulkan_engine)
set(CMAKE_CXX_STANDARD 20)

include_directories(
        ../external/stb
        ../external/threadpool
        ../external/imgui

        #brew headers
        /usr/local/include      
)

add_subdirectory(lib/ffmpeg)

link_directories(
        #brew lib
        /usr/local/lib
)

# add the executable
file(GLOB SOURCE_FILES CONFIGURE_DEPENDS  "*.h" "*.cpp")


add_executable(
        VulkanEngine
        ${SOURCE_FILES}
        ../external/threadpool/ThreadPool.h
        ../external/threadpool/ThreadPool.cpp
        ../external/imgui/imgui.cpp
        ../external/imgui/imgui_tables.cpp
        ../external/imgui/imgui_widgets.cpp
        ../external/imgui/imgui_impl_glfw.cpp
        ../external/imgui/imgui_impl_vulkan.cpp
        ../external/imgui/imgui_draw.cpp
)

target_link_libraries(VulkanEngine vulkan glfw assimp ffmpeg)
