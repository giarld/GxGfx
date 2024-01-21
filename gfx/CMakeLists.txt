cmake_minimum_required(VERSION 3.20)

# gx graphics library
project(GX-GFX)

set(TARGET_NAME gx-gfx)

add_subdirectory(external/vulkan)

file(GLOB_RECURSE TARGET_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

add_library(${TARGET_NAME}
        ${TARGET_SRC})
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/include/)

target_compile_definitions(${TARGET_NAME} PUBLIC USE_GP_API_VULKAN)
target_link_libraries(${TARGET_NAME} PUBLIC gx)
target_link_libraries(${TARGET_NAME} PRIVATE gfx-vk)
if (GX_USE_VULKAN_VMA)
    target_compile_definitions(${TARGET_NAME} PUBLIC USE_AMD_VULKAN_MEMORY_ALLOCATOR)
endif ()
