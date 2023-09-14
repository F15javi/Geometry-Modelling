
#include "glwidget.h"
#include <QtGui>
#include <GL/GLU.h>
#include "glut.h"
#include "mainwindow.h"
#include <list>
#include <iostream>
#include <algorithm>    
#include <iomanip>
GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    doIntersection       = false;
    doSelfIntersection   = false;
    epsilon_draw         = (float)0.05;
    epsilon_intersection = (float)0.000005;

    // Hier Punkte hinzufügen (schönere Startpositionen!)
    points.addPoint(-1.00, -0.5);
    points.addPoint(-0.75, -0.5);
    points.addPoint(-0.50, -0.5);
    points.addPoint(-0.25, -0.5);
    points.addPoint( 0.00, -0.5);

    points.addPoint( 0.25,  0.5);
    points.addPoint( 0.50,  0.5);
    points.addPoint( 0.75,  0.5);
    points.addPoint( 1.00,  0.5);
}

GLWidget::~GLWidget()
{
}

void GLWidget::paintGL()
{
    // clear
    glClear(GL_COLOR_BUFFER_BIT);

    // Koordinatensystem
    glColor3f(0.5,0.5,0.5);
    glBegin(GL_LINES);
    glVertex2f(-1.0, 0.0);
    glVertex2f( 1.0, 0.0);
    glVertex2f( 0.0,-1.0);
    glVertex2f( 0.0, 1.0);
    glEnd();
    glColor3f(1.0,0.0,0.0);

    // Punkte
    glPointSize(7.0);
    glBegin(GL_POINTS);
    for (int i=0; i<points.getCount(); i++) {
        glVertex2f(points.getPointX(i),points.getPointY(i));
    }
	
    glEnd();

    // Hüllpolygone zeichnen
    glColor3f(0.0,0.0,1.0);
    glBegin(GL_LINE_STRIP);
    for (int i=0; i<5; i++) {
        glVertex2f(points.getPointX(i),points.getPointY(i));
    }
    glEnd();
    glBegin(GL_LINE_STRIP);
    for (int i=5; i<points.getCount(); i++) {
    glVertex2f(points.getPointX(i),points.getPointY(i));
    }
    glEnd();

    // Kurve
    
    // AUFGABE: Draw curve here
    // use epsilon_draw for this
    Points aux;

    for (int i = 0; i < 5; i++) {
        aux.addPoint(points.getPointX(i), points.getPointY(i));

    }
    PlotBezier(aux, 5);
    Points aux2;

    for (int i = 5; i < points.getCount(); i++) {
        aux2.addPoint(points.getPointX(i), points.getPointY(i));

    }
    PlotBezier(aux2, 5);

    // Draw intersections
    if (doIntersection) {
        glColor3f(0.0,1.0,0.0);
        // AUFGABE: Draw sections here
        // use epsilon_intersection
        IntersectionBezier(aux, aux2);
    }
    if (doSelfIntersection) {
        glColor3f(1.0,0.0,1.0);
        // AUFGABE: Draw sections here
        // use epsilon_intersection
        SelfIntersectionBezier(aux);
    }
}
bool compare_x(const QPointF& first, const QPointF& second)//Predicate for sorting the Vector of points
{
    return (first.x() < second.x());
}

bool compare_y(const QPointF& first, const QPointF& second)//Predicate for sorting the Vector of points
{
    return (first.y() < second.y());
}
void GLWidget::PlotBezier(Points points, int k){

    
    for (double t = 0; t <= 1; t += 0.001) {
        QPointF point;

        point = getCasteljauPoint(points, points.getCount() - 1, 0, t);
        glColor3f(0.0, 1.0, 0.0);
        glPointSize(2);
        glBegin(GL_POINTS);
            glVertex2f(point.x(), point.y());
        
        glEnd();
        update();
    }
}

