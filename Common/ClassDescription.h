#ifndef __ClassDescription_h_
#define __ClassDescription_h_

#include <iostream>
#include <string>
#include <vector>

class MemberDescription;

/** Describes an ITK class. */
class ClassDescription {
public:
  ClassDescription();
  virtual ~ClassDescription();

  void SetClassName( const std::string & name ) {
    m_ClassName = name;
  }
  const std::string & GetClassName() const {
    return m_ClassName;
  }

  void SetNumberOfInputs( int numInputs ) {
    m_NumberOfInputs = numInputs;
  }
  int GetNumberOfInputs() const {
    return m_NumberOfInputs;
  }

  void SetBriefDescription( const std::string & description ) {
    m_BriefDescription = description;
  }
  const std::string & GetBriefDescription() const {
    return m_BriefDescription;
  }

  void SetDetailedDescription( const std::string & description ) {
    m_DetailedDescription = description;
  }
  const std::string & GetDetailedDescription() const {
    return m_DetailedDescription;
  }

  void SetPublicDeclarations( const std::string & declarations ) {
    m_PublicDeclarations = declarations;
  }
  const std::string & GetPublicDeclarations() const {
    return m_PublicDeclarations;
  }

  void AddMember( MemberDescription * memberDescription );

  void PrintSelf( std::ostream & os );

protected:
  std::string m_ClassName;
  int         m_NumberOfInputs;
  std::string m_BriefDescription;
  std::string m_DetailedDescription;

  std::string m_PublicDeclarations;

  std::vector< MemberDescription *> m_Members;

private:
};

#endif
