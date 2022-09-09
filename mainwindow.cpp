#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "eigen3/Eigen/Eigen"
#include "string"
#include "iostream"
#include "QFileDialog"
#include "OBJ_Loader.h"
#include "triangle.h"
#include "fragmentshader.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->widget->setMode(DisplayWindow::MODE::grid);
    OBJFile="D:/model/mona.obj";
    TexFile="D:/googledownload/mona-genshin-impact/source/Mona/Mona/Mona/Texture/3.png";
    loadFile();
    Eigen::Vector3f viewPos(0,0,10),view(0,0,-1);
    Eigen::Vector3f viewUp(0,1,0);
    ui->widget->setEyePosition(viewPos);
    ui->widget->setView(view);
    ui->widget->setViewUp(viewUp);
    ui->widget->updateViewMatrix();
    ui->widget->updateModelMatrix();
    ui->widget->render();
    ui->widget->setFragmentShader(FragmentShader::phongShader);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setFile()
{
    QString obj=QFileDialog::getOpenFileName(this,tr("obj file"),tr(".obj"));
    QString tex=QFileDialog::getOpenFileName(this,tr("texture file"),tr(".png"));
    OBJFile=obj.toStdString();
    TexFile=tex.toStdString();
}

void MainWindow::loadFile()
{
    ui->widget->triangleList.clear();
    objl::Loader Loader;
    Loader.LoadFile(OBJFile);
    FragmentShader::setTex(TexFile);
    FragmentShader::LoadTex();
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
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->widget->yAngle=value;
    ui->widget->updateModelMatrix();
    ui->widget->render();
}

void MainWindow::on_comboBox_activated(int index)
{
    if(index==0)
        ui->widget->setMode(DisplayWindow::MODE::grid);
    else
    {
        ui->widget->setMode(DisplayWindow::MODE::color);
        if(index==1)
        {
            ui->widget->setFragmentShader(FragmentShader::normalShader);
        }
        else if(index==2)
        {
            ui->widget->setFragmentShader(FragmentShader::phongShader);
        }
        else if(index==3)
        {
            ui->widget->setFragmentShader(FragmentShader::textureShader);
        }
    }
    ui->widget->render();
}

void MainWindow::on_kaS_valueChanged(int value)
{
    FragmentShader::setAmbientL(value*1.0/10);
    ui->widget->render();
    ui->kaL->setText(QString::asprintf("%.1f",value*1.0/10));
}

void MainWindow::on_ksS_valueChanged(int value)
{
    FragmentShader::setLightL(value*1.0/10);
    ui->widget->render();
    ui->ksL->setText(QString::asprintf("%.1f",value*1.0/10));

}

void MainWindow::on_loadBtn_clicked()
{
    setFile();
    loadFile();
    ui->widget->render();
}

void MainWindow::on_sizeDSB_valueChanged(double arg1)
{
    ui->widget->setSize(arg1);
    ui->widget->updateModelMatrix();
    ui->widget->render();
}

void MainWindow::on_upBtn_clicked()
{
    ui->widget->yOffset+=0.1;
    ui->widget->updateModelMatrix();
    ui->widget->render();
}

void MainWindow::on_downBtn_clicked()
{
    ui->widget->yOffset-=0.1;
    ui->widget->updateModelMatrix();
    ui->widget->render();
}

void MainWindow::on_leftBtn_clicked()
{
    ui->widget->xOffset-=0.1;
    ui->widget->updateModelMatrix();
    ui->widget->render();
}

void MainWindow::on_rightBtn_clicked()
{
    ui->widget->xOffset+=0.1;
    ui->widget->updateModelMatrix();
    ui->widget->render();
}


void MainWindow::on_xAngelhS_valueChanged(int value)
{
    ui->widget->xAngle=value;
    ui->widget->updateModelMatrix();
    ui->widget->render();
}
