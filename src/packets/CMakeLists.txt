include(ANHLibrary)

AddANHLibrary(packets
    DEPENDS 
    	libanh
	DEBUG_LIBRARIES 
	    ${TBB_DEBUG_LIBRARIES}
	OPTIMIZED_LIBRARIES 
	    ${TBB_LIBRARIES}
)