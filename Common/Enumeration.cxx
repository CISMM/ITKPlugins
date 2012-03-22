#include "Enumeration.h"

#include <iostream>

Enumeration
::Enumeration()
{
}

Enumeration
::~Enumeration()
{
}

void
Enumeration
::PrintSelf( std::ostream & os )
{
  os << "  Enumeration::" << std::endl;
  os << "    Name: " << m_Name << std::endl;
  os << "    Values: " << std::endl;

  for ( size_t i = 0; i < m_Enumerants.size(); ++i )
    {
    os << "      " << m_Enumerants[i].first << " = " << m_Enumerants[i].second << std::endl;
    }
}
