#include "Parser.h"

#include <fstream>

#include <libjson.h>
#include <JSONNode.h>

#include "Enumeration.h"
#include "MemberDescription.h"

Parser
::Parser()
{

}

Parser
::~Parser()
{

}

void
Parser
::SetFileName(const std::string & fileName)
{
  m_FileName = fileName;
}

const std::string &
Parser
::GetFileName() const
{
  return m_FileName;
}

bool
Parser
::Parse(ClassDescription * description)
{
  if ( !description )
    {
    std::cerr << "Null description object!" << std::endl;
    return false;
    }

  if ( m_FileName == "" )
    {
    std::cerr << "Empty file name set in Parser." << std::endl;
    return false;
    }

  // Open up the file and parse with the JSON parser
  std::filebuf fb;
  fb.open( m_FileName.c_str(), std::ios::in );
  std::istream is( &fb );
  std::string jsonString;
  std::string line;
  std::getline( is, line );
  while ( is )
    {
    jsonString += line;
    std::getline(is, line);
    }

  JSONNode* parent = (JSONNode*) json_parse( jsonString.c_str() );

  // Get the class name
  JSONNode::json_iterator iter = parent->find( "name" );
  if ( iter != parent->end() )
    {
    std::string pluginName( (*iter)->as_string().c_str() );
    description->SetPluginName( pluginName );
    }

  iter = parent->find( "template_code_filename" );
  if ( iter != parent->end() )
    {
    std::string templateCodeFileName( (*iter)->as_string().c_str() );
    description->SetTemplateCodeFileName( templateCodeFileName );
    }

  iter = parent->find( "itk_name" );
  if ( iter != parent->end() )
    {
    std::string itkClassName( (*iter)->as_string().c_str() );
    description->SetITKClassName( itkClassName );
    }

  iter = parent->find( "filter_type" );
  if ( iter != parent->end() )
    {
    std::string filterType( (*iter)->as_string().c_str() );
    description->SetFilterType( filterType );
    }

  // Get the number of inputs
  iter = parent->find( "number_of_inputs" );
  if ( iter != parent->end() )
    {
    int numInputs = atoi( (*iter)->as_string().c_str() );
    description->SetNumberOfInputs( numInputs );
    }
  else
    {
    // There always has to be one input for filters
    description->SetNumberOfInputs( 1 );
    }

  // Get documentation
  iter = parent->find( "briefdescription" );
  if ( iter != parent->end() )
    {
    std::string briefDescription( (*iter)->as_string().c_str() );
    description->SetBriefDescription( briefDescription );
    }

  iter = parent->find( "detaileddescription" );
  if ( iter != parent->end() )
    {
    std::string detailedDescription( (*iter)->as_string().c_str() );
    description->SetDetailedDescription( detailedDescription );
    }

  // Public declarations
  iter = parent->find( "public_declarations" );
  if ( iter != parent->end() )
    {
    std::string publicDeclarations( (*iter)->as_string().c_str() );
    description->SetPublicDeclarations( publicDeclarations );

    this->ParseEnums( publicDeclarations, description );
    }

  // Additional include files
  iter = parent->find( "include_files" );
  if ( iter != parent->end() )
    {
    // Iterate through the additional includes
    JSONNode includeArray = (*iter)->as_array();
    JSONNode::json_iterator includeIter = includeArray.begin();
    while ( includeIter != includeArray.end() )
      {
      std::string includeString( (*includeIter)->as_string().c_str() );
      description->AddIncludeFile( includeString );
      includeIter++;
      }
    }

  // Input pixel types
  iter = parent->find( "pixel_types" );
  if ( iter != parent->end() )
    {
    std::string pixelTypes( (*iter)->as_string().c_str() );
    description->SetPixelTypes( pixelTypes );
    }

  // Output pixel type
  iter = parent->find( "output_pixel_type" );
  if ( iter != parent->end() )
    {
    std::string outputPixelType( (*iter)->as_string().c_str() );
    description->SetOutputPixelType( outputPixelType );
    }

  iter = parent->find( "custom_set_input" );
  if ( iter != parent->end() )
    {
    std::string customSetInput( (*iter)->as_string().c_str() );
    description->SetCustomSetInput( customSetInput );
    }

  // Parse the members of the class
  JSONNode::json_iterator members = parent->find( "members" );
  if ( (*members)->type() != JSON_ARRAY )
    {
    std::cerr << "Expected 'members' node to be an array! " << std::endl;
    return false;
    }

  JSONNode memberArray = (*members)->as_array();
  JSONNode::json_iterator memberIter = memberArray.begin();
  while ( memberIter != memberArray.end() )
    {
    JSONNode * memberNode = *memberIter;
    iter = memberNode->find( "name" );
    if ( iter == memberNode->end() )
      {
      std::cerr << "Member with no name found! Skipping." << std::endl;
      memberIter++;
      continue;
      }
    
    MemberDescription *memberDescription = new MemberDescription();
    memberDescription->SetMemberName( std::string( (*iter)->as_string().c_str() ) );

    description->AddMember( memberDescription );

    iter = memberNode->find( "type" );
    if ( iter != memberNode->end() )
      {
      std::string typeName( (*iter)->as_string().c_str() );

      // Check if it's a declared type in the class
      size_t doubleColonPos = typeName.find_last_of( "::" );
      if ( doubleColonPos != std::string::npos && typeName.find( "std::vector" ) == std::string::npos )
        {
        // Declared type. Strip everything but the end
        typeName = typeName.substr( doubleColonPos+1 );
        }
      memberDescription->SetTypeName( typeName );
      }

    iter = memberNode->find( "default" );
    if ( iter != memberNode->end() )
      {
      std::string defaultValue( (*iter)->as_string().c_str() );

      // Check it if's a declared type in the class
      size_t doubleColonPos = defaultValue.find_last_of( "::" );
      if ( doubleColonPos != std::string::npos && defaultValue.find( "std::vector" ) == std::string::npos )
        {
        // Declared type. Strip everything but the end
        defaultValue = defaultValue.substr( doubleColonPos+1 );
        }

      memberDescription->SetDefaultValue( defaultValue );
      }

    iter = memberNode->find( "itk_type" );
    if ( iter != memberNode->end() )
      {
      std::string itkType( (*iter)->as_string().c_str() );
      memberDescription->SetITKType( itkType );
      }

    iter = memberNode->find( "custom_itk_cast" );
    if ( iter != memberNode->end() )
      {
      std::string customITKCast( (*iter)->as_string().c_str() );
      memberDescription->SetCustomITKCast( customITKCast );
      }

    memberIter++;
    }

  return true;
}

