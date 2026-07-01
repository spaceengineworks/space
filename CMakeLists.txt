cmake_minimum_required(VERSION 3.20)
project(space LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set(OPTIKOS_RENDERER "VULKAN" CACHE STRING "" FORCE)
set(OPTIKOS_PLATFORM "GLFW"   CACHE STRING "" FORCE)
set(OPTIKOS_INPUT    "GLFW"   CACHE STRING "" FORCE)

set(OPTIKOS_ENABLE_PROFILING OFF CACHE BOOL "" FORCE)

find_package(glfw3 CONFIG REQUIRED)
find_package(Vulkan REQUIRED MODULE)
find_package(unofficial-shaderc CONFIG REQUIRED)

add_subdirectory("C:/Users/ilanv/Optikos" optikos-build)

add_subdirectory(
    "C:/Users/ilanv/SERuntime" 
    "${CMAKE_CURRENT_BINARY_DIR}/SERuntime_build"
)

add_executable(space main.cpp)

target_include_directories(space PRIVATE 
    "C:/Users/ilanv/SERuntime/src"
    ${tracy_SOURCE_DIR}/public
)

target_link_libraries(space PRIVATE 
    Optikos::Optikos
    SERuntime
    glfw
    Tracy::TracyClient
)

if(OPTIKOS_RENDERER STREQUAL "VULKAN")
    target_link_libraries(space PRIVATE 
        Vulkan::Vulkan
        unofficial::shaderc::shaderc
    )
else()
    target_link_libraries(space PRIVATE 
        glad::glad
    )
endif()

target_compile_definitions(space PRIVATE
    OPTIKOS_RES_DIR="C:/Users/ilanv/Optikos/res/"
)

option(ENABLE_GPU_PROFILING "Enable GPU profiling" OFF)
if(ENABLE_GPU_PROFILING)
    target_compile_definitions(space PRIVATE ENABLE_GPU_PROFILING)
    target_compile_definitions(Optikos PRIVATE ENABLE_GPU_PROFILING)
    message(STATUS "GPU Profiling: ON")
endif()

optikos_copy_resources(space)

add_custom_command(TARGET space POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:SERuntime>
    $<TARGET_FILE_DIR:space>
)