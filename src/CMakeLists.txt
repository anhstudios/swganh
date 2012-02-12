cmake_minimum_required(VERSION 2.8.2)

project(swganh CXX)

configure_file (
  "${PROJECT_SOURCE_DIR}/version.h.in"
  "${PROJECT_BINARY_DIR}/version.h"
  )
  
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
    if(MSVC_VERSION LESS 1600)
    	message(FATAL_ERROR "Visual Studio 2010 or newer is required ")
    endif()
    
    SET(_WIN32_WINNT 0x0501 CACHE INTERNAL "Setting _WIN32_WINNT to 0x0501 for Windows XP minimum APIs")
    SET(WINVER 0x0501 CACHE INTERNAL "Setting WINVER to 0x0501 for Windows XP minimum APIs")
	
	# any custom flags that need set for VS compilers only should be set here
	if(MSVC AND NOT CMAKE_GENERATOR MATCHES "Visual Studio 7")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244 /wd4251 /wd4190 /wd4373 /wd4275")
        message(STATUS "- MSVC: Disabled generic compiletime warnings")
    endif()
    
    ADD_DEFINITIONS (/D _WIN32_WINNT=${_WIN32_WINNT})
    MESSAGE(STATUS "- MSVC: Set minimum Windows API version")
    
    ADD_DEFINITIONS (/D _CRT_SECURE_NO_WARNINGS /D _SCL_SECURE_NO_WARNINGS /D WIN32_LEAN_AND_MEAN /D NOGDI)
    MESSAGE(STATUS "- MSVC: Disabled NON-SECURE warnings")
        
    ADD_DEFINITIONS (/D __TBB_NO_IMPLICIT_LINKAGE=1)
    MESSAGE(STATUS "- MSVC: Disable automatic linking for TBB library")
    
    ADD_DEFINITIONS (/D _VARIADIC_MAX=10)
    MESSAGE(STATUS "- MSVC: Set variadic max to 10 for compatibility with all supported platforms")
endif()

# Use the static/multithreaded libraries.
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

if(DEFINED VENDOR_PREFIX)
    set(BOOST_ROOT ${VENDOR_PREFIX}/boost)
    set(GLM_ROOT ${VENDOR_PREFIX}/glm)
    set(GLOG_ROOT ${VENDOR_PREFIX}/glog)
    set(GTEST_ROOT ${VENDOR_PREFIX}/gtest)
    set(GMOCK_ROOT ${VENDOR_PREFIX}/gmock)
    set(LUA_ROOT ${VENDOR_PREFIX}/lua)
    set(ENV{MYSQL_DIR} ${VENDOR_PREFIX}/mysql-connector-c)
    set(MysqlConnectorCpp_ROOT ${VENDOR_PREFIX}/mysql-connector-cpp)
    set(NOISE_ROOT ${VENDOR_PREFIX}/noise)
    set(PYTHON_ROOT ${VENDOR_PREFIX}/python)
    
    if(MSVC10)        
        set(ENV{TBB_ARCH_PLATFORM} "ia32\\vc10")
    elseif(MSVC11)
        set(ENV{TBB_ARCH_PLATFORM} "ia32\\vc11")
    endif()
    
    set(TBB_INSTALL_DIR ${VENDOR_PREFIX}/tbb)
    set(TOLUAPP_ROOT ${VENDOR_PREFIX}/tolua++)
    set(ZLIB_ROOT ${VENDOR_PREFIX}/zlib)
endif()

find_package(PythonLibs REQUIRED)
find_package(Boost 1.45.0 COMPONENTS date_time regex thread system program_options filesystem REQUIRED)

set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost 1.45.0 COMPONENTS python REQUIRED)

find_package(Glm REQUIRED)
find_package(GLog REQUIRED)
find_package(GTest REQUIRED)
find_package(GMock REQUIRED)
find_package(MysqlConnectorC REQUIRED)
find_package(MysqlConnectorCpp REQUIRED)
find_package(TBB REQUIRED)
find_package(ZLib REQUIRED)


if(WIN32)
    include(Win32PostBuild)
endif()

if (ENABLE_TEST_REPORT)
    enable_testing()
endif()

# make sure the source directory and the binary directory (where any generated
# files will be found) are automatically searched for includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

## project libraries
add_subdirectory(anh)

## project plugins
add_subdirectory(plugins)

## project executables
add_subdirectory(swganh)