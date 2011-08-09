# Build the ZLib vendor library
set(ZLib_SOURCEDIR "${VENDOR_PREFIX}/src/ZLib")
set(ZLib_INCLUDEDIR "${ZLib_SOURCEDIR}")
set(ZLib_LIBRARYDIR "${ZLib_SOURCEDIR}")

list(APPEND vendor_args
	"-DZLib_INCLUDEDIR:PATH=${ZLib_INCLUDEDIR}")
list(APPEND vendor_args
	"-DZLib_LIBRARYDIR:PATH=${ZLib_LIBRARYDIR}")
	
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
	BUILD_IN_SOURCE 1
	INSTALL_COMMAND ""
)
