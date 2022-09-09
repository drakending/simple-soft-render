#ifndef PAYLOAD_H
#define PAYLOAD_H
#include "triangle.h"

class payload
{
public:
    Eigen::Vector3f shadingCoord,normal,color;
    Eigen::Vector2f pos,texCoord;
    payload(Eigen::Vector3f &_shadingCoord,Eigen::Vector3f &_normal,Eigen::Vector3f &_color,
            Eigen::Vector2f &_position,Eigen::Vector2f &_texCoord);
};

#endif // PAYLOAD_H
