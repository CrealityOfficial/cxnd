__assert_target(trimesh2)

set(SRCS cxnd/interface.h
		 cxnd/algrithm/dumplicate.h
		 cxnd/algrithm/dumplicate.cpp
		 cxnd/algrithm/merge.h
		 cxnd/algrithm/merge.cpp
		 )
		
set(INCS)
set(LIBS trimesh2)
set(DEFS)

if(BUILD_CXND_STATIC)
	__add_real_target(cxnd lib SOURCE ${SRCS}
								  LIB ${LIBS}
								  DEF ${DEFS}
								  INC ${INCS}
								  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
								  SOURCE_FOLDER
								  )
else()
	list(APPEND DEFS CXND_DLL)
	__add_real_target(cxnd dll SOURCE ${SRCS}
								  LIB ${LIBS}
								  DEF ${DEFS}
								  INC ${INCS}
								  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
								  SOURCE_FOLDER
								  )
endif()

