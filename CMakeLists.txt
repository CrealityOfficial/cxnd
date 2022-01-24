__assert_target(trimesh2)

set(SRCS cxnd/interface.h
		 cxnd/algrithm/dumplicate.h
		 cxnd/algrithm/dumplicate.cpp
		 cxnd/algrithm/merge.h
		 cxnd/algrithm/merge.cpp
		 cxnd/algrithm/a3d.h
		 cxnd/algrithm/a3d.cpp
		 cxnd/algrithm/modelextruder.h
		 cxnd/algrithm/modelextruder.cpp
		 cxnd/algrithm/raymesh.h
		 cxnd/algrithm/raymesh.cpp
		 cxnd/algrithm/rtsmesh.h
		 cxnd/algrithm/rtsmesh.cpp
		 cxnd/struct/triangleaabbnode.h
		 cxnd/struct/triangleaabbnode.cpp
		 cxnd/struct/triangleaabbtree.h
		 cxnd/struct/triangleaabbtree.cpp
		 cxnd/struct/ray.h
		 cxnd/struct/ray.cpp
		 cxnd/scene/snode.h
		 cxnd/scene/snode.cpp
		 cxnd/scene/camera.h
		 cxnd/scene/camera.cpp
		 cxnd/scene/cameraspace.h
		 cxnd/scene/cameraspace.cpp
		 cxnd/scene/cameracontrols.h
		 cxnd/scene/cameracontrols.cpp
		 cxnd/scene/orbitcontrols.h
		 cxnd/scene/orbitcontrols.cpp
		 cxnd/scene/pickable.h
		 cxnd/scene/pickable.cpp
		 cxnd/scene/selector.h
		 cxnd/scene/selector.cpp
		 cxnd/scene/eventhandlers.h
		 cxnd/scene/eventhandlers.cpp
		 cxnd/scene/clickeventchecker.h
		 cxnd/scene/clickeventchecker.cpp
		 cxnd/scene/eventdistributer.h
		 cxnd/scene/eventdistributer.cpp
		 cxnd/scene/idbase.h
		 cxnd/scene/idbase.cpp
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

if(CC_INSTALL_CXND)
	__install_directory_specif(${CMAKE_CURRENT_SOURCE_DIR}/ cxnd)
endif()

