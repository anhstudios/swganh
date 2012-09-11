#python directories
set(ANH_PYTHON_BINDINGS "")

add_subdirectory(app)
add_subdirectory(command)
add_subdirectory(combat)
add_subdirectory(equipment)
add_subdirectory(object)
add_subdirectory(social)
add_subdirectory(simulation)
add_subdirectory(sui)
add_subdirectory(terrain)
add_subdirectory(utilities)
add_subdirectory(weather)

add_custom_target(
    swgpy ALL
    DEPENDS
        swgpy_app swgpy_command swgpy_combat swgpy_object swgpy_social swgpy_simulation swgpy_utility
    COMMAND python "${CMAKE_SOURCE_DIR}/tools/swgpy/init_modules.py"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$\(Configuration\)/swgpy"
    VERBATIM
)
set_target_properties(swgpy
	PROPERTIES
	FOLDER "cmake_targets"
)

set_target_properties(swgpy
	PROPERTIES
	FOLDER "cmake_targets"
)
include(ANHExecutable)

AddANHExecutable(swganh
	DEPENDS
		swganh_lib
    ADDITIONAL_INCLUDE_DIRS
        ${Boost_INCLUDE_DIR}
        ${GLM_INCLUDE_DIR}
        ${MYSQL_INCLUDE_DIR}
        ${MYSQLCONNECTORCPP_INCLUDE_DIRS}
        ${PYTHON_INCLUDE_DIR}
        ${TBB_INCLUDE_DIRS}
        ${ZLIB_INCLUDE_DIR}
    ADDITIONAL_LIBRARY_DIRS
        ${Boost_LIBRARY_DIRS}	
    DEBUG_LIBRARIES
        ${Boost_CHRONO_LIBRARY_DEBUG}
        ${Boost_DATE_TIME_LIBRARY_DEBUG}
        ${Boost_LOG_LIBRARY_DEBUG}
        ${Boost_LOG_SETUP_LIBRARY_DEBUG}
        ${Boost_FILESYSTEM_LIBRARY_DEBUG}
        ${Boost_PROGRAM_OPTIONS_LIBRARY_DEBUG}
        ${Boost_PYTHON3_LIBRARY_DEBUG}
        ${Boost_REGEX_LIBRARY_DEBUG}
        ${Boost_SYSTEM_LIBRARY_DEBUG}
        ${Boost_THREAD_LIBRARY_DEBUG}
		${Boost_UNIT_TEST_FRAMEWORK_LIBRARY_DEBUG}
        ${MYSQL_LIBRARY_DEBUG}		
        ${MYSQLCONNECTORCPP_LIBRARY_DEBUG}
        ${PYTHON_LIBRARY}
        ${TBB_DEBUG_LIBRARIES}
        ${ZLIB_LIBRARY_DEBUG}
    OPTIMIZED_LIBRARIES
        ${Boost_CHRONO_LIBRARY_RELEASE}
        ${Boost_DATE_TIME_LIBRARY_RELEASE}
        ${Boost_LOG_LIBRARY_RELEASE}
        ${Boost_LOG_SETUP_LIBRARY_RELEASE}
        ${Boost_FILESYSTEM_LIBRARY_RELEASE}
        ${Boost_PROGRAM_OPTIONS_LIBRARY_RELEASE}
        ${Boost_PYTHON3_LIBRARY_RELEASE}
        ${Boost_REGEX_LIBRARY_RELEASE}
        ${Boost_SYSTEM_LIBRARY_RELEASE}
        ${Boost_THREAD_LIBRARY_RELEASE}
		${Boost_UNIT_TEST_FRAMEWORK_LIBRARY_RELEASE}
        ${MYSQL_LIBRARY_RELEASE}
        ${MYSQLCONNECTORCPP_LIBRARY_RELEASE}
        ${PYTHON_LIBRARY}
        ${TBB_LIBRARIES}
        ${ZLIB_LIBRARY_RELEASE}
)