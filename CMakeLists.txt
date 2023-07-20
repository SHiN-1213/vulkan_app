cmake_minimum_required(VERSION 3.25)
project(vulkan)

set(CMAKE_CXX_STANDARD 17)

include_directories(C:/glfw-3.3.8.bin.WIN64/glfw-3.3.8.bin.WIN64/include)
include_directories(C:/glm)
include_directories(C:/VulkanSDK/1.3.250.1/Include)
include_directories(C:/Program Files (x86) /Windows Kits/10/Include/10.0.22000.0/um)

link_directories(C:/VulkanSDK/1.3.250.1/Lib)
link_directories(C:/glfw-3.3.8.bin.WIN64/glfw-3.3.8.bin.WIN64/lib-mingw-w64)

add_executable(vulkan main.cpp reel.cpp reel.hpp vulkan_api.cpp vulkan_api.hpp reel_err.cpp reel_err.hpp)

target_link_libraries(vulkan vulkan-1.lib)
target_link_libraries(vulkan glfw3.lib)