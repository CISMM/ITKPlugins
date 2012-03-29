#include "MemberDescription.h"

#include <iostream>

MemberDescription
::MemberDescription() :
  m_CustomITKCast( "<undefined>" )
{
}

MemberDescription
::~MemberDescription()
{

}

int
MemberDescription
::GetNumberOfElements() const
{
  int vectorLength = 1;
  std::string defaultValue( this->GetDefaultValue() );
  if ( defaultValue.find( "std::vector" ) != std::string::npos )
    {
    size_t pos1 = defaultValue.find( "(" ) + 1;
    size_t pos2 = defaultValue.find( "," );
    std::string vectorLengthString = defaultValue.substr( pos1, pos2-pos1 );
    vectorLength = atoi( vectorLengthString.c_str() );
    }

  return vectorLength;
}

std::string
MemberDescription
::GetElementDefaultValue() const
{
  std::string defaultValue( this->GetDefaultValue() );
  std::string defaultElementString( defaultValue );
  if ( defaultValue.find( "std::vector" ) != std::string::npos )
    {
    size_t pos1 = defaultValue.find( "(" ) + 1;
    size_t pos2 = defaultValue.find( "," );
    std::string vectorLengthString = defaultValue.substr( pos1, pos2-pos1 );
    int vectorLength = atoi( vectorLengthString.c_str() );

    pos1 = defaultValue.find_first_not_of( ' ', pos2+1 );
    pos2 = defaultValue.find( ")" );
    defaultElementString = defaultValue.substr( pos1, pos2-pos1 );
    }

  return defaultElementString;
}

void
MemberDescription
::PrintSelf( std::ostream & os )
{
  os << "  MemberName: " << m_MemberName << std::endl;
  os << "    TypeName: " << m_TypeName << std::endl;
  os << "    DefaultValue: " << m_DefaultValue << std::endl;
  os << "    ITKType: " << m_ITKType << std::endl;
  os << "    CustomITKCast: " << m_CustomITKCast << std::endl;
}
