__files_group(${CMAKE_CURRENT_SOURCE_DIR}/cxnd/ SRCS)
		
set(INCS)
set(BUILD_SHARED_LIBS)
set(DEFS)

__add_common_library(cxnd)

if(CC_INSTALL_CXND)
	__install_directory_specif(${CMAKE_CURRENT_SOURCE_DIR}/cxnd cxnd)
endif()

