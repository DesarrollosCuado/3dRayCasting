#include "glwidget.h"
#define PI 3.14159265f

static void CheckErrorsGL(const char* location = NULL) {
  GLuint errnum;
  const char *errstr;
    while ((errnum = glGetError())!=GL_NO_ERROR) {
        errstr = reinterpret_cast<const char *>(gluErrorString(errnum));
        if(errstr)
        {
            if(location)
                qCritical() << "ErrorGL: " << errstr << " at " << location;
            else
                qCritical() << "ErrorGL " << errstr;
        } else {
            if(location)
                qCritical() << "ErrorGL: " << errnum << " at " << location;
            else
                qCritical() << "ErrorGL " << errnum;
        }
    }
}

GLWidget::GLWidget(QWidget *parent)
 : QGLWidget(QGLFormat(QGL::SampleBuffers), parent), scale(1.0f, 1.0f, 1.0f), trans(0.0f, 0.0f, 0.0f)
{
    if (!format().stencil())
        qWarning("Could not get stencil buffer; results will be suboptimal");
    if (!format().alpha())
        qWarning("Could not get alpha channel; results will be suboptimal");
    m_rotMatrix.SetIdentity();
    m_rotInvMatrix.SetIdentity();
    vs=NULL;
    fs=NULL;
    glsl=NULL;
    tf=NULL;
    volume = 0;
    tfID = 0;
    volTex8 = NULL;
    volTex16 = NULL;
    reloadFT = false;
    reloadVol = false;
}

GLWidget::~GLWidget()
{
    glDeleteLists(primitiveList,1);
    makeCurrent();
    if(glsl) delete glsl;
    if(vs) delete vs;
    if(fs) delete fs;
    if(tf) delete tf;
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
    zoom += float(distance)/100.0f;
    if(zoom<0.1)zoom=0.1;
    if(zoom>3.0)zoom=3.0;
    updateGL();
}

void GLWidget::setZTranslation(int distance)
{

    updateGL();
}

void GLWidget::initializeGL()
{
    vs = new GLVertexShader(":/GLSL/shaders/raycasting.vsh");
    qDebug()<<vs->log();
    fs = new GLFragmentShader(":/GLSL/shaders/raycasting.fsh");
    qDebug()<<fs->log();
    glsl=new GLProgram;

    glsl->attach(*vs);
    if(glsl->failed())
        qCritical("Error Compiled Vertex GLSL");
    qDebug()<<glsl->log();

    glsl->attach(*fs);
    if(glsl->failed())
        qCritical("Error Compiled Fragment GLSL");
    qDebug()<<glsl->log();

    zoom = 1.0f;

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

    glActiveTexture(GL_TEXTURE1_ARB);
    glEnable(GL_TEXTURE_1D);
    glGenTextures(1, &tfID);
    glBindTexture(GL_TEXTURE_1D, tfID);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S,  GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    RGBAf *pTexture = new RGBAf[TF_SIZE];
    memset(pTexture, 0, TF_SIZE * sizeof(RGBAf));
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glTexImage1D(GL_TEXTURE_1D, 0, 4, TF_SIZE, 0, GL_RGBA, GL_FLOAT, pTexture);
    delete [] pTexture;
    CheckErrorsGL("initializeGL-4");


    // Inicializar Tiempo para FPS
    m_time=QTime::currentTime();
    sec=0;
    fps=0;
    m_timer = new QTimer(this);
    m_timer->setInterval(1);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    m_timer->start();
}

