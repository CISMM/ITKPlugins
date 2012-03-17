set( libjson_SRC
  internalJSONNode.cpp
  JSON_Base64.cpp
  JSONAllocator.cpp
  JSONChildren.cpp
  JSONDebug.cpp
  JSONIterators.cpp
  JSONMemory.cpp
  JSONNode.cpp
  JSONNode_Mutex.cpp
  JSONPreparse.cpp
  JSONStream.cpp
  JSONValidator.cpp
  JSONWorker.cpp
  JSONWriter.cpp
  libjson.cpp
)

add_library( JSON ${libjson_SRC} )
