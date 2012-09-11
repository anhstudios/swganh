include(ANHPythonBinding)

AddANHPythonBinding(utility
    PREFIX_NAME swgpy
    DEPENDS
        swganh_lib	
    ADDITIONAL_INCLUDE_DIRS
        ${Boost_INCLUDE_DIR}
        ${GLM_INCLUDE_DIR}
        ${PYTHON_INCLUDE_DIR}
    ADDITIONAL_LIBRARY_DIRS
        ${Boost_LIBRARY_DIRS}
    DEBUG_LIBRARIES 
        ${Boost_LOG_LIBRARY_DEBUG}
        ${Boost_PYTHON3_LIBRARY_DEBUG}
        ${Boost_REGEX_LIBRARY_DEBUG}
        ${Boost_SYSTEM_LIBRARY_DEBUG}
        ${Boost_THREAD_LIBRARY_DEBUG}
        ${PYTHON_LIBRARY}
	OPTIMIZED_LIBRARIES 
        ${Boost_LOG_LIBRARY_RELEASE}
        ${Boost_PYTHON3_LIBRARY_RELEASE}
        ${Boost_REGEX_LIBRARY_RELEASE}
        ${Boost_SYSTEM_LIBRARY_RELEASE}
        ${Boost_THREAD_LIBRARY_RELEASE}
        ${PYTHON_LIBRARY}
)
