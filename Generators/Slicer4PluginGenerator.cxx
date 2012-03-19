#include "Slicer4PluginGenerator.h"

#include <fstream>
#include <iostream>

#include "ClassDescription.h"
#include "MemberDescription.h"

Slicer4PluginGenerator
::Slicer4PluginGenerator() :
  m_ClassDescription( NULL )
{
}

Slicer4PluginGenerator
::~Slicer4PluginGenerator()
{
}

void
Slicer4PluginGenerator
::SetClassDescription( ClassDescription * classDescription )
{
  m_ClassDescription = classDescription;
}

ClassDescription *
Slicer4PluginGenerator
::GetClassDescription() const
{
  return m_ClassDescription;
}

void
Slicer4PluginGenerator
::SetPluginPath( const std::string & path )
{
  m_PluginPath = path;
}

const std::string &
Slicer4PluginGenerator
::GetPluginPath() const
{
  return m_PluginPath;
}

bool
Slicer4PluginGenerator
::Generate()
{
  if ( !m_ClassDescription )
    {
    std::cerr << "No ClassDescription set in Slicer4PluginGenerator" << std::endl;
    return false;
    }

  if ( !this->GenerateXMLDescription() )
    {
    std::cerr << "Failed to generate XML description for plugin '" << m_PluginPath << "/"
              << m_ClassDescription->GetClassName() << "'.";
    return false;
    }

  if ( !this->GenerateCode() )
    {
    std::cerr << "Failed to generate code for plugin '" << m_ClassDescription->GetClassName()
              << "/" << m_ClassDescription->GetClassName() << "'.";
    return false;
    }
}

bool
Slicer4PluginGenerator
::GenerateXMLDescription()
{
  // Open file for writing
  std::string filePath( m_PluginPath );
  char lastChar = *( m_PluginPath.rbegin() );
  if ( lastChar == '/' || lastChar == '\\' )
    {
    filePath.append( m_ClassDescription->GetClassName() );
    }
  else
    {
    // Add delimiter
#ifdef WIN32
    filePath.append( "\\" );
#else
    filePath.append( "/" );
#endif
    filePath.append( m_ClassDescription->GetClassName() );
    filePath.append( ".xml" );
    }

  std::cout << "Writing Slicer4 plugin XML file for " << m_ClassDescription->GetClassName()
            << std::endl;

  std::ofstream os( filePath.c_str() );

  os << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
  os << "<executable>\n";

  os << "  <category>ITK</category>\n";
  os << "  <title>" << m_ClassDescription->GetClassName() << "</title>\n";
  os << "  <description>\n";
  os << m_ClassDescription->GetBriefDescription();
  os << "  </description>\n";
  os << "  <version>1.0</version>\n";
  os << "  <documentation-url></documentation-url>\n";
  os << "  <license></license>\n";
  os << "  <contributor>ITK Plugins Project</contributor>\n\n";

  this->WriteXMLParameters( os );

  os << "</executable>\n";

  os.flush();
  os.close();

  return true;
}

void
Slicer4PluginGenerator
::WriteXMLParameters( std::ostream & os )
{
  // IO parameters first
  os << "  <parameters>\n";
  os << "    <label>IO</label>\n";
  os << "    <description>Input/output volumes</description>\n";

  for (int i = 0; i < m_ClassDescription->GetNumberOfInputs(); ++i)
    {
    os << "    <image>\n";
    os << "      <name>inputVolume" << i << "</name>\n";
    os << "      <label>Input Volume " << i << "</label>\n";
    os << "      <channel>input</channel>\n";
    os << "      <index>" << i << "</index>\n";
    os << "      <description></description>\n";
    os << "    </image>\n";
    }
  os << "  </parameters>\n\n";

  // Class parameters second
  os << "  <parameters>\n";
  os << "    <label>" << m_ClassDescription->GetClassName() << " Parameters</label>\n";
  os << "    <description>Parameters for the " << m_ClassDescription->GetClassName()
     << " class in ITK.</description>\n";

  for (int i = 0; i < m_ClassDescription->GetNumberOfMemberDescriptions(); ++i)
    {
    const MemberDescription * member = m_ClassDescription->GetMemberDescription( i );
    std::string typeName = member->GetTypeName();
    if ( typeName == std::string( "double" ) )
      {
      os << "    <double>\n";
      os << "      <name>" << member->GetMemberName() << "</name>\n";
      os << "      <longFlag>" << member->GetMemberName() << "</longFlag>\n";
      //os << "      <description>" << member->GetBriefDescription() << "</description>\n";
      os << "      <label>" << member->GetMemberName() << "</label>\n";
      os << "      <default>" << member->GetDefaultValue() << "</default>\n";
      os << "    </double>\n";
      }
    else
      {
      std::cerr << "Unknown type name '" << typeName << "'." << std::endl;
      }
    }

  os << "  </parameters>\n\n";
}

