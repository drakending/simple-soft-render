#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#include <QWidget>
#include <eigen3/Eigen/Eigen>
#include "triangle.h"
#include "vector"
class DisplayWindow : public QWidget
{
    Q_OBJECT
private:
    int width;
    int height;
    int windowY(int y);
    QColor* pixelBuffer;
    Eigen::Vector3f eyePosition,view,viewUp;
    Eigen::Matrix4f viewMatrix,projectionMatrix,modelMatrix;
    std::vector<Eigen::Vector3f> position;
    std::vector<Eigen::Vector3f> indices;
    float eye_fov,aspect_ratio,zNear,zFar;

public:
    explicit DisplayWindow(QWidget *parent = nullptr);
    void clear();
    void drawPoint(Eigen::Vector3f &position,QColor color=Qt::black);
    void drawPoint(Eigen::Vector2f &position,QColor color=Qt::black);
    void drawLine(Eigen::Vector3f &pointA,Eigen::Vector3f &pointB,QColor color=Qt::black);
    void drawTriangle(Triangle &t);

    void setEyePosition(Eigen::Vector3f& a);
    void setView(Eigen::Vector3f&a);
    void setViewUp(Eigen::Vector3f&a);

    void loadPosition(const std::vector<Eigen::Vector3f> &positions);
    void loadIndices(const std::vector<Eigen::Vector3f>&indices);

    void updateViewMatrix();
    void updateProjectionMatrix();
    void updateModelMatrix(float angle);

    void render();

protected:
    virtual void paintEvent(QPaintEvent *event) override;

signals:

};

#endif // DISPLAYWINDOW_H
