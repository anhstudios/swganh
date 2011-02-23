# Build the MysqlConnectorCpp vendor library

set(mysql_cpp_source "${CMAKE_CURRENT_SOURCE_DIR}/vendor/mysql-connector-cpp")

ExternalProject_Add(mysql-connector-cpp
	DEPENDS boost mysql-connector-c
	PREFIX ${VENDOR_PREFIX}
	URL ${mysql_cpp_source}
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