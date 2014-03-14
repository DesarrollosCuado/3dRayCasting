#include "glwidget.h"
#define PI 3.14159265f

static void CheckErrorsGL(const char* location = NULL)
{
  GLuint errnum;
  const char *errstr;
  while (errnum = glGetError())
  {
    errstr = reinterpret_cast<const char *>(gluErrorString(errnum));
    if(errstr)
    {
      qCritical() << errstr;
    }
    else
    {
      qCritical() << "Error " << errnum;
    }

    if(location)
      qCritical() << " at " << location;
  }
}

GLWidget::GLWidget(QWidget *parent)
 : QGLWidget(parent), scale(1.0f, 1.0f, 1.0f), trans(0.0f, 0.0f, 0.0f)
{
    m_rotMatrix.SetIdentity();
    m_rotInvMatrix.SetIdentity();
}

GLWidget::~GLWidget()
{
    glDeleteLists(primitiveList,1);
    makeCurrent();
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(400, 400);
}

void GLWidget::setXTranslation(int distance)
{
    updateGL();
}

void GLWidget::setYTranslation(int distance)
{

    updateGL();
}

void GLWidget::setZTranslation(int distance)
{

    updateGL();
}

void GLWidget::initializeGL()
{
    vs = new GLVertexShader(":/GLSL/shaders/raycasting.vsh");
    fs = new GLFragmentShader(":/GLSL/shaders/raycasting.fsh");
    glsl=new GLProgram;

    glsl->attach(*vs);
    if(glsl->failed())
        qCritical("Error Compiled Vertex GLSL");

    glsl->attach(*fs);
    if(glsl->failed())
        qCritical("Error Compiled Fragment GLSL");

    CheckErrorsGL("initializeGL-1");

    primitiveList  = glGenLists(1);
    CheckErrorsGL("initializeGL-2");

    CVector4Df minTex(0.0f,0.0f,0.0f);
    CVector4Df maxTex(1.0f,1.0f,1.0f);
    CVector4Df verteces[8];
    verteces[0] = CVector4Df(minTex[0], maxTex[1], minTex[2], 1.0f);
    verteces[1] = CVector4Df(minTex[0], maxTex[1], maxTex[2], 1.0f);
    verteces[2] = CVector4Df(maxTex[0], maxTex[1], minTex[2], 1.0f);
    verteces[3] = CVector4Df(maxTex[0], maxTex[1], maxTex[2], 1.0f);
    verteces[4] = CVector4Df(minTex[0], minTex[1], minTex[2], 1.0f);
    verteces[5] = CVector4Df(minTex[0], minTex[1], maxTex[2], 1.0f);
    verteces[6] = CVector4Df(maxTex[0], minTex[1], minTex[2], 1.0f);
    verteces[7] = CVector4Df(maxTex[0], minTex[1], maxTex[2], 1.0f);

    glNewList(primitiveList, GL_COMPILE); // Cube
        glBegin(GL_TRIANGLES);
            glVertex3fv(verteces[3].v);glVertex3fv(verteces[2].v);glVertex3fv(verteces[1].v);
            glVertex3fv(verteces[2].v);glVertex3fv(verteces[0].v);glVertex3fv(verteces[1].v);


            glVertex3fv(verteces[4].v);glVertex3fv(verteces[6].v);glVertex3fv(verteces[5].v);
            glVertex3fv(verteces[6].v);glVertex3fv(verteces[7].v);glVertex3fv(verteces[5].v);


            glVertex3fv(verteces[0].v);glVertex3fv(verteces[4].v);glVertex3fv(verteces[1].v);
            glVertex3fv(verteces[4].v);glVertex3fv(verteces[5].v);glVertex3fv(verteces[1].v);

            glVertex3fv(verteces[6].v);glVertex3fv(verteces[2].v);glVertex3fv(verteces[7].v);
            glVertex3fv(verteces[2].v);glVertex3fv(verteces[3].v);glVertex3fv(verteces[7].v);


            glVertex3fv(verteces[3].v);glVertex3fv(verteces[1].v);glVertex3fv(verteces[7].v);
            glVertex3fv(verteces[1].v);glVertex3fv(verteces[5].v);glVertex3fv(verteces[7].v);

            glVertex3fv(verteces[4].v);glVertex3fv(verteces[0].v);glVertex3fv(verteces[6].v);
            glVertex3fv(verteces[0].v);glVertex3fv(verteces[2].v);glVertex3fv(verteces[6].v);
        glEnd();
    glEndList();
    CheckErrorsGL("initializeGL-3");
}

void GLWidget::paintGL()
{
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    //glsl->bind();

    glTranslatef(trans.v[0], trans.v[1], trans.v[2]); //traslada
    glTranslatef(0.0f,0.0f,-3.0f);
    glMultMatrixf(m_rotInvMatrix.m_vector);


    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glDepthMask(GL_TRUE);
    glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_ONE);

    glColor3f(0.5f, 0.5f, 0.5f);
    glTranslatef(-0.5f,-0.5f,-0.5f);

    glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_3D);
    glBindTexture(GL_TEXTURE_3D, volume);
    glsl->setInt("volume", 0);

    glActiveTextureARB(GL_TEXTURE1_ARB);
    glEnable(GL_TEXTURE_3D);
    glBindTexture(GL_TEXTURE_3D, integrals);
    glsl->setInt("integrals", 1);

    glCallList(primitiveList);

    CheckErrorsGL("paintGL"); //Print Cube
}

void GLWidget::resizeGL(int width, int height)
{
    glViewport(0,0,width,height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 1.33, 0.01f, 10.0f);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    static const float PIm = (3.1415926535897932/2.0);
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        CVector3Df mouseVec(-dx, dy, 0);
        CVector3Df perpendMouseVec;
        perpendMouseVec[0] =  cos(PIm) * mouseVec[0] + sin(PIm) * mouseVec[1];
        perpendMouseVec[1] = -sin(PIm) * mouseVec[0] + cos(PIm) * mouseVec[1];
        perpendMouseVec[2] = 0.0f;
        perpendMouseVec.Normalize();

        CMatrix4x4f rotmat;

        rotmat.SetIdentity();
        rotmat.Rotate(-mouseVec.Modulus()*0.01f, perpendMouseVec[0], perpendMouseVec[1], 0);
        m_rotInvMatrix.PostMult(rotmat);

        rotmat.SetIdentity();
        rotmat.Rotate(+mouseVec.Modulus()*0.01f, perpendMouseVec[0], perpendMouseVec[1], 0);
        m_rotMatrix.PreMult(rotmat);
        updateGL();
    }else if (event->buttons() & Qt::RightButton) {
        setXTranslation(dx);
        setYTranslation(dy);
    } if (event->buttons() & Qt::MiddleButton) {
        setXTranslation(dx);
        setZTranslation(dy);
    }
    lastPos = event->pos();
}