QPointF GLWidget::getCasteljauPoint(Points points, int r, int i, double t) {
    
    if (r == 0) {
        QPointF aux;
        aux.setX(points.getPointX(i));
        aux.setY(points.getPointY(i));
        return aux;

    }

    QPointF p1 = getCasteljauPoint(points, r - 1, i, t);
    QPointF p2 = getCasteljauPoint(points, r - 1, i + 1, t);
    QPointF res;
    
    res.setX((1 - t) * p1.x() + t * p2.x());
    res.setY((1 - t) * p1.y() + t * p2.y());
    
    return res;
}
float GLWidget::maxDifference(Points points) {

    float max = 0;

    for (int i = 0; i < points.getCount()-2; i++) {

        float SFD = points.getPointY(i + 2) - 2 * points.getPointY(i + 1) + points.getPointY(i);
        if (SFD > max) {
            max = SFD;
        }
    }
    return max;

}
void GLWidget::SelfIntersectionBezier(Points b) {
    std::vector<pair<QPointF, QPointF>> Seg_list;

    for (float t = 0; t < 1; t += 0.02) {
        QPointF point1;
        QPointF point2;

        point1 = getCasteljauPoint(b, b.getCount() - 1, 0, t);
        point2 = getCasteljauPoint(b, b.getCount() - 1, 0, t + 0.02);
        glColor3f(0.0, 1.0, 1.0);
        glPointSize(5);
        glBegin(GL_POINTS);
        //glVertex2f(point1.x(), point1.y());
        //glVertex2f(point2.x(), point2.y());

        glEnd();
        
        Seg_list.push_back(make_pair(point1,point2));
      
        
    }
    for (int i = 0; i < Seg_list.size()-1; i++) {
        for (int j = 0; j < Seg_list.size()-1; j++) {
            QPointF inter;

            inter = SelfIntersectionAux(Seg_list.at(i), Seg_list.at(j));


            glColor3f(1.0, 0.0, 1.0);
            glPointSize(5);
            glBegin(GL_POINTS);
            glVertex2f(inter.x(), inter.y());

            glEnd();
            update();
        }
    }

    

   
}
int GLWidget::IsPointInBoundingBox(float x1, float y1, float x2, float y2, float px, float py){

    float left, top, right, bottom; // Bounding Box For Line Segment

    // For Bounding Box

    if (x1 < x2){

        left = x1;

        right = x2;

    }

    else{
        left = x2;
        right = x1;

    }
    if (y1 < y2){
        top = y1;
        bottom = y2;
    }
    else{
        top = y1;
        bottom = y2;
    }

    if ((px + 0.01) >= left && (px - 0.01) <= right && (py + 0.01) >= top && (py - 0.01) <= bottom){
        return 1;
    }
    else {
        return 0;
    }
}
float GLWidget::Round(float val) {
    int dec = 10000;
    float res = floorf(val * dec) / dec;   /* Result: 37.77 */

    return res;
}
QPointF GLWidget::SelfIntersectionAux(pair<QPointF,QPointF> A, pair<QPointF, QPointF> B) {
    
    
   
    QPointF res;
    float interX = 0.0;
    float interY = 0.0;
    
    
    float dx1, dy1, dx2, dy2, m1,m2,c1,c2;
    dx1 = A.second.x()-A.first.x();
    dy1 = A.second.y() - A.first.y();

    m1 = dy1 / dx1;

    
    c1 = A.first.y() - m1 * A.first.x();

    dx2 = B.second.x() - B.first.x();
    dy2 = B.second.y() - B.first.y();
    m2 = dy2 / dx2;

    // y = mx + c

    // intercept c = y - mx

    c2 = B.first.y() - m2 * B.first.x(); // which is same as y2 - slope * x2


    QPointF end;
    if ((m1 - m2) == 0) {

        return end;
    }
    else{

        interX = (c2 - c1) / (m1 - m2);

        interY = m1 * interX + c1;

    }
    if (IsPointInBoundingBox(A.first.x(), A.first.y(), A.second.x(), A.second.y(), interX, interY) == 1 && IsPointInBoundingBox(B.first.x(), B.first.y(), B.second.x(), B.second.y(), interX, interY) == 1) {
        
        float P2x,P2y,P3x,P3y;
        P2x = Round(A.second.x());
        P2y = Round(A.second.y());
        P3x = Round(B.first.x());
        P3y = Round(B.first.y());

        float difx23 = abs(P2x - P3x);
        float dify23 = abs(P2y - P3y);
        
        float P1x, P1y, P4x, P4y;
        P1x = Round(A.first.x());
        P1y = Round(A.first.y());
        P4x = Round(B.second.x());
        P4y = Round(B.second.y());

        float difx14 = abs(P1x - P4x);
        float dify14 = abs(P1y - P4y);
        if ( difx23 != 0 && dify23 != 0 && difx14 != 0 && dify14 != 0) {
            res.setX(interX);
            res.setY(interY);
        
        }
    }

       

    

    return res;
}




