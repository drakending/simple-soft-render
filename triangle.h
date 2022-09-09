#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "eigen3/Eigen/Eigen"

class Triangle
{
public:
    Triangle();
    Triangle(Eigen::Vector3f &x,Eigen::Vector3f &y,Eigen::Vector3f &z);
    Eigen::Vector3f& getX();
    Eigen::Vector3f& getY();
    Eigen::Vector3f& getZ();
    Eigen::Vector3f& getColor(int i);
    Eigen::Vector2f& getTexCoord(int i);
    Eigen::Vector3f& getNormal(int i);

    Eigen::Vector3f& operator[](int i);

    void setColor(Eigen::Vector3f &x,int i);
    void setTexcoord(Eigen::Vector2f &x,int i);
    void setNormal(Eigen::Vector3f &x,int i);
    void setVertex(Eigen::Vector3f &x,int i);
private:
    Eigen::Vector3f vertex[3];
    Eigen::Vector3f color[3];
    Eigen::Vector2f texCoord[3];
    Eigen::Vector3f normal[3];
};
Eigen::Vector4f toVec4(Eigen::Vector3f &a,float i=1.0f);

Eigen::Vector3f toVec3(Eigen::Vector4f & a);

bool insideTriangle(Eigen::Vector4f _v[3],double x,double y);
#endif // TRIANGLE_H
