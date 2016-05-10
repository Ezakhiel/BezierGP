#pragma once
#include "../Core/LinearCombination3.h"
#include "../Core/RealSquareMatrices.h"


namespace cagd
{
class CyclicCurve3 : public LinearCombination3
{
	protected:
	GLuint _n; // order
	GLdouble _c_n ; // normalizingconstant
	GLdouble _lambda_n ; // phase change
	
        RealSquareMatrix _bc ; // binomialcoefficients

	GLdouble _CalculateNormalizingCoefficient(GLuint n);
	
	GLvoid _CalculateBinomialCoefficients(GLuint m,RealSquareMatrix &bc);
	
	public:
	//specialconstructor
	CyclicCurve3(GLuint n, GLenum data_usage_flag=GL_STATIC_DRAW);
	
	//redeclareanddefineinheritedpurevirtualmethods
	GLboolean BlendingFunctionValues(GLdouble u,RowMatrix<GLdouble>& values) const;
	GLboolean CalculateDerivatives(GLuint max_order_of_derivatives, GLdouble u,Derivatives& d) const;
	};
}
