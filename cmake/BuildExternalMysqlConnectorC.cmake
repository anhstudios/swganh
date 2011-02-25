# Build the MysqlConnectorC vendor library
set(MysqlConnectorC_ROOT "${VENDOR_PREFIX}/src/MysqlConnectorC-install")
set(MysqlConnectorC_INCLUDE_DIR "${MysqlConnectorC_ROOT}/include/mysql" CACHE PATH "" FORCE)
set(MysqlConnectorC_LIBRARY_DIRS "${MysqlConnectorC_ROOT}/lib/mysql" CACHE PATH "" FORCE)

ExternalProject_Add(MysqlConnectorC
	PREFIX ${VENDOR_PREFIX}	
	GIT_REPOSITORY https://github.com/anhstudios/mysql-connector-c.git
	GIT_TAG d31c0f88
	UPDATE_COMMAND ""
	PATCH_COMMAND ""
	BUILD_IN_SOURCE 1
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX:PATH=${MysqlConnectorC_ROOT}
)
