__cc_find(Trimesh2)
__assert_target(trimesh2)
__cc_find(Boost)
__assert_target(boost_nowide)

__files_group(${CMAKE_CURRENT_SOURCE_DIR}/cxnd/ SRCS)
		
set(INCS)
set(ILIBS trimesh2 boost_nowide)
set(DEFS)

__add_common_library(cxnd)

if(CC_INSTALL_CXND)
	__install_directory_specif(${CMAKE_CURRENT_SOURCE_DIR}/ cxnd)
endif()

