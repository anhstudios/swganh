# Build the Boost vendor library

set(__boost_source "${CMAKE_CURRENT_BINARY_DIR}/boost")
set(__boost_binary "${CMAKE_CURRENT_BINARY_DIR}/boost_build")

if(MSVC)
	set(__boost_lib_args
		-DENABLE_SHARED:BOOL=OFF
		-DENABLE_STATIC:BOOL=ON
	)
else()	
	set(__boost_lib_args
		-DENABLE_SHARED:BOOL=ON
		-DENABLE_STATIC:BOOL=OFF
	)
endif()

