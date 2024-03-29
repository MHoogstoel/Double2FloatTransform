#include <string>
#include <itkTransformFileReader.h>
#include <itkTransformFileWriter.h>
#include <itkMatrixOffsetTransformBase.h>
#include <itkTransformFactory.h>
#include <itkAffineTransform.h>
#include <list>
#include <string.h>

int main( int argc , char* argv[] )
{
	// if the input is incorrect
	if( argc != 3 )
	{
		std::cout<< argv[ 0 ] << " inputTransform outputTransform " << std::endl ;
		return 1 ;
	}
	
	/// Checking which transform is used (RegisterImages transform vs. ANTS transform)
	// Set parameters

	
	typedef itk::AffineTransform< double , 3 > AffineDoubleType ;
	typedef itk::AffineTransform< float , 3 > AffineFloatType ;
	typedef itk::MatrixOffsetTransformBase< double , 3 , 3 > MatrixOffsetTransformBaseDoubleType ;
	itk::TransformFactory<MatrixOffsetTransformBaseDoubleType>::RegisterTransform();
	itk::TransformFactory<AffineDoubleType>::RegisterTransform();
	
	AffineDoubleType::Pointer affineDouble ;
	AffineFloatType::Pointer affineFloat = AffineFloatType::New() ;
	
	itk::TransformFileReader::Pointer transformFile = itk::TransformFileReader::New() ;
	transformFile->SetFileName( argv[ 1 ] ) ;
	transformFile->Update() ;
	
	affineDouble = dynamic_cast< AffineDoubleType* > ( transformFile->GetTransformList()->front().GetPointer() ) ;
	
	//Parameters
	AffineDoubleType::ParametersType doubleParam ;
	AffineFloatType::ParametersType floatParam ;
	
	if (affineDouble) ///RegisterImages transform
	{
		doubleParam = affineDouble->GetParameters() ;
		floatParam.SetSize( doubleParam.GetSize() ) ;
		for( unsigned int i = 0 ; i < floatParam.GetSize() ; i++ )
		{
     //std::cout<<doubleParam.GetElement( i )<<std::endl;
			floatParam.SetElement( i , (float)doubleParam.GetElement( i ) ) ;
		}
		affineFloat->SetParameters( floatParam ) ;
  ///Fixed Parameters
		doubleParam = affineDouble->GetFixedParameters() ;
		floatParam.SetSize( doubleParam.GetSize() ) ;
  //std::cout<<"fixed"<<std::endl;
		for( unsigned int i = 0 ; i < floatParam.GetSize() ; i++ )
		{
     //std::cout<<doubleParam.GetElement( i )<<std::endl;
			floatParam.SetElement( i , (float)doubleParam.GetElement( i ) ) ;
		}
		affineFloat->SetFixedParameters( floatParam ) ;
	}
 
	else if (!affineDouble) ///ANTS transform
	{
		
		MatrixOffsetTransformBaseDoubleType::Pointer matrixOffsetTransformBaseDouble ;
		matrixOffsetTransformBaseDouble = dynamic_cast< MatrixOffsetTransformBaseDoubleType* > ( transformFile->GetTransformList()->front().GetPointer() ) ;
		if( !matrixOffsetTransformBaseDouble ) ///error message if the transform is not from RI or ANTS
		{
			std::cerr << "Input Transformation is not a matrix offset transform base with doubles or an affine transform with doubles" << std::endl ;
			return 1 ;
		}
		
		MatrixOffsetTransformBaseDoubleType::ParametersType doubleParam ;
		AffineFloatType::ParametersType floatParam ;
  ///Parameters
		doubleParam = matrixOffsetTransformBaseDouble->GetParameters() ;
		floatParam.SetSize( doubleParam.GetSize() ) ;
		for( unsigned int i = 0 ; i < floatParam.GetSize() ; i++ )
		{
     //std::cout<<doubleParam.GetElement( i )<<std::endl;
			floatParam.SetElement( i , (float)doubleParam.GetElement( i ) ) ;
		}
		affineFloat->SetParameters( floatParam ) ;
  ///Fixed Parameters
		doubleParam = matrixOffsetTransformBaseDouble->GetFixedParameters() ;
		floatParam.SetSize( doubleParam.GetSize() ) ;
  //std::cout<<"fixed"<<std::endl;
		for( unsigned int i = 0 ; i < floatParam.GetSize() ; i++ )
		{
     //std::cout<<doubleParam.GetElement( i )<<std::endl;
			floatParam.SetElement( i , (float)doubleParam.GetElement( i ) ) ;
		}
		affineFloat->SetFixedParameters( floatParam ) ;
	}
	
	itk::TransformFileWriter::Pointer transformWriter = itk::TransformFileWriter::New() ;
	transformWriter->SetFileName( argv[ 2 ] ) ;
	transformWriter->AddTransform( affineFloat ) ;
	transformWriter->Update() ;
	return 0 ;
}
