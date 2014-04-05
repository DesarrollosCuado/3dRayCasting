#include "raycast3d.h"
#include "wtransferfunction.h"

unsigned int ReadVolume::DDS_buffer = 0;
int ReadVolume::DDS_bufsize = 0;
int ReadVolume::DDS_bitcnt = 0;
int ReadVolume::DDS_pos = 0;
int ReadVolume::DDS_data_size = 0;
unsigned char *ReadVolume::DDS_data = NULL;

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
    for(int i=0;i<nPoints;++i) {
        QColor c(points.at(i).color);
        pNodes[i].m_b=c.blueF();
        pNodes[i].m_r=c.redF();
        pNodes[i].m_g=c.greenF();
        pNodes[i].m_x=points[i].pos.x()/512.0f;
        pNodes[i].m_y=points[i].pos.y()/255.0f;
    }
    ((RayCast3d*)_parent)->updateFT(pNodes, nPoints);
    delete [] pNodes;
}

void WTransferFunction::setPointers(vector < TFPoint > *_points)
{
    points = *_points;
    ui.widget->setPointers(points);
    updateTF();
}

void WTransferFunction::on_openVol_clicked()
{
    unsigned char *vol;
    CIntPoint3D size;
    CVector3Df scale;
    int b;
    if(ReadVolume::read(&vol, &size.values[0], &size.values[1], &size.values[2], &scale.v[0], &scale.v[1], &scale.v[2], &b)) {
        if(b==8)
            ((RayCast3d*)_parent)->setVol(vol, size, scale);
        else
            ((RayCast3d*)_parent)->setVol((unsigned short*)vol, size, scale);
    }
}
