include_directories( ${ITKPlugins_SOURCE_DIR}/Common )
include_directories( ${ITKPlugins_SOURCE_DIR}/Parser )
include_directories( ${ITKPlugins_SOURCE_DIR}/Generators )

set( PluginGenerator_SRCS
  ../PluginGenerator.h
  ../PluginGenerator.cxx
  )

if ( BUILD_SLICER4_PLUGINS )
  message( STATUS "Building Slicer 4 plugins enabled..." )
  add_subdirectory( Slicer4 )
endif()

if ( BUILD_PARAVIEW3_PLUGINS )
  message( "Building ParaView plugins enabled..." )
  add_subdirectory( ParaView3 )
endif()
