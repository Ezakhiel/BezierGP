#pragma once

#include "../core/DCoordinates3.h"

namespace cagd
{
    namespace spiral_on_cone
    {
        extern GLdouble u_min, u_max;

        DCoordinate3 d0(GLdouble);
        DCoordinate3 d1(GLdouble);
        DCoordinate3 d2(GLdouble);
    }

    namespace torus
    {
        extern GLdouble u_min, u_max;

        DCoordinate3 d0(GLdouble);
        DCoordinate3 d1(GLdouble);
    }

    namespace lissajou
    {
        extern GLdouble u_min, u_max;

        DCoordinate3 d0(GLdouble);
        DCoordinate3 d1(GLdouble);
        DCoordinate3 d2(GLdouble);
    }

    namespace hypo
    {
        extern GLdouble u_min, u_max;

        DCoordinate3 d0(GLdouble);
        DCoordinate3 d1(GLdouble);
        DCoordinate3 d2(GLdouble);
    }

    namespace cyclo
    {
        extern GLdouble u_min, u_max;

        DCoordinate3 d0(GLdouble);
        DCoordinate3 d1(GLdouble);
        DCoordinate3 d2(GLdouble);
    }

    namespace ellipse
    {
        extern GLdouble u_min, u_max;

        DCoordinate3 d0(GLdouble);
    }

    namespace helix
    {
        extern GLdouble u_min, u_max;

        DCoordinate3 d0(GLdouble);
    }

    namespace Klein
        {
            // possible shape parameters
            extern GLdouble m, a, b, c;

            // definition domain
            extern GLdouble u_min, u_max,
                            v_min, v_max;

            DCoordinate3 d00(GLdouble, GLdouble ); // zeroth order partial derivative, i.e. surface point

            DCoordinate3 d10(GLdouble, GLdouble ); // first order partial derivative in direction u

            DCoordinate3 d01(GLdouble, GLdouble ); // first order partial derivative in direction v
        }
    namespace Dupin
        {
            // possible shape parameters
           // extern GLdouble parameter_1, parameter_2, ...;

            // definition domain
            extern GLdouble u_min, u_max,
                            v_min, v_max;

            DCoordinate3 d00(GLdouble, GLdouble ); // zeroth order partial derivative, i.e. surface point

            DCoordinate3 d10(GLdouble, GLdouble ); // first order partial derivative in direction u

            DCoordinate3 d01(GLdouble, GLdouble ); // first order partial derivative in direction v
        }
    namespace Cylindrical
        {
            // possible shape parameters
           // extern GLdouble parameter_1, parameter_2, ...;

            // definition domain
            extern GLdouble u_min, u_max,
                            v_min, v_max;

            DCoordinate3 d00(GLdouble, GLdouble ); // zeroth order partial derivative, i.e. surface point

            DCoordinate3 d10(GLdouble, GLdouble ); // first order partial derivative in direction u

            DCoordinate3 d01(GLdouble, GLdouble ); // first order partial derivative in direction v
        }
    namespace Hyperboloidal
        {
            // possible shape parameters
           // extern GLdouble parameter_1, parameter_2, ...;

            // definition domain
            extern GLdouble u_min, u_max,
                            v_min, v_max;

            DCoordinate3 d00(GLdouble, GLdouble ); // zeroth order partial derivative, i.e. surface point

            DCoordinate3 d10(GLdouble, GLdouble ); // first order partial derivative in direction u

            DCoordinate3 d01(GLdouble, GLdouble ); // first order partial derivative in direction v
        }
}
