#pragma once

// GLU was removed from Qt starting from version 4.8

#include <GL/glew.h>
#include <QGLWidget>
#include <QGLFormat>
#include <QTimer>
#include <vector>

#include "../Parametric/ParametricCurves3.h"
#include "../Parametric/ParametricSurfaces3.h"
#include "../Core/GenericCurves3.h"
#include "../Core/TriangulatedMeshes3.h"
#include "../Core/Lights.h"
#include "../Core/Colors4.h"
#include "../Core/HCoordinates3.h"
#include "../Core/CyclicCurves3.h"
#include "../Bezier/BicubicBezierPatch.h"
#include "../Bezier/BicubicBezierIsoLine.h"

class GLWidget: public QGLWidget
{
    Q_OBJECT

private:

    // variables defining the projection matrix
    float       _aspect;            // aspect ratio of the rendering window
    float       _fovy;              // field of view in direction y
    float       _z_near, _z_far;    // distance of near and far clipping planes

    // variables defining the model-view matrix
    float       _eye[3], _center[3], _up[3];

    // variables needed by transformations
    int         _angle_x, _angle_y, _angle_z;
    double      _zoom;
    double      _trans_x, _trans_y, _trans_z;

    std::vector<cagd::ParametricCurve3*> _pc;
    cagd::CyclicCurve3 *cc;
    std::vector<cagd::GenericCurve3*> _ipc;
    int _curve_index;

    std::vector<cagd::ParametricSurface3*> _ps;
    int _surface_index;

    QTimer *_timer;
    GLfloat _angle;
    std::vector<cagd::TriangulatedMesh3*> _meshes;

    cagd::DirectionalLight *dl;

    //PROJECT

    // a bicubic Bezier patches
    cagd::BicubicBezierPatch patch;
    // t r i a n g u l a t e d meshes
	bool    _show_derivatives;
    cagd::TriangulatedMesh3* beforeinter, *afterinter;
	
	cagd::TriangulatedMesh3 *_t_mesh;
	cagd::TriangulatedMesh3 *_r_mesh;
	cagd::TriangulatedMesh3 *_b_mesh;
	
	
	GLuint _u_isoline_count;
    GLuint _v_isoline_count;

    bool    _t_enabled;
    bool    _r_enabled;
    bool    _b_enabled;
    bool    _show_u_iso_lines;
    bool    _show_v_iso_lines;
    bool    _show_u_iso_derivates;
    bool    _show_v_iso_derivates;
    //project end

private slots:
    void _animate();

public:
    // special and default constructor
    // the format specifies the properties of the rendering window
    GLWidget(QWidget* parent = 0, const QGLFormat& format = QGL::Rgba | QGL::DepthBuffer | QGL::DoubleBuffer);

    // redeclared virtual functions
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    //projekt
    void setControl();
    void makeMesh();
    void solveInterpol();

public slots:
    // public event handling methods/slots
    void set_angle_x(int value);
    void set_angle_y(int value);
    void set_angle_z(int value);

    void set_zoom_factor(double value);
    void setCurveIndex(int index);
    void setSurfaceIndex(int index);
    void set_trans_x(double value);
    void set_trans_y(double value);
    void set_trans_z(double value);
	
    void toggle_t(bool checked);
    void toggle_r(bool checked);
    void toggle_b(bool checked);
	
    void toggle_derivatives(bool enabled);
	
    void toggle_iso_u(bool checked);
    void toggle_iso_v(bool checked);

    void set_iso_u_div_count(int value);
    void set_iso_v_div_count(int value);

    void toggle_iso_u_der(bool checked);
    void toggle_iso_v_der(bool checked);
};
