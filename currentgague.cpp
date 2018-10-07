#include "currentgague.h"


currentGague::currentGague(QWidget *parent , float Imax) : QWidget(parent)
{

    // Widget
        widget = new QcGaugeWidget(parent);
        widget->setFixedSize(200,200);
    // Background
        bkg = widget -> addBackground(97 , DegreeStart , DegreeEnd);
        bkg->clearrColors();
        bkg->addColor(0 , Qt::white);
        bkg_center = widget -> addBackground(5 , 0 , 360);
        bkg_center->clearrColors();
        bkg_center->addColor(0 , Qt::black);
    // Labels
        label = widget->addLabel(20);
        label->setText(label_str);
        ms_needle.label = widget->addLabel(20);
        ms_needle.label->setAngle(90);
        ms_needle.label->setText("True RMS");

    //Needles
        peak_needle.label = widget->addLabel(35);
        peak_needle.label-> setText("Peak");
        peak_needle.label->setColor(Qt::red);
        peak_needle.label->setAngle(90);

        ms_needle.Item =   widget->addNeedle(85);
        peak_needle.Item = widget->addNeedle(85);
        ms_needle.Item->setColor(Qt::black);
        peak_needle.Item->setColor(Qt::red);
        ms_needle.Item->setNeedle(QcNeedleItem::TriangleNeedle);
        peak_needle.Item->setNeedle(QcNeedleItem::TriangleNeedle);
        peak_needle.Item->setDgereeRange(DegreeStart , DegreeEnd);
        ms_needle.Item->setDgereeRange(DegreeStart , DegreeEnd);

    // Degree Items
        DegreeItem =    widget->addDegrees(93);
        DegreeItem->    setDgereeRange(DegreeStart , DegreeEnd);
        DegreeItem->    setSubDegree(false);
        DegreeSubItem = widget->addDegrees(95);
        DegreeSubItem-> setDgereeRange(DegreeStart , DegreeEnd);
        DegreeSubItem-> setSubDegree(true);

    //Values
        Values = widget->addValues(65);
        Values->setDgereeRange(DegreeStart , DegreeEnd);
        Values->setMinValue(0);
        Values->setMaxValue(Imax);

    // Arc
        widget->addArc(97)->setDgereeRange(DegreeStart , DegreeEnd);

    // Color Band
        ColorBand = widget->addColorBand(92);
        QList<QPair<QColor,float>> BandColors;
        for(int i=0; i< 4 ; i++){
            BandColors.append(pair[i]);
         }
        ColorBand->setColors(BandColors);
        ColorBand->setDgereeRange(DegreeStart , DegreeEnd);

}
