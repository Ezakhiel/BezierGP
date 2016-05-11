#include "GLWidget.h"
#include <iostream>
#include "Core/Matrices.h"
#include "Test/TestFunctions.h"
#include "Core/Materials.h"
#include "Core/Constants.h"
#include "Core/CyclicCurves3.h"


using namespace std;
using namespace cagd;

//--------------------------------
// special and default constructor
//--------------------------------
GLWidget::GLWidget(QWidget *parent, const QGLFormat &format): QGLWidget(format, parent)
        //GLWidget::GLWidget(QWidget *parent): QGLWidget(parent)
{
    _timer = new QTimer(this);
    _timer->setInterval(0);

    connect(_timer, SIGNAL(timeout()), this , SLOT(_animate()));
}

//--------------------------------------------------------------------------------------
// this virtual function is called once before the first call to paintGL() or resizeGL()
//--------------------------------------------------------------------------------------
void GLWidget::initializeGL()
{
    // creating a perspective projection matrix
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    _aspect = (float)width() / (float)height();
    _z_near = 1.0;
    _z_far = 1000.0;
    _fovy = 45.0;

    gluPerspective(_fovy, _aspect, _z_near, _z_far);

    // setting the model view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    _eye[0] = _eye[1] = 0.0, _eye[2] = 6.0;
    _center[0] = _center[1] = _center[2] = 0.0;
    _up[0] = _up[2] = 0.0, _up[1] = 1.0;

    gluLookAt(_eye[0], _eye[1], _eye[2], _center[0], _center[1], _center[2], _up[0], _up[1], _up[2]);

    // enabling depth test
    glEnable(GL_DEPTH_TEST);

    // setting the color of background
    glClearColor(0.0, 0.0, 0.0, 1.0);

    // initial values of transformation parameters
    _zoom = 1.0;
    _angle_x = _angle_y = _angle_z = 0.0;
    _trans_x = _trans_y = _trans_z = 0.0;

    // initializing the OpenGL Extension Wrangler library
    glewInit();

    // init light
    HCoordinate3    direction(0.0,0.0,1.0,0.0);
    Color4          ambient(0.4,0.4,0.4,1.0);
    Color4          diffuse(0.8,0.8,0.8,1.0);
    Color4          specular(1.0,1.0,1.0,1.0);
    dl = new DirectionalLight(GL_LIGHT0, direction, ambient, diffuse, specular);
    //if(dl)
    //    delete dl, dl=0;
    // init surfaces

    _ps.resize(5);
    _meshes.resize(8);

    //   _surface_index = 0;
    _meshes[0] = new TriangulatedMesh3();
    if ( _meshes[0]->LoadFromOFF("../QTFramework/Models/mouse.off",true))
        if ( _meshes[0]->UpdateVertexBufferObjects(GL_DYNAMIC_DRAW))
        {
        _angle = 0.0;
        //_timer->start();
    }

    _meshes[1] = new TriangulatedMesh3();
    if ( _meshes[1]->LoadFromOFF("../QTFramework/Models/elephant.off",true))
        if ( _meshes[1]->UpdateVertexBufferObjects(GL_DYNAMIC_DRAW))
        {
        //problem solved
    }


    TriangularMatrix<ParametricSurface3::PartialDerivative> surface_der3(2);
    surface_der3(0,0) = Klein::d00;
    surface_der3(1,0) = Klein::d10;
    surface_der3(1,1) = Klein::d01;
    _ps[0] = new ParametricSurface3(surface_der3,Klein::u_min,Klein::u_max,Klein::v_min,Klein::v_max);
    surface_der3(0,0) = Dupin::d00;
    surface_der3(1,0) = Dupin::d10;
    surface_der3(1,1) = Dupin::d01;
    _ps[1] = new ParametricSurface3(surface_der3,Dupin::u_min,Dupin::u_max,Dupin::v_min,Dupin::v_max);
    surface_der3(0,0) = Cylindrical::d00;
    surface_der3(1,0) = Cylindrical::d10;
    surface_der3(1,1) = Cylindrical::d01;
    _ps[2] = new ParametricSurface3(surface_der3,Cylindrical::u_min,Cylindrical::u_max,Cylindrical::v_min,Cylindrical::v_max);
    surface_der3(0,0) = Hyperboloidal::d00;
    surface_der3(1,0) = Hyperboloidal::d10;
    surface_der3(1,1) = Hyperboloidal::d01;
    _ps[3] = new ParametricSurface3(surface_der3,Hyperboloidal::u_min,Hyperboloidal::u_max,Hyperboloidal::v_min,Hyperboloidal::v_max);

    // cout << surface_der3 << endl;

    GLuint v_point_count = 200;
    GLuint u_point_count = 200;
    GLenum surface_usage_flag = GL_DYNAMIC_DRAW;
    _meshes[2] = _ps[0]->GenerateImage(v_point_count,u_point_count,surface_usage_flag);
    _meshes[3] = _ps[1]->GenerateImage(v_point_count,u_point_count,surface_usage_flag);
    _meshes[4] = _ps[2]->GenerateImage(v_point_count,u_point_count,surface_usage_flag);
    _meshes[5] = _ps[3]->GenerateImage(v_point_count,u_point_count,surface_usage_flag);

    //PROJEKT

    this->setControl();
    this->makeMesh();
    this->solveInterpol();

    _meshes[6] = beforeinter;
    //_meshes[7] = afterinter;

    //PROJEKT

    if (!_meshes[2]||!_meshes[3]||!_meshes[4]||!_meshes[5])
        cout<<"could not generate mesh"<<endl;
    if (!_meshes[2]->UpdateVertexBufferObjects(GL_DYNAMIC_DRAW)||
        !_meshes[3]->UpdateVertexBufferObjects(GL_DYNAMIC_DRAW)||
        !_meshes[4]->UpdateVertexBufferObjects(GL_DYNAMIC_DRAW)||
        !_meshes[5]->UpdateVertexBufferObjects(GL_DYNAMIC_DRAW))
        cout <<"could not update surface vertex"<<endl;

    // initialize curves
    //  _curve_index = 0;
    _pc.resize(5);
    _ipc.resize(5);
    RowMatrix<ParametricCurve3::Derivative> derivative3(3);
    RowMatrix<ParametricCurve3::Derivative> derivative2(2);

    derivative3(0) = spiral_on_cone::d0;
    derivative3(1) = spiral_on_cone::d1;
    derivative3(2) = spiral_on_cone::d2;
    _pc[0] = new ParametricCurve3(derivative3,spiral_on_cone::u_min,spiral_on_cone::u_max);


    derivative3(0) = lissajou::d0;
    derivative3(1) = lissajou::d1;
    derivative3(2) = lissajou::d2;
    _pc[1] = new ParametricCurve3(derivative3,lissajou::u_min,lissajou::u_max);


    derivative3(0) = hypo::d0;
    derivative3(1) = hypo::d1;
    derivative3(2) = hypo::d2;
    _pc[2] = new ParametricCurve3(derivative3,hypo::u_min,hypo::u_max);


    derivative3(0) = cyclo::d0;
    derivative3(1) = cyclo::d1;
    derivative3(2) = cyclo::d2;
    _pc[3] = new ParametricCurve3(derivative3,cyclo::u_min,cyclo::u_max);



    derivative2(0) = torus::d0;
    derivative2(1) = torus::d1;
    _pc[4] = new ParametricCurve3(derivative2,torus::u_min,torus::u_max);



    cc = new cagd::CyclicCurve3(5);

    int split = 2*5+1;

    GLdouble step = 2*PI/split;

    for (int i = 0; i < split;i++){
        (*cc)[i][0] = sin(i * step);
        (*cc)[i][1] = cos(i * step);
        (*cc)[i][2] = -2.0 + 4.0 * (GLdouble)rand() / (GLdouble)RAND_MAX;//
    }

    cc->UpdateVertexBufferObjectsOfData(GL_STATIC_DRAW);
    //    cc->RenderData(GL_LINE_LOOP);


    //init curve images
    GLuint div_point_count = 200;
    GLenum usage_flag = GL_STATIC_DRAW;
    _ipc.resize(7);
    _ipc[0] = _pc[0]->GenerateImage(div_point_count, usage_flag);


    _ipc[1] = _pc[1]->GenerateImage(div_point_count, usage_flag);


    _ipc[2] = _pc[2]->GenerateImage(div_point_count, usage_flag);


    _ipc[3] = _pc[3]->GenerateImage(div_point_count, usage_flag);


    _ipc[4] = _pc[4]->GenerateImage(div_point_count, usage_flag);

    _ipc[5] = cc->GenerateImage(2,div_point_count,usage_flag);

    if (!_ipc[0] || ! _ipc[1] || ! _ipc[2] || ! _ipc[3] || ! _ipc[4] || ! _ipc[5])
    {
        //handle error
    }
    if (!_ipc[0]->UpdateVertexBufferObjects(usage_flag)
        || !_ipc[1]->UpdateVertexBufferObjects(usage_flag)
        || !_ipc[2]->UpdateVertexBufferObjects(usage_flag)
        || !_ipc[3]->UpdateVertexBufferObjects(usage_flag)
        || !_ipc[4]->UpdateVertexBufferObjects(usage_flag)
        || !_ipc[5]->UpdateVertexBufferObjects(usage_flag))
        {
        std::cout << "Could not create vertex buffer object 0" << std::endl;
    }




}

