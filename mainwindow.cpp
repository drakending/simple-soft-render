#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "eigen3/Eigen/Eigen"
#include "triangle.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    std::vector<Eigen::Vector3f> position{{2, 0, -2},{0, 2, -2},{-2, 0, -2}};
    ui->widget->loadPosition(position);
    std::vector<Eigen::Vector3f> index{{0,1,2}};
    ui->widget->loadIndices(index);
    Eigen::Vector3f viewPos(0,0,5),view(0,0,-1);
    Eigen::Vector3f viewUp(0,1,0);
    ui->widget->setEyePosition(viewPos);
    ui->widget->setView(view);
    ui->widget->setViewUp(viewUp);
    ui->widget->updateViewMatrix();
    ui->widget->render();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->widget->updateModelMatrix(value);
    ui->widget->render();
}
