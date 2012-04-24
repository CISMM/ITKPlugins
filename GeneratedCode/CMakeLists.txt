include_directories( "/Users/quammen/dev/packages/SimpleITK/Code/Common/include/" )
include_directories( "/Users/quammen/dev/packages/SimpleITK/Code/BasicFilters/src" )

if ( BUILD_SLICER4_PLUGINS )
  add_subdirectory( Slicer4 )
endif()

if ( BUILD_PARAVIEW3_PLUGINS )
  add_subdirectory( ParaView3 )
endif()
