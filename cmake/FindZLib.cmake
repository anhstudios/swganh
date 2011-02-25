FIND_PATH(ZLib_INCLUDE_DIR zlib.h
	PATH
	    $ENV{ZLib_ROOT}
	    ${ZLib_ROOT}
    HINTS
        $ENV{ZLib_ROOT}
        ${ZLib_ROOT}
        $ENV{ZLib_ROOT}/include
        ${ZLib_ROOT}/include
)
MARK_AS_ADVANCED(ZLib_INCLUDE_DIR)

FIND_LIBRARY(ZLib_LIBRARY_DEBUG
    NAMES z zlibd zlibd.lib
    PATH
        $ENV{ZLib_ROOT}
        ${ZLib_ROOT}
    HINTS
	    $ENV{ZLib_ROOT}/lib
	    ${ZLib_ROOT}/lib
        $ENV{ZLib_ROOT}/Debug
        ${ZLib_ROOT}/Debug
)

FIND_LIBRARY(ZLib_LIBRARY_RELEASE
    NAMES z zlib zlib.lib
    PATH
        $ENV{ZLib_ROOT}
        ${ZLib_ROOT}
    HINTS
		$ENV{ZLib_ROOT}/lib
	    ${ZLib_ROOT}/lib
        $ENV{ZLib_ROOT}/Release
        ${ZLib_ROOT}/Release
)
        
IF(ZLib_INCLUDE_DIR)
	if(ZLib_LIBRARY_DEBUG OR ZLib_LIBRARY_RELEASE)
    	SET(ZLib_FOUND TRUE)
	endif()
ELSE()
    message(${ZLib_INCLUDE_DIR})
    message(${ZLib_LIBRARY_DEBUG})
    message(${ZLib_LIBRARY_RELEASE})
ENDIF()

IF(ZLib_FOUND)
    IF (NOT ZLib_FIND_QUIETLY)
        MESSAGE(STATUS "Found ZLib")
    ENDIF()
ELSE()
    IF (ZLib_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "Could not find ZLib")
    ENDIF()
ENDIF()
