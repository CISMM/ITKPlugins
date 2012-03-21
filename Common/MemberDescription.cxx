#include "MemberDescription.h"

MemberDescription
::MemberDescription() :
  m_CustomITKCast( "<undefined>" )
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
  os << "ITKType: " << m_ITKType << std::endl;
  os << "CustomITKCast: " << m_CustomITKCast << std::endl;
}
