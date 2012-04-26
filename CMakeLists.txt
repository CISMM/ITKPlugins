project( ITKPlugins )

cmake_minimum_required( VERSION 2.8 )

# Choose the target applications for which the plugins should be built
option( BUILD_SLICER4_PLUGINS "Build plugins for Slicer 4.0" OFF )
option( BUILD_PARAVIEW3_PLUGINS "Build plugins for ParaView 3.14" OFF )

set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ITKPlugins_BINARY_DIR}/lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ITKPlugins_BINARY_DIR}/bin )
if ( "${CMAKE_CFG_INTDIR}" STREQUAL "." )
  set( ITKPlugins_EXECUTABLE_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )
else ()
  set( ITKPlugins_EXECUTABLE_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR} )
endif()

# Find the SimpleITK source directory
find_path( SimpleITK_SOURCE_DIR
  SimpleITK/CMakeLists.txt
  HINTS ../SimpleITK
  DOC "The location of the CMakeLists.txt file at the top level of the SimpleITK source directory."
)
if (NOT SimpleITK_SOURCE_DIR)
  message( ERROR "Could not find CMakeLists.txt file for SimpleITK" )
endif()

# Turn off security warnings in MSVC
if( MSVC )
  add_definitions( -D_SCL_SECURE_NO_WARNINGS )
endif()

#######################################
# Quiet warnings from the JSON library.
#######################################
if ( CMAKE_BUILD_TYPE STREQUAL "Debug" )
  add_definitions( -DJSON_DEBUG )
endif()
if ( CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" OR
     CMAKE_BUILD_TYPE STREQUAL "Release" )
  add_definitions( -DNDEBUG )
endif()

# Configure the ITKPluginsConfigure.h file
configure_file( "${ITKPlugins_SOURCE_DIR}/ITKPluginsConfigure.h.in"
  "${ITKPlugins_BINARY_DIR}/ITKPluginsConfigure.h" @ONLY
  )

include_directories( "${ITKPlugins_BINARY_DIR}" )


#----------------------------------------------------------
# Place all options to go into sitkConfigure.h here. We configure our own
# version here to avoid headaches compiling on Windows

#-----------------------------------------------------------------------------
# SimpleITK build configuration options.
set(SITK_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})


 #-----------------------------------------------------------
# Place all checks and try compile variable for sitkConfigure.h here

# check for C++0x static_assert
try_compile(SITK_SUPPORTS_STATIC_ASSERT
  ${ITKPlugins_BINARY_DIR}/CMakeTmp
  ${SimpleITK_SOURCE_DIR}/CMake/static_assert.cxx )


include(CheckIncludeFileCXX)

# check for the required tr1/functional header
check_include_file_cxx( tr1/functional SITK_HAS_STLTR1_TR1_FUNCTIONAL )
check_include_file_cxx( functional SITK_HAS_STLTR1_FUNCTIONAL )

# check for the required tr1/type_traits header
check_include_file_cxx( tr1/type_traits SITK_HAS_STLTR1_TR1_TYPE_TRAITS )
check_include_file_cxx( type_traits SITK_HAS_STLTR1_TYPE_TRAITS )

# check for the optional tr1/unordered_map header
check_include_file_cxx( tr1/unordered_map SITK_HAS_STLTR1_TR1_UNORDERED_MAP )
check_include_file_cxx( unordered_map SITK_HAS_STLTR1_UNORDERED_MAP )
if ( ${SITK_HAS_STLTR1_UNORDERED_MAP} )
  set( SITK_UNORDERED_MAP_FUNCTIONAL_DEFINITIONS "-DSITK_HAS_STLTR1_UNORDERED_MAP")
elseif ( ${SITK_HAS_STLTR1_TR1_UNORDERED_MAP} )
  set( SITK_UNORDERED_MAP_FUNCTIONAL_DEFINITIONS "-DSITK_HAS_STLTR1_TR1_UNORDERED_MAP" )
endif()
try_compile(SITK_UNORDERED_MAP_FUNCTIONAL
  ${ITKPlugins_BINARY_DIR}/CMakeTmp
  ${SimpleITK_SOURCE_DIR}/CMake/tr1_unordered_map.cxx
  COMPILE_DEFINITIONS "${SITK_UNORDERED_MAP_FUNCTIONAL_DEFINITIONS}"
  OUTPUT_VARIABLE out )

configure_file( ${SimpleITK_SOURCE_DIR}/sitkConfigure.h.in
  ${ITKPlugins_BINARY_DIR}/sitkConfigure.h )



# Add all the subdirectories
add_subdirectory( Common )
add_subdirectory( Generators )
add_subdirectory( Parser )
add_subdirectory( Utilities )

# Generate plugin code
# Code: #+ - Tested, compiles, #- Tested, fails

