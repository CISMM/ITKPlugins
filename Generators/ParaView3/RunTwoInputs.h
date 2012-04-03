vtkInformation *inInfo0 = inputVector[0]->GetInformationObject(0);
vtkImageData *input0 = vtkImageData::SafeDownCast
  (inInfo0->Get(vtkDataObject::DATA_OBJECT()));
if (!input0)
  {
  vtkErrorMacro("Input 0 is not of type vtkImageData");
  return 0;
  }

vtkInformation *inInfo1 = inputVector[1]->GetInformationObject(0);
vtkImageData *input1 = vtkImageData::SafeDownCast
  (inInfo1->Get(vtkDataObject::DATA_OBJECT()));
if (!input1)
  {
  vtkErrorMacro("Input 1 is not of type vtkImageData");
  return 0;
  }

vtkInformation *outInfo = outputVector->GetInformationObject(0);
vtkImageData *output = vtkImageData::SafeDownCast
  (outInfo->Get(vtkDataObject::DATA_OBJECT()));
if (!output)
  {
  vtkErrorMacro("Output is not of type vtkImageData");
  return 0;
  }

switch ( input0->GetScalarType() )
  {
#ifdef ITK_UCHAR_TYPE
  case VTK_UNSIGNED_CHAR:
    this->Run( static_cast< unsigned char * >( 0 ), static_cast< unsigned char * >( 0 ),
              input0, input1, output );
    break;
#endif
#ifdef ITK_CHAR_TYPE
  case VTK_CHAR:
  case VTK_SIGNED_CHAR:
    this->Run( static_cast< char * >( 0 ), static_cast< char * >( 0 ),
              input0, input1, output );
    break;
#endif
#ifdef ITK_USHORT_TYPE
  case VTK_UNSIGNED_SHORT:
    this->Run( static_cast< unsigned short * >( 0 ), static_cast< unsigned short * >( 0 ),
              input0, input1, output );
    break;
#endif
#ifdef ITK_SHORT_TYPE
  case VTK_SHORT:
    this->Run( static_cast< short * >( 0 ), static_cast< short * >( 0 ),
              input0, input1, output );
    break;
#endif
#ifdef ITK_UINT_TYPE
  case VTK_UNSIGNED_INT:
    this->Run( static_cast< unsigned int * >( 0 ), static_cast< unsigned int * >( 0 ),
              input0, input1, output );
    break;
#endif
#ifdef ITK_INT_TYPE
  case VTK_INT:
    this->Run( static_cast< int * >( 0 ), static_cast< int * >( 0 ),
              input0, input1, output );
    break;
#endif
#ifdef ITK_ULONG_TYPE
  case VTK_UNSIGNED_LONG:
    this->Run( static_cast< unsigned long * >( 0 ), static_cast< unsigned long * >( 0 ),
              input0, input1, output );
    break;
#endif
#ifdef ITK_LONG_TYPE
  case VTK_LONG:
    this->Run( static_cast< long * >( 0 ), static_cast< long * >( 0 ),
              input0, input1, output );
    break;
#endif
#ifdef ITK_FLOAT_TYPE
  case VTK_FLOAT:
    this->Run( static_cast< float * >( 0 ), static_cast< float * >( 0 ),
              input0, input1, output );
    break;
#endif
#ifdef ITK_DOUBLE_TYPE
  case VTK_DOUBLE:
    this->Run( static_cast< double * >( 0 ), static_cast< double * >( 0 ),
              input0, input1, output );
    break;
#endif

  default:
    vtkErrorMacro( "Unknown pixel type encountered in input0" );
    break;
  }
