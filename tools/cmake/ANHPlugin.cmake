# AddMMOServerLibrary is a standardized way to build libraries in the MMOServer
# project. Particularly on windows platforms this manages all the
# machinary to set up a default environment and creating/building/running unit
# unit tests an afterthought for developers.
#
# Function Definition:
#
# AddANHPlugin(library_name
#     DEPENDS [ARGS] [args1...]           	     # Dependencies on other MMOServer projects
#     ADDITIONAL_INCLUDE_DIRS [ARGS] [args1...]  # Additional directories to search for includes
#     ADDITIONAL_SOURCE_DIRS [ARGS] [args1...]   # Additional directories to search for files to include in the project
#     DEBUG_LIBRARIES [ARGS] [args1....]         # Additional debug libraries to link the project against
#     OPTIMIZED_LIBRARIES [ARGS] [args1...])     # Additional optimized libraries to link the project against
#
#
########################
# Simple Example Usage:
########################
#
# include(ANHPlugin)
#
# AddANHPlugin(Common
#     MMOSERVER_DEPS
#         Utils
# )
#
#
#########################
# Complex Example Usage:
#########################
# include(ANHPlugin)
#
# AddANHPlugin(ScriptEngine
#     DEPENDS
#         Utils
#         Common
#     SOURCES # disables source lookup and uses this list
#         ${SOURCES}
#     TEST_SOURCES # when source lookups are disabled use these tests
#         ${TEST_SOURCES}
#     ADDITIONAL_SOURCE_DIRS
#         ${CMAKE_CURRENT_SOURCE_DIR}/glue_files
#     ADDITIONAL_INCLUDE_DIRS
#         ${LUA_INCLUDE_DIR}
#         ${NOISE_INCLUDE_DIR}
#         ${TOLUAPP_INCLUDE_DIR}
#     DEBUG_LIBRARIES
#         ${LUA_LIBRARY_DEBUG}
#         ${NOISE_LIBRARY_DEBUG}
#         ${TOLUAPP_LIBRARY_DEBUG}
#     OPTIMIZED_LIBRARIES
#         ${LUA_LIBRARY_RELEASE}
#         ${NOISE_LIBRARY_RELEASE}
#         ${TOLUAPP_LIBRARY_RELEASE}
# )
#

INCLUDE(CMakeMacroParseArguments)

