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
private:
    Eigen::Vector3f vertex[3];

};
Eigen::Vector4f toVec4(Eigen::Vector3f & a);



Eigen::Vector3f toVec3(Eigen::Vector4f & a);

#endif // TRIANGLE_H
