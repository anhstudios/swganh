# Build the Glm vendor library
set(Glm_INCLUDEDIR "${VENDOR_PREFIX}/src/Glm" CACHE PATH "" FORCE)

ExternalProject_Add(Glm
	PREFIX ${VENDOR_PREFIX}
	GIT_REPOSITORY https://github.com/anhstudios/glm.git
	GIT_TAG d2645c74
	UPDATE_COMMAND ""
	CONFIGURE_COMMAND ""
	BUILD_IN_SOURCE 1
	BUILD_COMMAND ""
	INSTALL_COMMAND ""
)
