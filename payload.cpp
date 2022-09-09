#include "payload.h"

payload::payload(Eigen::Vector3f &_shadingCoord, Eigen::Vector3f &_normal, Eigen::Vector3f &_color, Eigen::Vector2f &_position, Eigen::Vector2f &_texCoord)
    :shadingCoord(_shadingCoord),normal(_normal),color(_color),pos(_position),texCoord(_texCoord)
{

}
