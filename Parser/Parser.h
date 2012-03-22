#ifndef __Parser_h_
#define __Parser_h_

#include <string>

#include "ClassDescription.h"

class Parser {
public:
  Parser();
  virtual ~Parser();

  void SetFileName(const std::string & fileName);
  const std::string & GetFileName() const;

  /** Parses the file set through SetFileName() and stores the results
   * in the ClassDescription object passed in. */
  bool Parse(ClassDescription * description);

protected:
  std::string m_FileName;

private:

  void ParseEnums( const std::string & publicDeclarations, ClassDescription * classDescription );
};

#endif
