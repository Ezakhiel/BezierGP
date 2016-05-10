#include <cmath>
#include "TestFunctions.h"
#include "../core/Constants.h"

using namespace cagd;
using namespace std;

GLdouble spiral_on_cone::u_min = -6.2831;
GLdouble spiral_on_cone::u_max = +6.2831;

DCoordinate3 spiral_on_cone::d0(GLdouble u)
{
    return DCoordinate3(u * cos(u), u * sin(u), u);
}

DCoordinate3 spiral_on_cone::d1(GLdouble u)
{
    GLdouble c = cos(u), s = sin(u);
    return DCoordinate3(c - u * s, s + u * c, 1.0);
}

DCoordinate3 spiral_on_cone::d2(GLdouble u)
{
    GLdouble c = cos(u), s = sin(u);
    return DCoordinate3(-2.0 * s - u * c, 2.0 * c - u * s, 0);
}

GLdouble torus::u_min = 0.0;
GLdouble torus::u_max = 18.8493;

DCoordinate3 torus::d0(GLdouble u)
{
    return DCoordinate3((2+cos(2*u/3))*cos(u), (2+cos(2*u/3))*sin(u), sin(2*u/3));
}

DCoordinate3 torus::d1(GLdouble u)
{
    return DCoordinate3(-(2+cos(2*u/3))*sin(u)-2/3*sin(2*u/3)*cos(u), (2+cos(2*u/3))*cos(u)-2/3*sin(2*u/3)*sin(u), 2/3*cos(2*u/3));
}

GLdouble lissajou::u_min = -1.0;
GLdouble lissajou::u_max = +1.0;

DCoordinate3 lissajou::d0(GLdouble u)
{
    return DCoordinate3(sin(5*u + 1.5707), sin(4*u), 0);
}

DCoordinate3 lissajou::d1(GLdouble u)
{
    return DCoordinate3(5*cos(5*u + 1.5707), 4*cos(4*u), 0);
}

DCoordinate3 lissajou::d2(GLdouble u)
{
    return DCoordinate3(-25*sin(5*u + 1.5707), -16*sin(4*u), 0);
}

GLdouble hypo::u_min = -3.0;
GLdouble hypo::u_max = +3.0;

DCoordinate3 hypo::d0(GLdouble u)
{
    return DCoordinate3(5*cos(u)+cos(5*u), 5*sin(u)-sin(5*u), 0);
}

DCoordinate3 hypo::d1(GLdouble u)
{
    return DCoordinate3(-5*sin(u)-5*sin(5*u), 5*cos(u)-5*cos(5*u), 0);
}

DCoordinate3 hypo::d2(GLdouble u)
{
    return DCoordinate3(-5*cos(u)-25*cos(5*u), -5*sin(u)+25*sin(5*u), 0);
}

GLdouble cyclo::u_min = 0.0;
GLdouble cyclo::u_max = 125.1324;

DCoordinate3 cyclo::d0(GLdouble u)
{
    return 0.1 * DCoordinate3(2*(u-sin(u)), 2*(1-cos(u)), 0);
}

DCoordinate3 cyclo::d1(GLdouble u)
{
    return 0.1 * DCoordinate3(2*(1-cos(u)), 2*sin(u), 0);
}

DCoordinate3 cyclo::d2(GLdouble u)
{
    return 0.1 * DCoordinate3(2*sin(u), 2*cos(u), 0);
}

GLdouble ellipse::u_min = -6.0;
GLdouble ellipse::u_max = +6.0;

DCoordinate3 ellipse::d0(GLdouble u)
{
    return DCoordinate3(6 * cos(u), 4* sin(u), 0);
}

GLdouble helix::u_min = 0;
GLdouble helix::u_max = 75.39;

DCoordinate3 helix::d0(GLdouble u)
{
    return 0.2* DCoordinate3(cos(u),sin(u), u/7);
}

GLdouble Klein::u_min = 0;
GLdouble Klein::v_min = 0;
GLdouble Klein::u_max = 2*PI;
GLdouble Klein::v_max = 2*PI;
GLdouble Klein::a = 6;
GLdouble Klein::b = 4 * sqrt(2);
GLdouble Klein::c = 2;
GLdouble Klein::m = 3;

