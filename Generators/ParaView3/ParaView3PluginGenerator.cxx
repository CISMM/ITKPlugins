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

  os << "    <SourceProxy name=\"" << classDescription->GetPluginName() << "\" class=\"vtkITK"
     << classDescription->GetPluginName() << "ImageFilter\" label=\""
     << this->SplitCAMLCaseString( classDescription->GetPluginName() ) << "\">\n";

  os << "      <Documentation\n";
  os << "        long_help=\"\"\n"; // Should put detailed description here
  os << "        short_help=\""
     << this->SubstituteString( "\n", "\\n", classDescription->GetBriefDescription() ) << "\">\n";
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
    for (int i = 0; i < numberOfElements; ++i)
      {
      if ( elementDefaultValue == "true" )
        {
        os << "1";
        }
      else if ( elementDefaultValue == "false" )
        {
        os << "0";
        }
      else
        {
        os << elementDefaultValue;
        }
      if ( i < numberOfElements-1 )
        {
        os << " ";
        }
      else
        {
        os << "\">\n";
        }
      }

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
  bool success = this->GenerateHeaderCode();
  if ( !success )
    {
    return false;
    }

  success = this->GenerateCXXCode();
  if ( !success )
    {
    return false;
    }

  return true;
}

bool
ParaView3PluginGenerator
::GenerateHeaderCode()
{
  // Useful variables
  ClassDescription * classDescription = this->GetClassDescription();
  std::string itkClassName( this->GetITKClassName() );
  std::string vtkClassName( this->GetVTKClassName() );

  // Open file for writing
  std::string headerFilePath( this->GetPluginPath() );
  char lastChar = *( this->GetPluginPath().rbegin() );
  if ( lastChar != '/' || lastChar != '\\' )
    {
    // Add delimiter
#ifdef WIN32
    headerFilePath.append( "\\" );
#else
    headerFilePath.append( "/" );
#endif
    }
  headerFilePath.append( vtkClassName );
  headerFilePath.append( ".h" );

  std::ofstream os( headerFilePath.c_str() );

  os << "#ifndef __vtk" << classDescription->GetPluginName() << "_h_\n";
  os << "#define __vtk" << classDescription->GetPluginName() << "_h_\n\n";

  os << "#include \"vtkITKImageFilter.h\"\n\n";

  os << "class vtkImageExport;\n";
  os << "class vtkImageImport;\n\n";

  os << "class VTK_EXPORT " << vtkClassName << " : public vtkITKImageFilter\n";
  os << "{\n";
  os << "public:\n";
  os << "  static " << vtkClassName << "* New();\n";
  os << "  vtkTypeMacro(" << vtkClassName << ", vtkITKImageFilter);\n\n";

  // Write members
  for (int i = 0; i < classDescription->GetNumberOfMemberDescriptions(); ++i)
    {
    const MemberDescription * member = classDescription->GetMemberDescription( i );

    if ( classDescription->IsEnumerationType( member->GetTypeName() ) )
      {
      // Write enumeration
      const Enumeration * enumeration = classDescription->GetEnumeration( member->GetTypeName() );
      os << "  typedef enum {\n";
      int j = 0;
      for (j = 0; j < enumeration->GetNumberOfEnumerants()-1; ++j)
        {
        os << "    " << enumeration->GetEnumerantName( j ) << " = "
           << enumeration->GetEnumerantValue(j ) << ",\n";
        }
      os << "    " << enumeration->GetEnumerantName( j ) << " = "
         << enumeration->GetEnumerantValue( j ) << "\n";
      os << "  } " << member->GetTypeName() << ";\n\n";
      }
    
    if ( member->GetNumberOfElements() == 1 )
      {
      os << "  // Set/get " << member->GetMemberName() << " variable\n";
      os << "  vtkSetMacro(" << member->GetMemberName() << ", " 
         << this->GetVTKTypeName( member->GetTypeName() ) << ");\n";
      os << "  vtkGetMacro(" << member->GetMemberName() << ", "
         << this->GetVTKTypeName( member->GetTypeName() ) << ");\n\n";
      }
    else
      {
      os << "  // Set/get " << member->GetMemberName() << " variable\n";
      os << "  vtkSetVectorMacro(" << member->GetMemberName() << ", " 
         << this->GetVTKTypeName( member->GetTypeName() )
         << ", " << member->GetNumberOfElements() << ");\n";
      os << "  vtkGetVectorMacro(" << member->GetMemberName() << ", "
         << this->GetVTKTypeName( member->GetElementTypeName() )
         << ", " << member->GetNumberOfElements() << ");\n\n";
      }

    }

  // Protected members
  os << "protected:\n";
  os << "  " << vtkClassName << "();\n";
  os << "  virtual ~" << vtkClassName << "();\n\n";

  os << "  virtual int FillInputPortInformation(int port, vtkInformation *info);\n\n";

  os << "  virtual int RequestUpdateExtent(vtkInformation *, vtkInformationVector **, vtkInformationVector *);\n\n";

  os << "  void PrintSelf(ostream& os, vtkIndent indent);\n\n";

  // Private members
  os << "private:\n";

  os << "  " << vtkClassName << "(const " << vtkClassName << "&); // Purposely not implemented.\n";
  os << "  void operator=(const " << vtkClassName << "&); // Purposely not implemented.\n\n";

  // Run method declaration
  os << "  template< class TInputImage >\n";
  os << "  void Run(TInputImage * input);\n\n";

  for (int i = 0; i < classDescription->GetNumberOfMemberDescriptions(); ++i)
    {
    const MemberDescription * member = classDescription->GetMemberDescription( i );

    if ( member->GetNumberOfElements() == 1 )
      {
      os << "  " << this->GetVTKTypeName( member->GetTypeName() ) << " " << member->GetMemberName() << ";\n\n";
      }
    else
      {
      os << "  " << this->GetVTKTypeName( member->GetTypeName() ) << " " << member->GetMemberName() << "["
         << member->GetNumberOfElements() << "];\n\n";
      }
    }

  os << "};\n\n";

  os << "#endif\n";

  os.flush();
  os.close();

  return true;
}