void GLWidget::paintGL()
{
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearDepth(1.0);
    glLoadIdentity();
    CheckErrorsGL("paintGL-Init");

    //glTranslatef(trans.v[0], trans.v[1], trans.v[2]); //traslada
    glTranslatef(0.0f,0.0f,-3.0f);
    glMultMatrixf(m_rotInvMatrix.m_vector);
    CheckErrorsGL("paintGL-Matrix");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
    //glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_ONE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glScalef(volScale[0]*zoom, volScale[1]*zoom, volScale[2]*zoom);
    glTranslatef(-0.5f,-0.5f,-0.5f);

    CheckErrorsGL("paintGL-CubeCenter");

    glsl->bind();

    glActiveTexture(GL_TEXTURE1_ARB);
    glEnable(GL_TEXTURE_1D);
    if(reloadFT) {
        glBindTexture(GL_TEXTURE_1D, tfID);
        RGBAf *pTexture = new RGBAf[TF_SIZE];
        memset(pTexture, 0, TF_SIZE * sizeof(RGBAf));
        tf->GetPostClassificationMap(pTexture);
        glTexSubImage1D(GL_TEXTURE_1D, 0, 0, TF_SIZE, GL_RGBA, GL_FLOAT, pTexture);
        delete [] pTexture;
        reloadFT = false;
        CheckErrorsGL("paintGL-TF-Upload");
    }else{
        glBindTexture(GL_TEXTURE_1D, tfID);
        CheckErrorsGL("paintGL-TF-Bind");
    }
    glsl->setInt("tf", 1);

    //CVector3Df voxelSize(0.5f/float(volSize[0]), 0.5f/float(volSize[1]), 0.5f/float(volSize[2]));
    glsl->setFloat("voxelSize", 1.0f/(float)std::max(volSize[0], std::max(volSize[1], volSize[2])));//voxelSize.Modulus());

    glActiveTexture(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_3D);
    if(reloadVol) {
        zoom = 1.0f;
        if(volume)
            glDeleteTextures(1, &volume);

        glGenTextures(1, &volume);
        glBindTexture(GL_TEXTURE_3D, volume);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        if(volBits==8) {
            glTexImage3D(GL_TEXTURE_3D, 0, 1, volSize[0], volSize[1], volSize[2], 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, volTex8);
            delete [] volTex8;
            volTex8 = NULL;
        }else if(volBits==16) {
            glTexImage3D(GL_TEXTURE_3D, 0, 1, volSize[0], volSize[1], volSize[2], 0, GL_LUMINANCE, GL_UNSIGNED_SHORT, volTex16);
            delete [] volTex16;
            volTex16 = NULL;
        }
        CheckErrorsGL("paintGL-Vol-Upload");

        reloadVol = false;
    }else{
        if(volume)
            glBindTexture(GL_TEXTURE_3D, volume);
        CheckErrorsGL("paintGL-Vol-Bind");
    }
    glsl->setInt("volume", 0);

    glCallList(primitiveList);

    CheckErrorsGL("paintGL-Cube"); //Print Cube

    glsl->unbind();
    glDisable(GL_BLEND);

    //FPS counter
    ++fps;
    const int gamesT=m_time.msecsTo(QTime::currentTime());
    const int secAct=gamesT/1000;
    static float ratio = 0.0;
    if(secAct!=sec){
        ratio=double(fps)/double(secAct-sec);
        sec=secAct;
        setWindowTitle(QString("FPS: ")+QString::number(ratio));
        fps=0;
    }

    // Display Debug Text
    glColor3f(1.0f,1.0f,1.0f);
    QString debugDisplay=QString("FPS: ")+QString::number(ratio)+QString(" Time: ")+QString::number(gamesT);
    renderText(10,10,debugDisplay);
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
    }else if (event->buttons() & Qt::RightButton) {
        setXTranslation(dx);
        setYTranslation(dy);
    } if (event->buttons() & Qt::MiddleButton) {
        setXTranslation(dx);
        setZTranslation(dy);
    }
    updateGL();
    lastPos = event->pos();
}

void GLWidget::setFT(CTransferFunction *_tf) {
    if(tf) delete tf;
    tf = _tf;
    reloadFT = true;
    updateGL();
}

void GLWidget::setVol(unsigned char *vol, CIntPoint3D size, CVector3Df scale) {
    volBits = 8;
    volSize[0] = size[0];
    volSize[1] = size[1];
    volSize[2] = size[2];
    volScale[0] = scale[0];
    volScale[1] = scale[1];
    volScale[2] = scale[2];
    volTex8 = new unsigned char[volSize[0]*volSize[1]*volSize[2]];
    memcpy(volTex8, vol, sizeof(unsigned char)*volSize[0]*volSize[1]*volSize[2]);
    reloadVol = true;
    updateGL();
}

void GLWidget::setVol(unsigned short *vol, CIntPoint3D size, CVector3Df scale) {
    volBits = 16;
    volSize[0] = size[0];
    volSize[1] = size[1];
    volSize[2] = size[2];
    volScale[0] = scale[0];
    volScale[1] = scale[1];
    volScale[2] = scale[2];
    volTex16 = new unsigned short[volSize[0]*volSize[1]*volSize[2]];
    memcpy(volTex16, vol, sizeof(unsigned short)*volSize[0]*volSize[1]*volSize[2]);
    reloadVol = true;
    updateGL();
}


