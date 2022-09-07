#include "triangle.h"

Triangle::Triangle()
{
    for(int i=0;i<3;i++)
    {
        vertex[i].x()=vertex[i].y()=vertex[i].z()=0;
    }
    color<<0,0,0;
}

Triangle::Triangle(Eigen::Vector3f& inx, Eigen::Vector3f &iny, Eigen::Vector3f &inz)
{
    vertex[0].x()=inx.x(),vertex[0].y()=inx.y(),vertex[0].z()=inx.z();
    vertex[1].x()=iny.x(),vertex[1].y()=iny.y(),vertex[1].z()=iny.z();
    vertex[2].x()=inz.x(),vertex[2].y()=inz.y(),vertex[2].z()=inz.z();
    color<<0,0,0;
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

Eigen::Vector3f &Triangle::getColor()
{
    return color;
}

Eigen::Vector3f &Triangle::operator[](int i)
{
    return vertex[i];
}

void Triangle::setColor(Eigen::Vector3f &x)
{
    color.x()=x.x();
    color.y()=x.y();
    color.z()=x.z();
}


Eigen::Vector4f toVec4(Eigen::Vector3f &a)
{
    return Eigen::Vector4f(a.x(),a.y(),a.z(),1.0);
}

Eigen::Vector3f toVec3(Eigen::Vector4f &a)
{
    return Eigen::Vector3f(a.x(),a.y(),a.z());
}

bool insideTriangle(Eigen::Vector4f _v[], double x, double y)
{
    Eigen::Vector3f p0p1(_v[0].x() - _v[1].x(), _v[0].y() - _v[1].y(),1.0f);
    Eigen::Vector3f p1p2(_v[1].x() - _v[2].x(), _v[1].y() - _v[2].y(), 1.0f);
    Eigen::Vector3f p2p0(_v[2].x() - _v[0].x(), _v[2].y() - _v[0].y(), 1.0f);

    Eigen::Vector3f p0p(_v[0].x() - x, _v[0].y() - y, 1.0f);
    Eigen::Vector3f p1p(_v[1].x() - x, _v[1].y() - y, 1.0f);
    Eigen::Vector3f p2p(_v[2].x() - x, _v[2].y() - y, 1.0f);

    if (p0p1.cross(p0p).z() > 0.f) {
        return p1p2.cross(p1p).z() > 0.f && p2p0.cross(p2p).z() > 0.f;
    }else {
        return p1p2.cross(p1p).z() < 0.f && p2p0.cross(p2p).z() < 0.f;
    }

}
