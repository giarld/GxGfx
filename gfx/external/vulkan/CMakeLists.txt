cmake_minimum_required(VERSION 3.20)

project(GFX-VULKAN)

file(GLOB_RECURSE TARGET_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

if (APPLE OR IOS)
    list(APPEND TARGET_SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/gvk_instance.mm
    )
endif ()

set(TARGET_NAME gfx-vk)

add_library(${TARGET_NAME} STATIC
        ${TARGET_SRC}
)

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${TARGET_NAME} PUBLIC gx)

if (IOS)
    target_link_libraries(${TARGET_NAME} PUBLIC "-framework OpenGLES  -framework Metal -framework UIKit -framework CoreGraphics -framework QuartzCore")
elseif (APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(METAL_LIBRARY Metal)
    find_library(QUARTZCORE_LIBRARY QuartzCore)
    mark_as_advanced(COCOA_LIBRARY)
    mark_as_advanced(METAL_LIBRARY)
    mark_as_advanced(QUARTZCORE_LIBRARY)
    target_link_libraries(${TARGET_NAME} PUBLIC ${COCOA_LIBRARY} ${METAL_LIBRARY} ${QUARTZCORE_LIBRARY})
endif ()
