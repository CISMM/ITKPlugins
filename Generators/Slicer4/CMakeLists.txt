set( Slicer4Generator_SRCS
  GenerateSlicer4Plugin.cxx
  Slicer4PluginGenerator.h
  Slicer4PluginGenerator.cxx
  )

# This is the executable that actually generates the plugin code for Slicer 4
add_executable( GenerateSlicer4Plugin
  ${Slicer4Generator_SRCS}
  ${PluginGenerator_SRCS}
  )

target_link_libraries( GenerateSlicer4Plugin
  Common
  Parser
  JSON
  )
