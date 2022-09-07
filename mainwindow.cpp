#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "eigen3/Eigen/Eigen"
#include "triangle.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    std::vector<Eigen::Vector3f> pos
            {
                    {2, 0, -2},
                    {0, 2, -2},
                    {-2, 0, -2},
                    {3.5, -1, -5},
                    {2.5, 1.5, -5},
                    {-1, 0.5, -5}
            };

    std::vector<Eigen::Vector3f> ind
            {
                    {0, 1, 2},
                    {3, 4, 5}
            };

    ui->widget->loadPosition(pos);
    ui->widget->loadIndices(ind);
    std::vector<Eigen::Vector3f> color{{0,1,0},{1,0,0}};
    ui->widget->loadColors(color);
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
