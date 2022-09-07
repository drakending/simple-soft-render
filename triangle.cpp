#include "triangle.h"

Triangle::Triangle()
{
    for(int i=0;i<3;i++)
    {
        vertex[i].x()=vertex[i].y()=vertex[i].z()=0;
    }
}

Triangle::Triangle(Eigen::Vector3f& inx, Eigen::Vector3f &iny, Eigen::Vector3f &inz)
{
    vertex[0].x()=inx.x(),vertex[0].y()=inx.y(),vertex[0].z()=inx.z();
    vertex[1].x()=iny.x(),vertex[1].y()=iny.y(),vertex[1].z()=iny.z();
    vertex[2].x()=inz.x(),vertex[2].y()=inz.y(),vertex[2].z()=inz.z();
}

Eigen::Vector3f &Triangle::getX()
{
    return vertex[0];
}

Eigen::Vector3f &Triangle::getY()
{
    return vertex[1];
}

Eigen::Vector3f &Triangle::getZ()
{
    return vertex[2];
}


Eigen::Vector4f toVec4(Eigen::Vector3f &a)
{
    return Eigen::Vector4f(a.x(),a.y(),a.z(),1.0);
}

Eigen::Vector3f toVec3(Eigen::Vector4f &a)
{
    return Eigen::Vector3f(a.x(),a.y(),a.z());
}
