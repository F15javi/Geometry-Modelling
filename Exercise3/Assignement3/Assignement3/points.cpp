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

float Points::getDistance(int i, float x, float y)
{
    QPointF testpnt = QPointF(x,y);
    QPointF distpnt = pointlist[i] - testpnt;
    return distpnt.x()*distpnt.x() + distpnt.y() * distpnt.y();
}

int Points::getClosestPoint(float x, float y)
{
    int result = 0;
    float distance = getDistance(0,x,y);
    float mindist = distance;
    for (int i = 0; i < pointlist.size(); i++) {
        distance = getDistance(i,x,y);
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
