#include "wfunction.h"
#include "wtransferfunction.h"

#define SENSITIVE 8
WFunction::WFunction(QWidget *parent)
	: QWidget(parent)
{
	controls = NULL;
	action = none;
    point = -1;
	haschanged = false;
    this->setAutoFillBackground(true);
	this->setBackgroundRole(QPalette::Base);
	//fondo=QImage("fondoFT.gif");
    img=QImage(512,1,QImage::Format_ARGB32);
}

WFunction::~WFunction()
{

}


void WFunction::DrawFunc(const vector < TFPoint > &ptr, QPainter *p)
{
	QPen penDef(QColor(125, 125, 125, 255), 4);
	p->setOpacity(0.8f);
	QPen penRec(QColor(0, 0, 0, 255), 6);
	p->setPen(penDef);
	p->setOpacity(0.5f);
    for (int i = 0; i < ptr.size()-1; ++i)
	{
        p->drawLine(ptr[i].pos.x(), 255 - ptr[i].pos.y(), ptr[i + 1].pos.x(), 255 - ptr[i + 1].pos.y());
	}	
	p->setOpacity(1.0f);
    for (int i = 0; i < ptr.size(); ++i)
	{
        penRec.setColor(QColor(ptr[i].color));
		p->setPen(penRec);
        p->drawRect(-3 + ptr[i].pos.x(), -3 + 255 - ptr[i].pos.y(), 6, 6);
	}
}

void WFunction::paintEvent(QPaintEvent * paintEvent)
{	
	static QRectF target(0.0, 0.0, 2048.0, 256.0);
	static QRectF sourceFondo(0.0, 0.0, 2048.0, 256.0);
	static QRectF sourceImage(0.0, 0.0, 2048.0, 1.0);

	if (controls == NULL) return;
    vector < TFPoint > &points = *controls;


	QPainter p(this);
	p.setOpacity(1.0f);
	QPen pen(QColor(0, 0, 0, 255), 1);
	p.fillRect (0, 0, 2048, 256, QColor(255, 255, 255, 255));
	//p.drawImage(target,fondo,sourceFondo);

    for (int i = 0; i < points.size()-1; ++i)
	{
        QColor step(points[i].color);
        QColor init(points[i].color);
        QColor fin(points[i+1].color);
        float difX = points[i+1].pos.x()-points[i].pos.x();
        float alphaInit = float(points[i].pos.y())/255.0f;
        float alphaFin=float(points[i+1].pos.y())/255.0f;
		init.setAlphaF(alphaInit);
		fin.setAlphaF(alphaFin);
		float pAlpha=(alphaFin-alphaInit)/difX;
		float pR=(fin.redF()-init.redF())/difX;
		float pG=(fin.greenF()-init.greenF())/difX;
		float pB=(fin.blueF()-init.blueF())/difX;	
		int p=0;
        for(int j=points[i].pos.x();j<points[i+1].pos.x();++j){
			++p;
			img.setPixel(j,0,step.rgba());
            step.setRedF(CLAMP((float)init.redF()+pR*p,0.0f,1.0f));
            step.setGreenF(CLAMP((float)init.greenF()+pG*p,0.0f,1.0f));
            step.setBlueF(CLAMP((float)init.blueF()+pB*p,0.0f,1.0f));
            step.setAlphaF(CLAMP((float)init.alphaF()+pAlpha*p,0.0f,1.0f));
		}
	}

	p.drawImage(target,img,sourceImage);
	p.setOpacity(1.0f);
	pen = QPen(QColor(125, 125, 125, 255), 4);
	p.setPen(pen);
    DrawFunc(*controls, &p);
}

void WFunction::setPointers(vector <TFPoint> &_alpha)
{
    controls = &_alpha;
	update();
}

void WFunction::mouseMoveEvent(QMouseEvent *mouseEvent)
{
	if (controls == NULL) return;
    vector < TFPoint > &points = *controls;
	int x = mouseEvent->x();
	int y = 255 - mouseEvent->y();

	switch (action)
	{
		case moving:
			if (point >= 0)
			{
				
				if (point == 0 || point == controls->size() - 1)
				{
                    points[point].pos.setY(CLAMP(y, 0, 255));
					haschanged = true;
				}
                else
				{
                    points[point].pos.setX(CLAMP(x, points[point - 1].pos.x()+1, points[point + 1].pos.x()-1));
                    points[point].pos.setY(CLAMP(y, 0, 255));
					haschanged = true;
				}
				if (haschanged) update();
			}
			break;
		case creating:
			if (point > 0)
            {
                haschanged = true;
                points[point].pos.setX(CLAMP(x, points[point - 1].pos.x()+1, points[point + 1].pos.x()-1));
                points[point].pos.setY(CLAMP(y, 0, 255));
                update();
			}
			break;
    }
	
}
void WFunction::mouseDoubleClickEvent(QMouseEvent *mouseEvent)
{
	if (controls == NULL) return;
    vector < TFPoint > &points = *controls;
	int x = mouseEvent->x();
	int y = 255 - mouseEvent->y();
    for (int i = 0; i < points.size(); ++i)
	{
        if (x >= points[i].pos.x() - 5 && x <= points[i].pos.x() + SENSITIVE &&
            y >= points[i].pos.y() - 5 && y <= points[i].pos.y() + SENSITIVE)
		{
            points[i].color=QColorDialog::getColor(points[i].color).rgb();
			break;
		}
	}
    update();
    ((WTransferFunction*)_parent)->updateTF();
}

void WFunction::mousePressEvent(QMouseEvent *mouseEvent)
{
	if (controls == NULL) return;
    vector < TFPoint > &points = *controls;
	int x = mouseEvent->x();
	int y = 255 - mouseEvent->y();
	action = none;
	if(mouseEvent->button()==Qt::LeftButton)
	{
        for (int i = 0; i < points.size(); ++i)
		{
            if (x >= points[i].pos.x() - SENSITIVE && x <= points[i].pos.x() + SENSITIVE &&
                y >= points[i].pos.y() - SENSITIVE && y <= points[i].pos.y() + SENSITIVE)
			{
				action = moving;
				point = i;
				break;
			}
		}

		if (action == none)
		{
            for (int i = 0; i < points.size() - 1; ++i)
			{
                if (x >= points[i].pos.x() && x <= points[i + 1].pos.x())
				{
					point = i + 1;
					action = creating;
                    QPoint p3(x, y);
                    controls->insert(points.begin() + point, TFPoint(p3));
					update();
					haschanged = true;
					break;
				}
			}
			
		}
	}else if(mouseEvent->button()==Qt::MidButton){

	}else if(mouseEvent->button()==Qt::RightButton){
        for (int i = 0; i < points.size(); ++i)
		{
            if(points[i].pos.x()>1 && (*controls)[i].pos.x()<4094){
                if (x >= points[i].pos.x() - SENSITIVE && x <= points[i].pos.x() + SENSITIVE &&
                    y >= points[i].pos.y() - SENSITIVE && y <= points[i].pos.y() + SENSITIVE)
				{
                    points.erase((*controls).begin()+i);
					haschanged = true;
					update();
					break;
				}
			}
		}
	}
}
void WFunction::mouseReleaseEvent(QMouseEvent *mouseEvent)
{
	if (controls == NULL) return;
	if (haschanged)
        ((WTransferFunction*)_parent)->updateTF();
    haschanged = false;
}
