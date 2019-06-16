cmake_minimum_required(VERSION 3.10)
project(mymodule)

macro (add_sources)
    file (RELATIVE_PATH _relPath "${PROJECT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
    foreach (_src ${ARGN})
        if (_relPath)
            list (APPEND SRCS "${_relPath}/${_src}")
        else()
            list (APPEND SRCS "${_src}")
        endif()
    endforeach()
    if (_relPath)
        # propagate SRCS to parent directory
        set (SRCS ${SRCS} PARENT_SCOPE)
    endif()
endmacro()


option(USE_CLANG "build application with clang" ON)

if(USE_CLANG)
SET (CMAKE_C_FLAGS_INIT                "-Wall -std=c++14")
SET (CMAKE_C_FLAGS_DEBUG_INIT          "-g")
SET (CMAKE_C_FLAGS_MINSIZEREL_INIT     "-Os -DNDEBUG")
SET (CMAKE_C_FLAGS_RELEASE_INIT        "-O4 -DNDEBUG")
SET (CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "-O2 -g")

SET (CMAKE_CXX_FLAGS_INIT                "-Wall")
SET (CMAKE_CXX_FLAGS_DEBUG_INIT          "-g")
SET (CMAKE_CXX_FLAGS_MINSIZEREL_INIT     "-Os -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELEASE_INIT        "-O4 -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "-O2 -g")
SET (CMAKE_TOOLCHAIN_PREFIX              "llvm-")
endif(USE_CLANG)

set(Boost_USE_STATIC_LIBS ON) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME ON) 
find_package(Boost 1.69 COMPONENTS date_time regex thread serialization system log log_setup program_options graph REQUIRED) 

include_directories(${Boost_INCLUDE_DIRS})


add_sources(test_p30.cpp test_58.cpp test_60.cpp test_p62.cpp)

add_executable(test_p30 test_p30.cpp)
add_executable(test_p58 test_p58.cpp)
add_executable(test_p60 test_p60.cpp)
add_executable(test_p62 test_p62.cpp)

target_link_libraries(test_p30 LINK_PUBLIC ${Boost_LIBRARIES} )
target_link_libraries(test_p58 LINK_PUBLIC ${Boost_LIBRARIES} )

