include_directories( "${ITKPlugins_SOURCE_DIR}/Generators/ParaView3" )
include_directories( "/Users/quammen/dev/build/SimpleITK-build" )

set( GENERATED_CODE_DIRECTORY "${ITKPlugins_BINARY_DIR}/GeneratedCode/ParaView3" )

foreach( PLUGIN_NAME ${Plugin_JSON} )

  add_custom_command( OUTPUT "${GENERATED_CODE_DIRECTORY}/${PLUGIN_NAME}.cxx"
    COMMAND "${ITKPlugins_BINARY_DIR}/bin/GenerateParaView3Plugin"
    "/Users/quammen/dev/packages/SimpleITK/Code/BasicFilters/json/${PLUGIN_NAME}.json"
    "${GENERATED_CODE_DIRECTORY}"
    "--cxx"
    DEPENDS GenerateParaView3Plugin
    WORKING_DIRECTORY "${ITKPlugins_BINARY_DIR}"
    VERBATIM
    )

  add_custom_command( OUTPUT "${GENERATED_CODE_DIRECTORY}/${PLUGIN_NAME}.xml"
    COMMAND "${ITKPlugins_BINARY_DIR}/bin/GenerateParaView3Plugin"
    "/Users/quammen/dev/packages/SimpleITK/Code/BasicFilters/json/${PLUGIN_NAME}.json"
    "${GENERATED_CODE_DIRECTORY}"
    "--xml"
    DEPENDS GenerateParaView3Plugin
    WORKING_DIRECTORY "${ITKPlugins_BINARY_DIR}"
    VERBATIM
    )

  set( XML_DEPENDENCIES "${XML_DEPENDENCIES}"
    "${GENERATED_CODE_DIRECTORY}/${PLUGIN_NAME}.xml"
    )

endforeach()

add_custom_target( ParaViewXML ls
  DEPENDS ${XML_DEPENDENCIES}
  )
