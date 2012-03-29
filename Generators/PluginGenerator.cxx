#include "PluginGenerator.h"

#include <fstream>
#include <iostream>
#include <string>

#include "ClassDescription.h"
#include "Enumeration.h"
#include "MemberDescription.h"

PluginGenerator
::PluginGenerator() :
  m_ClassDescription( NULL )
{
}

PluginGenerator
::~PluginGenerator()
{
}

void
PluginGenerator
::SetClassDescription( ClassDescription * classDescription )
{
  m_ClassDescription = classDescription;
}

ClassDescription *
PluginGenerator
::GetClassDescription() const
{
  return m_ClassDescription;
}

void
PluginGenerator
::SetPluginPath( const std::string & path )
{
  m_PluginPath = path;
}

const std::string &
PluginGenerator
::GetPluginPath() const
{
  return m_PluginPath;
}

std::string
PluginGenerator
::SplitCAMLCaseString( const std::string & input )
{
  size_t inputSize = input.size();
  std::string outputString( input.substr( 0, 1) );
  bool previousCapital = true;
  for ( size_t i = 1; i < inputSize; ++i )
    {
    if ( this->IsCapitalLetter( input[i] ) )
      {
      if ( !previousCapital )
        {
        outputString.append( " " );
        }
      previousCapital = true;
      }
    else
      {
      previousCapital = false;
      }

    outputString.append( input.substr( i, 1 ) );
    }

    return outputString;
}

std::string
PluginGenerator
::SubstituteString( const std::string & toFind, const std::string & toReplace,
                    const std::string & input )
{
  std::string output( input );
  size_t nextPos = input.find( toFind );
  while ( nextPos != std::string::npos )
    {
    output.replace( nextPos, toFind.size(), toReplace );
    nextPos = output.find( toFind );
    }

  return output;
}

int
PluginGenerator
::GetNumberOfInputs()
{
  if ( !this->GetClassDescription() )
    {
    return 0;
    }

  int numberOfInputs = this->GetClassDescription()->GetNumberOfInputs();

  // We assume filters need at least one input
  if ( numberOfInputs < 1 ) numberOfInputs++;

  return numberOfInputs;
}
