FIND_PATH(GLog_INCLUDE_DIR glog/logging.h
    PATH
        $ENV{GLog_ROOT}
        ${GLog_ROOT}
    HINTS
        $ENV{GLog_ROOT}/include
        ${GLog_ROOT}/include
        $ENV{GLog_ROOT}/src/windows
        ${GLog_ROOT}/src/windows
)
MARK_AS_ADVANCED(GLog_INCLUDE_DIR)

FIND_LIBRARY(GLog_LIBRARY_DEBUG
    NAMES GLog glog libglog glog.lib libglog.lib
    PATH
        $ENV{GLog_ROOT}
        ${GLog_ROOT}
    HINTS
	    $ENV{GLog_ROOT}/lib
	    ${GLog_ROOT}/lib
        $ENV{GLog_ROOT}/Debug
        ${GLog_ROOT}/Debug
)

FIND_LIBRARY(GLog_LIBRARY_RELEASE
    NAMES GLog glog libglog glog.lib libglog.lib
    PATH
        $ENV{GLog_ROOT}
        ${GLog_ROOT}
    HINTS
		$ENV{GLog_ROOT}/lib
		${GLog_ROOT}/lib
        $ENV{GLog_ROOT}/Release
        ${GLog_ROOT}/Release
)

IF(GLog_INCLUDE_DIR)
	if(GLog_LIBRARY_DEBUG OR GLog_LIBRARY_RELEASE)
    	SET(GLog_FOUND TRUE)
	endif()
ENDIF()

IF(GLog_FOUND)
    IF (NOT GLog_FIND_QUIETLY)
        MESSAGE(STATUS "Found GLog")
    ENDIF()
ELSE()
    IF (GLog_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "Could not find GLog")
    ENDIF()
ENDIF()
