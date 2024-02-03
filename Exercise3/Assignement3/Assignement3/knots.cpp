#include "knots.h"
#include <cstdio>

Knots::Knots():Points()
{
    insertKnot(0.0);
    insertKnot(1.0);
}

void Knots::insertKnot(float value)
{
    if (value < 0.0 || value > 1.0) return;
    float xcoord = value * 1.8 - 0.9;
    if (pointlist.size()<2) {
        pointlist.append(QPointF(xcoord,-0.9));
        return;
    }
    int i = 0;
    while (i<pointlist.size() && getValue(i)<value) {
        i++;
    }
    if (i<pointlist.size()) {
            pointlist.insert(i,QPointF(xcoord,-0.9));
    }
    else {
        printf("Knoten nicht einfügbar bei %f\n",value);
    }
}

void Knots::insertKnotX(float x)
{
    insertKnot((x+0.9) / 1.8);
}

void Knots::setValueX(int i, float x)
{
    // do not move first and last knot
    if (i<=0 || i>=pointlist.size()-1) return;

    float xsafe = x;
    if (xsafe < pointlist[i-1].x()) { xsafe = pointlist[i-1].x(); }
    if (xsafe > pointlist[i+1].x()) { xsafe = pointlist[i+1].x(); }
    pointlist[i].setX(xsafe);
}

float Knots::getValue(int i)
{
    return (pointlist[i].x()+0.9) / 1.8;
}
