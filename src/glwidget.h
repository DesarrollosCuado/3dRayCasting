#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <algorithm>
#include <GL/glu.h>
#include <QtGui>
#include <QtOpenGL>

#include "glextensions.h"
#include "glshaders.h"

#include "Math/Vector3Df.h"
#include "Math/Matrix4x4f.h"
#include "transferfunction.h"

using namespace std;

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void resizeGL(int width, int height);
    void setFT(CTransferFunction *tf);
    void setVol(unsigned char *vol, int w, int h, int z);
    void setVol(unsigned short *vol, int w, int h, int z);

protected:
    void initializeGL();
    void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void setXTranslation(int distance);
    void setYTranslation(int distance);
    void setZTranslation(int distance);

private:
    bool reloadFT;
    bool reloadVol;
    QPoint lastPos;
    GLuint primitiveList;
    GLuint volume;
    GLuint tfID;
    QTimer *m_timer;
    QTime m_time;
    unsigned int fps;
    unsigned int sec;
    unsigned int volSize[3];
    unsigned char volBits;
    unsigned char *volTex8;
    unsigned short *volTex16;
    CMatrix4x4f m_rotMatrix;
    CMatrix4x4f m_rotInvMatrix;
    CVector3Df scale;
    CVector3Df trans;
    GLProgram *glsl;
    CTransferFunction *tf;
    GLVertexShader *vs;
    GLFragmentShader *fs;
};

#endif

