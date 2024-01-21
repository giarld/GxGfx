cmake_minimum_required(VERSION 3.20)

add_executable(TestGfx
        src/test_gfx.cpp
)

target_link_libraries(TestGfx gx-gfx gx-x)
