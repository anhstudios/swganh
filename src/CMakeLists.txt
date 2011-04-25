cmake_minimum_required(VERSION 2.8.2)

project(swganh CXX)

set(swganh_VERSION_MAJOR 1)
set(swganh_VERSION_MINOR 0)
set(swganh_VERSION_PATCH 0)
set(swganh_VERSION 
	${swganh_VERSION_MAJOR}.${swganh_VERSION_MINOR}.${swganh_VERSION_PATCH})

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../tools/cmake/modules")
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
    if(MSVC_VERSION LESS 1600)
    	message(FATAL_ERROR "Visual Studio 2010 or newer is required ")
    endif()
	
	# any custom flags that need set for VS compilers only should be set here
	if(MSVC AND NOT CMAKE_GENERATOR MATCHES "Visual Studio 7")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244 /wd4251")
        message(STATUS "- MSVC: Disabled generic compiletime warnings")
    endif()
    
    ADD_DEFINITIONS (/D _CRT_SECURE_NO_WARNINGS /D _SCL_SECURE_NO_WARNINGS)
    MESSAGE(STATUS "- MSVC: Disabled NON-SECURE warnings")
        
    ADD_DEFINITIONS (/D __TBB_NO_IMPLICIT_LINKAGE=1)
    MESSAGE(STATUS "- MSVC: Disable automatic linking for TBB library")
endif()

# Use the static/multithreaded libraries.
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

## find/configure vendor libraries
set(ENV{MYSQL_DIR} ${MysqlConnectorC_ROOT})

find_package(PythonLibs REQUIRED)
find_package(Boost 1.45.0 COMPONENTS date_time regex thread system program_options filesystem serialization REQUIRED)

set(BOOST_LIBRARYDIR ${BoostShared_LIBRARYDIR})
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
    ## create a target that gathers up the necessary dll's from the above
    ## dependencies and places them in the runtime directory for this project
    add_custom_target(DEPS
        DEPENDS Glm GLog GTest GMock MysqlConnectorC MysqlConnectorCpp TBB ZLib    
    )

    add_custom_command(
        TARGET DEPS
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/bin/Debug/"
        VERBATIM
    )

    get_filename_component(BOOST_PYTHON_DLL_NAME ${Boost_PYTHON_LIBRARY_DEBUG} NAME_WE)
    get_filename_component(BOOST_PYTHON_DLL_PATH ${Boost_PYTHON_LIBRARY_DEBUG} PATH)
    add_custom_command(
        TARGET DEPS
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${BOOST_PYTHON_DLL_PATH}/${BOOST_PYTHON_DLL_NAME}.dll" "${PROJECT_BINARY_DIR}/bin/Debug/"
        VERBATIM
    )
    
    get_filename_component(GLOG_DLL_NAME ${GLOG_LIBRARY} NAME_WE)
    get_filename_component(GLOG_DLL_PATH ${GLOG_LIBRARY} PATH)
    add_custom_command(
        TARGET DEPS
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${GLOG_DLL_PATH}/${GLOG_DLL_NAME}.dll" "${PROJECT_BINARY_DIR}/bin/Debug/"
        VERBATIM
    )
    
    get_filename_component(MYSQL_CONNECTOR_C_DLL_NAME ${MYSQL_LIB} NAME_WE)
    get_filename_component(MYSQL_CONNECTOR_C_DLL_PATH ${MYSQL_LIB} PATH)
    add_custom_command(
        TARGET DEPS
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${MYSQL_CONNECTOR_C_DLL_PATH}/libmysql.dll" "${PROJECT_BINARY_DIR}/bin/Debug/"
        VERBATIM
    )
    
    
    get_filename_component(MYSQL_CONNECTOR_CPP_DLL_NAME ${MYSQLCONNECTORCPP_LIBRARY} NAME_WE)
    get_filename_component(MYSQL_CONNECTOR_CPP_DLL_PATH ${MYSQLCONNECTORCPP_LIBRARY} PATH)
    add_custom_command(
        TARGET DEPS
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${MYSQL_CONNECTOR_CPP_DLL_PATH}/${MYSQL_CONNECTOR_CPP_DLL_NAME}.dll" "${PROJECT_BINARY_DIR}/bin/Debug/"
        VERBATIM
    )
    
    get_filename_component(TBB_CPP_DLL_NAME ${TBB_LIBRARY_DEBUG} NAME_WE)
    get_filename_component(TBB_CPP_DLL_PATH ${TBB_LIBRARY_DEBUG} PATH)
    add_custom_command(
        TARGET DEPS
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${TBB_INSTALL_DIR}/build/vc10/ia32/Debug/tbb_debug.dll" "${PROJECT_BINARY_DIR}/bin/Debug/"
        VERBATIM
    )
    
    get_filename_component(TBB_CPP_DLL_NAME ${TBB_MALLOC_LIBRARY_DEBUG} NAME_WE)
    get_filename_component(TBB_CPP_DLL_PATH ${TBB_MALLOC_LIBRARY_DEBUG} PATH)
    add_custom_command(
        TARGET DEPS
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${TBB_INSTALL_DIR}/build/vc10/ia32/Debug/tbbmalloc_debug.dll" "${PROJECT_BINARY_DIR}/bin/Debug/"
        VERBATIM
    )
    
    get_filename_component(ZLIB_DLL_NAME ${ZLIB_LIBRARY} NAME_WE)
    get_filename_component(ZLIB_DLL_PATH ${ZLIB_LIBRARY} PATH)
    add_custom_command(
        TARGET DEPS
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${ZLIB_DLL_PATH}/zlibd1.dll" "${PROJECT_BINARY_DIR}/bin/Debug/"
        VERBATIM
    )
    
endif()

enable_testing()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

## project libraries
add_subdirectory(anh)

## project modules
add_subdirectory(test_module)
add_subdirectory(mod_anh_transform)
add_subdirectory(api)

## project executables
add_subdirectory(zone)
add_subdirectory(script_bindings)
add_subdirectory(pipeline)
