include(ANHLibrary)

AddANHLibrary(libanh
	ADDITIONAL_INCLUDE_DIRS
	    ${Boost_INCLUDE_DIR}
	    ${MYSQL_INCLUDE_DIR}
        ${MysqlConnectorCpp_INCLUDE_DIR}
	    ${TBB_INCLUDE_DIRS}
	ADDITIONAL_LINK_DIRS
	    ${Boost_LIBRARY_DIRS}
	    ${TBB_LIBRARY_DIRS}
)
