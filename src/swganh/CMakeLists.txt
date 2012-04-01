#python directories
add_subdirectory(app)
add_subdirectory(object)
add_subdirectory(combat)
add_subdirectory(social)
add_subdirectory(simulation)

add_custom_target(
	swgpy
	DEPENDS
		swgpy_app_binding swgpy_object_binding swgpy_combat_binding swgpy_simulation_binding swgpy_social_binding
    COMMAND python "${CMAKE_SOURCE_DIR}/tools/swgpy/init_modules.py"
	WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$\(Configuration\)/swgpy"
    VERBATIM
)
include(ANHExecutable)

AddANHExecutable(swganh
    DEPENDS
        anh_lib
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
        ${MYSQL_LIBRARY_RELEASE}
        ${MYSQLCONNECTORCPP_LIBRARY_RELEASE}
        ${PYTHON_LIBRARY}
        ${TBB_LIBRARIES}
        ${ZLIB_LIBRARY_RELEASE}
)
