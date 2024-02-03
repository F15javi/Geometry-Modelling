#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->doubleSpinBox_epsilon_draw,SIGNAL(valueChanged(double)),ui->glwidget,SLOT(setEpsilonDraw(double)));
}

MainWindow::~MainWindow()
{
    delete ui;
}
