#ifndef __ParaView3PlugingGenerator_h_
#define __ParaView3PlugingGenerator_h_

#include <string>

#include "PluginGenerator.h"

class ParaView3PluginGenerator : public PluginGenerator {
public:
  ParaView3PluginGenerator();
  virtual ~ParaView3PluginGenerator();

  bool Generate();

  bool GenerateXML();
  bool GenerateCode();

protected:

private:
  bool IsBoolVectorType( const std::string & typeName ) {
    return ( typeName == "bool" || typeName == "std::vector<bool>" );
  }

  bool IsIntVectorType( const std::string & typeName ) {
    return ( typeName == "bool" || typeName == "int" || typeName == "unsigned int" ||
             typeName == "int" || typeName == "uint8_t" || typeName == "uint32_t" ||
             typeName == "uint64_t" || typeName == "std::vector<int>" ||
             typeName == "std::vector<unsigned int>" || typeName == "std::vector<uint32_t>" ||
             typeName == "std::vector<bool>" );
  }
  
  bool IsDoubleVectorType( const std::string & typeName ) {
    return ( typeName == "double" || typeName == "std::vector<double>" );
  }

};

#endif
