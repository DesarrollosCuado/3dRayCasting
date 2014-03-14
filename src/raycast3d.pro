TEMPLATE = app
DEPENDPATH += . ../src/ft ./src/ft ../ft ./ft
INCLUDEPATH += . ../src/ft ./src/ft ../ft ./ft

# Input
HEADERS += raycast3d.h \
    glwidget.h \
    glshaders.h \
    glextensions.h \
    transferfunction.h \
    wfunction.h \
    wtransferfunction.h\
    util.h \
    Math/Interval.h \
    Math/IntPoint3D.h \
    Math/Matrix4x4f.h \
    Math/Vector3Df.h


FORMS += raycast3d.ui wtransferfunction.ui
SOURCES += raycast3d.cpp \
    glwidget.cpp \
    main.cpp \
    glshaders.cpp \
    glextensions.cpp \
    transferfunction.cpp \
    wfunction.cpp \
    wtransferfunction.cpp \
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
