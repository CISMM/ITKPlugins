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

void
ClassDescription
::PrintSelf( std::ostream & os )
{
  os << "ClassName: " << m_ClassName << std::endl;
  os << "NumberOfInputs: " << m_NumberOfInputs << std::endl;
  os << "BriefDescription: " << m_BriefDescription << std::endl;
  os << "DetailedDescription: " << m_DetailedDescription << std::endl;

  os << "PublicDeclarations:  " << m_PublicDeclarations << std::endl;

  for (int i = 0; i < m_Members.size(); ++i)
    {
    m_Members[i]->PrintSelf( os );
    }
}
