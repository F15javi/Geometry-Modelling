#include "glwidget.h"
#include <QtGui>
#include <GL/glu.h>
#include "glut.h"
#include "mainwindow.h"

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    epsilon_draw = 0.05;

    // Hier Punkte hinzufügen: Schönere Startpositionen und anderer Grad!
    points.addPoint(-1.00,  0.5);
    points.addPoint(-0.30, -0.25);
    points.addPoint( 0.00,  0.5);
    points.addPoint( 0.30, -0.25);
    points.addPoint( 1.00,  0.5);

    knots.insertKnot(0.05);
    knots.insertKnot(0.1);
    knots.insertKnot(0.3);
    knots.insertKnot(0.4);
    knots.insertKnot(0.5);
    knots.insertKnot(0.7);
    knots.insertKnot(0.9);
    knots.insertKnot(0.95);
    copyPoints = points;
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

    // Kontrollunkte zeichnen
    glPointSize(7.0);
    glBegin(GL_POINTS);
    glColor3f(1.0,0.0,0.0);
    for (int i=0; i<copyPoints.getCount(); i++) {
        glVertex2f(copyPoints.getPointX(i), copyPoints.getPointY(i));
    }
    glEnd();
    glPointSize(7.0);
    glBegin(GL_POINTS);
    glColor3f(0.0, 1.0, 0.0);
    for (int i = 0; i < points.getCount(); i++) {
        glVertex2f(points.getPointX(i), points.getPointY(i));
    }
    glEnd();
    
    // Kontrollpolygon zeichnen
    glColor3f(0.0,0.0,1.0);
    glBegin(GL_LINE_STRIP);
    for (int i=0; i< copyPoints.getCount(); i++) {
        glVertex2f(copyPoints.getPointX(i), copyPoints.getPointY(i));
    }
    glEnd();

    // Knoten zeichnen
    glColor3f(0.0,1.0,1.0);
    glBegin(GL_LINE_STRIP);
    for (int i=0; i<knots.getCount(); i++) {
        glVertex2f(knots.getPointX(i),knots.getPointY(i));
    }
    glEnd();
    glColor3f(1.0,0.0,1.0);
    glBegin(GL_POINTS);
    for (int i=0; i<knots.getCount(); i++) {
        glVertex2f(knots.getPointX(i),knots.getPointY(i));
    }
    glEnd();

    // Kurve zeichnen
    // AUFGABE: Dibujar curva aquí
    // usar epsilon_draw
    //draw_curve();
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
        if (clickedKnot >= 0) {
            knots.setValueX(clickedKnot,posF.x());
        }
        else {
            points.setPointX(clickedPoint,posF.x());
            points.setPointY(clickedPoint,posF.y());
        }
        update();
    }
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    QPointF posF = transformPosition(pos);
    if (event->buttons() & Qt::LeftButton) {
        clickedPoint = points.getClosestPoint(posF.x(),posF.y());
        clickedKnot = knots.getClosestPoint(posF.x(),posF.y());
        if (points.getDistance(clickedPoint,posF.x(),posF.y()) <=
             knots.getDistance(clickedKnot, posF.x(),posF.y()))
        { // point was clicked
            points.setPointX(clickedPoint,posF.x());
            points.setPointY(clickedPoint,posF.y());
            clickedKnot = -1;
        }
        else
        { // knot was clicked
            knots.setValueX(clickedKnot,posF.x());
            clickedPoint = -1;
        }
    }
    if (event->buttons() & Qt::RightButton) {
        Knot_Insertion(posF.x());
        knots.insertKnotX(posF.x());		
		// AUFGABE: Inserte nudos en una curva B-Spline aquí.

    }
    update();
}

void GLWidget::setEpsilonDraw(double value)
{
    epsilon_draw = value;
}

void GLWidget::Knot_Insertion(float x) {

    int i = 0;
    int r = find_knot(x);
    int n = 3;

    while (i < points.getCount()-1) {

        if (i <= r - n) {

           newPoints.addPoint(points.getPointX(i), points.getPointY(i));
        }
        else if (r - n < i && i <= r) {
            float alpha = (x - knots.getPointX(i)) / (knots.getPointX(i + n) - knots.getPointX(i));
            float new_x = (1 - alpha) * points.getPointX(i - 1) + alpha * points.getPointX(i);
            float new_y = (1 - alpha) * points.getPointY(i - 1) + alpha * points.getPointY(i);
            newPoints.addPoint(new_x, new_y);


        }
        else if(i > r) {
            newPoints.addPoint(points.getPointX(i - 1), points.getPointY(i - 1));

        }
        i++;
    }
    points = newPoints;
    
}
float GLWidget::find_knot(float x){

    int i = 0;
    float res;

    while (i < knots.getCount()-1) {

        float debug1 = knots.getPointX(i);
        float debug2 = knots.getPointX(i+1);


        if (x >= knots.getPointX(i) && x < knots.getPointX(i + 1)) {
            res = i+1;
        }
        

        
        
        i++;
    }

    return res;


}
void GLWidget::draw_curve() {
    int n = 3;
    int i;
    int j;
    int m = points.getCount();
    for (i = j; i < m; i++) {

        float x = points.getPointX(i) * recFuncX(i,j,n);
        float y = points.getPointY(i) * recFuncY(i,j,n);
        glColor3f(1.0, 1.0, 1.0);
        glPointSize(2);
        glBegin(GL_POINTS);
        glVertex2f(x, y);
        glEnd();
        update();
    }
    
}
float GLWidget::recFuncX(int i, int j,int n) {
    
    float res;

    if (j == 0) {
        res = points.getPointX(i);
        return res;
    }
    else if (j > 0) {

        float aux = (n - (j - 1)) * ((recFuncX(i, j - 1, n) - recFuncX(i - 1, j - 1, n)) / (knots.getValue(i + n - (j - 1)) - knots.getValue(i)));
        return aux;
    }

}
float GLWidget::recFuncY(int i, int j, int n) {

    float res;

    if (j == 0) {
        res = points.getPointY(i);
        return res;
    }
    else if (j > 0) {

        float aux = (n - (j - 1)) * ((recFuncY(i, j - 1, n) - recFuncY(i - 1, j - 1, n)) / (knots.getValue(i + n - (j - 1)) - knots.getValue(i)));
        return aux;
    }

}