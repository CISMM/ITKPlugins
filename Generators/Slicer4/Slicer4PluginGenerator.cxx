#include "Slicer4PluginGenerator.h"

#include <fstream>
#include <iostream>
#include <string>

#include "ClassDescription.h"
#include "Enumeration.h"
#include "MemberDescription.h"

Slicer4PluginGenerator
::Slicer4PluginGenerator()
{
}

Slicer4PluginGenerator
::~Slicer4PluginGenerator()
{
}

bool
Slicer4PluginGenerator
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
Slicer4PluginGenerator
::GenerateXML()
{
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
  filePath.append( this->GetClassDescription()->GetPluginName() );
  filePath.append( ".xml" );

  std::cout << "Writing Slicer4 plugin XML file for " << this->GetClassDescription()->GetPluginName()
            << std::endl;

  std::ofstream os( filePath.c_str() );

  os << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
  os << "<executable>\n";

  os << "  <category>ITK</category>\n";
  //os << "  <title>" << this->GetClassDescription()->GetPluginName() <<
  //"</title>\n";
  os << "  <title>" << this->SplitCAMLCaseString( this->GetClassDescription()->GetPluginName() ) << "</title>\n";

  os << "  <description>\n";
  os << this->GetClassDescription()->GetBriefDescription();
  os << "  </description>\n";
  os << "  <version>1.0</version>\n";
  os << "  <documentation-url></documentation-url>\n";
  os << "  <license></license>\n";
  os << "  <contributor>ITK Plugins Project</contributor>\n";

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
  // Class parameters first
  os << "  <parameters>\n";
  os << "    <label>" << this->SplitCAMLCaseString( this->GetClassDescription()->GetPluginName() )
     << " Parameters</label>\n";
  os << "    <description>Parameters for the " << this->GetClassDescription()->GetPluginName()
     << " class in ITK.</description>\n";

  for (int i = 0; i < this->GetClassDescription()->GetNumberOfMemberDescriptions(); ++i)
    {
    const MemberDescription * member = this->GetClassDescription()->GetMemberDescription( i );
    std::string typeName = member->GetTypeName();
    std::string xmlTypeName;
    bool typeKnown = true;

    if ( typeName == "bool" )
      {
      xmlTypeName = "boolean";
      }
    else if ( typeName == "unsigned int" || typeName == "int" ||
              typeName == "uint8_t" || typeName == "uint32_t" || typeName == "uint64_t" )
      {
      xmlTypeName = "integer";
      }
    else if ( typeName == "float" )
      {
      xmlTypeName = "float";
      }
    else if ( typeName == "double" )
      {
      xmlTypeName = "double";
      }
    else if ( typeName == "std::vector<int>" ||
              typeName == "std::vector<unsigned int>" ||
              typeName == "std::vector<uint32_t>" ||
              typeName == "std::vector<bool>" )
      {
      xmlTypeName = "integer-vector";
      }
    else if ( typeName == "std::vector<double>" )
      {
      xmlTypeName = "double-vector";
      }
    else if ( this->GetClassDescription()->IsEnumerationType( typeName ) )
      {
      xmlTypeName = "string-enumeration";
      }
    else
      {
      std::cerr << "Unknown type name '" << typeName << "'." << std::endl;
      typeKnown = false;
      }

    if ( typeKnown )
      {
      os << "    <" << xmlTypeName << ">\n";
      os << "      <name>plugins" << member->GetMemberName() << "</name>\n";
      os << "      <longflag>--" << member->GetMemberName() << "</longflag>\n";
      //os << "      <description>" << member->GetBriefDescription() << "</description>\n";
      os << "      <label>" << this->SplitCAMLCaseString( member->GetMemberName() ) << "</label>\n";
      os << "      <default>";

      std::string defaultValue = member->GetDefaultValue();
      if ( defaultValue.find( "std::vector" ) != std::string::npos )
        {
        size_t pos1 = defaultValue.find( "(" ) + 1;
        size_t pos2 = defaultValue.find( "," );
        std::string vectorLengthString = defaultValue.substr( pos1, pos2-pos1 );
        int vectorLength = atoi( vectorLengthString.c_str() );

        pos1 = defaultValue.find_first_not_of( ' ', pos2+1 );
        pos2 = defaultValue.find( ")" );
        std::string defaultElementString = defaultValue.substr( pos1, pos2-pos1 );

        for ( int i = 0; i < vectorLength-1; ++i )
          {
          os << defaultElementString << ",";
          }
        os << defaultElementString;
        }
      else if ( xmlTypeName == "integer-enumeration" )
        {
        // TODO - replace with value of enum corresponding to
        // enumerant name
        os << 0;
        }
      else
        {
        os << defaultValue;
        }
      os << "</default>\n";

      // Write enumeration possibilities
      if ( xmlTypeName == "string-enumeration" )
        {
        const Enumeration * enumeration = this->GetClassDescription()->GetEnumeration( typeName );
        if ( enumeration != NULL )
          {
          for ( int i = 0; i < enumeration->GetNumberOfEnumerants(); ++i )
            {
            os << "      <element>" << enumeration->GetEnumerantName( i ) << "</element>\n";
            }
          }
        else
          {
          std::cerr << "No enumeration '" << typeName << "' found!" << std::endl;
          }
        }

      os << "    </" << xmlTypeName << ">\n";
      }

    }

  os << "  </parameters>\n";

  // IO parameters second
  os << "  <parameters>\n";
  os << "    <label>IO</label>\n";
  os << "    <description>Input/output volumes</description>\n";

  for (int i = 0; i < this->GetNumberOfInputs(); ++i)
    {
    os << "    <image>\n";
    os << "      <name>inputVolume" << i << "</name>\n";
    os << "      <label>Input Volume " << i << "</label>\n";
    os << "      <channel>input</channel>\n";
    os << "      <index>" << i << "</index>\n";
    os << "      <description></description>\n";
    os << "    </image>\n";
    }

  os << "    <image>\n";
  os << "      <name>outputVolume</name>\n";
  os << "      <label>Output Volume</label>\n";
  os << "      <channel>output</channel>\n";
  os << "      <index>" << this->GetClassDescription()->GetNumberOfInputs()+1 << "</index>\n";
  os << "      <description>Filter output</description>\n";
  os << "    </image>\n";

  os << "  </parameters>\n";
}

