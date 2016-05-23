#pragma once

#include "../core/TensorProductSurfaces3.h"
#include "../core/GenericCurves3.h"
#include <vector>

namespace cagd
{
    class BicubicBezierPatch: public TensorProductSurface3
    {
    private:
        GLuint  _vbo_derivatives;
        GLuint  _ibo_derivatives;

	//iso-lines
	std::vector<cagd::GenericCurve3*> u_iso_lines, v_iso_lines;
	GLuint u_iso_lines_count, v_iso_lines_count;
	
	// copy 3 float to a pointer location
        void    push3f(GLfloat *&coordinate, const DCoordinate3 &data);
	// copy 1 unsigned short int to a pointer location
        void    push1us(GLushort *&coordinate, const GLushort data);
	//neighbours
	BicubicBezierPatch *_t;
	BicubicBezierPatch *_r;
	BicubicBezierPatch *_b;
	
    public:
        BicubicBezierPatch();
	
        GLboolean UBlendingFunctionValues(GLdouble u_know, RowMatrix<GLdouble>& blending_values) const;
        GLboolean VBlendingFunctionValues(GLdouble v_know, RowMatrix<GLdouble>& blending_values) const;
        GLboolean CalculatePartialDerivatives(GLdouble u,GLdouble v, PartialDerivatives& pd) const;

        GLvoid    DeleteVertexBufferObjectsOfData();
        GLboolean RenderData(GLenum render_mode = GL_LINE_STRIP) const;
        GLboolean UpdateVertexBufferObjectsOfData(GLenum usage_flag = GL_STATIC_DRAW) ;

        GLvoid    DeleteVertexBufferObjectsOfDerivatives();
        GLboolean UpdateVertexBufferObjectsOfDerivatives();
        GLboolean RenderDerivatives();

        BicubicBezierPatch* GetT();
        BicubicBezierPatch* GetR();
        BicubicBezierPatch* GetB();
        void    ExtendT(Matrix<DCoordinate3> data);
        void    ExtendR(Matrix<DCoordinate3> data);
        void    ExtendB(Matrix<DCoordinate3> data);
        void DeleteT();
        void DeleteR();
        void DeleteB();

        ~BicubicBezierPatch();

        //isoLINES
        GenericCurve3* GenerateVIsoLine(GLdouble u_t, GLuint div_point_count, GLuint max_order_of_derivates=2, GLenum usage_flag = GL_STATIC_DRAW);
        GenericCurve3* GenerateUIsoLine(GLdouble v_t, GLuint div_point_count, GLuint max_order_of_derivates=2, GLenum usage_flag = GL_STATIC_DRAW);

        GLvoid GenerateUIsoLines(GLuint line_count,GLuint div_point_count, GLuint max_order_of_derivates=2, GLenum usage_flag = GL_STATIC_DRAW);
        GLvoid GenerateVIsoLines(GLuint line_count,GLuint div_point_count, GLuint max_order_of_derivates=2, GLenum usage_flag = GL_STATIC_DRAW);
        GLvoid RenderUIsoLines(GLuint order);
        GLvoid RenderVIsoLines(GLuint order);

        GLvoid RenderNet();


    };
}
