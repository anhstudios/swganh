# Build the MysqlConnectorCpp vendor library
set(MysqlConnectorCpp_ROOT "${VENDOR_PREFIX}/src/MysqlConnectorCpp")

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
		-DMYSQL_LIB_DIR:PATH=${MysqlConnectorC_LIBRARY_DIRS}
		-DMYSQL_INCLUDE_DIR:PATH=${MysqlConnectorC_INCLUDE_DIR}
	INSTALL_COMMAND ""
)