//-----------------------
// the rendering function
//-----------------------
void GLWidget::paintGL()
{
    // clears the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // stores/duplicates the original model view matrix
    glPushMatrix();

    // applying transformations
    glRotatef(_angle_x, 1.0, 0.0, 0.0);
    glRotatef(_angle_y, 0.0, 1.0, 0.0);
    glRotatef(_angle_z, 0.0, 0.0, 1.0);

    glTranslated(_trans_x, _trans_y, _trans_z);

    glScaled(_zoom, _zoom, _zoom);

    //   cout <<"surface_index:"<<_surface_index<<endl;
    //   cout <<"curve_ind:"<<_curve_index<<endl;
    if (_surface_index > 0)
    {
        glEnable(GL_LIGHTING);

        glEnable(GL_NORMALIZE);

        if(dl)
        {
            dl->Enable();
            MatFBRuby.Apply();
            // cout <<"surface_index:"<<_surface_index<<endl;
            if (_surface_index == 8){
            glEnable(GL_BLEND);
            glDepthMask(GL_FALSE);
            glBlendFunc(GL_SRC_ALPHA,GL_ONE);
            MatFBTurquoise.Apply();
             _meshes[_surface_index-1]->Render();
            glDepthMask(GL_TRUE);
            glDisable(GL_BLEND);
        }else
            if (_meshes[_surface_index-1])
            {

                _meshes[_surface_index-1]->Render();


            }

            dl->Disable();
        }
    }


    //freetheallocatedmemoryofthelightsource

    // render your geometry
    if (_curve_index > 0)
    {
        glDisable(GL_LIGHTING);

        glDisable(GL_NORMALIZE);
        // glEnable(GL_LIGHT0);
        if (_curve_index > 5) // if CyclicCurve
        {
            glPointSize(5.0);
            glColor3f(0.9, 0.9, 0.9);
            cc->RenderData(GL_LINE_LOOP);
            cc->RenderData(GL_POINTS);

        }
        if (_ipc[_curve_index-1])
        {
            glColor3f(1.0, 0.0, 0.0);
            _ipc[_curve_index-1]->RenderDerivatives(0, GL_LINE_STRIP);
            glPointSize(5.0);
            //                _curve_index = 0;
            glColor3f(0.0, 0.5, 0.0);
            _ipc[_curve_index-1]->RenderDerivatives(1,GL_LINES);
            _ipc[_curve_index-1]->RenderDerivatives(1,GL_POINTS);

            glColor3f(0.1, 0.5, 0.9);
            _ipc[_curve_index-1]->RenderDerivatives(2,GL_LINES);
            _ipc[_curve_index-1]->RenderDerivatives(2,GL_POINTS);


        }
    }


    /*  glBegin(GL_TRIANGLES);
            glColor3f(1.0, 0.0, 0.0);
            glVertex3f(1.0, 0.0, 0.0);

            glColor3f(0.0, 1.0, 0.0);
            glVertex3f(0.0, 1.0, 0.0);

            glColor3f(0.0, 0.0, 1.0);
            glVertex3f(0.0, 0.0, 1.0);
        glEnd();*/

    // pops the current matrix stack, replacing the current matrix with the one below it on the stack,
    // i.e., the original model view matrix is restored
    glPopMatrix();
}

