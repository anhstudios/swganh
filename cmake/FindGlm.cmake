FIND_PATH(Glm_INCLUDE_DIR glm/glm.hpp
    PATH
        $ENV{Glm_ROOT}
        ${Glm_ROOT}
    HINTS
        $ENV{Glm_ROOT}
        ${Glm_ROOT}
        $ENV{Glm_ROOT}/include
        ${Glm_ROOT}/include
)
MARK_AS_ADVANCED(Glm_INCLUDE_DIR)

IF(Glm_INCLUDE_DIR)
    SET(Glm_FOUND TRUE)
ENDIF()

IF(Glm_FOUND)
    IF (NOT Glm_FIND_QUIETLY)
        MESSAGE(STATUS "Found Glm")
    ENDIF()
ELSE()
    IF (Glm_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "Could not find Glm")
    ENDIF()
ENDIF()