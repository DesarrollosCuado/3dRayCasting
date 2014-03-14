# #####################################################################
# Automatically generated by qmake (2.01a) mar mar 24 01:02:56 2009
# #####################################################################
TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += raycast3d.h \
    glwidget.h \
    glshaders.h \
    glextensions.h \
    Math/Interval.h \
    Math/IntPoint3D.h \
    Math/Matrix4x4f.h \
    Math/Vector3Df.h \
    Math/Vector4Df.h
FORMS += raycast3d.ui
SOURCES += raycast3d.cpp \
    glwidget.cpp \
    main.cpp \
    glshaders.cpp \
    glextensions.cpp \
    Math/Interval.cpp \
    Math/IntPoint3D.cpp \
    Math/Matrix4x4f.cpp \
    Math/Vector3Df.cpp \
    Math/Vector4Df.cpp
QT += opengl
OTHER_FILES += shaders/raycasting.fsh shaders/raycasting.vsh

unix:LIBS += -lGLU

RESOURCES += \
    files.qrc
