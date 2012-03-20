set( Generator_SRCS
  GenerateSlicer4Plugin.cxx
)

set( Generator_SRCS
  "${Generator_SRCS}"
  Slicer4PluginGenerator.h
  Slicer4PluginGenerator.cxx
  )

# This is the executable that actually generates the plugin code for Slicer 4
add_executable( GenerateSlicer4Plugin ${Generator_SRCS} )
target_link_libraries( GenerateSlicer4Plugin
  Common
  Parser
  JSON
  )
