#ifndef FRAGMENTSHADER_H
#define FRAGMENTSHADER_H

#include "payload.h"
#include "eigen3/Eigen/Eigen"
#include "algorithm"
#include "vector"

#include "QPixmap"
#include "QImage"
#include "QString"

namespace FragmentShader {

   Eigen::Vector3f normalShader(payload &pl);
   Eigen::Vector3f phongShader(payload &pl);
   Eigen::Vector3f textureShader(payload &pl);

   Eigen::Vector3f colorFromTex(float u,float v);

   void setAmbientL(float i);
   void setLightL(float i);
   void showImage();
   void setTex(std::string a);
   void LoadTex();
};

#endif // FRAGMENTSHADER_H
