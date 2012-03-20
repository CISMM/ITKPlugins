#include "ClassDescription.h"

#include "MemberDescription.h"

ClassDescription
::ClassDescription()
{

}

ClassDescription
::~ClassDescription()
{

}

void
ClassDescription
::AddMember( MemberDescription * memberDescription )
{
  m_Members.push_back( memberDescription );
}

const MemberDescription *
ClassDescription
::GetMemberDescription( int index )
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
::PrintSelf( std::ostream & os )
{
  os << "PluginName: " << m_PluginName << std::endl;
  os << "ITKClassName: " << m_ITKClassName << std::endl;
  os << "NumberOfInputs: " << m_NumberOfInputs << std::endl;
  os << "BriefDescription: " << m_BriefDescription << std::endl;
  os << "DetailedDescription: " << m_DetailedDescription << std::endl;

  os << "PublicDeclarations:  " << m_PublicDeclarations << std::endl;

  for (unsigned int i = 0; i < m_Members.size(); ++i)
    {
    m_Members[i]->PrintSelf( os );
    }
}
