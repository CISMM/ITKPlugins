int main( int argc, char * argv[] )
{
  PARSE_ARGS;

  itk::ImageIOBase::IOPixelType     pixelType;
  itk::ImageIOBase::IOComponentType componentType;

  try
    {
    itk::GetImageType( inputVolume0, pixelType, componentType );

    // This filter handles all types
    switch( componentType )
      {

#ifdef ITK_UCHAR_TYPE
      case itk::ImageIOBase::UCHAR:
        return Run( argc, argv, static_cast< unsigned char >(0) );
        break;
#endif

#ifdef ITK_CHAR_TYPE
      case itk::ImageIOBase::CHAR:
        return Run( argc, argv, static_cast< char >(0) );
        break;
#endif

#ifdef ITK_USHORT_TYPE
      case itk::ImageIOBase::USHORT:
        return Run( argc, argv, static_cast< unsigned short >(0) );
        break;
#endif

#ifdef ITK_SHORT_TYPE
      case itk::ImageIOBase::SHORT:
        return Run( argc, argv, static_cast< short >(0) );
        break;
#endif

#ifdef ITK_UINT_TYPE
      case itk::ImageIOBase::UINT:
        return Run( argc, argv, static_cast< unsigned int >(0) );
        break;
#endif

#ifdef ITK_INT_TYPE
      case itk::ImageIOBase::INT:
        return Run( argc, argv, static_cast< int >(0) );
        break;
#endif

#ifdef ITK_ULONG_TYPE
      case itk::ImageIOBase::ULONG:
        return Run( argc, argv, static_cast< unsigned long >(0) );
        break;
#endif

#ifdef ITK_LONG_TYPE
      case itk::ImageIOBase::LONG:
        return Run( argc, argv, static_cast< long >(0) );
        break;
#endif

#ifdef ITK_FLOAT_TYPE
      case itk::ImageIOBase::FLOAT:
      {

#ifdef ITK_COMPLEX_FLOAT_TYPE
      if ( pixelType == itk::ImageIOBase::COMPLEX )
        return Run( argc, argv, static_cast< std::complex< float > >(0) );
      else
#endif

        return Run( argc, argv, static_cast< float >(0) );
      }
        break;
#endif

#ifdef ITK_DOUBLE_TYPE
      case itk::ImageIOBase::DOUBLE:
      {

#ifdef ITK_COMPLEX_DOUBLE_TYPE
      if ( pixelType == itk::ImageIOBase::COMPLEX )
        return Run( argc, argv, static_cast< std::complex< double > >(0) );
      else
#endif

        return Run( argc, argv, static_cast< double >(0) );
      }
        break;
#endif

#ifdef ITK_COMPLEX_FLOAT_TYPE
        //case itk::ImageIOBase::
#endif

      case itk::ImageIOBase::UNKNOWNCOMPONENTTYPE:
      default:
        std::cout << "unknown component type" << std::endl;
        break;
      }
    }
  catch( itk::ExceptionObject & excep )
    {
    std::cerr << argv[0] << ": exception caught !" << std::endl;
    std::cerr << excep << std::endl;

    return EXIT_FAILURE;
    }
  catch( std::exception & excep )
    {
    std::cerr << "STD exception caught !" << std::endl;
    std::cerr << excep.what() << std::endl;

    return EXIT_FAILURE;
    }
  catch( ... )
    {
    std::cerr << "Unknown exception caught !" << std::endl;

    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
