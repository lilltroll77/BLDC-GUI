#ifndef CURRENTGAGUE_H
#define CURRENTGAGUE_H

#include <QWidget>
#include "qcgaugewidget.h"

struct needle_t{
   QcLabelItem* label;
   QcNeedleItem* Item;
};

class currentGague : public QWidget
{
    Q_OBJECT
public:
    explicit currentGague(QWidget *parent = nullptr, float Imax=32.0f);

signals:

public slots:


private:
    QcGaugeWidget* widget;
    QcBackgroundItem *bkg;
    QcBackgroundItem *bkg_center;
    needle_t peak_needle;
    needle_t ms_needle;
    QcValuesItem* Values;
    QcDegreesItem* DegreeSubItem;
    QcDegreesItem* DegreeItem;
    int DegreeStart = 0;
    int DegreeEnd =   180;
    QPair<QColor,float> pair[4]={{Qt::green , 15},{Qt::darkGreen , 65},{Qt::yellow , 80},{Qt::red , 100}};
    QcColorBand* ColorBand;
    QcLabelItem* label;
    const QString label_str = "CoilCurrent [A]";
    //QcNeedleItem* peak_needle;
    //QcLabelItem* peak_needle_label;
    //QcNeedleItem* ms_needle;
    //QcLabelItem* ms_needle_label;

};

#endif // CURRENTGAGUE_H
