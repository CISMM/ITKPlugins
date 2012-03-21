project( ITKPlugins )

cmake_minimum_required( VERSION 2.8 )

# Choose the target applications for which the plugins should be built
option( BUILD_SLICER4_PLUGINS "Build plugins for Slicer 4.0" OFF )
option( BUILD_PARAVIEW3_PLUGINS "Build plugins for ParaView 3.14" OFF )

set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ITKPlugins_BINARY_DIR}/lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ITKPlugins_BINARY_DIR}/bin )

#######################################
# Quiet warnings from the JSON library.
#######################################
if ( CMAKE_BUILD_TYPE STREQUAL "Debug" )
  add_definitions( -DJSON_DEBUG )
endif()
if ( CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" )
  add_definitions( -DNDEBUG )
endif()

add_subdirectory( Common )
add_subdirectory( Generators )
add_subdirectory( Parser )
add_subdirectory( Utilities )

# Generate plugin code
# Code: #+ - Tested, compiles, #- Tested, fails

set( Plugin_JSON
  # These compile
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
  ComplexToImaginary
  ComplexToModulus
  ComplexToPhase
  ComplexToReal
  ConnectedComponent
  ConnectedThreshold
  ConstantPad
  Cos
  Crop
  CurvatureAnisotropicDiffusion
  CurvatureFlow
  DanielssonDistanceMap
  Derivative
  DiscreteGaussian
  Divide
  DoubleThreshold
  Exp
  ExpNegative
  FFTShift
  GradientAnisotropicDiffusion
  GradientMagnitudeRecursiveGaussian
  HConcave
  HConvex
  HMaxima
  HMinima
  HuangThreshold
  IntensityWindowing
  IntermodesThreshold
  InvertIntensity
  IsoContourDistance
  IsoDataThreshold
  IsolatedWatershed
  KittlerIllingworthThreshold
  LabelContour
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
  RenyiEntropyThreshold
  RescaleIntensity
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
  Tan
  TriangleThreshold
  WrapPad
  Xor
  YenThreshold
  ZeroCrossingBasedEdgeDetection

  # These don't compile
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
  #-CheckerBoard
  #-ClosingByReconstruction
  #-Convolution
  #-DilateObjectMorphology
  #-DivideByConstant
  #-DivideConstantBy
  #-EdgePotential
  #-ErodeObjectMorphology
  #-Extract
  #-FFTConvolution
  #-FastMarching
  #-Flip
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
  #-HausdorffDistance
  #-HistogramMatching
  #-InverseFFT
  #-LabelMapMask
  #-LabelMapOverlay
  #-LabelMapToBinary
  #-LabelMapToLabel
  #-LabelMapToRGB
  #-LabelOverlay
  #-LabelToRGB
  #-MagnitudeAndPhaseToComplex
  #-MorphologicalGradient
  #-N4BiasFieldCorrection
  #-OpeningByReconstruction
  #-Paste
  #-PermuteAxes
  #-RealAndImaginaryToComplex
  #-RecursiveGaussian
  #-RegionOfInterest
  #-RelabelComponent
  #-Resample
  #-ScalarConnectedComponent
  #-ScalarToRGBColormap
  #-ShapeDetectionLevelSet
  #-SmoothingRecursiveGaussian
  #-Threshold
  #-ThresholdMaximumConnectedComponents
  #-VectorConnectedComponent
  #-VectorIndexSelectionCast
  #-VectorMagnitude
  #-VotingBinary
  #-VotingBinaryHoleFilling
  #-VotingBinaryIterativeHoleFilling
  #-WhiteTopHat
  #-ZeroCrossing
)

if ( BUILD_SLICER4_PLUGINS )

  set( EXTENSION_NAME ITKPluginsExtension )
  find_package( Slicer REQUIRED )
  include( ${Slicer_USE_FILE} )

endif()

add_subdirectory( GeneratedCode )
