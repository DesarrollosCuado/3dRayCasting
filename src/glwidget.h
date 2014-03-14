#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <algorithm>
#ifdef __unix__
    #include <GL/glext.h>
#endif
#include <GL/glu.h>

#include <QtGui>
#include <QtOpenGL>

#include "glextensions.h"
#include "glshaders.h"

#include "Math/Vector3Df.h"
#include "Math/Matrix4x4f.h"

using namespace std;

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    GLFragmentShader *fs;

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void resizeGL(int width, int height);

    void setXTranslation(int distance);
    void setYTranslation(int distance);
    void setZTranslation(int distance);

protected:
    void initializeGL();
    void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    QPoint lastPos;
    GLuint primitiveList;
    CMatrix4x4f m_rotMatrix;
    CMatrix4x4f m_rotInvMatrix;
    CVector3Df scale;
    CVector3Df trans;
    GLProgram *glsl;
    GLVertexShader *vs;
};

#endif