set( WORKING_JSON
  Abs
  AbsoluteValueDifference
  Acos
  Add
  AddConstantTo
  And
  ApproximateSignedDistanceMap
  Asin
  Atan
  Atan2
  Bilateral
  BinaryContour
  BinaryMagnitude
  BinaryMinMaxCurvatureFlow
  BinaryProjection
  BinaryThinning
  BinaryThreshold
  BinomialBlur
  BoundedReciprocal
  CannyEdgeDetection
  CheckerBoard
  ComplexToImaginary
  ComplexToModulus
  ComplexToPhase
  ComplexToReal
  ConnectedComponent
  ConnectedThreshold
  Convolution
  ConstantPad
  Cos
  Crop
  CurvatureAnisotropicDiffusion
  CurvatureFlow
  DanielssonDistanceMap
  Derivative
  DiscreteGaussian
  Divide
  DivideByConstant
  DivideConstantBy
  DoubleThreshold
  Exp
  ExpNegative
  FFTConvolution
  FFTShift
  Flip
  GradientAnisotropicDiffusion
  GradientMagnitudeRecursiveGaussian
  HausdorffDistance
  HConcave
  HConvex
  HMaxima
  HMinima
  HuangThreshold
  IntensityWindowing
  IntermodesThreshold
  InverseDeconvolution
  InverseFFT
  InvertIntensity
  IsoContourDistance
  IsoDataThreshold
  IsolatedWatershed
  KittlerIllingworthThreshold
  LabelContour
  LandweberDeconvolution
  Laplacian
  LaplacianRecursiveGaussian
  LaplacianSharpening
  LiThreshold
  Log
  Log10
  Mask
  MaskNegated
  Maximum
  MaximumEntropyThreshold
  MaximumProjection
  Mean
  MeanProjection
  MedianProjection
  Median
  MinMaxCurvatureFlow
  Minimum
  MinimumProjection
  MirrorPad
  Modulus
  MomentsThreshold
  Multiply
  MultiplyByConstant
  NeighborhoodConnected
  Noise
  Normalize
  NormalizeToConstant
  Not
  Or
  OtsuThreshold
  Pow
  PowToConstant
  ProjectedLandweberDeconvolution
  RelabelComponent
  RenyiEntropyThreshold
  RescaleIntensity
  RichardsonLucyDeconvolution
  ScalarConnectedComponent
  ShanbhagThreshold
  ShiftScale
  Shrink
  Sigmoid
  SignedDanielssonDistanceMap
  SignedMaurerDistanceMap
  SimpleContourExtractor
  Sin
  SobelEdgeDetection
  Sqrt
  Square
  SquaredDifference
  StandardDeviationProjection
  Subtract
  SubtractConstantBy
  SumProjection
  TikhonovDeconvolution
  Tan
  Threshold
  TriangleThreshold
  VotingBinary
  VotingBinaryHoleFilling
  VotingBinaryIterativeHoleFilling
  WienerDeconvolution
  WrapPad
  Xor
  YenThreshold
  ZeroCrossingBasedEdgeDetection
)

  set( FAILING_JSON
  # These don't compile yet
  #-BinaryClosingByReconstruction
  #-BinaryDilate
  #-BinaryErode
  #-BinaryFillhole
  #-BinaryGrindPeak
  #-BinaryMedian
  #-BinaryMorphologicalClosing
  #-BinaryMorphologicalOpening
  #-BinaryOpeningByReconstruction
  #-BlackTopHat
  #-BoxMean
  #-BoxSigma
  #-ClosingByReconstruction - kernel image filter type
  #-DilateObjectMorphology
  #-EdgePotential - vector pixels
  #-ErodeObjectMorphology
  #-Extract - names
  #-FastMarching
  #-ForwardFFT
  #-GeodesicActiveContourLevelSet
  #-GrayscaleDilate
  #-GrayscaleErode
  #-GrayscaleFillhole
  #-GrayscaleGeodesicDilate
  #-GrayscaleGeodesicErode
  #-GrayscaleGrindPeak
  #-GrayscaleMorphologicalClosing
  #-GrayscaleMorphologicalOpening
  #-HistogramMatching
  #-LabelMapMask
  #-LabelMapOverlay
  #-LabelMapToBinary
  #-LabelMapToLabel
  #-LabelMapToRGB
  #-LabelOverlay
  #-LabelToRGB
  #-MagnitudeAndPhaseToComplex
  #-MorphologicalGradient - kernel image
  #-N4BiasFieldCorrection
  #-OpeningByReconstruction
  #-Paste
  #-PermuteAxes - CLP generation fails
  #-RealAndImaginaryToComplex
  #-RecursiveGaussian
  #-RegionOfInterest
  #-Resample - malformed XML
  #-ScalarToRGBColormap
  #-ShapeDetectionLevelSet - default pixel type is float, JSON doesn't change it
  #-SmoothingRecursiveGaussian
  #-ThresholdMaximumConnectedComponents
  #-VectorConnectedComponent
  #-VectorIndexSelectionCast
  #-VectorMagnitude
  #-WhiteTopHat - kernel filter
  #-ZeroCrossing - pixel type list
)

set( Plugin_JSON
  ${WORKING_JSON}
)

if ( BUILD_SLICER4_PLUGINS )

  set( EXTENSION_NAME ITKPluginsExtension )
  find_package( Slicer REQUIRED )
  include( ${Slicer_USE_FILE} )

endif()

if ( BUILD_PARAVIEW3_PLUGINS )

  find_package( ParaView REQUIRED )
  include( ${PARAVIEW_USE_FILE} )

  # Need to include ITK separately because ParaView doesn't know anything about it
  find_package( ITK REQUIRED )
  include( ${ITK_USE_FILE} )

endif()

add_subdirectory( GeneratedCode )