void GLWidget::IntersectionBezier(Points b, Points c) {

    std::vector<QPointF> B;
    std::vector<QPointF> C;
    // transfor to a vector of points
    for (int i = 0; i < b.getCount(); i++) {
        QPointF aux;
        aux.setX(b.getPointX(i));
        aux.setY(b.getPointY(i));

        B.push_back(aux);

    }
    for (int i = 0; i < c.getCount(); i++) {
        QPointF aux;
        aux.setX(c.getPointX(i));
        aux.setY(c.getPointY(i));

        C.push_back(aux);

    }
    //sort 
    sort(B.begin(), B.end(), compare_y);
    sort(C.begin(), C.end(), compare_y);


    sort(B.begin(), B.end(), compare_x);
    sort(C.begin(), C.end(), compare_x);

    
    //check if the convex hulls intersects each other

    int inter = 0;
    if (B.at(0).x() <= C.at(0).x() || B.at(B.size()-1).x() >= C.at(0).x()) {
        if (B.at(0).y() <= C.at(0).y() || B.at(B.size()-1).y() >= C.at(0).x()) {
        
            inter = 1;
        
        }
    }else if (B.at(0).x() <= C.at(C.size()-1).x() || B.at(B.size()-1).x() >= C.at(C.size()-1).x()) {
        if (B.at(0).y() <= C.at(C.size()-1).y() || B.at(B.size()-1).y() >= C.at(C.size()-1).x()) {

            inter = 1;

        }
    }
    if (inter != 0) {
        if (b.getCount() * (b.getCount()-1) * maxDifference(b) > epsilon_intersection) {
            QPointF res;
            Points lowA;
            Points highA;
            

                
            for (float  t = 0; t < 0.5; t += 0.1) {
                res = getCasteljauPoint(b, b.getCount() - 1, 0, t);
                lowA.addPoint(res.x(), res.y());

            }

                    

                
            for (float t = 0.5; t <= 1; t += 0.1) {
                res = getCasteljauPoint(b, b.getCount() - 1, 0, t);
                highA.addPoint(res.x(), res.y());

            }
                
                
           

           IntersectionBezier(lowA, c);
           IntersectionBezier(highA, c);

            
            
        }
        else if (c.getCount() * (c.getCount() - 1) * maxDifference(c) > epsilon_intersection) {
            QPointF res;
            Points lowD;
            Points highD;
            float t;

            

            for (float t = 0; t < 0.5; t += 0.1) {
                res = getCasteljauPoint(c, c.getCount() - 1, 0, t);
                lowD.addPoint(res.x(), res.y());

            }
                
            for (float t = 0.5; t <= 1; t += 0.1) {
                res = getCasteljauPoint(c, c.getCount() - 1, 0, t);
                highD.addPoint(res.x(), res.y());

            }
                
                
            
            IntersectionBezier(b, lowD);
            IntersectionBezier(b, highD);


        }
        else {
            //Intersect the line segments b0bm and c0cn;

            QPointF q0;
            QPointF qM;
            QPointF c0;
            QPointF cN;
            float c1;
            float c2;

            float interX;
            float interY;

            q0.setX(b.getPointX(0));
            q0.setY(b.getPointY(0));

            qM.setX(b.getPointX(b.getCount()-1));
            qM.setY(b.getPointY(b.getCount()-1));
        
            c0.setX(c.getPointX(0));
            c0.setY(c.getPointY(0));

            cN.setX(c.getPointX(c.getCount()-1));
            cN.setY(c.getPointY(c.getCount()-1));


            float slopeQ = (qM.y() - q0.y()) / (qM.x() - q0.x());
            float slopeC = (cN.y() - c0.y()) / (cN.x() - c0.x());

            c1 = q0.y() - slopeQ * q0.x();
            c2 = c0.y() - slopeC * c0.x();

            interX = (c2 - c1) / (slopeQ - slopeC);

            interY = slopeQ * interX + c1;
            glColor3f(0.0, 0.0, 1.0);
            glPointSize(5);
            glBegin(GL_POINTS);
            glVertex2f(interX, interY);

            glEnd();
            update();

        }

    }
}




void GLWidget::initializeGL()
{
    resizeGL(width(),height());
}

void GLWidget::resizeGL(int width, int height)
{
    aspectx=1.0;
    aspecty=1.0;
    if (width>height) {
        aspectx=float(width)/height;
    }
    else {
        aspecty=float(height)/width;
    }
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-aspectx,aspectx,-aspecty,aspecty);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

QPointF GLWidget::transformPosition(QPoint p)
{
    return QPointF((2.0*p.x()/width() - 1.0)*aspectx,-(2.0*p.y()/height() - 1.0)*aspecty);
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        QPoint pos = event->pos();
        QPointF posF = transformPosition(pos);
        points.setPointX(clickedPoint,posF.x());
        points.setPointY(clickedPoint,posF.y());
        update();
    }
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        QPoint pos = event->pos();
        QPointF posF = transformPosition(pos);
        clickedPoint = points.getClosestPoint(posF.x(),posF.y());
        points.setPointX(clickedPoint,posF.x());
        points.setPointY(clickedPoint,posF.y());
        update();
    }
}

void GLWidget::mouseDoubleClickEvent(QMouseEvent *)
{
}

void GLWidget::setIntersection(int state)
{
    doIntersection = (state == Qt::Checked);
    update();
}

void GLWidget::setSelfIntersection(int state)
{
    doSelfIntersection = (state == Qt::Checked);
    update();
}

void GLWidget::setEpsilonDraw(double value)
{
    epsilon_draw = value;
}

void GLWidget::setEpsilonIntersection(double value)
{
    epsilon_intersection = value;
}
