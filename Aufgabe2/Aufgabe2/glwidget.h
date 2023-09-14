#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include "points.h"
using namespace std;

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    GLWidget(QWidget *parent=0);
    ~GLWidget();

public slots:
    void setIntersection       (int    state);
    void setSelfIntersection   (int    state);
    void setEpsilonDraw        (double value);
    void setEpsilonIntersection(double value);

protected:
    void paintGL              ();
    void initializeGL         ();
    void resizeGL             (int width, int height);
    void mouseMoveEvent       (QMouseEvent *event);
    void mousePressEvent      (QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void PlotBezier(Points points, int k);
    void IntersectionBezier(Points b, Points c);
    void SelfIntersectionBezier(Points b);
    QPointF SelfIntersectionAux(pair<QPointF, QPointF> A, pair<QPointF, QPointF> B);
    int IsPointInBoundingBox(float x1, float y1, float x2, float y2, float px, float py);
    float Round(float val);





private:
    QPointF transformPosition(QPoint p);
    QPointF getCasteljauPoint(Points points, int r, int i, double t);
    int Intersection(Points b, Points c);
    float maxDifference(Points points);


    Points  points;
    Points  points1;
    Points  points2;


	float   aspectx, aspecty;
    float   epsilon_draw, epsilon_intersection;
    int     clickedPoint;
    bool    doIntersection,doSelfIntersection;
};



#endif // GLWIDGET_H
