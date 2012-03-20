project( ITKPlugins )

cmake_minimum_required( VERSION 2.8 )

# Choose the target applications for which the plugins should be built
option( BUILD_SLICER4_PLUGINS "Build plugins for Slicer 4.0" OFF )
option( BUILD_PARAVIEW3_PLUGINS "Build plugins for ParaView 3.14" OFF )

set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ITKPlugins_BINARY_DIR}/lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ITKPlugins_BINARY_DIR}/bin )

#######################################
# Quiet warnings from the JSON library.
#######################################
if ( CMAKE_BUILD_TYPE STREQUAL "Debug" )
  add_definitions( -DJSON_DEBUG )
endif()
if ( CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" )
  add_definitions( -DNDEBUG )
endif()

add_subdirectory( Common )
add_subdirectory( Generators )
add_subdirectory( Parser )
add_subdirectory( Utilities )

# Generate plugin code
set( Plugin_JSON
  AddConstantTo
  DivideByConstant
)

if ( BUILD_SLICER4_PLUGINS )

  set( EXTENSION_NAME ITKPluginsExtension )
  find_package( Slicer REQUIRED )
  include( ${Slicer_USE_FILE} )

endif()

add_subdirectory( GeneratedCode )
