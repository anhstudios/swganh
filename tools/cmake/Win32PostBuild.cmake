
string(REPLACE "/" "\\" WIN_PROJECT_BINARY_DIR "${CMAKE_BINARY_DIR}")
string(REPLACE "/" "\\" WIN_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}")

## create a target that gathers up the necessary dll's from the above
## dependencies and places them in the runtime directory for this project
add_custom_target(DEPS)

set_target_properties(DEPS
	PROPERTIES
	FOLDER "cmake_targets"
)

get_filename_component(BOOST_PYTHON_DLL_PATH ${Boost_PYTHON3_LIBRARY_DEBUG} PATH)
string(REPLACE "/" "\\" BOOST_PYTHON_DLL_PATH "${BOOST_PYTHON_DLL_PATH}")
add_custom_command(
    TARGET DEPS
    POST_BUILD
    COMMAND xcopy "${BOOST_PYTHON_DLL_PATH}\\*-gd-*.dll" "${WIN_PROJECT_BINARY_DIR}\\bin\\Debug" /D /I /Y /s
    COMMAND xcopy "${BOOST_PYTHON_DLL_PATH}\\*-mt-1*.dll" "${WIN_PROJECT_BINARY_DIR}\\bin\\Release" /D /I /Y /s
    VERBATIM
)

get_filename_component(MYSQL_CONNECTOR_C_DLL_PATH ${MYSQL_LIB} PATH)
string(REPLACE "/" "\\" MYSQL_CONNECTOR_C_DLL_PATH "${MYSQL_CONNECTOR_C_DLL_PATH}\\..")
add_custom_command(
    TARGET DEPS
    POST_BUILD
    COMMAND xcopy "${MYSQL_CONNECTOR_C_DLL_PATH}\\$\(Configuration\)\\*.dll" "${WIN_PROJECT_BINARY_DIR}\\bin\\$\(Configuration\)" /D /I /Y /s
    VERBATIM
)

get_filename_component(MYSQL_CONNECTOR_CPP_DLL_PATH ${MYSQLCONNECTORCPP_LIBRARY_DEBUG} PATH)
string(REPLACE "/" "\\" MYSQL_CONNECTOR_CPP_DLL_PATH "${MYSQL_CONNECTOR_CPP_DLL_PATH}\\..")
add_custom_command(
    TARGET DEPS
    POST_BUILD
    COMMAND xcopy "${MYSQL_CONNECTOR_CPP_DLL_PATH}\\$\(Configuration\)\\*.dll" "${WIN_PROJECT_BINARY_DIR}\\bin\\$\(Configuration\)" /D /I /Y /s
    VERBATIM
)

get_filename_component(PYTHON_DLL_PATH ${PYTHON_LIBRARY} PATH)
string(REPLACE "/" "\\" PYTHON_DLL_PATH "${PYTHON_DLL_PATH}\\..\\DLLs")
add_custom_command(
    TARGET DEPS
    POST_BUILD
    COMMAND xcopy "${PYTHON_DLL_PATH}\\python3.dll" "${WIN_PROJECT_BINARY_DIR}\\bin\\$\(Configuration\)" /D /I /Y /s
    VERBATIM
)

get_filename_component(ZLIB_DLL_PATH ${ZLIB_LIBRARY_DEBUG} PATH)
string(REPLACE "/" "\\" ZLIB_DLL_PATH "${ZLIB_DLL_PATH}\\..")
add_custom_command(
    TARGET DEPS
    POST_BUILD
    COMMAND xcopy "${ZLIB_DLL_PATH}\\$\(Configuration\)\\*.dll" "${WIN_PROJECT_BINARY_DIR}\\bin\\$\(Configuration\)" /D /I /Y /s
    VERBATIM
)

foreach(configuration ${CMAKE_CONFIGURATION_TYPES})
    if(NOT EXISTS "${PROJECT_BINARY_DIR}/bin/${configuration}/config/swganh.cfg")
    configure_file(
        "${PROJECT_SOURCE_DIR}/data/config/swganh.cfg.in"
        "${PROJECT_BINARY_DIR}/bin/${configuration}/config/swganh.cfg"
        @ONLY)
    endif()
endforeach()

add_custom_command(
    TARGET DEPS
    POST_BUILD
    COMMAND for /R \"${WIN_PROJECT_SOURCE_DIR}\\plugins\\\" %%a IN \(*.cfg\) do \(xcopy \"%%a\" \"${WIN_PROJECT_BINARY_DIR}\\bin\\$\(Configuration\)\\config\\plugins\\\" /D /I /Y\)
)

add_custom_command(
    TARGET DEPS
    POST_BUILD
    COMMAND for /R \"${WIN_PROJECT_SOURCE_DIR}\\src\\\" %%a IN \(*.cfg\) do \(xcopy \"%%a\" \"${WIN_PROJECT_BINARY_DIR}\\bin\\$\(Configuration\)\\config\\plugins\\\" /D /I /Y\)
)
