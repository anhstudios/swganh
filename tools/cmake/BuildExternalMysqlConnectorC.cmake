# Build the MysqlConnectorC vendor library

set(MysqlConnectorC_ROOT "${VENDOR_PREFIX}/src/MysqlConnectorC")

list(APPEND vendor_args
	"-DMysqlConnectorC_ROOT:PATH=${MysqlConnectorC_ROOT}")
	
ExternalProject_Add(MysqlConnectorC
	PREFIX ${VENDOR_PREFIX}	
	GIT_REPOSITORY https://github.com/anhstudios/mysql-connector-c.git
	GIT_TAG d31c0f88
	UPDATE_COMMAND ""
	PATCH_COMMAND ""
	BUILD_IN_SOURCE 1
	INSTALL_COMMAND ""
)
