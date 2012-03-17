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
  os << m_MemberName << std::endl;
}
