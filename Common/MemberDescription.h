#ifndef __MemberDescription_h_
#define __MemberDescription_h_

#include <string>
#include <iostream>

class MemberDescription {
public:
  MemberDescription();
  virtual ~MemberDescription();

  void SetMemberName( const std::string & memberName ) {
    m_MemberName = memberName;
  }
  const std::string & GetMemberName() const {
    return m_MemberName;
  }

  void SetTypeName( const std::string & typeName ) {
    m_TypeName = typeName;
  }
  const std::string & GetTypeName() const {
    return m_TypeName;
  }

  void SetDefaultValue( const std::string & defaultValue ) {
    m_DefaultValue = defaultValue;
  }
  const std::string & GetDefaultValue() const {
    return m_DefaultValue;
  }

  void PrintSelf( std::ostream & os );

protected:

private:
  std::string m_MemberName;
  std::string m_TypeName;
  std::string m_DefaultValue;
};

#endif