//----------------------------------------------------------------------------
// when the main window is resized one needs to redefine the projection matrix
//----------------------------------------------------------------------------
void GLWidget::resizeGL(int w, int h)
{
    // setting the new size of the rendering context
    glViewport(0, 0, w, h);

    // redefining the projection matrix
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    _aspect = (float)w / (float)h;

    gluPerspective(_fovy, _aspect, _z_near, _z_far);

    // switching back to the model view matrix
    glMatrixMode(GL_MODELVIEW);

    updateGL();
}


void GLWidget::_animate()
{
    if (_surface_index > 0)
    {
        GLfloat *vertex = _meshes[_surface_index-1]->MapVertexBuffer(GL_READ_WRITE);
        GLfloat *normal = _meshes[_surface_index-1]->MapNormalBuffer(GL_READ_ONLY);
        _angle += DEG_TO_RADIAN;
        if (_angle >= TWO_PI) _angle -= TWO_PI;

        GLfloat scale = sin(_angle) / 3000.0;
        for (GLuint i = 0; i < _meshes[_surface_index-1]->VertexCount(); i++)
        {
            for (GLuint coordinate = 0; coordinate < 3; ++coordinate , ++vertex , ++normal)
                *vertex += scale * (*normal);
        }

        _meshes[_surface_index-1]->UnmapVertexBuffer();
        _meshes[_surface_index-1]->UnmapNormalBuffer();

        updateGL();
    }
}

