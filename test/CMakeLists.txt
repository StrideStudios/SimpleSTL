# CMake Parent File Requirements
cmake_minimum_required(VERSION 3.31...4.2)

set(CMAKE_CXX_STANDARD 17)

# Project Definition
project(
        SimpleSTLTest
)

function(addTest Name)

    add_executable(SimpleSTL-${Name}
        "${Name}.cpp"
    )
    # Prevent MSVC from complaining about template usage
    target_compile_options(SimpleSTL-${Name} PRIVATE /bigobj)
    target_include_directories(SimpleSTL-${Name} PUBLIC ../include)
    target_link_libraries(SimpleSTL-${Name} SimpleSTL)
endfunction()

addTest(Test)
addTest(ThreadedTest)
addTest(ReleaseTest)