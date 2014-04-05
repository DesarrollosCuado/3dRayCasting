
#include "glextensions.h"
#ifdef __unix__
    #include <GL/glext.h>
#endif
#include "raycast3d.h"
#include "wtransferfunction.h"
#include <QtGui>
#include <QGLWidget>

static void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
   Q_UNUSED(context);

   QString dt = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
   QString txt = QString("[%1] ").arg(dt);

   switch (type)
   {
      case QtDebugMsg:
         txt += QString("{Debug} \t\t %1").arg(msg);
         break;
      case QtWarningMsg:
         txt += QString("{Warning} \t %1").arg(msg);
         break;
      case QtCriticalMsg:
         txt += QString("{Critical} \t %1").arg(msg);
         break;
      case QtFatalMsg:
         txt += QString("{Fatal} \t\t %1").arg(msg);
         //abort();
         break;
   }

   QFile outFile("LogFile.log");
   outFile.open(QIODevice::WriteOnly | QIODevice::Append);

   QTextStream textStream(&outFile);
   textStream << txt << endl;
   textStream.flush();
   printf("%s\n", txt.toStdString().c_str());

   if(type==QtFatalMsg) abort();
}

inline bool matchString(const char *extensionString, const char *subString)
{
    int subStringLength = strlen(subString);
    return (strncmp(extensionString, subString, subStringLength) == 0)
            && ((extensionString[subStringLength] == ' ') || (extensionString[subStringLength] == '\0'));
}

bool necessaryExtensionsSupported()
{
    const char *extensionString = reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS));
    const char *p = extensionString;

    const int GL_ARB_VS = 1;
    const int GL_ARB_FS = 2;
    const int GL_ARB_SO = 4;
    int extensions = 0;

    while (*p) {

        if (matchString(p, "GL_ARB_vertex_shader"))
            extensions |= GL_ARB_VS;
        else if (matchString(p, "GL_ARB_fragment_shader"))
            extensions |= GL_ARB_FS;
        else if (matchString(p, "GL_ARB_shader_objects"))
            extensions |= GL_ARB_SO;

        while ((*p != ' ') && (*p != '\0'))
            ++p;

        if (*p == ' ')
            ++p;
    }

    return (extensions == 7);
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qInstallMessageHandler(customMessageHandler);

    if ((QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_Version_1_5) == 0) {
        QMessageBox::critical(0, "OpenGL features missing","OpenGL version 1.5 or higher is required to run this project.\nThe program will now exit.");
        return -1;
    }

    QGLWidget *widget = new QGLWidget(QGLFormat(QGL::SampleBuffers));
    widget->makeCurrent();
    if (!necessaryExtensionsSupported()) {
        QMessageBox::critical(0, "OpenGL features missing", "The OpenGL extensions required to run this project are missing.\nThe program will now exit.");
        delete widget;
        return -2;
    }
    if (!getGLExtensionFunctions().resolve(widget->context())) {
        QMessageBox::critical(0, "OpenGL features missing", "Failed to resolve OpenGL functions required to run this project.\nThe program will now exit.");
        delete widget;
        return -3;
    }
    delete widget;

    qDebug()<<"Extenciones soportadas: "<<(const char*)glGetString(GL_EXTENSIONS);
    RayCast3d w;
    WTransferFunction tf(&w);
    tf.setWindowFlags(Qt::Tool);
    tf._parent = &w;
    vector <TFPoint> tmp;
    tmp.push_back(TFPoint(QPoint(0,0), QColor(0,0,0)));
    tmp.push_back(TFPoint(QPoint(512,255), QColor(255,255,255)));
    tf.setPointers(&tmp);
    w.show();
    tf.show();
    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    return a.exec();
}
