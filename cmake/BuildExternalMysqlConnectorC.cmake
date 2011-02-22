# Build the MysqlConnectorC vendor library

set(mysql_c_source "${CMAKE_CURRENT_SOURCE_DIR}/vendor/mysql-connector-c")
set(mysql_c_build "${CMAKE_CURRENT_BINARY_DIR}/vendor/mysql-connector-c-build")
set(mysql_c_binary "${CMAKE_CURRENT_BINARY_DIR}/vendor/mysql-connector-c")

if(WIN32)
	set(MysqlConnectorC_CONFIGEXE_PATH "${mysql_c_binary}/bin/mysql_config")
else()
	
endif()

ExternalProject_Add(mysql-connector-c
	DOWNLOAD_COMMAND ""
	UPDATE_COMMAND ""
	PATCH_COMMAND ""
	SOURCE_DIR ${mysql_c_source}
	BINARY_DIR ${mysql_c_build}
	CMAKE_ARGS
    	-DCMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG:PATH=${mysql_c_binary}/../mysql-connector-c/lib
    	-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=${mysql_c_binary}/../mysql-connector-c/lib
    	-DCMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE:PATH=${mysql_c_binary}/../mysql-connector-c/lib
    	-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=${mysql_c_binary}/../mysql-connector-c/lib
		-DCMAKE_INSTALL_PREFIX:PATH=${mysql_c_binary}/../mysql-connector-c
)

# Set the path to the built MysqlConnectorC for use by other projects
set(MysqlConnectorC_INCLUDE_DIR "${mysql_c_binary}/include/mysql" CACHE PATH "" FORCE)
set(MysqlConnectorC_LIBRARY_DIRS "${mysql_c_binary}/lib/mysql" CACHE PATH "" FORCE)
