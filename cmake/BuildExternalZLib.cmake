# Build the ZLib vendor library
set(zlib_source "${CMAKE_CURRENT_SOURCE_DIR}/vendor/zlib")

if(WIN32)
	set(patch_command del zconf.h)
else()
	set(patch_command rm -f zconf.h)
endif()

ExternalProject_Add(zlib
	PREFIX ${VENDOR_PREFIX}
	URL ${zlib_source}
	UPDATE_COMMAND ""
	PATCH_COMMAND ${patch_command}
	INSTALL_COMMAND ""
)

# Set the path to the built ZLib for use by other projects
set(ZLib_INCLUDE_DIR "${zlib_source}" CACHE PATH "" FORCE)
set(ZLib_LIBRARY_DIRS "${zlib_binary}/lib" CACHE PATH "" FORCE)