cmake_minimum_required(VERSION 2.8.7)

project(swganh CXX)

# current version
set(swganh_VERSION_MAJOR 0)
set(swganh_VERSION_MINOR 4)
set(swganh_VERSION_PATCH 0)
set(swganh_VERSION 
	${swganh_VERSION_MAJOR}.${swganh_VERSION_MINOR}.${swganh_VERSION_PATCH})
	
# set the path for custom cmake scripts
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tools/cmake")

## Global Configuration Options

# Some automated systems need an xml report which requires a second run of the
# tests. To prevent these unnecessary runnings during normal builds the
# following option is set with a default of false.
option(ENABLE_TEST_REPORT
    "Enables the generation of a test report when running tests" false)

option(USE_SYSTEM_LIBRARIES
    "Uses system libraries instead of downloading them" false)

if(ENABLE_TEST_REPORT)
    enable_testing()
endif()

## compiler checks
include(CheckCXXCompilerFlag)

if(CMAKE_COMPILER_IS_GNUCXX)
    CHECK_CXX_COMPILER_FLAG(-Wno-invalid-offsetof HAS_NO_INVALID_OFFSETOF)
    if(HAS_NO_INVALID_OFFSETOF)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-invalid-offsetof")
    endif()

    CHECK_CXX_COMPILER_FLAG(-Wno-write-strings HAS_NO_WRITE_STRINGS)
    if(HAS_NO_WRITE_STRINGS)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-write-strings")
    endif()

    CHECK_CXX_COMPILER_FLAG(-std=c++0x HAS_CPP0X_SUPPORT)
    IF(HAS_CPP0X_SUPPORT)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    ENDIF()
endif()

if(MSVC)
message(STATUS " - MSVC: Version ${MSVC_VERSION}")
    if(MSVC_VERSION LESS 1700)
    	message(FATAL_ERROR "Visual Studio 11 or newer is required ")
    endif()
    
    SET(_WIN32_WINNT 0x0601 CACHE INTERNAL "Setting _WIN32_WINNT to 0x0601 for Windows Vista minimum APIs")
    SET(WINVER 0x0601 CACHE INTERNAL "Setting WINVER to 0x0601 for Windows Vista minimum APIs")
	
    ADD_DEFINITIONS (/D _WIN32_WINNT=${_WIN32_WINNT})
    MESSAGE(STATUS "- MSVC: Set minimum Windows API version")
    
    ADD_DEFINITIONS (/D _CRT_SECURE_NO_WARNINGS /D _SCL_SECURE_NO_WARNINGS /D WIN32_LEAN_AND_MEAN /D NOGDI)
    MESSAGE(STATUS "- MSVC: Disabled NON-SECURE warnings")
        
    ADD_DEFINITIONS (/D _VARIADIC_MAX=10)
    MESSAGE(STATUS "- MSVC: Set variadic max to 10 for compatibility with all supported platforms")
endif()

# Use the static/multithreaded libraries.
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)

if (IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/vendor)
    set(VENDOR_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/vendor)
endif()

if(DEFINED VENDOR_PREFIX)
    set(BOOST_ROOT ${VENDOR_PREFIX}/boost)
    set(GLM_ROOT ${VENDOR_PREFIX}/glm)
    set(GTEST_ROOT ${VENDOR_PREFIX}/gtest)
    set(GMOCK_ROOT ${VENDOR_PREFIX}/gmock)
    set(ENV{MYSQL_DIR} ${VENDOR_PREFIX}/mysql-connector-c)
    set(MysqlConnectorCpp_ROOT ${VENDOR_PREFIX}/mysql-connector-cpp)
    set(PYTHON_ROOT ${VENDOR_PREFIX}/python)    
    set(ZLIB_ROOT ${VENDOR_PREFIX}/zlib)
endif()

find_package(Boost 1.49.0 COMPONENTS date_time filesystem log program_options python3 regex system thread REQUIRED)
find_package(Glm REQUIRED)
find_package(GTest REQUIRED)
find_package(GMock REQUIRED)
find_package(MysqlConnectorC REQUIRED)
find_package(MysqlConnectorCpp REQUIRED)
find_package(PythonLibs REQUIRED)
find_package(ZLib REQUIRED)

add_subdirectory(src)
