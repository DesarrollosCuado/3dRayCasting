#ifndef WTRANSFERFUNCTION_H
#define WTRANSFERFUNCTION_H

#include <QWidget>
#include "transferfunction.h"
#include "wfunction.h"
#include "ui_wtransferfunction.h"

class WTransferFunction : public QWidget
{
	Q_OBJECT

public:
	WTransferFunction(QWidget *parent = 0);
    ~WTransferFunction();
    vector < TFPoint > points;
    void *_parent;
    void updateTF();
    void setPointers(vector < TFPoint > *_points);
	Ui::WTransferFunctionClass ui;

private:
    //void resizeEvent(QResizeEvent *e);

private slots:
    void openVolume();

public slots:
};

#endif // WTRANSFERFUNCTION_H
