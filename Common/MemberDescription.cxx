#include "MemberDescription.h"

MemberDescription
::MemberDescription()
{

}

MemberDescription
::~MemberDescription()
{

}

void
MemberDescription
::PrintSelf( std::ostream & os )
{
  os << "MemberName: " << m_MemberName << std::endl;
  os << "TypeName: " << m_TypeName << std::endl;
  os << "DefaultValue: " << m_DefaultValue << std::endl;
}
