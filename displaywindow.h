#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#include <QWidget>
#include <eigen3/Eigen/Eigen>
#include "triangle.h"
#include "vector"
#include "payload.h"
class DisplayWindow : public QWidget
{
    Q_OBJECT
public:
    enum MODE{
        grid,color
    };
private:
    int width;
    int height;
    float size;


    int windowY(int y);
    QColor* pixelBuffer;
    float * zBuffer;
    Eigen::Vector3f eyePosition,view,viewUp;
    Eigen::Matrix4f viewMatrix,projectionMatrix,modelMatrix;
    std::vector<Eigen::Vector3f> position;
    std::vector<Eigen::Vector3f> indices;
    std::vector<Eigen::Vector3f> colors;
    float eye_fov,aspect_ratio,zNear,zFar;

    DisplayWindow::MODE mode;

    std::function<Eigen::Vector3f(payload&)> fragmentShader;

public:
    float xOffset,yOffset;
    float xAngle,yAngle,zAngle;

    std::vector<Triangle*> triangleList;

    explicit DisplayWindow(QWidget *parent = nullptr);
    void clear();
    void drawPoint(Eigen::Vector3f &position,QColor color=Qt::black);
    void drawPoint(Eigen::Vector2f &position,QColor color=Qt::black);
    void drawLine(Eigen::Vector3f &pointA,Eigen::Vector3f &pointB,QColor color=Qt::black);
    void drawTriangle(Triangle &t);

    void setEyePosition(Eigen::Vector3f& a);
    void setView(Eigen::Vector3f&a);
    void setViewUp(Eigen::Vector3f&a);
    void setMode(DisplayWindow::MODE in_mode);
    void setFragmentShader(std::function<Eigen::Vector3f(payload&)>_fragmentShader);
    void setSize(float i);

    void loadPosition(const std::vector<Eigen::Vector3f> &positions);
    void loadIndices(const std::vector<Eigen::Vector3f>&indices);
    void loadColors(const std::vector<Eigen::Vector3f>&col);

    void updateViewMatrix();
    void updateProjectionMatrix();
    void updateModelMatrix();

    void render();
    void setRenderMode(DisplayWindow::MODE in_mode);

protected:
    virtual void paintEvent(QPaintEvent *event) override;

signals:

};
QColor vec2Col(Eigen::Vector3f & col);
#endif // DISPLAYWINDOW_H
