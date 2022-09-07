#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "eigen3/Eigen/Eigen"
#include "string"
#include "iostream"
#include "OBJ_Loader.h"
#include "triangle.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->widget->setMode(DisplayWindow::MODE::grid);
    objl::Loader Loader;
    std::string filename="output.png";
    std::string obj_path="models/spot";
    bool loadout=Loader.LoadFile("D:/googledownload/Hw3/Assignment3/Code/models/spot/spot_triangulated_good.obj");
    for(auto mesh:Loader.LoadedMeshes)
    {
        for(int i=0;i<mesh.Vertices.size();i+=3)
        {
            Triangle* t= new Triangle();
            for(int j=0;j<3;j++)
            {
                Eigen::Vector3f ver(mesh.Vertices[i+j].Position.X,mesh.Vertices[i+j].Position.Y,mesh.Vertices[i+j].Position.Z),
                        nor(mesh.Vertices[i+j].Normal.X,mesh.Vertices[i+j].Normal.Y,mesh.Vertices[i+j].Normal.Z);
                Eigen::Vector2f tex(mesh.Vertices[i+j].TextureCoordinate.X, mesh.Vertices[i+j].TextureCoordinate.Y);
                t->setVertex(ver,j);
                t->setNormal(nor,j);
                t->setTexcoord(tex,j);
            }
            ui->widget->triangleList.push_back(t);
        }
    }
    Eigen::Vector3f viewPos(0,0,10),view(0,0,-1);
    Eigen::Vector3f viewUp(0,1,0);
    ui->widget->setEyePosition(viewPos);
    ui->widget->setView(view);
    ui->widget->setViewUp(viewUp);
    ui->widget->updateViewMatrix();
    ui->widget->updateModelMatrix(-140);
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

void MainWindow::on_meshBtn_clicked(bool checked)
{
    if(checked)
    {
        ui->widget->setMode(DisplayWindow::MODE::grid);

    }
    else
    {
        ui->widget->setMode(DisplayWindow::MODE::color);
    }
    ui->widget->render();
}
