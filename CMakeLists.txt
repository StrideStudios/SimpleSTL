# CMake Parent File Requirements
cmake_minimum_required(VERSION 3.31...4.2)

# Project Definition
project(
        SimpleSTL
        DESCRIPTION "A simple header only library to allow for easy use of any STL container"
        VERSION 1.0.0
        LANGUAGES CXX C
)

option(SIMPLESTL_TEST "Enable Testing SimpleSTL Features" ${PROJECT_IS_TOP_LEVEL})

add_library(SimpleSTL INTERFACE
        # Base Classes
        include/sstl/Pair.h
        include/sstl/Container.h

        # Sequence Containers
        include/sstl/Vector.h
        include/sstl/Deque.h
        include/sstl/List.h
        include/sstl/ForwardList.h
        include/sstl/Array.h

        # Sorted Sequence Containers
        include/sstl/MinHeap.h
        include/sstl/MaxHeap.h

        # Container Adaptors
        include/sstl/Queue.h
        include/sstl/Stack.h

        # Associative Containers
        include/sstl/Set.h
        include/sstl/MultiSet.h
        include/sstl/Map.h
        include/sstl/MultiMap.h

        # Sorted Associative Containers
        include/sstl/PrioritySet.h
        include/sstl/PriorityMultiSet.h
        include/sstl/PriorityMap.h
        include/sstl/PriorityMultiMap.h
)

# If not overridden, STL CSS Standard is the same as parent
if(NOT SimpleSTL_CXX_STANDARD)
    set(SimpleSTL_CXX_STANDARD ${CMAKE_CXX_STANDARD})
endif ()

# If CXX Standard is not defined, fallback to c++20
if(NOT SimpleSTL_CXX_STANDARD)
    set(SimpleSTL_CXX_STANDARD 20)
    message(WARNING "SimpleSTL: Could not find parent CXX Standard; Using default.")
endif()

message(STATUS "SimpleSTL: Using CXX Version ${SimpleSTL_CXX_STANDARD}")

# Create Macro with CXX Version
target_compile_definitions(SimpleSTL INTERFACE CXX_VERSION=${SimpleSTL_CXX_STANDARD})

# Ensure target is compiled with CXX Version
target_compile_features(SimpleSTL INTERFACE "cxx_std_${SimpleSTL_CXX_STANDARD}")

# Include Files
target_include_directories(SimpleSTL INTERFACE
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

if(SIMPLESTL_TEST)
    enable_testing()
    add_subdirectory(test)
endif()