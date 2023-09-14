#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->checkBox_Intersection,             SIGNAL(stateChanged(int   )),ui->glwidget,SLOT(setIntersection       (int   )));
    QObject::connect(ui->checkBox_SelfIntersection,         SIGNAL(stateChanged(int   )),ui->glwidget,SLOT(setSelfIntersection   (int   )));
    QObject::connect(ui->doubleSpinBox_epsilon_draw,        SIGNAL(valueChanged(double)),ui->glwidget,SLOT(setEpsilonDraw        (double)));
    QObject::connect(ui->doubleSpinBox_epsilon_intersection,SIGNAL(valueChanged(double)),ui->glwidget,SLOT(setEpsilonIntersection(double)));
}

MainWindow::~MainWindow()
{
    delete ui;
}