FUNCTION(AddANHPlugin name)
    PARSE_ARGUMENTS(ANHPLUGIN "DEPENDS;SOURCES;TEST_SOURCES;ADDITIONAL_LIBRARY_DIRS;ADDITIONAL_INCLUDE_DIRS;ADDITIONAL_SOURCE_DIRS;DEBUG_LIBRARIES;OPTIMIZED_LIBRARIES" "" ${ARGN})

    LIST(LENGTH SOURCES __source_files_list_length)
    LIST(LENGTH ANHPLUGIN_DEBUG_LIBRARIES _debug_list_length)
    LIST(LENGTH ANHPLUGIN_OPTIMIZED_LIBRARIES _optimized_list_length)
    LIST(LENGTH ANHPLUGIN_DEPENDS _project_deps_list_length)
    LIST(LENGTH ANHPLUGIN_ADDITIONAL_INCLUDE_DIRS _includes_list_length)
    LIST(LENGTH ANHPLUGIN_ADDITIONAL_LIBRARY_DIRS _librarydirs_list_length)
    LIST(LENGTH ANHPLUGIN_ADDITIONAL_SOURCE_DIRS _sources_list_length)

    # Grab all of the source files and all of the unit test files.
    FILE(GLOB_RECURSE SOURCES *.cc *.cpp *.h)
    list(APPEND SOURCES ${ANHPLUGIN_SOURCES})
    FILE(GLOB_RECURSE HEADERS *.h)
    list(APPEND HEADERS ${ANHPLUGIN_HEADERS})
    FILE(GLOB_RECURSE TEST_SOURCES *_unittest.cc *_unittest.cpp mock_*.h)
    list(APPEND TEST_SOURCES ${ANHPLUGIN_TEST_SOURCES})
    FILE(GLOB_RECURSE BINDINGS *_binding.cc *_binding.cpp py_*.h py_*.cc)
    list(APPEND BINDINGS ${ANHPLUGIN_BINDINGS})

    FOREACH(__source_file ${SOURCES})
        STRING(REGEX REPLACE "(${CMAKE_CURRENT_SOURCE_DIR}/)((.*/)*)(.*)" "\\2" __source_dir "${__source_file}")
        STRING(REGEX REPLACE "(${CMAKE_CURRENT_SOURCE_DIR}/${__source_dir})(.*)" "\\2" __source_filename "${__source_file}")

        STRING(REPLACE "/" "\\\\" __source_group "${__source_dir}")
        SOURCE_GROUP("${__source_group}" FILES ${__source_file})

        # check to see if this application specifies an explicit main file
        STRING(SUBSTRING ${__source_filename} 0 5 __main_check)
        STRING(COMPARE EQUAL "main." "${__main_check}" __is_main)
        IF(__is_main)
            SET(MAIN_EXISTS ${__source_file})
        ENDIF()
    ENDFOREACH()

    # if python bindings have been specified generate a module
    LIST(LENGTH BINDINGS _bindings_list_length)
    IF(_bindings_list_length GREATER 0)
        list(REMOVE_ITEM SOURCES ${BINDINGS})
    ENDIF()

    # if unit tests have been specified break out the project into a library to make it testable
    LIST(LENGTH TEST_SOURCES _test_sources_list_length)
    IF(_test_sources_list_length GREATER 0)
        SET(__project_library "${name}_lib")

        list(REMOVE_ITEM SOURCES ${MAIN_EXISTS})

        AddANHLibrary(${name}
            DEPENDS
                ${ANHPLUGIN_DEPENDS}
            SOURCES
                ${SOURCES}
            HEADERS
                ${HEADERS}
            TEST_SOURCES
                ${TEST_SOURCES}
            ADDITIONAL_INCLUDE_DIRS
                ${ANHPLUGIN_ADDITIONAL_INCLUDE_DIRS}
            ADDITIONAL_LIBRARY_DIRS
                ${ANHPLUGIN_ADDITIONAL_LIBRARY_DIRS}
            DEBUG_LIBRARIES
                ${ANHPLUGIN_DEBUG_LIBRARIES}
            OPTIMIZED_LIBRARIES
                ${ANHPLUGIN_OPTIMIZED_LIBRARIES}
        )

        set(SOURCES ${MAIN_EXISTS})
        list(APPEND ANHPLUGIN_DEPENDS ${__project_library})
        list(REVERSE ANHPLUGIN_DEPENDS)
    ENDIF()

    IF(_includes_list_length GREATER 0)
        INCLUDE_DIRECTORIES(${ANHPLUGIN_ADDITIONAL_INCLUDE_DIRS})
    ENDIF()

    IF(_librarydirs_list_length GREATER 0)
        LINK_DIRECTORIES(${ANHPLUGIN_ADDITIONAL_LIBRARY_DIRS})
    ENDIF()

    # Create the Common library
    ADD_LIBRARY(${name} MODULE ${SOURCES})
    SET_TARGET_PROPERTIES(${name} PROPERTIES
        PREFIX ""
	    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugins/
	    LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/plugins
	    LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/plugins
	    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/plugins
	    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/plugins)

    # If a project library was created link to it
    IF(DEFINED __project_library)
        TARGET_LINK_LIBRARIES(${name}
            ${__project_library}
        )
    ENDIF()

    IF(_project_deps_list_length GREATER 0)
        ADD_DEPENDENCIES(${name} ${ANHPLUGIN_DEPENDS})
        ADD_DEPENDENCIES(${name} DEPS)
		TARGET_LINK_LIBRARIES(${name} ${ANHPLUGIN_DEPENDS})
    ENDIF()

    IF(_debug_list_length GREATER 0)
        FOREACH(debug_library ${ANHPLUGIN_DEBUG_LIBRARIES})
            if (NOT ${debug_library} MATCHES ".*NOTFOUND")
                TARGET_LINK_LIBRARIES(${name} debug ${debug_library})
            endif()
        ENDFOREACH()
    ENDIF()

    IF(_optimized_list_length GREATER 0)
        FOREACH(optimized_library ${ANHPLUGIN_OPTIMIZED_LIBRARIES})
            if (NOT ${optimized_library} MATCHES ".*NOTFOUND")
                TARGET_LINK_LIBRARIES(${name} optimized ${optimized_library})
            endif()
        ENDFOREACH()
    ENDIF()

    IF (WIN32)
        # Set the default output directory for binaries for convenience.
        set(RUNTIME_OUTPUT_BASE_DIRECTORY "${CMAKE_BINARY_DIR}")

		# set the default output directory for the shared library for convenience
		SET_TARGET_PROPERTIES(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${RUNTIME_OUTPUT_BASE_DIRECTORY}/bin")
		ADD_DEFINITIONS ( -DDLL_EXPORTS )
    ENDIF()

ENDFUNCTION()
