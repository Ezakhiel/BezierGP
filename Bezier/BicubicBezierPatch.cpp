#include "BicubicBezierPatch.h"
#include "BicubicBezierIsoLine.h"
#include <iostream>
#include "../core/Materials.h"

using namespace std;

using namespace cagd;

BicubicBezierPatch::BicubicBezierPatch(): TensorProductSurface3(0.0, 1.0, 0.0, 1.0, 4, 4)
{

}

GLboolean BicubicBezierPatch::UBlendingFunctionValues(
        GLdouble u_knot, RowMatrix<GLdouble>& blending_values) const
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

GLboolean BicubicBezierPatch::VBlendingFunctionValues(
        GLdouble v_knot, RowMatrix<GLdouble>& blending_values) const
{
    if (v_knot < 0.0 || v_knot > 1.0)
        return GL_FALSE;

    blending_values.ResizeColumns(4);

    GLdouble v = v_knot, v2 = v * v, v3 = v2 * v , w = 1.0 - v, w2 = w * w, w3 = w2 * w;

    blending_values(0) = w3;
    blending_values(1) = 3.0 * w2 * v;
    blending_values(2) = 3.0 * w * v2;
    blending_values(3) = v3;

    cout << blending_values << endl;

    return GL_TRUE;
}

GLboolean BicubicBezierPatch::CalculatePartialDerivatives(
        GLdouble u, GLdouble v, PartialDerivatives& pd) const
{
    if ( u < 0.0 || u > 1.0 || v < 0.0 || v > 1.0)
        return GL_FALSE;

    RowMatrix<GLdouble> u_blending_values(4), d1_u_blending_values(4);

    GLdouble u2 = u * u, u3 = u2 * u, wu = 1.0 - u, wu2= wu * wu, wu3 = wu2 * wu;

    u_blending_values(0)=wu3;
    u_blending_values(1)=3.0 * wu2 * u;
    u_blending_values(2)=3.0 * wu *u2;
    u_blending_values(3)=u3;

    d1_u_blending_values(0) = -3.0 * wu2;
    d1_u_blending_values(1) = -6.0 * wu * u + 3.0 * wu2;
    d1_u_blending_values(2) = -3.0 * u2 + 6.0 * wu * u;
    d1_u_blending_values(3) = 3.0 * u2;

    RowMatrix<GLdouble> v_blending_values(4), d1_v_blending_values(4);

    GLdouble v2 = v * v, v3= v2 * v, wv = 1.0 - v, wv2 = wv * wv, wv3 = wv2 * wv;

    v_blending_values(0) = wv3;
    v_blending_values(1) = 3.0 * wv2 * v;
    v_blending_values(2) = 3.0 * wv *v2;
    v_blending_values(3) = v3;

    d1_v_blending_values(0) = -3.0 * wv2;
    d1_v_blending_values(1) = -6.0 * wv * v + 3.0 * wv2;
    d1_v_blending_values(2) = -3.0 * v2 + 6.0 * wv * v;
    d1_v_blending_values(3) = 3.0 * v2;

    pd.LoadNullVectors();
    for ( GLuint row = 0; row < 4; ++row)
    {
        DCoordinate3 aux_d0_v, aux_d1_v;
        for (GLuint column = 0; column < 4; ++column)
        {
            aux_d0_v += _data(row, column) * v_blending_values(column);
            aux_d1_v += _data(row, column) * d1_v_blending_values(column);
        }
        pd.point  += aux_d0_v * u_blending_values(row);
        pd.diff1u += aux_d0_v * d1_u_blending_values(row);
        pd.diff1v += aux_d1_v * u_blending_values(row);
    }

    return GL_TRUE;
}

GLvoid BicubicBezierPatch::DeleteVertexBufferObjectsOfData()
{
    if (_vbo_data)
    {
        glDeleteBuffers(1, &_vbo_data);
        _vbo_data = 0;
    }
}

GLboolean BicubicBezierPatch::RenderData(GLenum render_mode) const
{
	//call renders
    return GL_TRUE;
}

GLboolean BicubicBezierPatch::UpdateVertexBufferObjectsOfData(GLenum usage_flag)
{
	//updateVBO
    return GL_TRUE;
}

GLvoid BicubicBezierPatch::DeleteVertexBufferObjectsOfDerivatives()
{
    if (_vbo_derivatives)
    {
        glDeleteBuffers(1, &_vbo_derivatives);
        _vbo_derivatives = 0;
    }

    if (_ibo_derivatives)
    {
        glDeleteBuffers(1, &_ibo_derivatives);
        _ibo_derivatives = 0;
    }
}

void BicubicBezierPatch::push3f(GLfloat *&coordinate, const DCoordinate3 &data)
{
    coordinate[0] = data.x();
    coordinate[1] = data.y();
    coordinate[2] = data.z();
   // coordinate += 3;
}

void BicubicBezierPatch::push1us(GLushort *&coordinate, const GLushort data)
{
    *coordinate = data;
  //  ++coordinate;
}

