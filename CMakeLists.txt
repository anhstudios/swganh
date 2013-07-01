# This file is part of SWGANH which is released under the MIT license.
# See file LICENSE or go to http://swganh.com/LICENSE

# Prevent in-source builds unless they are forced. In-source builds tend to
# pollute the source directories with generated cmake and other build related
# files.

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    if(NOT DEFINED WITH_IN_SOURCE_BUILD)
        message(FATAL_ERROR
            "\n CMake generation for swganh is not allowed within the source directory!"
            "\n Remove the CMakeCache.txt file and CMakeFiles directory and try again"
            "\n from another folder, e.g.:"
            "\n "
            "\n    rm CMakeCache.txt"
            "\n    rm -rf CMakeFiles"
            "\n    mkdir build"
            "\n    cd build"
            "\n    cmake .."
            "\n "
        )
    endif()
endif()

cmake_minimum_required(VERSION 2.8.11)

cmake_policy(VERSION 2.8.11.2013-0628-g46c95)
cmake_policy(SET CMP0022 NEW)

project(swganh CXX)

# current version
set(swganh_VERSION_MAJOR 0)
set(swganh_VERSION_MINOR 7)
set(swganh_VERSION_PATCH 0)
set(swganh_VERSION
    ${swganh_VERSION_MAJOR}.${swganh_VERSION_MINOR}.${swganh_VERSION_PATCH})

# set the path for custom cmake scripts
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tools/cmake")
include(${CMAKE_CURRENT_SOURCE_DIR}/tools/cmake/macros.cmake)

#-----------------------------------------------------------------------------
# Redirect output files

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# DLL should be in the same directory as EXE under Windows
if(WIN32)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
else(WIN32)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif(WIN32)

#-----------------------------------------------------------------------------
## Global Configuration Options

# Some automated systems need an xml report which requires a second run of the
# tests. To prevent these unnecessary runnings during normal builds the
# following option is set with a default of false.
option(ENABLE_TEST_REPORT
    "Enables the generation of a test report when running tests" OFF)

option(USE_SYSTEM_LIBRARIES
    "Uses system libraries instead of downloading them" OFF)

option(BUILD_PLUGINS
    "Build additional plugins" OFF)

option(BUILD_DOCS
    "Explicitly force building the documentation" OFF)

option(BUILD_EXAMPLES
    "Explicitly force building of examples" OFF)

option(TREAT_WARNINGS_AS_ERRORS
    "Treat all warnings as errors" ON)

if(ENABLE_TEST_REPORT)
    enable_testing()
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake_targets")

## compiler checks
include(CheckCXXCompilerFlag)

if(CMAKE_COMPILER_IS_GNUCXX)
    ANH_CHECK_CXX_COMPILER_FLAG(CXX_WARNINGS CXX_WARN_ALL -Wall)
    ANH_CHECK_CXX_COMPILER_FLAG(CXX_WARNINGS CXX_WARN_NO_INVALID_OFFSETOF -Wno-invalid-offsetof)
    ANH_CHECK_CXX_COMPILER_FLAG(CXX_WARNINGS CXX_WARN_NO_WRITE_STRINGS -Wno-write-strings)
    ANH_CHECK_CXX_COMPILER_FLAG(CXX_WARNINGS CXX_WARN_NO_CPP11_SUPPORT -Wno-invalid-offsetof)
    ANH_CHECK_CXX_COMPILER_FLAG(CXX_WARNINGS CXX_WARN_NO_CPP11_SUPPORT -std=c++11)

    if(TREAT_WARNINGS_AS_ERRORS)
        ANH_CHECK_CXX_COMPILER_FLAG(CXX_WARNINGS CXX_WARN_AS_ERROR -Werror)
        ANH_CHECK_CXX_COMPILER_FLAG(CXX_WARNINGS CXX_PEDANTIC_ERRORS -pedantic-errors)
        ANH_CHECK_CXX_COMPILER_FLAG(CXX_WARNINGS CXX_NO_UNUSED_ERRORS -Wno-unused-variable)
    endif()

    set(AdditionalBoostLibs chrono regex)
    add_definitions(-DBOOST_ASIO_DISABLE_MOVE)
    find_library(PTHREAD_LIBRARY NAMES pthread)
    find_library(DL_LIBRARY NAMES dl)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    ANH_CHECK_CXX_COMPILER_FLAG(CXX_WARNINGS CXX_WARN_ALL -Wall)
    ANH_CHECK_CXX_COMPILER_FLAG(CXX_WARNINGS CXX_WARN_NO_WRITE_STRINGS -Wno-write-strings)
    ANH_CHECK_CXX_COMPILER_FLAG(CXX_WARNINGS CXX_WARN_NO_INVALID_OFFSETOF -Wno-invalid-offsetof)
    ANH_CHECK_CXX_COMPILER_FLAG(CXX_WARNINGS CXX_WARN_NO_CPP11_SUPPORT -std=c++11)
    ANH_CHECK_CXX_COMPILER_FLAG(CXX_WARNINGS CXX_WARN_NO_LIBCPP_SUPPORT -stdlib=libc++)

    if(TREAT_WARNINGS_AS_ERRORS)
        ANH_CHECK_CXX_COMPILER_FLAG(CXX_WARNINGS CXX_WARN_AS_ERROR -Werror)
        ANH_CHECK_CXX_COMPILER_FLAG(CXX_WARNINGS CXX_PEDANTIC_ERRORS -pedantic-errors)
        ANH_CHECK_CXX_COMPILER_FLAG(CXX_WARNINGS CXX_NO_UNUSED_ERRORS -Wno-unused-variable)
        ANH_CHECK_CXX_COMPILER_FLAG(CXX_WARNINGS CXX_NO_UNNEEDED_ERRORS -Wno-unneeded-internal-declaration)
        ANH_CHECK_CXX_COMPILER_FLAG(CXX_WARNINGS CXX_NO_GNU_ERRORS -Wno-gnu)
        ANH_CHECK_CXX_COMPILER_FLAG(CXX_WARNINGS CXX_NO_ARRAY_BOUNDS_ERRORS -Wno-array-bounds)
        ANH_CHECK_CXX_COMPILER_FLAG(CXX_WARNINGS CXX_NO_UNUSED_FUNCTION_ERRORS -Wno-unused-function)
    endif()

    set(AdditionalBoostLibs chrono regex)
    add_definitions(-DBOOST_ASIO_DISABLE_MOVE)

    if(CMAKE_GENERATOR STREQUAL Xcode)
        set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
        set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
    endif()
