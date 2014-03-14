#ifndef DM3D_H
#define DM3D_H

#include <QtGui>
#include <complex>
#include <math.h>
#include <algorithm>
#include <qmessagebox.h>
#include <qfile.h>
#include <qfiledialog.h>
#include "ui_raycast3d.h"

using namespace std;

class RayCast3d : public QMainWindow
{
    Q_OBJECT

public:
    RayCast3d(QWidget *parent = 0);
    ~RayCast3d();
    void resizeEvent(QResizeEvent * e);
    void keyPressEvent(QKeyEvent * e);

private:
    Ui::RayCast3dClass ui;

private slots:
};

#endif // DM3D_H
