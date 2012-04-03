#ifndef __PluginGenerator_h_
#define __PluginGenerator_h_

#include <string>

class ClassDescription;

class PluginGenerator {
public:
  PluginGenerator();
  virtual ~PluginGenerator();

  void SetClassDescription( ClassDescription * classDescription );
  ClassDescription * GetClassDescription() const;

  void SetPluginPath( const std::string & path );
  const std::string & GetPluginPath() const;

  

protected:
  int GetNumberOfInputs();

  // Splits a string of text in CAML case by inserting a space before
  // every capital letter except the first and except for groups of
  // capital letters
  std::string SplitCAMLCaseString( const std::string & input );

  bool IsCapitalLetter( char letter ) {
    return letter >= 'A' && letter <= 'Z';
  }

  std::string SubstituteString( const std::string & toFind, const std::string & toReplace,
                                const std::string & input );

  bool WritePixelTypeDefinitions( std::ostream & os );

private:
  ClassDescription * m_ClassDescription;
  std::string        m_PluginPath;

};

#endif
