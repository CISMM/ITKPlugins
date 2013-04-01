include_directories( "${ITKPlugins_SOURCE_DIR}/Generators/ParaView3" )
include_directories( ${ITK_INCLUDE_DIRS} )

set( GENERATED_CODE_DIRECTORY "${ITKPlugins_BINARY_DIR}/GeneratedCode/ParaView3" )

foreach( PLUGIN_NAME ${Plugin_JSON} )

  set( VTK_FILE_PREFIX "vtkITK${PLUGIN_NAME}" )

  # Each plugin has its own directory. I'm not sure if this is needed,
  # but it may solve a problem I'm having with ParaView sometimes not
  # loading plugins.
  set( PLUGIN_DIRECTORY "${GENERATED_CODE_DIRECTORY}/${PLUGIN_NAME}" )
  file( MAKE_DIRECTORY "${PLUGIN_DIRECTORY}" )

  # Generate plugin CMakeLists.txt file here
  configure_file( "PluginCMakeLists.txt.in"
    "${PLUGIN_DIRECTORY}/CMakeLists.txt" @ONLY )
  configure_file( "ITKPluginsGUI.xml.in"
    "${PLUGIN_DIRECTORY}/ITKPluginsGUI.xml"
    )

  add_subdirectory( "${PLUGIN_DIRECTORY}" "${PLUGIN_DIRECTORY}" )

endforeach()
