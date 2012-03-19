if ( BUILD_SLICER_PLUGINS )
  message( "Building Slicer plugins enabled..." )
  add_subdirectory( Slicer )
endif()

if ( BUILD_PARAVIEW_PLUGINS )
  message( "Building ParaView plugins enabled..." )
  add_subdirectory( ParaView )
endif()

set( Generator_SRCS
  GeneratePlugins.cxx
)

if ( BUILD_SLICER_PLUGINS )
  set( Generator_SRCS
    "${Generator_SRCS}"
    Slicer4PluginGenerator.h
    Slicer4PluginGenerator.cxx
    )
endif()

if ( BUILD_PARAVIEW_PLUGINS )
  set( Generator_SRCS
    "${Generator_SRCS}"
    ParaView3PluginGenerator.h
    ParaView3PluginGenerator.cxx
    )
endif()

include_directories( ${ITKPlugins_SOURCE_DIR}/Common )
include_directories( ${ITKPlugins_SOURCE_DIR}/Parser )

# This is the executable that actually generates the plugin code
add_executable( GeneratePlugins ${Generator_SRCS} )
target_link_libraries( GeneratePlugins
  Common
  Parser
  JSON
  )
