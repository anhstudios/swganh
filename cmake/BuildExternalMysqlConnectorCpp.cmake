# Build the MysqlConnectorCpp vendor library

set(mysql_cpp_source "${CMAKE_CURRENT_SOURCE_DIR}/vendor/mysql-connector-cpp")
set(mysql_cpp_binary "${CMAKE_CURRENT_BINARY_DIR}/vendor/mysql-connector-cpp-build")

ExternalProject_Add(mysql-connector-cpp
	DEPENDS boost mysql-connector-c
	DOWNLOAD_DIR ${mysql_cpp_binary}
	URL ${mysql_cpp_source}
	UPDATE_COMMAND ""
	PATCH_COMMAND ""
	SOURCE_DIR ${mysql_cpp_binary}
	BINARY_DIR ${mysql_cpp_binary}
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