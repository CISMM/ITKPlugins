#ifndef __Slicer4PluginGenerator_h_
#define __Slicer4PluginGenerator_h_

#include <string>

#include "PluginGenerator.h"

class ClassDescription;
class MemberDescription;

class Slicer4PluginGenerator : public PluginGenerator {
public:
  Slicer4PluginGenerator();
  virtual ~Slicer4PluginGenerator();

  bool Generate();

  bool GenerateXML();
  void WriteXMLParameters( std::ostream & os );

  bool GenerateCode();

protected:

private:
  bool WritePixelTypeDefinitions( std::ostream & os );

  void WriteEnumerationCode( std::ostream & os );

};

#endif

