set( Generator_SRCS
  GenerateParaView3Plugin.cxx
)

set( Generator_SRCS
  "${Generator_SRCS}"
  ParaView3PluginGenerator.h
  ParaView3PluginGenerator.cxx
  )

# This is the executable that actually generates the plugin code for ParaView 3
add_executable( GenerateParaView3Plugin ${Generator_SRCS} )
target_link_libraries( GenerateParaView3Plugin
  Common
  Parser
  JSON
  )
