#include "BicubicBezierIsoLine.h"
#include <iostream>

using namespace cagd;
using namespace std;

BicubicBezierIsoLine::BicubicBezierIsoLine(GLenum usage_flag): LinearCombination3(0.0,1.0,4,usage_flag)
{
}

GLboolean BicubicBezierIsoLine::BlendingFunctionValues(GLdouble u_knot, RowMatrix<GLdouble> &blending_values) const
{
    if (u_knot < 0.0 || u_knot > 1.0)
        return GL_FALSE;

    blending_values.ResizeColumns(4);

    GLdouble u = u_knot , u2 = u * u , u3 = u2 * u , w = 1.0 - u , w2 = w * w, w3 = w2 * w;

    blending_values(0) = w3;
    blending_values(1) = 3.0 * w2 * u;
    blending_values(2) = 3.0 * w * u2;
    blending_values(3) = u3;

    return GL_TRUE;
}

GLboolean BicubicBezierIsoLine::CalculateDerivatives(GLuint max_order_of_derivatives, GLdouble u, Derivatives &d) const
{

    if (u<0.0 || u>1.0)
        return GL_FALSE;

    if (max_order_of_derivatives<0 || max_order_of_derivatives>3)
        return GL_FALSE;

    d.ResizeRows(max_order_of_derivatives + 1);


    GLdouble val[4];
    GLdouble u2 = u * u , u3 = u2 * u , w = 1.0 - u , w2 = w * w, w3 = w2 * w;
    DCoordinate3 sum;

    val[0] = w3;
    val[1] = 3.0 * w2 * u;
    val[2] = 3.0 * w * u2;
    val[3] = u3;

    for (GLuint j=0; j<4; j++)
            sum += _data[j]*val[j];
    d(0) = sum;

    if (max_order_of_derivatives > 0)
        {
            sum[0] = sum[1] = sum[2] = 0.0;

            val[0] = -3.0 * (u - 1) * (u - 1);
            val[1] = 9 * u2 - 12 * u + 3;
            val[2] = 3.0 * (2 - 3 * u) * u;
            val[3] = 2 * u2;

            for (GLuint j=0; j<4; j++)
                sum += _data[j]*val[j];
            d(1) = sum;
        }

        if (max_order_of_derivatives > 1)
        {
            sum[0] = sum[1] = sum[2] = 0.0;

            val[0] = 6.0 - 6.0 * u;
            val[1] = 6.0 * (3.0 * u - 2.0);
            val[2] = 6.0 - 18.0 * u;
            val[3] = 4.0 * u;

            for (GLuint j=0; j<4; j++)
                sum += _data[j]*val[j];
            d(2) = sum;
        }

        // VAJON KELL 3-ad rendu derivalt ??? IDK
    return GL_TRUE;
}
