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
        include/Pair.h
        include/Container.h

        # Sequence Containers
        include/Vector.h
        include/Deque.h
        include/List.h
        include/ForwardList.h
        include/Array.h

        # Sorted Sequence Containers
        include/MinHeap.h
        include/MaxHeap.h

        # Container Adaptors
        include/Queue.h
        include/Stack.h

        # Associative Containers
        include/Set.h
        include/MultiSet.h
        include/Map.h
        include/MultiMap.h

        # Sorted Associative Containers
        include/PrioritySet.h
        include/PriorityMultiSet.h
        include/PriorityMap.h
        include/PriorityMultiMap.h
)

# If not overridden, STL CSS Standard is the same as parent
if(NOT SimpleSTL_CXX_STANDARD)
    set(SimpleSTL_CXX_STANDARD ${CMAKE_CXX_STANDARD})
endif ()

# If CXX Standard is not defined, fallback to c++17
if(NOT SimpleSTL_CXX_STANDARD)
    set(SimpleSTL_CXX_STANDARD 17)
    message(WARNING "SimpleSTL: Could not find parent CXX Standard; Using default.")
endif()

message(STATUS "SimpleSTL: Using CXX Version ${SimpleSTL_CXX_STANDARD}")

# Create Macro with CXX Version
target_compile_definitions(SimpleSTL INTERFACE CXX_VERSION=${SimpleSTL_CXX_STANDARD})

# Ensure target is compiled with CXX Version
target_compile_features(SimpleSTL INTERFACE "cxx_std_${SimpleSTL_CXX_STANDARD}")

if(SIMPLESTL_TEST)
    enable_testing()
    add_subdirectory(test)
endif()