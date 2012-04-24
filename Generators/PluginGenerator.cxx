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
    nextPos = output.find( toFind, nextPos + toReplace.size() );
    }

  return output;
}


bool
PluginGenerator
::WritePixelTypeDefinitions( std::ostream & os )
{
  bool ucharType  = false;
  bool charType   = false;
  bool ushortType = false;
  bool shortType  = false;
  bool uintType   = false;
  bool intType    = false;
  bool ulongType  = false;
  bool longType   = false;
  bool floatType  = false;
  bool doubleType = false;
  bool complexFloatType  = false;
  bool complexDoubleType = false;

  std::string pixelTypes = this->GetClassDescription()->GetPixelTypes();
  if ( pixelTypes == "BasicPixelIDTypeList" ||
       pixelTypes == "typelist::Append<BasicPixelIDTypeList, ComplexPixelIDTypeList>::Type" ||
       pixelTypes == "NonLabelPixelIDTypeList" )
    {
    ucharType  = true;
    charType   = true;
    ushortType = true;
    shortType  = true;
    uintType   = true;
    intType    = true;
    ulongType  = true;
    longType   = true;
    floatType  = true;
    doubleType = true;
    }
  else if ( pixelTypes == "ComplexPixelIDTypeList" ||
            pixelTypes == "typelist::Append<BasicPixelIDTypeList, ComplexPixelIDTypeList>::Type" ||
            pixelTypes == "NonLabelPixelIDTypeList" )
    {
    complexFloatType  = true;
    complexDoubleType = true;
    }
  else if ( pixelTypes == "IntegerPixelIDTypeList" )
    {
    ucharType  = true;
    charType   = true;
    ushortType = true;
    shortType  = true;
    uintType   = true;
    intType    = true;
    ulongType  = true;
    longType   = true;
    }
  else if ( pixelTypes ==  "LabelPixelIDTypeList" )
    {
    std::cerr << "Unsupported pixel type list 'LabelPixelIDTypeList'" << std::endl;
    return false;
    }
  else if ( pixelTypes == "NonLabelPixelIDTypeList" )
    {
    std::cerr << "Unsupported pixel type list 'NonLabelPixelIDTypeList'" << std::endl;
    return false;
    }
  else if ( pixelTypes == "RealPixelIDTypeList" )
    {
    floatType  = true;
    doubleType = true;
    }
  else if ( pixelTypes == "RealVectorPixelIDTypeList" )
    {
    std::cerr << "Unsupported pixel type list 'RealVectorPixelIDTypeList'" << std::endl;
    return false;
    }
  else if ( pixelTypes == "ScalarPixelIDTypeList" )
    {
    std::cerr << "Unsupported pixel type list 'ScalarPixelIDTypeList'" << std::endl;
    return false;
    }
  else if ( pixelTypes == "VectorPixelIDTypeList" )
    {
    std::cerr << "Unsupported pixel type list 'VectorPixelIDTypeList'" << std::endl;
    return false;
    }
  else if ( pixelTypes == "typelist" )
    {
    std::cerr << "Unsupported pixel type list 'typelist'" << std::endl;
    return false;
    }
  else
    {
    std::cerr << "Unrecognized pixel type list '" << pixelTypes << std::endl;
    return false;
    }

  if ( ucharType )  os << "#define ITK_UCHAR_TYPE\n";
  if ( charType )   os << "#define ITK_CHAR_TYPE\n";
  if ( ushortType)  os << "#define ITK_USHORT_TYPE\n";
  if ( shortType )  os << "#define ITK_SHORT_TYPE\n";
  if ( uintType )   os << "#define ITK_UINT_TYPE\n";
  if ( intType )    os << "#define ITK_INT_TYPE\n";
  if ( ulongType )  os << "#define ITK_ULONG_TYPE\n";
  if ( longType )   os << "#define ITK_LONG_TYPE\n";
  if ( floatType )  os << "#define ITK_FLOAT_TYPE\n";
  if ( doubleType ) os << "#define ITK_DOUBLE_TYPE\n";
  if ( complexFloatType )  os << "#define ITK_COMPLEX_FLOAT_TYPE\n";
  if ( complexDoubleType ) os << "#define ITK_COMPLEX_DOUBLE_TYPE\n";

  return true;
}

bool
PluginGenerator
::InsertFile( std::ostream & os, const std::string & fileName )
{
  // Attempt to open the file name
  std::filebuf fb;
  if ( fb.open( fileName.c_str(), std::ios::in ) )
    {
    std::istream is( &fb );

    // Read the file contents and send to the ostream
    std::string line;
    std::getline( is, line );
    while ( is )
      {
      os << "  " << line << "\n";
      std::getline(is, line);
      }

    return true;
    }

  return false;
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
