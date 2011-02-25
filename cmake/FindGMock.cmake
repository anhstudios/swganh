FIND_PATH(GMock_INCLUDE_DIR gmock/gmock.h
	PATH
	    $ENV{GMock_ROOT}
	    ${GMock_ROOT}
    HINTS
        $ENV{GMock_ROOT}/include
        ${GMock_ROOT}/include
)
MARK_AS_ADVANCED(GMock_INCLUDE_DIR)

FIND_LIBRARY(GMock_LIBRARY_DEBUG
    NAMES gmock gmock.lib
    PATH
        $ENV{GMock_ROOT}
        ${GMock_ROOT}
    HINTS
	    $ENV{GMock_ROOT}/lib
	    ${GMock_ROOT}/lib
        $ENV{GMock_ROOT}/Debug
        ${GMock_ROOT}/Debug
)

FIND_LIBRARY(GMock_LIBRARY_RELEASE
    NAMES gmock gmock.lib
    PATH
        $ENV{GMock_ROOT}
        ${GMock_ROOT}
    HINTS
		$ENV{GMock_ROOT}/lib
	    ${GMock_ROOT}/lib
        $ENV{GMock_ROOT}/Release
        ${GMock_ROOT}/Release
)
        
IF(GMock_INCLUDE_DIR)
	if (GMock_LIBRARY_DEBUG OR GMock_LIBRARY_RELEASE)
    	SET(GMock_FOUND TRUE)
	endif()
ENDIF()

IF(GMock_FOUND)
    IF (NOT GMock_FIND_QUIETLY)
        MESSAGE(STATUS "Found GMock")
    ENDIF()
ELSE()
    IF (GMock_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "Could not find GMock")
    ENDIF()
ENDIF()