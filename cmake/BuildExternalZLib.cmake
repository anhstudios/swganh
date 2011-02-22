# Build the ZLib vendor library

set(zlib_source "${CMAKE_CURRENT_BINARY_DIR}/vendor/zlib")
set(zlib_binary "${CMAKE_CURRENT_BINARY_DIR}/vendor/zlib-build")

if(WIN32)
	set(patch_command del zconf.h)
else()
	set(patch_command rm -f zconf.h)
endif()

ExternalProject_Add(zlib
	GIT_REPOSITORY https://github.com/anhstudios/zlib.git
	GIT_TAG master
	UPDATE_COMMAND ""
	PATCH_COMMAND ${patch_command}
	SOURCE_DIR ${zlib_source}
	BINARY_DIR ${zlib_binary}
	INSTALL_COMMAND ""
)

# Set the path to the built ZLib for use by other projects
set(ZLib_INCLUDE_DIR "${zlib_source}" CACHE PATH "" FORCE)
set(ZLib_LIBRARY_DIRS "${zlib_binary}/lib" CACHE PATH "" FORCE)