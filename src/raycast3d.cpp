#include "raycast3d.h"


RayCast3d::RayCast3d(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);
}

RayCast3d::~RayCast3d()
{

}

void RayCast3d::resizeEvent(QResizeEvent * e){
	ui.widget->setGeometry(0,0,geometry().width(),geometry().height());
}

void RayCast3d::keyPressEvent(QKeyEvent * e ){

	ui.widget->updateGL();
}
