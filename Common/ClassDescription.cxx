#include "ClassDescription.h"

#include "Enumeration.h"
#include "MemberDescription.h"

ClassDescription
::ClassDescription() :
  m_NumberOfInputs( 0 )
{
}

ClassDescription
::~ClassDescription()
{
  for ( size_t i = 0; i < m_Enumerations.size(); ++i )
    {
    delete m_Enumerations[i];
    }
  for ( size_t i = 0; i < m_Members.size(); ++i )
    {
    delete m_Members[i];
    }
}

void
ClassDescription
::AddMember( MemberDescription * memberDescription )
{
  m_Members.push_back( memberDescription );
}

const MemberDescription *
ClassDescription
::GetMemberDescription( int index ) const
{
  // No bounds checking
  return m_Members[ index ];
}

int
ClassDescription
::GetNumberOfMemberDescriptions() const
{
  return static_cast< int >( m_Members.size() );
}

void
ClassDescription
::AddEnumeration( Enumeration * enumeration )
{
  m_Enumerations.push_back( enumeration );
}

const Enumeration *
ClassDescription
::GetEnumeration( int index ) const
{
  // No bounds checking
  return m_Enumerations[index];
}

int
ClassDescription
::GetNumberOfEnumerations() const
{
  return static_cast< int >( m_Enumerations.size() );
}

void
ClassDescription
::PrintSelf( std::ostream & os )
{
  os << "PluginName: " << m_PluginName << std::endl;
  os << "ITKClassName: " << m_ITKClassName << std::endl;
  os << "NumberOfInputs: " << m_NumberOfInputs << std::endl;
  os << "BriefDescription: " << m_BriefDescription << std::endl;
  os << "DetailedDescription: " << m_DetailedDescription << std::endl;

  os << "PublicDeclarations:  " << m_PublicDeclarations << std::endl;

  os << "Enumerations: " << std::endl;
  for ( size_t i = 0; i < m_Enumerations.size(); ++i)
    {
    m_Enumerations[i]->PrintSelf( os );
    }

  os << "Members: " << std::endl;
  for ( size_t i = 0; i < m_Members.size(); ++i)
    {
    m_Members[i]->PrintSelf( os );
    }

}
