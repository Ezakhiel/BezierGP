SOURCES += \
    main.cpp \
    GUI/MainWindow.cpp \
    GUI/GLWidget.cpp \
    GUI/SideWidget.cpp \
    Core/RealSquareMatrices.cpp \
    Test/TestFunctions.cpp \
    Core/LinearCombination3.cpp \
    Core/GenericCurves3.cpp \
    Parametric/ParametricCurves3.cpp \
    Core/TriangulatedMeshes3.cpp \
    Core/Lights.cpp \
    Core/Materials.cpp \
    Core/CyclicCurves3.cpp \
    Parametric/ParametricSurfaces3.cpp \
    Core/TensorProductSurfaces3.cpp \
    Bezier/BicubicBezierPatch.cpp \
    Shader/ShaderPrograms.cpp \
    Bezier/BicubicBezierCompositeSurfaces3.cpp

HEADERS += \
    GUI/MainWindow.h \
    GUI/GLWidget.h \
    GUI/SideWidget.h \
    Core/Matrices.h \
    Core/DCoordinates3.h \
    Core/RealSquareMatrices.h \
    Test/TestFunctions.h \
    Core/LinearCombination3.h \
    Core/GenericCurves3.h \
    Core/Constants.h \
    Parametric/ParametricCurves3.h \
    Core/TriangulatedMeshes3.h \
    Core/TriangularFaces.h \
    Core/TCoordinates4.h \
    Core/Lights.h \
    Core/HCoordinates3.h \
    Core/Exceptions.h \
    Core/Colors4.h \
    Core/Materials.h \
    Core/CyclicCurves3.h \
    Parametric/ParametricSurfaces3.h \
    Core/TensorProductSurfaces3.h \
    Bezier/BicubicBezierPatch.h \
    Bezier/BicubicBezierCompositeSurfaces3.h \
    Shader/ShaderPrograms.h

FORMS += \
    GUI/MainWindow.ui \
    GUI/SideWidget.ui \

QT += opengl

CONFIG += console

INCLUDEPATH += ./Dependencies/Include
LIBS += ./Dependencies/Lib/GL/glew32.dll ./Dependencies/Lib/GL/glew32.lib

OTHER_FILES += \
    Models/mouse.off \
    Models/elephant.off
