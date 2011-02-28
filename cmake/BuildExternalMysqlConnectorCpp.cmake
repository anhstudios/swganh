# Build the MysqlConnectorCpp vendor library
set(MysqlConnectorCpp_ROOT "${VENDOR_PREFIX}/src/MysqlConnectorCpp")

ExternalProject_Add(MysqlConnectorCpp
	DEPENDS Boost MysqlConnectorC
	PREFIX ${VENDOR_PREFIX}	
	GIT_REPOSITORY https://github.com/anhstudios/mysql-connector-cpp.git
	GIT_TAG 94c4bf8
	UPDATE_COMMAND ""
	PATCH_COMMAND ""
	BUILD_IN_SOURCE 1
	CMAKE_ARGS
		-DBOOST_INCLUDEDIR:PATH=${Boost_INCLUDEDIR}
		-DMYSQL_DIR:PATH=${MysqlConnectorC_ROOT}
	INSTALL_COMMAND ""
)
