#include "points.h"

Points::Points()
{
}

void Points::addPoint(float x,float y)
{
    pointlist.append(QPointF(x,y));
}

float Points::getPointX(int i)
{
    return pointlist[i].x();
}

float Points::getPointY(int i)
{
    return pointlist[i].y();
}

void Points::setPointX(int i, float x)
{
    return pointlist[i].setX(x);
}

void Points::setPointY(int i, float y)
{
    return pointlist[i].setY(y);
}

int Points::getCount()
{
    return pointlist.size();
}

int Points::getClosestPoint(float x, float y)
{
    int result = 0;
    QPointF testpnt = QPointF(x,y);
    QPointF distpnt = pointlist.first()-testpnt;
    float distance = distpnt.x()*distpnt.x() + distpnt.y() * distpnt.y();
    float mindist = distance;
    for (int i = 0; i < pointlist.size(); i++) {
        QPointF& pnt = pointlist[i];
        distpnt = pnt-testpnt;
        distance = distpnt.x()*distpnt.x() + distpnt.y() * distpnt.y();
        if (distance < mindist) {
            mindist = distance;
            result = i;
        }
    }
    return result;
}

void Points::moveClosestPoint(float x, float y, float tox, float toy)
{
    int pntnum = getClosestPoint(x,y);
    pointlist[pntnum].setX(tox);
    pointlist[pntnum].setY(toy);
}
