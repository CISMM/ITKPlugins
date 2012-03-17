#include "Parser.h"

#include <fstream>

#include <libjson.h>
#include <JSONNode.h>

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
    std::string className( (*iter)->as_string().c_str() );
    description->SetClassName( className );
    }

  // Get the number of inputs
  iter = parent->find( "number_of_inputs" );
  if ( iter != parent->end() )
    {
    int numInputs = atoi( (*iter)->as_string().c_str() );
    description->SetNumberOfInputs( numInputs );
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
    if ( iter != memberNode->end() )
      {
      std::cout << (*iter)->as_string().c_str() << std::endl;
      }
    else
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
      std::cout << (*iter)->as_string().c_str() << std::endl;
      }

    iter = memberNode->find( "default" );
    if ( iter != memberNode->end() )
      {
      std::cout << (*iter)->as_string().c_str() << std::endl;
      }

    memberIter++;
    }

  return true;
}
