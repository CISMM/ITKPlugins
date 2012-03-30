include_directories( "${ITKPlugins_SOURCE_DIR}/Generators/ParaView3" )
include_directories( "/Users/quammen/dev/build/SimpleITK-build" )

set( GENERATED_CODE_DIRECTORY "${ITKPlugins_BINARY_DIR}/GeneratedCode/ParaView3" )

foreach( PLUGIN_NAME ${Plugin_JSON} )

  set( VTK_FILE_PREFIX "vtkITK${PLUGIN_NAME}ImageFilter" )

  add_custom_command(
    OUTPUT
      "${GENERATED_CODE_DIRECTORY}/${VTK_FILE_PREFIX}.h"
      "${GENERATED_CODE_DIRECTORY}/${VTK_FILE_PREFIX}.cxx"
      "${GENERATED_CODE_DIRECTORY}/${PLUGIN_NAME}.xml"
    COMMAND
      "${ITKPlugins_BINARY_DIR}/bin/GenerateParaView3Plugin"
      "/Users/quammen/dev/packages/SimpleITK/Code/BasicFilters/json/${PLUGIN_NAME}.json"
      "${GENERATED_CODE_DIRECTORY}"
    DEPENDS GenerateParaView3Plugin
    WORKING_DIRECTORY "${ITKPlugins_BINARY_DIR}"
    VERBATIM
    )

  set( GENERATED_SOURCE_FILES "${GENERATED_SOURCE_FILES}"
    "${GENERATED_CODE_DIRECTORY}/${VTK_FILE_PREFIX}.h"
    "${GENERATED_CODE_DIRECTORY}/${VTK_FILE_PREFIX}.cxx"
    "${GENERATED_CODE_DIRECTORY}/${PLUGIN_NAME}.xml"
    )

  add_paraview_plugin( ${PLUGIN_NAME} "1.0"
    SERVER_MANAGER_XML "${GENERATED_CODE_DIRECTORY}/${PLUGIN_NAME}.xml"
    SERVER_MANAGER_SOURCES
      "${GENERATED_CODE_DIRECTORY}/${VTK_FILE_PREFIX}.cxx"
      vtkITKImageFilter.h
      vtkITKImageFilter.cxx
    )

  target_link_libraries( ${PLUGIN_NAME}
    ${ITK_LIBRARIES}
    )

endforeach()

add_custom_target( ParaViewSource ALL ls
  DEPENDS ${GENERATED_SOURCE_FILES}
  )
