include_directories( "${SimpleITK_SOURCE_DIR}/Code/Common/include/" )
include_directories( "${SimpleITK_SOURCE_DIR}/Code/BasicFilters/src" )

if ( BUILD_SLICER4_PLUGINS )
  add_subdirectory( Slicer4 )
endif()

if ( BUILD_PARAVIEW3_PLUGINS )
  add_subdirectory( ParaView3 )
endif()