bool
ParaView3PluginGenerator
::GenerateCXXCode()
{
  // Useful variables
  const ClassDescription * classDescription = this->GetClassDescription();
  std::string itkClassName( this->GetITKClassName() );
  std::string vtkClassName( this->GetVTKClassName() );

  // Open file for writing
  std::string headerFilePath( this->GetPluginPath() );
  char lastChar = *( this->GetPluginPath().rbegin() );
  if ( lastChar != '/' || lastChar != '\\' )
    {
    // Add delimiter
#ifdef WIN32
    headerFilePath.append( "\\" );
#else
    headerFilePath.append( "/" );
#endif
    }
  headerFilePath.append( vtkClassName );
  headerFilePath.append( ".cxx" );

  std::ofstream os( headerFilePath.c_str() );

  //os << "#include \"vtkITKImageFilter.h\"\n\n";

  os << "#include <itk" << itkClassName << ".h>\n";
  os << "#include <" << vtkClassName << ".h>\n\n";

  os << "#include <tr1/functional>\n";
  os << "#include <tr1/type_traits>\n\n";

  // Add additional files requested in the ClassDescription
  for ( int i = 0; i < this->GetClassDescription()->GetNumberOfIncludeFiles(); ++i )
    {
    os << "#include <" << this->GetClassDescription()->GetIncludeFile( i ) << ">\n";
    }
  os << "\n";

  os << "#include <vtkObjectFactory.h>\n";
  os << "#include <vtkImageExport.h>\n";
  os << "#include <vtkImageImport.h>\n";
  os << "#include <vtkImageData.h>\n";
  os << "#include <vtkInformationVector.h>\n";
  os << "#include <vtkInformation.h>\n";
  os << "#include <vtkStreamingDemandDrivenPipeline.h>\n\n";

  os << "vtkStandardNewMacro(" << vtkClassName << ");\n\n";

  // Constructor
  os << vtkClassName << "::" << vtkClassName << "()\n";
  os << "{\n";

  // Set the number of input and output ports
  os << "  this->SetNumberOfInputPorts(" << classDescription->GetNumberOfInputs() << ");\n";
  os << "  this->SetNumberOfOutputPorts(1);\n\n";

  os << "  this->Init();\n";

  // Initialize default values
  for ( int i = 0; i < classDescription->GetNumberOfMemberDescriptions(); ++i )
    {
    const MemberDescription * member = classDescription->GetMemberDescription( i );

    if ( classDescription->IsEnumerationType( member->GetTypeName() ) )
      {
      os << "  this->" << member->GetMemberName() << " = " << member->GetDefaultValue() << ";\n";
      }
    else
      {
      int numberOfElements = member->GetNumberOfElements(); 
      if ( numberOfElements == 1 )
        {
        os << "  this->" << member->GetMemberName() << " = " << member->GetDefaultValue();
        if ( member->GetTypeName() == "bool" )
          {
          os << " ? 1 : 0";
          }
        os << ";\n";
        }
      else
        {
        for (int i = 0; i < numberOfElements; ++i)
          {
          os << "  this->" << member->GetMemberName() << "[" << i << "] = " << member->GetElementDefaultValue();
          if ( member->GetElementTypeName() == "bool" )
            {
            os << " ? 1 : 0";
            }
          os << ";\n";
          }
        }
      }
    }    

  os << "}\n\n";

  // Destructor
  os << vtkClassName << "::~" << vtkClassName << "()\n";
  os << "{\n";
  os << "}\n\n";

  // RequestUpdateExtent() method
  os << "int " << vtkClassName << "::RequestUpdateExtent(\n";
  os << "  vtkInformation * vtkNotUsed(request),\n";
  os << "  vtkInformationVector **inputVector,\n";
  os << "  vtkInformationVector *outputVector)\n";
  os << "{\n";

  os << "  int inWExt[6];\n";
  
  for (int i = 0; i < classDescription->GetNumberOfInputs(); ++i)
    {
    os << "  vtkInformation *inInfo" << i << " = inputVector[" << i << "]->GetInformationObject(0);\n";
    os << "  inInfo" << i << "->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(), inWExt);\n";
    os << "  inInfo" << i << "->Set(vtkStreamingDemandDrivenPipeline::UPDATE_EXTENT(), inWExt, 6);\n\n";
    }

  os << "return 1;\n";

  os << "}\n";

  // FillInputPortInformation() method
  os << "int " << vtkClassName << "::FillInputPortInformation(int port, vtkInformation *info)\n";
  os << "{\n";
  os << "  info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), \"vtkImageData\");\n";
  os << "  info->Set(vtkAlgorithm::INPUT_IS_REPEATABLE(), 0);\n";
  os << "  info->Set(vtkAlgorithm::INPUT_IS_OPTIONAL(), 0);\n\n";

  os << "  if (port == 0";
  for (int i = 1; i < classDescription->GetNumberOfInputs(); ++i)
    {
    os << " || port == " << i;
    }
  os << ")\n";
  os << "    {\n";
  os << "    return 1;\n";
  os << "    }\n\n";

  os << "  return 0;\n";
  os << "}\n\n";

  // Run method
  os << "template< class TInputImage >\n";
  os << "void " << vtkClassName << "::Run(TInputImage * input)\n";
  os << "{\n";

  // Instantiate the ITK filter
  os << "  typedef TInputImage InputImageType;\n";

  for (int i = 1; i < classDescription->GetNumberOfInputs(); ++i)
    {
    os << "  typedef TInputImage InputImageType" << i+1 << ";\n";
    }

  if ( this->GetClassDescription()->GetOutputPixelType() != "" )
    {
    // Custom pixel type
    os << "  typedef itk::Image< " << this->GetClassDescription()->GetOutputPixelType() << ", 3 > OutputImageType;\n";
    }
  else
    {
    // Standard pixel type
    os << "  typedef TInputImage OutputImageType;\n\n";
    }

  if ( this->GetClassDescription()->GetFilterType() != "" )
    {
    // Custom filter type declaration from JSON file
    os << "  typedef " << this->GetClassDescription()->GetFilterType() << " FilterType;\n";
    }
  else if ( this->GetClassDescription()->GetTemplateCodeFileName() == "KernelImageFilter" )
    {
    // Kernel filtering class of filters
    os << "  typedef itk::" << itkClassName << "< InputImageType, InputImageType, InputImageType > FilterType;\n";
    }
  else
    {
    // Standard filter type declaration
    os << "  typedef itk::" << itkClassName << "< ";
    for ( int i = 0; i < this->GetNumberOfInputs(); ++i )
      {
      os << "InputImageType, ";
      }
    os << " OutputImageType > FilterType;\n";
    }

  os << "  typename FilterType::Pointer filter = FilterType::New();\n\n";

  // Set the input
  //this->WriteSetInputCode( os );
  os << "\n";

  // Pass the member variable values from VTK to ITK
  for (int i = 0; i < classDescription->GetNumberOfMemberDescriptions(); ++i)
    {
    const MemberDescription * member = classDescription->GetMemberDescription( i );
    
    if ( member->GetCustomITKCast() != "<undefined>" && !this->GetClassDescription()->IsEnumerationType( member->GetTypeName() ) )
      {
      std::string code( member->GetCustomITKCast() );
      code = this->SubstituteString( "this->m_", "", code );
      code = this->SubstituteString( "m_", "", code );

      os << "  " << code << "\n";

      continue;
      }
    if ( classDescription->IsEnumerationType( member->GetTypeName() ) )
      {
      os << "  filter->Set" << member->GetMemberName() << "(static_cast<typename FilterType::"
         << member->GetTypeName() << ">(this->Get"
         << member->GetMemberName() << "()));\n";
      }
    else
      {
      // Turn bools into ints
      if ( member->GetElementTypeName() == "bool" )
        {
        int numberOfElements = member->GetNumberOfElements(); 
        if ( numberOfElements == 1 )
          {
          os << "  filter->Set" << member->GetMemberName() << "(this->Get" << member->GetMemberName() << "() != 0);\n\n";
          }
        else
          {
          os << "  int tmp" << member->GetMemberName() << "[" << numberOfElements << "];\n";
          os << "  this->Get" << member->GetMemberName() << "(tmp" << member->GetMemberName() << ");\n\n";

          os << "  bool b" << member->GetMemberName() << "[" << numberOfElements << "];\n";
          for (int i = 0; i < numberOfElements; ++i)
            {
            os << "  b" << member->GetMemberName() << "[" << i << "] = (tmp" << member->GetMemberName() << "[" << i << "] != 0);\n";
            }
          os << "  filter->Set(b" << member->GetMemberName() << ");\n";
          }
        }
      else
        {
        os << "  filter->Set" << member->GetMemberName() << "(this->Get"
           << member->GetMemberName() << "());\n";
        }
      }
    }

  os << "\n" << "  filter->Update();\n\n";

  // End the Run method
  os << "}\n\n";

  // PrintSelf method
  os << "void " << vtkClassName << "::PrintSelf(ostream& os, vtkIndent indent)\n";
  os << "{\n";
  os << "  this->Superclass::PrintSelf(os, indent);\n\n";

  for (int i = 0; i < classDescription->GetNumberOfMemberDescriptions(); ++i)
    {
    const MemberDescription * member = classDescription->GetMemberDescription( i );
    os << "  os << indent << " << "\"" << member->GetMemberName() << ":\" << this->"
       << member->GetMemberName() << " << \"\\n\";\n";
    }

  os << "}\n";

  os.flush();
  os.close();

  return true;
}

