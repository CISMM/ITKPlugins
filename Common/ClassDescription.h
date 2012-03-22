#ifndef __ClassDescription_h_
#define __ClassDescription_h_

#include <iostream>
#include <string>
#include <vector>

class MemberDescription;
class Enumeration;

/** Describes an ITK class. */
class ClassDescription {
public:
  ClassDescription();
  virtual ~ClassDescription();

  void SetPluginName( const std::string & name ) {
    m_PluginName = name;
  }
  const std::string & GetPluginName() const {
    return m_PluginName;
  }

  void SetITKClassName( const std::string & name ) {
    m_ITKClassName = name;
  }
  const std::string & GetITKClassName() const {
    return m_ITKClassName;
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

  void SetPixelTypes( const std::string & pixelTypes ) {
    m_PixelTypes = pixelTypes;
  }
  const std::string & GetPixelTypes() const {
    return m_PixelTypes;
  }

  void SetOutputPixelType( const std::string & outputType ) {
    m_OutputPixelType = outputType;
  }
  const std::string & GetOutputPixelType() const {
    return m_OutputPixelType;
  }

  void AddMember( MemberDescription * memberDescription );
  const MemberDescription * GetMemberDescription( int index ) const;

  int GetNumberOfMemberDescriptions() const;

  void AddEnumeration( Enumeration * enumeration );
  const Enumeration * GetEnumeration( int index ) const;

  int GetNumberOfEnumerations() const;

  void PrintSelf( std::ostream & os );

protected:
  std::string m_PluginName;
  std::string m_ITKClassName;
  int         m_NumberOfInputs;
  std::string m_BriefDescription;
  std::string m_DetailedDescription;

  std::string m_PublicDeclarations;
  std::string m_PixelTypes;
  std::string m_OutputPixelType;

  std::vector< Enumeration * >      m_Enumerations;
  std::vector< MemberDescription *> m_Members;

private:
};

#endif