//-----------------------------------
// implementation of the public slots
//-----------------------------------

void GLWidget::set_angle_x(int value)
{
    if (_angle_x != value)
    {
        _angle_x = value;
        updateGL();
    }
}

void GLWidget::set_angle_y(int value)
{
    if (_angle_y != value)
    {
        _angle_y = value;
        updateGL();
    }
}

void GLWidget::set_angle_z(int value)
{
    if (_angle_z != value)
    {
        _angle_z = value;
        updateGL();
    }
}

void GLWidget::set_zoom_factor(double value)
{
    if (_zoom != value)
    {
        _zoom = value;
        updateGL();
    }
}

void GLWidget::set_trans_x(double value)
{
    if (_trans_x != value)
    {
        _trans_x = value;
        updateGL();
    }
}

void GLWidget::set_trans_y(double value)
{
    if (_trans_y != value)
    {
        _trans_y = value;
        updateGL();
    }
}

void GLWidget::set_trans_z(double value)
{
    if (_trans_z != value)
    {
        _trans_z = value;
        updateGL();
    }
}

void GLWidget::setCurveIndex(int index)
{
    _curve_index = index;
    _surface_index = 0;
    if (index != 0)
        updateGL();
}

void GLWidget::setSurfaceIndex(int index)
{
    _surface_index = index;
    _curve_index = 0;
    if (index != 0)
        updateGL();
}

void GLWidget::setControl(){
    //define the controlnet of the bicubic Bezier patch
    patch.SetData(0,0,DCoordinate3(-2.0,-2.0,0.0));
    patch.SetData(0,1,DCoordinate3(-2.0,-1.0,0.0));
    patch.SetData(0,2,DCoordinate3(-2.0,1.0,0.0));
    patch.SetData(0,3,DCoordinate3(-2.0,2.0,0.0));

    patch.SetData(1,0,DCoordinate3(-1.0,-2.0,0.0));
    patch.SetData(1,1,DCoordinate3(-1.0,-1.0,2.0));
    patch.SetData(1,2,DCoordinate3(-1.0,1.0,2.0));
    patch.SetData(1,3,DCoordinate3(-1.0,2.0,0.0));

    patch.SetData(2,0,DCoordinate3(1.0,-2.0,0.0));
    patch.SetData(2,1,DCoordinate3(1.0,-1.0,2.0));
    patch.SetData(2,2,DCoordinate3(1.0,1.0,2.0));
    patch.SetData(2,3,DCoordinate3(1.0,2.0,0.0));

    patch.SetData(3,0,DCoordinate3(2.0,-2.0,0.0));
    patch.SetData(3,1,DCoordinate3(2.0,-1.0,0.0));
    patch.SetData(3,2,DCoordinate3(2.0,1.0,0.0));
    patch.SetData(3,3,DCoordinate3(2.0,2.0,0.0));
}

void GLWidget::makeMesh(){
    //generate the mesh of the surface patch
    beforeinter = patch.GenerateImage(30 ,30 ,GL_STATIC_DRAW);
    if ( beforeinter)
        beforeinter->UpdateVertexBufferObjects();
}

void GLWidget::solveInterpol(){
    //define an interpolation problem:
    RowMatrix<GLdouble> uKnotVector(4);
    //1:create a knot vector in u-direction
    uKnotVector(0) = 0.0;
    uKnotVector(1) = 1.0 / 3.0;
    uKnotVector(2) = 2.0 / 3.0;
    uKnotVector(3) = 1.0;
    //2:create a knot vector in v-direction
    ColumnMatrix<GLdouble> vKnotVector(4);
    vKnotVector(0)=0.0;
    vKnotVector(1)=1.0/3.0;
    vKnotVector(2)=2.0/3.0;
    vKnotVector(3)=1.0;
    //3:define a matrix of data points, e.g. set them to the original control points
    Matrix<DCoordinate3> dataPointsToInterpolate(4,4);
    for (GLuint row = 0;row<4;++row)
        for (GLuint column = 0;column<4;++column)
            patch.GetData(row,column,dataPointsToInterpolate(row,column));
    //4:solve the interpolatio nproblem and generate the mesh of the interpolating patch

    if(patch.UpdateDataForInterpolation(uKnotVector,vKnotVector,dataPointsToInterpolate))
    {
        afterinter=patch.GenerateImage(30,30,GL_STATIC_DRAW);
        if(afterinter)
            afterinter->UpdateVertexBufferObjects();
    }
}