GLboolean BicubicBezierPatch::UpdateVertexBufferObjectsOfDerivatives()
{
    DeleteVertexBufferObjectsOfDerivatives();

    glGenBuffers(1, &_vbo_derivatives);

    if (!_vbo_derivatives)
    {
        DeleteVertexBufferObjectsOfDerivatives();
        return GL_FALSE;
    }

    glBindBuffer(GL_ARRAY_BUFFER, _vbo_derivatives);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 16 * 3, 0, GL_STATIC_DRAW);

    GLfloat *coordinate = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    if (!coordinate)
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        DeleteVertexBufferObjectsOfDerivatives();
        return GL_FALSE;
    }

    // corner points
    push3f(coordinate, _data(0, 0));
    push3f(coordinate, _data(0, 1));
    push3f(coordinate, _data(1, 0));
    push3f(coordinate, _data(1, 1));


    // first order derivatives in v direction
    push3f(coordinate, _data(0, 0) + _data(0, 2));
    push3f(coordinate, _data(0, 1) + _data(0, 3));
    push3f(coordinate, _data(1, 0) + _data(1, 2));
    push3f(coordinate, _data(1, 1) + _data(1, 3));

    // first order derivatives in u direction
    push3f(coordinate, _data(0, 0) + _data(2, 0));
    push3f(coordinate, _data(0, 1) + _data(2, 1));
    push3f(coordinate, _data(1, 0) + _data(3, 0));
    push3f(coordinate, _data(1, 1) + _data(3, 1));

    // twist vectors
    push3f(coordinate, _data(0, 0) + _data(2, 2));
    push3f(coordinate, _data(0, 1) + _data(2, 3));
    push3f(coordinate, _data(1, 0) + _data(3, 2));
    push3f(coordinate, _data(1, 1) + _data(3, 3));


    if (!glUnmapBuffer(GL_ARRAY_BUFFER))
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        DeleteVertexBufferObjectsOfDerivatives();
        return GL_FALSE;
    }

    // index buffer

    glGenBuffers(1, &_ibo_derivatives);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo_derivatives);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 24, 0,
        GL_STATIC_DRAW);

    GLushort *ind = (GLushort*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);

    // v
    push1us(ind, 0);
    push1us(ind, 4);

    push1us(ind, 1);
    push1us(ind, 5);

    push1us(ind, 2);
    push1us(ind, 6);

    push1us(ind, 3);
    push1us(ind, 7);

    // u
    push1us(ind, 0);
    push1us(ind, 8);

    push1us(ind, 1);
    push1us(ind, 9);

    push1us(ind, 2);
    push1us(ind, 10);

    push1us(ind, 3);
    push1us(ind, 11);

    // t
    push1us(ind, 0);
    push1us(ind, 12);

    push1us(ind, 1);
    push1us(ind, 13);

    push1us(ind, 2);
    push1us(ind, 14);

    push1us(ind, 3);
    push1us(ind, 15);

    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

    return GL_TRUE;
}



GLboolean BicubicBezierPatch::RenderDerivatives()
{

/*
    MatFBEmerald.Apply();
    glEnableClientState(GL_VERTEX_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo_derivatives);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo_derivatives);
            glVertexPointer(3, GL_FLOAT, 0, (const GLvoid *)0);

            glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableClientState(GL_VERTEX_ARRAY);


    MatFBPearl.Apply();

    glEnableClientState(GL_VERTEX_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo_derivatives);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo_derivatives);
            glVertexPointer(3, GL_FLOAT, 0, (const GLvoid *)0);

            glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (char *)NULL + 8 * sizeof(GLushort));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableClientState(GL_VERTEX_ARRAY);

    MatFBRuby.Apply();

    glEnableClientState(GL_VERTEX_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo_derivatives);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo_derivatives);
            glVertexPointer(3, GL_FLOAT, 0, (const GLvoid *)0);

            glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (char *)NULL + 16 * sizeof(GLushort));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableClientState(GL_VERTEX_ARRAY);
	
    return GL_TRUE;
    */


}

BicubicBezierPatch::~BicubicBezierPatch()
{
    DeleteVertexBufferObjectsOfDerivatives();
}

void BicubicBezierPatch::ExtendT(Matrix<DCoordinate3> data)
{
    _t = new BicubicBezierPatch;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            _t->SetData(i, j, data(i,j));
}

void BicubicBezierPatch::ExtendR(Matrix<DCoordinate3> data)
{
    _r = new BicubicBezierPatch;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            _r->SetData(i, j, data(i,j));
}

void BicubicBezierPatch::ExtendB(Matrix<DCoordinate3> data)
{
    _b = new BicubicBezierPatch;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            _b->SetData(i, j, data(i,j));
}

BicubicBezierPatch* BicubicBezierPatch::GetT()
{
    return _t;
}

BicubicBezierPatch* BicubicBezierPatch::GetR()
{
    return _r;
}

BicubicBezierPatch* BicubicBezierPatch::GetB()
{
    return _b;
}

