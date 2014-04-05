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
#include "transferfunction.h"
#include "Math/IntPoint3D.h"

using namespace std;

class RayCast3d : public QMainWindow
{
    Q_OBJECT

public:
    RayCast3d(QWidget *parent = 0);
    ~RayCast3d();
    void resizeEvent(QResizeEvent * e);
    void keyPressEvent(QKeyEvent * e);
    void updateFT(CTFNode *pNodes, int nPoint);
    void setVol(unsigned char *vol, CIntPoint3D size, CVector3Df scale);
    void setVol(unsigned short *vol, CIntPoint3D size, CVector3Df scale);


private:
    Ui::RayCast3dClass ui;

private slots:
};

#endif // DM3D_H
