#ifndef POINTS_H
#define POINTS_H
#include <QList>
#include <QPointF>

class Points
{
public:
    Points();
    void  addPoint (float x, float y);
    float getPointX(int i);
    float getPointY(int i);
    void  setPointX(int i, float x);
    void  setPointY(int i, float y);
    int   getCount ();
    float getDistance(int i, float x, float y);
    int   getClosestPoint( float x, float y);
    void  moveClosestPoint(float x, float y, float tox, float toy);
protected:
    QList<QPointF> pointlist;
};

#endif // POINTS_H
