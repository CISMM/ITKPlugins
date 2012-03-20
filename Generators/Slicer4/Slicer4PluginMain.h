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
      case itk::ImageIOBase::UCHAR:
        return Run( argc, argv, static_cast< unsigned char >(0) );
        break;

      case itk::ImageIOBase::CHAR:
        return Run( argc, argv, static_cast< char >(0) );
        break;

      case itk::ImageIOBase::USHORT:
        return Run( argc, argv, static_cast< unsigned short >(0) );
        break;

      case itk::ImageIOBase::SHORT:
        return Run( argc, argv, static_cast< short >(0) );
        break;

      case itk::ImageIOBase::UINT:
        return Run( argc, argv, static_cast< unsigned int >(0) );
        break;

      case itk::ImageIOBase::INT:
        return Run( argc, argv, static_cast< int >(0) );
        break;

      case itk::ImageIOBase::ULONG:
        return Run( argc, argv, static_cast< unsigned long >(0) );
        break;

      case itk::ImageIOBase::LONG:
        return Run( argc, argv, static_cast< long >(0) );
        break;

      case itk::ImageIOBase::FLOAT:
        return Run( argc, argv, static_cast< float >(0) );
        break;

      case itk::ImageIOBase::DOUBLE:
        return Run( argc, argv, static_cast< double >(0) );
        break;

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