void
Parser
::ParseEnums( const std::string & publicDeclarations, ClassDescription * classDescription )
{
  // See if there are any enums in the public declarations
  // If so, parse all of them
  if ( publicDeclarations.find( "enum" ) == std::string::npos )
    {
    return;
    }

  // Iterate over the enum declarations
  std::string remaining( publicDeclarations );

  while ( true )
    {
    size_t typenamePos = remaining.find( "typedef" );
    size_t semicolonPos = remaining.find( ";" );
    if ( typenamePos == std::string::npos || semicolonPos == std::string::npos )
      {
      return;
      }

    std::string enumString = remaining.substr( typenamePos, semicolonPos-typenamePos+1 );
    std::cout << "enumString: " << enumString << std::endl;
    this->ParseOneEnum( enumString, classDescription );
    remaining = remaining.substr( semicolonPos + 1 );
    }
}

void
Parser
::ParseOneEnum( const std::string & enumDeclaration, ClassDescription * classDescription )
{

  // Get the right curly brace position
  size_t rightBracePos = enumDeclaration.find( '}' );
  if ( rightBracePos == std::string::npos )
    {
    std::cerr << "No right curly brace '}' found in enum declaration." << std::endl;
    return;
    }

  // Get the name
  size_t namePos = enumDeclaration.find_first_not_of( ' ', rightBracePos+1 );
  if ( namePos == std::string::npos )
    {
    std::cerr << "No name given for enum" << std::endl;
    return;
    }
  size_t endNamePos = enumDeclaration.find_last_not_of( ';' );
  if ( endNamePos == std::string::npos )
    {
    std::cerr << "Error while getting name of enum" << std::endl;
    return;
    }
  std::string enumName = enumDeclaration.substr( namePos, endNamePos-namePos+1 );
  //std::cout << enumName << std::endl;

  // Get the individual enumerants
  size_t leftBracePos = enumDeclaration.find( '{' );
  if ( leftBracePos == std::string::npos )
    {
    std::cerr << "No left curly brace '{' found in enum declaration." << std::endl;
    return;
    }

  // Create an Enumeration object
  Enumeration * enumeration = new Enumeration();
  enumeration->SetName( enumName );

  // Default value to be used if none is specified in the enum
  // declaration
  int enumerantValue = 0;

  size_t startPos = leftBracePos+1;
  while ( startPos < rightBracePos )
    {
    // Find the start of the enum name
    size_t enumStartPos = enumDeclaration.find_first_not_of( ' ', startPos );
    if ( enumStartPos == rightBracePos )
      {
      break;
      }

    // Get the entire enum substring up to the comma (may include an
    // assigned integer value)
    size_t enumEndPos = enumDeclaration.find_first_of( ',', enumStartPos );
    if ( enumEndPos == std::string::npos )
      {
      // We must be at the last enumerant. Capture up to the right
      // curly brace
      enumEndPos = rightBracePos;
      }
    else
      {
      // Back up one from the comma
      //enumEndPos--;
      }

    // Prepare for next iteration
    startPos = enumEndPos+1;

    // Now process the enumerant string
    std::string enumerantString = enumDeclaration.substr( enumStartPos, enumEndPos-enumStartPos );

    // Trim the enumerant string on the left and right
    size_t begin = enumerantString.find_first_not_of( ' ' );
    size_t end = enumerantString.find_last_not_of( ' ' );
    std::string trimmed = enumerantString.substr( begin, end-begin+1 );

    // Now, split the enumerant string by the equals sign if it is
    // present
    size_t equalSignPos = enumerantString.find( '=' );
    std::string enumerantName;
    if ( equalSignPos != std::string::npos )
      {
      // Parse the name
      size_t enumerantNameEndPos = trimmed.find_last_not_of( ' ', equalSignPos-1 );
      enumerantName = trimmed.substr( 0, enumerantNameEndPos+1 );

      // Parse the value
      size_t valueStartPos = trimmed.find_first_not_of( ' ', equalSignPos+1 );
      std::string valueString = trimmed.substr( valueStartPos );

      enumerantValue = atoi( valueString.c_str() );
      }
    else
      {
      enumerantName = trimmed;
      }

    // Add the enumerant and its value to the enumeration
    enumeration->AddEnumerant( enumerantName, enumerantValue++ );
    }

  // Finally, add the enumeration to the class description
  classDescription->AddEnumeration( enumeration );
}
