#ifndef WFunction_H
#define WFunction_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QPaintDevice>
#include <QColorDialog>
#include <QPoint>
#include <vector>
#include <QColor>
#include <QRgb>

using namespace std;

#define CLAMP(x, minx, maxx) max(min(x, maxx), minx)
#define PI 3.1415926535897932

class MyMath{
public:
    static float degreesToRadians(float degrees)
    {
            return (degrees * PI) / 180.0f;
    }

};

struct TFPoint
{
public:
    TFPoint(const TFPoint &o): pos(o.pos), color(o.color){ }
    TFPoint(const QPoint &q): pos(q) { }
    TFPoint(const QRgb &r) : pos(), color(r) { }
    TFPoint() : pos(), color(QColor(255,255,255).rgba()) { }
    TFPoint(const QPoint &q, const QRgb &r) : pos(q), color(r) { }
    TFPoint(const QPoint &q, const QColor &r) :  pos(q), color(r.rgba()) { }

    QPoint pos;
    QRgb color;
};

class WFunction : public QWidget
{
	Q_OBJECT

public:
	enum ACTION { none, moving, creating };
	WFunction(QWidget *parent = 0);
	~WFunction();
	int *arr;
	QPoint lastPos;
    vector < TFPoint > *controls;
	QWidget *_parent;
	ACTION action;
	int point;
    bool haschanged;
	QImage fondo;
    QImage img;	
    void DrawFunc(const vector < TFPoint > &ptr, QPainter *p);
    void setPointers(vector <TFPoint> &ptr);
	void paintEvent(QPaintEvent * paintEvent);
	void mouseDoubleClickEvent(QMouseEvent *mouseEvent);
	void mouseMoveEvent(QMouseEvent *mouseEvent);
	void mousePressEvent(QMouseEvent *mouseEvent);
	void mouseReleaseEvent(QMouseEvent *mouseEvent);
};

#endif // WFunction_H
