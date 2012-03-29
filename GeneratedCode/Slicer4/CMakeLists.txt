include_directories( "${ITKPlugins_SOURCE_DIR}/Generators/Slicer4" )
include_directories( "/Users/quammen/dev/build/SimpleITK-build" )

foreach( PLUGIN_NAME ${Plugin_JSON} )

  # Hack to get slicerMacroBuildCLI to stop causing a fatal error when it can't find
  # the not-yet-generated XML file
  set( GENERATED_CODE_DIRECTORY "${ITKPlugins_BINARY_DIR}/GeneratedCode/Slicer4" )
  file( WRITE "${GENERATED_CODE_DIRECTORY}/${PLUGIN_NAME}.xml" )

    add_custom_command( OUTPUT "${GENERATED_CODE_DIRECTORY}/${PLUGIN_NAME}.cxx"
      COMMAND "${ITKPlugins_BINARY_DIR}/bin/GenerateSlicer4Plugin"
        "/Users/quammen/dev/packages/SimpleITK/Code/BasicFilters/json/${PLUGIN_NAME}.json"
        "${GENERATED_CODE_DIRECTORY}"
        "--cxx"
      DEPENDS GenerateSlicer4Plugin
      WORKING_DIRECTORY "${ITKPlugins_BINARY_DIR}"
      VERBATIM
      )

   add_custom_command( OUTPUT "${GENERATED_CODE_DIRECTORY}/${PLUGIN_NAME}.xml"
      COMMAND "${ITKPlugins_BINARY_DIR}/bin/GenerateSlicer4Plugin"
        "/Users/quammen/dev/packages/SimpleITK/Code/BasicFilters/json/${PLUGIN_NAME}.json"
        "${GENERATED_CODE_DIRECTORY}"
        "--xml"
      DEPENDS GenerateSlicer4Plugin
      WORKING_DIRECTORY "${ITKPlugins_BINARY_DIR}"
      VERBATIM
      )

  slicerMacroBuildCLI(
    NAME "${PLUGIN_NAME}"
    TARGET_LIBRARIES ${ITK_LIBRARIES}
    CLI_XML_FILE
      "${GENERATED_CODE_DIRECTORY}/${PLUGIN_NAME}.xml"
    CLI_SHARED_LIBRARY_WRAPPER_CXX 
      "${GENERATED_CODE_DIRECTORY}/${PLUGIN_NAME}.cxx"
    )

  add_dependencies( "${PLUGIN_NAME}Lib" "ITKPlugins_${PLUGIN_NAME}_CXX" )

  file( REMOVE "${GENERATED_CODE_DIRECTORY}/${PLUGIN_NAME}.xml" )

endforeach()