endif()

if(MSVC)
    message(STATUS " - MSVC: Version ${MSVC_VERSION}")

    if(MSVC_VERSION LESS 1700)
        message(FATAL_ERROR "Visual Studio 11 or newer is required ")
    endif()

    SET(_WIN32_WINNT 0x0601 CACHE INTERNAL "Setting _WIN32_WINNT to 0x0601 for Windows Vista minimum APIs")
    SET(WINVER ${_WIN32_WINNT} CACHE INTERNAL "Setting WINVER to 0x0601 for Windows Vista minimum APIs")

    ADD_DEFINITIONS (/D _WIN32_WINNT=${_WIN32_WINNT})
    MESSAGE(STATUS "- MSVC: Set minimum Windows API version")

    ADD_DEFINITIONS (/D _CRT_SECURE_NO_WARNINGS /D _SCL_SECURE_NO_WARNINGS /D WIN32_LEAN_AND_MEAN /D NOGDI)
    MESSAGE(STATUS "- MSVC: Disabled NON-SECURE warnings")

    ADD_DEFINITIONS (/D _VARIADIC_MAX=10)
    MESSAGE(STATUS "- MSVC: Set variadic max to 10 for compatibility with dependencies")

    if(TREAT_WARNINGS_AS_ERRORS)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /WX")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /WX")
        message(STATUS "- MSVC: Treating all warnings as errors")
    endif()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251")

    ADD_DEFINITIONS (/D _SECURE_SCL=0)
    MESSAGE(STATUS "- MSVC: set iterator debugging to 0")

    add_definitions(-DHAVE_ROUND)
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_WARNINGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_WARNINGS}")

# Use the static/multithreaded libraries.
add_definitions(-DBOOST_ALL_NO_LIB)
add_definitions(-DBOOST_ALL_DYN_LINK)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)

# Check for a vendor directory in the source directory and in
# the directory above.
if(NOT DEFINED VENDOR_PREFIX)
    if (IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/vendor)
        set(VENDOR_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/vendor)
    elseif(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../vendor)
        set(VENDOR_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/../vendor)
    endif()
endif()

if(DEFINED VENDOR_PREFIX)
    set(BOOST_ROOT ${VENDOR_PREFIX})
    set(GLM_ROOT ${VENDOR_PREFIX})
    set(ENV{MYSQL_DIR} ${VENDOR_PREFIX})
    set(MysqlConnectorCpp_ROOT ${VENDOR_PREFIX})
    set(PYTHON_ROOT ${VENDOR_PREFIX})
    set(TURTLE_ROOT ${VENDOR_PREFIX})
    set(ZLIB_ROOT ${VENDOR_PREFIX})
endif()

if (NOT WIN32)
    find_package(TBB REQUIRED)
endif()

find_package(Boost 1.53.0 COMPONENTS date_time filesystem log_setup log program_options python3 system thread unit_test_framework ${AdditionalBoostLibs} REQUIRED)
find_package(Glm REQUIRED)
find_package(MysqlConnectorC REQUIRED)
find_package(MysqlConnectorCpp REQUIRED)
find_package(PythonLibs REQUIRED)
find_package(Turtle REQUIRED)
find_package(ZLib REQUIRED)

# make sure the source directory and the binary directory (where any generated
# files will be found) are automatically searched for includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_BINARY_DIR}/src)

add_subdirectory(src)

if(BUILD_PLUGINS)
    add_subdirectory(plugins)
endif()

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs AND BUILD_DOCS)
    add_subdirectory(docs)
endif()
