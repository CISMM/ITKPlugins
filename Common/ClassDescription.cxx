#include "ClassDescription.h"

#include "Enumeration.h"
#include "MemberDescription.h"

ClassDescription
::ClassDescription() :
  m_NumberOfInputs( 0 ),
  m_CustomSetInput( "<undefined>" )
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

const Enumeration *
ClassDescription
::GetEnumeration( const std::string & name ) const
{
  for ( int i = 0; i < this->GetNumberOfEnumerations(); ++i )
    {
    if ( m_Enumerations[i]->GetName() == name )
      {
      return m_Enumerations[i];
      }
    }

  return NULL;
}

int
ClassDescription
::GetNumberOfEnumerations() const
{
  return static_cast< int >( m_Enumerations.size() );
}

bool
ClassDescription
::IsEnumerationType( const std::string & name )
{
  for ( int i = 0; i < this->GetNumberOfEnumerations(); ++i )
    {
    if ( this->GetEnumeration( i )->GetName() == name )
      {
      return true;
      }
    }

  return false;
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
