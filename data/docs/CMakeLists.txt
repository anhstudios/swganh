
if(NOT DEFINED SPHINX_THEME)
	set(SPHINX_THEME default)
endif()

if(NOT DEFINED SPHINX_THEME_DIR)
	set(SPHINX_THEME_DIR)
endif()
	
# configured documentation tools and intermediate build results
set(BINARY_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/_build")
	
# Sphinx cache with pickled ReST documents
set(SPHINX_CACHE_DIR "${CMAKE_CURRENT_BINARY_DIR}/_doctrees")

# HTML output directory
set(SPHINX_HTML_DIR "${CMAKE_BINARY_DIR}/docs/html")
	
if(WIN32)
    foreach(configuration ${CMAKE_CONFIGURATION_TYPES})    
        configure_file(
	        "${CMAKE_CURRENT_SOURCE_DIR}/conf.py.in"
	        "${BINARY_BUILD_DIR}/${configuration}/conf.py"
	        @ONLY)
	endforeach()
	
	set(BINARY_BUILD_DIR "${BINARY_BUILD_DIR}/$<CONFIGURATION>")
    set(SPHINX_CACHE_DIR "${SPHINX_CACHE_DIR}/$<CONFIGURATION>")
    set(SPHINX_HTML_DIR "${SPHINX_HTML_DIR}/$<CONFIGURATION>")
else()
    configure_file(
	    "${CMAKE_CURRENT_SOURCE_DIR}/conf.py.in"
	    "${BINARY_BUILD_DIR}/conf.py"
	    @ONLY)    
endif()

add_custom_target(swganh_docs ALL
    ${SPHINX_EXECUTABLE}
	-q -b html
	-c "${BINARY_BUILD_DIR}"
	-d "${SPHINX_CACHE_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}/source"
	"${SPHINX_HTML_DIR}"
	COMMENT "Building HTML documentation with Sphinx"
)

add_dependencies(swganh_docs anh_lib swganh_lib swganh_binding)
