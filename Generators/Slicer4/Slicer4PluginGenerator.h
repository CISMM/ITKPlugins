#ifndef __Slicer4PluginGenerator_h_
#define __Slicer4PluginGenerator_h_

#include <string>

class ClassDescription;

class Slicer4PluginGenerator {
public:
  Slicer4PluginGenerator();
  virtual ~Slicer4PluginGenerator();

  void SetClassDescription( ClassDescription * classDescription );
  ClassDescription * GetClassDescription() const;

  void SetPluginPath( const std::string & path );
  const std::string & GetPluginPath() const;

  bool Generate();

  bool GenerateXML();
  void WriteXMLParameters( std::ostream & os );

  bool GenerateCode();

protected:

private:
  ClassDescription * m_ClassDescription;
  
  std::string m_PluginPath;

  bool WritePixelTypeDefinitions( std::ostream & os );

  int GetNumberOfInputs();
};

#endif

