# CMake Parent File Requirements
cmake_minimum_required(VERSION 3.31...4.2)

# Project Definition
project(
        SimpleSTL
        DESCRIPTION "A simple header only library to allow for easy use of any STL container"
        VERSION 1.0.0
        LANGUAGES CXX C
)

option(SIMPLESTL_TEST "Enable testing SimpleSTL features" ${PROJECT_IS_TOP_LEVEL})

# Automatically call init on a type when created, can be used with vararg constructor as well, with the init args being after the ctor args
option(SIMPLESTL_INIT "Enable the auto init ability" On)

# Automatically call a destroy function on the type when destroyed, release will overwrite this behavior
option(SIMPLESTL_DESTROY "Enable the auto destroy ability" On)

# Initially disabled unless for testing, Release can be dangerous if used unknowingly, should only be enabled if you know what it does
# Allows you to specify a release function in the memory's types, which removes all deletion capability from the pointer,
# giving the ability to self-release the memory.  Can cause memory leaks if used improperly.
option(SIMPLESTL_RELEASE "Enable the release auto function ability" ${PROJECT_IS_TOP_LEVEL})

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

# If CXX Standard is not defined, fallback to c++17
if(NOT SimpleSTL_CXX_STANDARD)
    set(SimpleSTL_CXX_STANDARD 17)
    message(WARNING "SimpleSTL: Could not find parent CXX standard; Using default.")
endif()

message(STATUS "SimpleSTL: Using CXX version ${SimpleSTL_CXX_STANDARD}")

# Create Macro with CXX Version
target_compile_definitions(SimpleSTL INTERFACE CXX_VERSION=${SimpleSTL_CXX_STANDARD})

if(SIMPLESTL_INIT)
    target_compile_definitions(SimpleSTL INTERFACE SIMPLESTL_INIT)
endif()
if(SIMPLESTL_DESTROY)
    target_compile_definitions(SimpleSTL INTERFACE SIMPLESTL_DESTROY)
endif()
if(SIMPLESTL_RELEASE)
    target_compile_definitions(SimpleSTL INTERFACE SIMPLESTL_RELEASE)
endif()

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