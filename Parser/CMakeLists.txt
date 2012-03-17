set( Parser_SRCS
  Parser.h
  Parser.cxx
)

include_directories(
  ${ITKPlugins_SOURCE_DIR}/Common
  ${ITKPlugins_SOURCE_DIR}/Utilities/ExternalLibs/libjson
  )

add_library( Parser ${Parser_SRCS} )

