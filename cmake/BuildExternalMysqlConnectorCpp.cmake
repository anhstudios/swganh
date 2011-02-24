# Build the MysqlConnectorCpp vendor library

set(mysql_cpp_source "${VENDOR_PREFIX}/src/mysql-connector-cpp")

ExternalProject_Add(MysqlConnectorCpp
	DEPENDS Boost MysqlConnectorC
	PREFIX ${VENDOR_PREFIX}	
	GIT_REPOSITORY https://github.com/anhstudios/mysql-connector-cpp.git
	GIT_TAG 329bbc49
	UPDATE_COMMAND ""
	PATCH_COMMAND ""
	BUILD_IN_SOURCE 1
	CMAKE_ARGS
		-DBOOST_INCLUDEDIR:PATH=${Boost_INCLUDE_DIR}
		-DBOOST_LIBRARYDIR:PATH=${Boost_LIBRARY_DIRS}
		-DMYSQL_LIB:PATH=${MysqlConnectorC_LIBRARY_DIRS}
		-DMYSQL_INCLUDE_DIR:PATH=${MysqlConnectorC_INCLUDE_DIR}
	INSTALL_COMMAND ""
)


# Set the path to the built MysqlConnectorCpp for use by other projects
set(MysqlConnectorCpp_INCLUDE_DIR "${mysql_cpp_binary}/include" CACHE PATH "" FORCE)
set(MysqlConnectorCpp_LIBRARY_DIRS "${mysql_cpp_binary}/lib" CACHE PATH "" FORCE)