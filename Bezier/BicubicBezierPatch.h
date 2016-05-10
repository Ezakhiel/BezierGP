#pragma once

#include "../core/TensorProductSurfaces3.h"

namespace cagd
{
    class BicubicBezierPatch: public TensorProductSurface3
    {
    public:
        BicubicBezierPatch();

        GLboolean UBlendingFunctionValues(GLdouble u_know, RowMatrix<GLdouble>& blending_values) const;
        GLboolean VBlendingFunctionValues(GLdouble v_know, RowMatrix<GLdouble>& blending_values) const;
        GLboolean CalculatePartialDerivatives(GLdouble u,GLdouble v, PartialDerivatives& pd) const;
    };
}
