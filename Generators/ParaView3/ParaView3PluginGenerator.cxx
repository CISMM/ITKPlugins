#include "ParaView3PluginGenerator.h"

#include <fstream>
#include <iostream>
#include <string>

#include "ClassDescription.h"
#include "Enumeration.h"
#include "MemberDescription.h"

ParaView3PluginGenerator
::ParaView3PluginGenerator()
{
}

ParaView3PluginGenerator
::~ParaView3PluginGenerator()
{
}

bool
ParaView3PluginGenerator
::Generate()
{
  if ( !this->GetClassDescription() )
    {
    std::cerr << "No ClassDescription set in Slicer4PluginGenerator" << std::endl;
    return false;
    }

  if ( !this->GenerateXML() )
    {
    std::cerr << "Failed to generate XML description for plugin '" << this->GetPluginPath() << "/"
              << this->GetClassDescription()->GetPluginName() << "'.";
    return false;
    }

  if ( !this->GenerateCode() )
    {
    std::cerr << "Failed to generate code for plugin '" << this->GetClassDescription()->GetPluginName()
              << "/" << this->GetClassDescription()->GetPluginName() << "'.";
    return false;
    }

  return true;
}

bool
ParaView3PluginGenerator
::GenerateXML()
{
  ClassDescription * classDescription = this->GetClassDescription();

  // Open file for writing
  std::string filePath( this->GetPluginPath() );
  char lastChar = *( this->GetPluginPath().rbegin() );
  if ( lastChar != '/' && lastChar != '\\' )
    {
    // Add delimiter
#ifdef WIN32
    filePath.append( "\\" );
#else
    filePath.append( "/" );
#endif
    }
  filePath.append( classDescription->GetPluginName() );
  filePath.append( ".xml" );

  std::cout << "Writing ParaView3 plugin XML file for " << classDescription->GetPluginName()
            << std::endl;

  std::ofstream os( filePath.c_str() );

  os << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
  os << "<ServerManagerConfiguration>\n";
  os << "  <ProxyGroup name=\"filters\">\n";

  os << "    <SourceProxy name=\"" << classDescription->GetPluginName() << "\" class=\"vtk"
     << classDescription->GetPluginName() << "\" label=\""
     << this->SplitCAMLCaseString( classDescription->GetPluginName() ) << "\">\n";

  os << "      <Documentation\n";
  os << "        long_help=\"\"\n"; // Should put detailed description here
  os << "        short_help=\""
     << this->SubstituteString( "\n", "\\n", classDescription->GetBriefDescription() ) << "\"\n";
  os << "      </Documentation>\n\n";
  
  // Write input descriptions
  for (int i = 0; i < this->GetNumberOfInputs(); ++i)
    {
    os << "      <InputProperty\n";
    os << "         name=\"Input" << i+1 << "\"\n";
    os << "         port_index=\"" << i << "\"\n";
    os << "         command=\"SetInputConnection\">\n";

    os << "        <ProxyGroupDomain name=\"groups\">\n";
    os << "          <Group name=\"sources\"/>\n";
    os << "          <Group name=\"filters\"/>\n";
    os << "        </ProxyGroupDomain>\n";

    os << "        <DataTypeDomain name=\"input_type\">\n";
    os << "          <DataType value=\"vtkImageData\"/>\n";
    os << "        </DataTypeDomain>\n";

    os << "        <Documentation>\n";
    os << "          Set input image " << i+1 << "\n";
    os << "        </Documentation>\n";

    os << "      </InputProperty>\n\n";
    }

  for (int i = 0; i < classDescription->GetNumberOfMemberDescriptions(); ++i)
    {
    const MemberDescription * member = classDescription->GetMemberDescription( i );

    std::string typeName = member->GetTypeName();
    std::string xmlTypeName;
    bool typeKnown = true;
    
    if ( this->IsIntVectorType( typeName ) || classDescription->IsEnumerationType( typeName ) )
      {
      xmlTypeName = "IntVectorProperty";
      }
    else if ( this->IsDoubleVectorType( typeName ) )
      {
      xmlTypeName = "DoubleVectorProperty";
      }
    else
      {
      std::cerr << "Unknown type '" << typeName << "' encountered!" << std::endl;
      return false;
      }

    os << "      <" << xmlTypeName << "\n";
    os << "         name=\"" << member->GetMemberName() << "\"\n";
    os << "         command=\"Set" << member->GetMemberName() << "\"\n";
    int numberOfElements = member->GetNumberOfElements();
    os << "         number_of_elements=\"" << numberOfElements << "\"\n";
    os << "         default_values=\"";

    // Print default values
    std::string elementDefaultValue = member->GetElementDefaultValue();
    for (int i = 0; i < numberOfElements-1; ++i)
      {
      os << elementDefaultValue << " ";
      }
    os << elementDefaultValue << "\">\n";

    if ( this->IsBoolVectorType( typeName ) )
      {
      os << "        <BooleanDomain name=\"bool\"/>\n";
      }

    if ( classDescription->IsEnumerationType( typeName ) )
      {
      os << "        <EnumerationDomain name=\"enum\">\n";
      
      const Enumeration * enumeration = classDescription->GetEnumeration( typeName );
      for (int j = 0; j < enumeration->GetNumberOfEnumerants(); ++j )
        {
        os << "          <Entry value=\"" << enumeration->GetEnumerantValue( j )
           << "\" text=\"" << enumeration->GetEnumerantName( j ) << "\"/>\n";
        }

      os << "        </EnumerationDomain>\n";
      }

    // Finish the XML element
    os << "      </" << xmlTypeName << ">\n\n";

    }

  os << "    </SourceProxy>\n";
  os << "  </ProxyGroup>\n";
  os << "</ServerManagerConfiguration>\n";

  os.flush();
  os.close();

  return true;
}

bool
ParaView3PluginGenerator
::GenerateCode()
{
  return true;
}