bool
Slicer4PluginGenerator
::GenerateCode()
{
  // Open file for writing
  std::string filePath( this->GetPluginPath() );
  char lastChar = *( this->GetPluginPath().rbegin() );
  if ( lastChar != '/' || lastChar != '\\' )
    {
    // Add delimiter
#ifdef WIN32
    filePath.append( "\\" );
#else
    filePath.append( "/" );
#endif
    }
  filePath.append( this->GetClassDescription()->GetPluginName() );
  filePath.append( ".cxx" );

  std::cout << "Writing Slicer4 plugin CXX file for " << this->GetClassDescription()->GetPluginName()
            << std::endl;

  // Useful variables
  std::string filterName( this->GetClassDescription()->GetPluginName() );
  filterName.append( "ImageFilter" );

  // Override the filter name if it is explicit in the JSON file
  if ( this->GetClassDescription()->GetITKClassName() != "" )
    {
    filterName = this->GetClassDescription()->GetITKClassName();
    }

  std::ofstream os( filePath.c_str() );

  os << "#include \"itkPluginUtilities.h\"\n\n";

  os << "#include <tr1/functional>\n";
  os << "#include <tr1/type_traits>\n";

  os << "#include <itkImageFileReader.h>\n";
  os << "#include <itkImageFileWriter.h>\n";
  os << "#include <itk" << filterName << ".h>\n\n";

  // Add additional files requested in the ClassDescription
  for ( int i = 0; i < this->GetClassDescription()->GetNumberOfIncludeFiles(); ++i )
    {
    os << "#include <" << this->GetClassDescription()->GetIncludeFile( i ) << ">\n";
    }

  os << "#include \"" << this->GetClassDescription()->GetPluginName() << "CLP.h\"\n\n";

  if ( !this->WritePixelTypeDefinitions( os ) )
    {
    return false;
    }

  os << "\n\n";

  this->WriteEnumerationCode( os );

  os << "namespace\n";
  os << "{\n\n";

  os << "template< class TInput >\n";
  os << "int Run( int argc, char * argv[], TInput )\n";
  os << "{\n\n";

  os << "  PARSE_ARGS;\n\n";

  os << "  typedef itk::Image< TInput, 2 > InputImageType;\n";
  for ( int i = 1; i < this->GetClassDescription()->GetNumberOfInputs(); ++i )
    {
    os << "  typedef itk::Image< TInput, 2 > InputImageType" << i+1 << ";\n";
    }

  if ( this->GetClassDescription()->GetOutputPixelType() != "" )
    {
    // Custom pixel type
    os << "  typedef itk::Image< " << this->GetClassDescription()->GetOutputPixelType() << ", 2 > OutputImageType;\n";
    }
  else
    {
    // Standard pixel type
    os << "  typedef itk::Image< TInput, 2 > OutputImageType;\n\n";
    }

  for ( int i = 0; i < this->GetClassDescription()->GetNumberOfInputs(); ++i )
    {
    os << "  typedef itk::ImageFileReader< InputImageType  > InputReaderType" << i << ";\n";
    os << "  typename InputReaderType" << i << "::Pointer inputReader" << i
       << " = InputReaderType" << i << "::New();\n";
    os << "  inputReader" << i << "->SetFileName( inputVolume" << i << ".c_str() );\n";
    os << "  inputReader" << i << "->Update();\n";
    os << "  typename InputImageType::Pointer image" << i+1 << " = inputReader" << i << "->GetOutput();\n\n";
    }

  os << "  typedef itk::ImageFileWriter< OutputImageType > OutputWriterType;\n\n";
  os << "  typename OutputWriterType::Pointer outputWriter = OutputWriterType::New();\n";
  os << "  outputWriter->SetFileName( outputVolume.c_str() );\n\n";

  if ( this->GetClassDescription()->GetFilterType() != "" )
    {
    // Custom filter type declaration from JSON file
    os << "  typedef " << this->GetClassDescription()->GetFilterType() << " FilterType;\n";
    }
  else if ( this->GetClassDescription()->GetTemplateCodeFileName() == "KernelImageFilter" )
    {
    // Kernel filtering class of filters
    os << "  typedef itk::" << filterName << "< InputImageType, InputImageType, InputImageType > FilterType;\n";
    }
  else
    {
    // Standard filter type declaration
    os << "  typedef itk::" << filterName << "< ";
    for ( int i = 0; i < this->GetNumberOfInputs(); ++i )
      {
      os << "InputImageType, ";
      }
    os << " OutputImageType > FilterType;\n";
    }

  os << "  typename FilterType::Pointer filter = FilterType::New();\n\n";
  os << "  itk::PluginFilterWatcher watcher( filter, \"" << filterName << "\", CLPProcessInformation );\n\n";

  std::cout << "Number of inputs: " << this->GetClassDescription()->GetCustomSetInput() << std::endl;
  if ( this->GetClassDescription()->GetCustomSetInput() == "<undefined>" )
    {
    for ( int i = 0; i < this->GetClassDescription()->GetNumberOfInputs(); ++i )
      {
      // First input gets special treatment to avoid
      // "Input Primary is required but not set" exception
      if ( i == 0 )
        {
        os << "  filter->SetInput( inputReader" << i << "->GetOutput() );\n";
        }
      else
        {
        os << "  filter->SetInput( " << i << ", inputReader" << i << "->GetOutput() );\n";
        }
      }
    }
  else
    {
    // Custom input code from the JSON file
    std::string code( this->GetClassDescription()->GetCustomSetInput() );
    code = this->SubstituteString( "this->m_", "plugins", code );
    code = this->SubstituteString( "m_", "plugins", code );
    os << "  " << code << "\n";
    }

  // Set parameters
  for (int i = 0; i < this->GetClassDescription()->GetNumberOfMemberDescriptions(); ++i)
    {
    const MemberDescription * member = this->GetClassDescription()->GetMemberDescription( i );

    // If there is custom set code, use it instead of generating our
    // own
    if ( member->GetCustomITKCast() != "<undefined>" && !this->GetClassDescription()->IsEnumerationType( member->GetTypeName() ) )
      {
      std::string code( member->GetCustomITKCast() );
      code = this->SubstituteString( "this->m_", "plugins", code );
      code = this->SubstituteString( "m_", "plugins", code );

      os << "  " << code << "\n";

      continue;
      }

    if ( member->GetTypeName().find( "vector" ) != std::string::npos )
      {
      os << "  " << member->GetITKType() << " tmp" << member->GetMemberName() << ";\n";

      for ( int i = 0; i < 3; ++i )
        {
        os << "  tmp" << member->GetMemberName() << "[" << i << "] = plugins" << member->GetMemberName() << "[" << i << "];\n";
        }
      os << "  filter->Set" << member->GetMemberName() << "( tmp" << member->GetMemberName()
         << " );\n";
      }
    else
      {
      // Non-vector types
      if ( this->GetClassDescription()->IsEnumerationType( member->GetTypeName() ) )
        {
        // Convert from enum name to enum value
        const Enumeration * enumeration = this->GetClassDescription()->GetEnumeration( member->GetTypeName() );
        os << "\n  " << enumeration->GetName() << " tmp" << member->GetMemberName() << ";\n";


        for ( int i = 0; i < enumeration->GetNumberOfEnumerants(); ++i )
          {
          os << "  ";
          if ( i > 0 )
            {
            os << "else ";
            }

          os << "if ( plugins" << member->GetMemberName() << " == \"" << enumeration->GetEnumerantName( i ) << "\" )\n";
          os << "    {\n";
          //os << "    filter->Set" << member->GetMemberName() << "(
          //FilterType::" << enumeration->GetEnumerantName( i ) << "
          //);\n";
          os << "    tmp" << member->GetMemberName() << " = " << enumeration->GetEnumerantName( i ) << ";\n";
          os << "    }\n";
          }
        os << "  else\n";
        os << "    {\n";
        os << "    std::cout << \"Unknown " << enumeration->GetName() << " given\" << std::endl;\n";
        os << "    }\n";

        // Now write out any custom code for the enumeration,
        // modifying the assumed variable names to fit out purposes
        if ( member->GetCustomITKCast() != "<undefined>" )
          {
          std::string code( member->GetCustomITKCast() );
          code = this->SubstituteString( "this->m_", "tmp", code );
          code = this->SubstituteString( "m_", "tmp", code );

          os << "  " << code << "\n";
          }
        }
      else
        {
        os << "  filter->Set" << member->GetMemberName() << "( plugins" << member->GetMemberName()
           << " );\n";
        }
      }
    }

  os << "\n";

  os << "  outputWriter->SetInput( filter->GetOutput() );\n";
  os << "  outputWriter->SetUseCompression( 1 );\n";
  os << "  outputWriter->Update();\n\n";

  os << "  return EXIT_SUCCESS;\n";
  os << "}\n\n";

  os << "} // end namespace\n\n";

  os << "#include \"Slicer4PluginMain.h\"\n";

  os.flush();
  os.close();

  return true;
}

bool
Slicer4PluginGenerator
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

void
Slicer4PluginGenerator
::WriteEnumerationCode( std::ostream & os )
{
  for ( int i = 0; i < this->GetClassDescription()->GetNumberOfEnumerations(); ++i )
    {
    const Enumeration * enumeration = this->GetClassDescription()->GetEnumeration( i );

    os << "typedef enum {\n";
    for ( int j = 0; j < enumeration->GetNumberOfEnumerants(); ++j )
      {
      os << "  " << enumeration->GetEnumerantName( j ) << " = " << enumeration->GetEnumerantValue( j );
      if ( j < enumeration->GetNumberOfEnumerants()-1 )
        {
        os << ",";
        }
      os << "\n";
      }

    os << "} " << enumeration->GetName() << ";\n\n";
    }
}
