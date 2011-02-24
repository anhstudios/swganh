# Build the ZLib vendor library
set(zlib_source "${VENDOR_PREFIX}/src/zlib")

if(WIN32)
	set(patch_command del zconf.h)
else()
	set(patch_command rm -f zconf.h)
endif()

ExternalProject_Add(ZLib
	PREFIX ${VENDOR_PREFIX}	
	GIT_REPOSITORY https://github.com/anhstudios/zlib.git
	GIT_TAG f3c9192b
	UPDATE_COMMAND ""
	PATCH_COMMAND ${patch_command}
	INSTALL_COMMAND ""
)

# Set the path to the built ZLib for use by other projects
set(ZLib_INCLUDE_DIR "${zlib_source}" CACHE PATH "" FORCE)
set(ZLib_LIBRARY_DIRS "${VENDOR_PREFIX}/src/zlib-build/$(Configuration)" CACHE PATH "" FORCE)