#include "wtransferfunction.h"
#include "raycast3d.h"

WTransferFunction::WTransferFunction(QWidget *parent) : _parent(parent), QWidget(parent)
{
	ui.setupUi(this);
    ui.widget->_parent=this;
}

WTransferFunction::~WTransferFunction()
{

}

void WTransferFunction::updateTF()
{
    int nPoints=points.size();
    CTFNode *pNodes=new CTFNode[nPoints];
    for(int i=0;i<nPoints;++i){
        QColor c(points.at(i).color);
        pNodes[i].m_b=c.blueF();
        pNodes[i].m_r=c.redF();
        pNodes[i].m_g=c.greenF();
        pNodes[i].m_x=points[i].pos.x()/511.0f;
        pNodes[i].m_y=points[i].pos.y()/255.0f;
    }
    ((RayCast3d*)_parent)->updateFT(pNodes, nPoints);
    delete pNodes;
}

void WTransferFunction::setPointers(vector < TFPoint > *_points)
{
    points = *_points;
    ui.widget->setPointers(points);
}

void WTransferFunction::openVolume()
{

}