std::string
ParaView3PluginGenerator
::GetITKClassName()
{
  std::string filterName( this->GetClassDescription()->GetPluginName() );
  filterName.append( "ImageFilter" );

  // Override the filter name if it is explicit in the JSON file
  if ( this->GetClassDescription()->GetITKClassName() != "" )
    {
    filterName = this->GetClassDescription()->GetITKClassName();
    }

  return filterName;
}


std::string
ParaView3PluginGenerator
::GetVTKClassName()
{
  const ClassDescription * classDescription = this->GetClassDescription();
  std::string vtkClassName( "vtkITK" );
  vtkClassName.append( classDescription->GetPluginName() );
  vtkClassName.append( "ImageFilter" );

  return vtkClassName;
}

std::string
ParaView3PluginGenerator
::GetVTKTypeName( const std::string & typeName )
{
  if ( this->GetClassDescription()->IsEnumerationType( typeName ) )
    {
    return typeName;
    }
  else if ( this->IsBoolVectorType( typeName ) )
    {
    return std::string( "int" );
    }
  else if ( this->IsIntVectorType( typeName ) )
    {
    return std::string( "int" );
    }
  else if ( this->IsDoubleVectorType( typeName ) )
    {
    return std::string( "double" );
    }

  std::cerr << "Unknown input typeName in GetVTKTypeName" << std::endl;
  return std::string();
}

void
ParaView3PluginGenerator
::WriteSetInputCode( std::ostream & os)
{
  if ( this->GetClassDescription()->GetCustomSetInput() == "<undefined>" )
    {
    for ( int i = 0; i < this->GetClassDescription()->GetNumberOfInputs(); ++i )
      {
      // First input gets special treatment to avoid
      // "Input Primary is required but not set" exception
      if ( i == 0 )
        {
        os << "  filter->SetInput( input" << i << "->GetOutput() );\n";
        }
      else
        {
        os << "  filter->SetInput( " << i << ", input" << i << "->GetOutput() );\n";
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
}