void BicubicBezierPatch::DeleteT()
{
    _t->DeleteVertexBufferObjectsOfDerivatives();
    _t->DeleteVertexBufferObjectsOfData();
    delete _t;
}

void BicubicBezierPatch::DeleteR()
{
    _r->DeleteVertexBufferObjectsOfDerivatives();
    _r->DeleteVertexBufferObjectsOfData();
    delete _r;
}

void BicubicBezierPatch::DeleteB()
{
    _b->DeleteVertexBufferObjectsOfDerivatives();
    _b->DeleteVertexBufferObjectsOfData();
    delete _b;
}

GenericCurve3* BicubicBezierPatch::GenerateVIsoLine(GLdouble u_t, GLuint div_point_count, GLuint max_order_of_derivates, GLenum usage_flag)
{
    BicubicBezierIsoLine * arc = new BicubicBezierIsoLine(usage_flag);

    RowMatrix<GLdouble> u_t_functionvalues;
    arc->BlendingFunctionValues(u_t, u_t_functionvalues);

    for (GLuint j=0; j<4; j++)
    {
        DCoordinate3 sum;
        for (GLuint i=0; i<4; i++)

            sum = sum + _data(i, j)*u_t_functionvalues(i);
        (*arc)[j] = sum;
    }

    GenericCurve3* result = arc->GenerateImage(max_order_of_derivates, div_point_count, usage_flag);

    delete arc; arc = 0;

    return result;
}

GenericCurve3* BicubicBezierPatch::GenerateUIsoLine(GLdouble v_t, GLuint div_point_count, GLuint max_order_of_derivates, GLenum usage_flag)
{
    BicubicBezierIsoLine * arc = new BicubicBezierIsoLine(usage_flag);

    RowMatrix<GLdouble> v_t_functionvalues;
    arc->BlendingFunctionValues(v_t, v_t_functionvalues);

    for (GLuint i=0; i<4; i++)
    {
        DCoordinate3 sum;
        for (GLuint j=0; j<4; j++)
            sum += _data(i, j)*v_t_functionvalues(j);
        (*arc)[i] = sum;
    }

    GenericCurve3* result = arc->GenerateImage(max_order_of_derivates, div_point_count, usage_flag);

    delete arc; arc = 0;

    return result;
}

GLvoid BicubicBezierPatch::GenerateUIsoLines(GLuint line_count, GLuint div_point_count, GLuint max_order_of_derivates, GLenum usage_flag)
{
    u_iso_lines_count = line_count;
    u_iso_lines.clear();
    u_iso_lines.resize(u_iso_lines_count);
    for (GLuint i=0; i<u_iso_lines_count; ++i)
    {
        u_iso_lines[i] = this->GenerateUIsoLine((GLdouble) (i+1) / (u_iso_lines_count+1), div_point_count, max_order_of_derivates, usage_flag);
        if (!u_iso_lines[i]->UpdateVertexBufferObjects())
            cout << "vbo error" << endl;
    }
}

GLvoid BicubicBezierPatch::GenerateVIsoLines(GLuint line_count, GLuint div_point_count, GLuint max_order_of_derivates, GLenum usage_flag)
{
    v_iso_lines_count = line_count;
    v_iso_lines.clear();
    v_iso_lines.resize(v_iso_lines_count);
    for (GLuint i=0; i<v_iso_lines_count; ++i)
    {
        v_iso_lines[i] = this->GenerateVIsoLine((GLdouble) (i+1) / (v_iso_lines_count+1), div_point_count, max_order_of_derivates, usage_flag);
        if (!v_iso_lines[i]->UpdateVertexBufferObjects())
            cout << "vbo error" << endl;
    }
}

GLvoid BicubicBezierPatch::RenderUIsoLines(GLuint order)
{
    glDisable(GL_LIGHTING);
    glColor3f(0.1, 0.5, 0.9);
    glPointSize(5.0);
    glLineWidth(2.0);
    for (GLuint i=0; i<u_iso_lines_count; ++i)
    {
        if (order == 0)
            u_iso_lines[i]->RenderDerivatives(order, GL_LINE_STRIP);
        else
            u_iso_lines[i]->RenderDerivatives(order, GL_LINES);
    }

    glEnable(GL_LIGHTING);
}

GLvoid BicubicBezierPatch::RenderVIsoLines(GLuint order)
{
    glDisable(GL_LIGHTING);
    glColor3f(0.1, 0.5, 0.9);
    glPointSize(5.0);
    glLineWidth(2.0);
    for (GLuint i=0; i<v_iso_lines_count; ++i)
    {
        if (order == 0)
            v_iso_lines[i]->RenderDerivatives(order, GL_LINE_STRIP);
        else
            v_iso_lines[i]->RenderDerivatives(order, GL_LINES);
    }
	
    glEnable(GL_LIGHTING);
}

GLvoid BicubicBezierPatch::RenderNet()
{
    glDisable(GL_LIGHTING);
    glColor3f(0.0, 0.5, 0.0);
    glPointSize(5.0);
    glLineWidth(2.0);
}
