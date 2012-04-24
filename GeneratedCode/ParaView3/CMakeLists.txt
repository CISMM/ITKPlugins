include_directories( "${ITKPlugins_SOURCE_DIR}/Generators/ParaView3" )
include_directories( "${SimpleITK_DIR}" )
include_directories( "${ITK_INCLUDE_DIRS}" )

set( GENERATED_CODE_DIRECTORY "${ITKPlugins_BINARY_DIR}/GeneratedCode/ParaView3" )

foreach( PLUGIN_NAME ${Plugin_JSON} )

  set( VTK_FILE_PREFIX "vtkITK${PLUGIN_NAME}ImageFilter" )

  # Each plugin has its own directory. I'm not sure if this is needed,
  # but it may solve a problem I'm having with ParaView sometimes not
  # loading plugins.
  set( PLUGIN_DIRECTORY "${GENERATED_CODE_DIRECTORY}/${PLUGIN_NAME}" )
  file( MAKE_DIRECTORY "${PLUGIN_DIRECTORY}" )

  set( GENERATED_SOURCE_FILES "${GENERATED_SOURCE_FILES}"
    "${PLUGIN_DIRECTORY}/${VTK_FILE_PREFIX}.h"
    "${PLUGIN_DIRECTORY}/${VTK_FILE_PREFIX}.cxx"
    "${PLUGIN_DIRECTORY}/${PLUGIN_NAME}.xml"
    )

  # Generate plugin CMakeLists.txt file here
  configure_file( "PluginCMakeLists.txt.in"
    "${PLUGIN_DIRECTORY}/CMakeLists.txt" @ONLY )

  add_subdirectory( "${PLUGIN_DIRECTORY}" "${PLUGIN_DIRECTORY}" )

endforeach()

add_custom_target( ParaViewSource ALL ls
  DEPENDS ${GENERATED_SOURCE_FILES}
  )