DCoordinate3 Klein::d00(GLdouble u, GLdouble v) // zeroth order partial derivative, i.e. surface point
{
    return DCoordinate3((m*(c-a*cos(u)*cos(v))+b*b*cos(u))/(a-c*cos(u)*cos(v)),
                        (b*sin(u)*(a-m*cos(v)))/(a-c*cos(u)*cos(v)),
                        (b*sin(v)*(c*cos(u)-m))/(a-c*cos(u)*cos(v)));
}
DCoordinate3 Klein::d10(GLdouble u, GLdouble v) // first order partial derivative in direction u
{ // TODO
    return DCoordinate3(-((c*c-a*a)*m*cos(v)+a*b*b)*sin(u)/((c*cos(v)*cos(u)-a)*(c*cos(v)*cos(u)-a)),
                        -b*(m*cos(v)-a)*(a*cos(u)-c*cos(v))/((c*cos(v)*cos(u)-a)*(c*cos(v)*cos(u)-a)),
                        b*c*(m*cos(v)-a)*sin(v)*sin(u)/((c*cos(v)*cos(u)-a)*(c*cos(v)*cos(u)-a)));
}
DCoordinate3 Klein::d01(GLdouble u, GLdouble v) // first order partial derivative in direction v
{//TODO
    return DCoordinate3(-cos(u)*(b*b*c*cos(u)+(c*c-a*a)*m)*sin(v)/((c*cos(u)*cos(v)-a)*(c*cos(u)*cos(v)-a)),
                        -a*b*(c*cos(u)-m)*sin(u)*sin(v)/((c*cos(u)*cos(v)-a)*(c*cos(u)*cos(v)-a)),
                        b*(c*cos(u)-m)*(a*cos(v)-c*cos(u))/((c*cos(u)*cos(v)-a)*(c*cos(u)*cos(v)-a)));
}

GLdouble Dupin::u_min = 0;
GLdouble Dupin::v_min = 0;
GLdouble Dupin::u_max = 2*PI;
GLdouble Dupin::v_max = 2*PI;

DCoordinate3 Dupin::d00(GLdouble u, GLdouble v) // zeroth order partial derivative, i.e. surface point
{
    return DCoordinate3(3*cos(u)+(1.0/2)*(1+ cos(2*u))*sin(v)-(1.0/2)*sin(2*u)*sin(2*v),
                        3*sin(u)+(1.0/2)*sin(2*u)*sin(v)-(1.0/2)*(1-cos(2*u))*sin(2*v),
                        cos(u)*sin(2*v)+sin(u)*sin(v));
}
DCoordinate3 Dupin::d10(GLdouble u, GLdouble v) // first order partial derivative in direction u
{
    return DCoordinate3((-sin(v)*sin(2*u))-sin(2*v)*cos(2*u)-3*sin(u),
                        (-sin(2*v)*sin(2*u))+sin(v)*cos(2*u)+3*cos(u),
                        sin(v)*cos(u)-sin(2*v)*sin(u));
}
DCoordinate3 Dupin::d01(GLdouble u, GLdouble v) // first order partial derivative in direction v
{
    return DCoordinate3(((cos(2*u)+1)*cos(v))/2-sin(2*u)*cos(2*v),
                        (sin(2*u)*cos(v))/2-(1-cos(2*u))*cos(2*v),
                        2*cos(u)*cos(2*v)+sin(u)*cos(v));
}




GLdouble Cylindrical::u_min = 0;
GLdouble Cylindrical::v_min = 0;
GLdouble Cylindrical::u_max = 2;
GLdouble Cylindrical::v_max = (2*PI)*2;

DCoordinate3 Cylindrical::d00(GLdouble u, GLdouble v) // zeroth order partial derivative, i.e. surface point
{
    return DCoordinate3((2+u)*cos(v),
                        (2+u)*sin(v),
                        v);
}
DCoordinate3 Cylindrical::d10(GLdouble u, GLdouble v) // first order partial derivative in direction u
{
    return DCoordinate3(cos(v),
                        sin(v),
                        0);
}
DCoordinate3 Cylindrical::d01(GLdouble u, GLdouble v) // first order partial derivative in direction v
{
    return DCoordinate3(-(u+2)*sin(v),
                        (u+2)*cos(v),
                        1);
}

GLdouble Hyperboloidal::u_min = 0;
GLdouble Hyperboloidal::v_min = 0;
GLdouble Hyperboloidal::u_max = 3;
GLdouble Hyperboloidal::v_max = 2*PI;

DCoordinate3 Hyperboloidal::d00(GLdouble u, GLdouble v) // zeroth order partial derivative, i.e. surface point
{
    return DCoordinate3((1+cosh(u-3/2.0))*sin(v),
                        (1+cosh(u-3/2.0))*cos(v),
                        sinh(u-3/2.0));
}
DCoordinate3 Hyperboloidal::d10(GLdouble u, GLdouble v) // first order partial derivative in direction u
{
    return DCoordinate3(sin(v)*sinh(u-3/2.0),
                        cos(v)*sinh(u-3/2.0),
                        cosh(u-3/2.0));
}
DCoordinate3 Hyperboloidal::d01(GLdouble u, GLdouble v) // first order partial derivative in direction v
{
    return DCoordinate3((cosh(u-3/2.0)+1)*cos(v),
                        -(cosh(u-3/2.0)+1)*sin(v),
                        0);
}

