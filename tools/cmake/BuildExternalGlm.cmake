# Build the Glm vendor library
set(Glm_INCLUDEDIR "${VENDOR_PREFIX}/src/Glm" CACHE PATH "" FORCE)

list(APPEND vendor_args
	"-DGlm_INCLUDEDIR:PATH=${Glm_INCLUDEDIR}")
	
ExternalProject_Add(Glm
	PREFIX ${VENDOR_PREFIX}
	GIT_REPOSITORY https://github.com/anhstudios/glm.git
	GIT_TAG 1f3bec5b
	UPDATE_COMMAND ""
	CONFIGURE_COMMAND ""
	BUILD_IN_SOURCE 1
	BUILD_COMMAND ""
	INSTALL_COMMAND ""
)