bool
Slicer4PluginGenerator
::GenerateCode()
{
  // Open file for writing
  std::string filePath( m_PluginPath );
  char lastChar = *( m_PluginPath.rbegin() );
  if ( lastChar == '/' || lastChar == '\\' )
    {
    filePath.append( m_ClassDescription->GetClassName() );
    }
  else
    {
    // Add delimiter
#ifdef WIN32
    filePath.append( "\\" );
#else
    filePath.append( "/" );
#endif
    filePath.append( m_ClassDescription->GetClassName() );
    filePath.append( ".cxx" );
    }

  std::cout << "Writing Slicer4 plugin CXX file for " << m_ClassDescription->GetClassName()
            << std::endl;

  // Useful variables
  std::string filterName( m_ClassDescription->GetClassName() );
  filterName.append( "ImageFilter" );

  std::ofstream os( filePath.c_str() );

  os << "#include \"itkPluginUtilities.h\"\n\n";

  os << "#include <itkImageFileReader.h>\n";
  os << "#include <itkImageFileWriter.h>\n";
  os << "#include <itk" << filterName << ".h>\n\n";

  os << "#include \"" << filterName << "CLP.h\n\n";

  os << "namespace\n";
  os << "{\n\n";

  os << "template< class TInput >\n";
  os << "int Run( int argc, char * argv[], TInput )\n";
  os << "{\n\n";

  os << "  PARSE_ARGS;\n\n";

  os << "  typedef itk::Image< TInput,  3 > InputImageType;\n";
  os << "  typedef itk::Image< TOutput, 3 > OutputImageType;\n\n";

  os << "  typedef itk::ImageFileReader< InputImageType  > InputReaderType;\n";
  os << "  typedef itk::ImageFileWriter< OutputImageType > OutputWriterType;\n\n";

  os << "  inputReader->SetFileName( inputVolume0.c_str() );\n";
  os << "  outputWriter->SetFileName( outputVolume.c_str() );\n\n";

  os << "  typedef itk::" << filterName << "< InputImageType, OutputImageType > FilterType;\n";
  os << "  typename FilterType::Pointer filter = FilterType::New();\n\n";

  os << "  itk::PluginFilterWatcher watcher( filter, \"" << filterName << "\", CLPProcessInformation );\n\n";

  os << "  filter->SetInput( inputReader0->GetOutput() );\n";

  // Set parameters
  for (int i = 0; i < m_ClassDescription->GetNumberOfMemberDescriptions(); ++i)
    {
    const MemberDescription * member = m_ClassDescription->GetMemberDescription( i );
    os << "  filter->Set" << member->GetMemberName() << "( " << member->GetMemberName()
       << ");\n";
    }

  os << "\n";

  os << "  outputWriter->SetInput( filter->GetOutput() );\n";
  os << "  outputWriter->SetUseCompression( 1 );\n";
  os << "  outputWriter->Update();\n\n";

  os << "  return EXIT_SUCCESS;\n";
  os << "}\n\n";

  os << "int main( int argc, char* argv[] )\n";
  os << "{\n\n";

  os << "  PARSE_ARGS;\n\n";

  os << "  itk::ImageIOBase::IOPixelType     pixelType;\n";
  os << "  itk::ImageIOBase::IOComponentType componentType;\n\n";

  os << "  //#include \"RunDispatchLevelOne.h\"\n\n";

  os << "  return EXIT_SUCCESS;\n";
  os << "}\n\n";

  os << "} // end namespace\n\n";


  os.flush();
  os.close();

  return true;
}

