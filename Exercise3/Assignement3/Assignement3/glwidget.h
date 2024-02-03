#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include "knots.h"

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    GLWidget(QWidget *parent=0);
    ~GLWidget();
public slots:
    void setEpsilonDraw        (double value);
protected:
    void paintGL              ();
    void initializeGL         ();
    void resizeGL             (int width, int height);
    void mouseMoveEvent       (QMouseEvent *event);
    void mousePressEvent      (QMouseEvent *event);
    void Knot_Insertion       (float x);
    float find_knot           (float x);
    void draw_curve           ();
    float recFuncX            (int i, int j, int n);
    float recFuncY            (int i, int j, int n);

private:
    QPointF transformPosition(QPoint p);
    float aspectx, aspecty;
    Points points;
    Points copyPoints;

    Points newPoints;

    Knots knots;
    int clickedPoint;
    int clickedKnot;
    float epsilon_draw;
};



#endif // GLWIDGET_H
