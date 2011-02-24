# Build the Glm vendor library
set(glm_source "${VENDOR_PREFIX}/src/glm")

ExternalProject_Add(Glm
	PREFIX ${VENDOR_PREFIX}	
	GIT_REPOSITORY https://github.com/anhstudios/glm.git
	GIT_TAG d2645c74
	UPDATE_COMMAND ""
	PATCH_COMMAND ""
	INSTALL_COMMAND ""
)

# Set the path to the built ZLib for use by other projects
set(GLM_INCLUDE_DIR "${glm_source}" CACHE PATH "" FORCE)