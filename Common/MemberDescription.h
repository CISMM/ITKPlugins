#ifndef __MemberDescription_h_
#define __MemberDescription_h_

#include <string>

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

  void SetITKType( const std::string & itkType ) {
    m_ITKType = itkType;
  }
  const std::string & GetITKType() const {
    return m_ITKType;
  }

  void SetCustomITKCast( const std::string & customITKCast ) {
    m_CustomITKCast = customITKCast;
  }
  const std::string & GetCustomITKCast() const {
    return m_CustomITKCast;
  }

  int GetNumberOfElements() const;

  std::string GetElementDefaultValue() const;

  void PrintSelf( std::ostream & os );

protected:

private:
  std::string m_MemberName;
  std::string m_TypeName;
  std::string m_DefaultValue;
  std::string m_ITKType;
  std::string m_CustomITKCast;
};

#endif
