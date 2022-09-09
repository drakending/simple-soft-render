#include "fragmentshader.h"
#include "QPixmap"
#include "QImage"
#include "QString"
#include "QColor"
#include "iostream"
#include "QtDebug"
#include "QRgb"
float ambientL=5;
float lightL=5;
std::string texFile;
QImage *Tex=NULL;
Eigen::Vector3f FragmentShader::normalShader(payload &pl)
{
    Eigen::Vector3f return_color = (pl.normal.normalized() + Eigen::Vector3f(1.0f, 1.0f, 1.0f)) / 2.f;
    return return_color;
}
static Eigen::Vector3f reflect(const Eigen::Vector3f& vec, const Eigen::Vector3f& axis)
{
    auto costheta = vec.dot(axis);
    return (2 * costheta * axis - vec).normalized();
}
struct light
{
    Eigen::Vector3f position;
    Eigen::Vector3f intensity;
};
Eigen::Vector3f FragmentShader::phongShader(payload &pl)
{
    Eigen::Vector3f ka = Eigen::Vector3f(0.005, 0.005, 0.005);
    Eigen::Vector3f kd = pl.color;
    Eigen::Vector3f ks = Eigen::Vector3f(0.7937, 0.7937, 0.7937);

    auto l1 = light{{20, 20, 20}, {lightL, lightL, lightL}};
    auto l2 = light{{-20, 20, 0}, {lightL, lightL, lightL}};

    std::vector<light> lights = {l1, l2};
    Eigen::Vector3f amb_light_intensity{ambientL, ambientL, ambientL};
    Eigen::Vector3f eye_pos{0, 0, 10};

    float p = 150;

    Eigen::Vector3f color = pl.color;
    Eigen::Vector3f point = pl.shadingCoord;
    Eigen::Vector3f normal = pl.normal;

    Eigen::Vector3f result_color = {0, 0, 0};
    for (auto& light : lights)
    {
        Eigen::Vector3f l = (light.position - point).normalized();      // 光
        Eigen::Vector3f v = (eye_pos - point).normalized();		        // 眼
        Eigen::Vector3f h = (l + v).normalized();                       // 半程向量
        double r_2 = (light.position - point).dot(light.position - point);
        Eigen::Vector3f Ld = kd.cwiseProduct(light.intensity / r_2) * std::max(0.0f, normal.dot(l));    //cwiseProduct()函数允许Matrix直接进行点对点乘法,而不用转换至Array。
        Eigen::Vector3f Ls = ks.cwiseProduct(light.intensity / r_2) * std::pow(std::max(0.0f, normal.dot(h)), p);
        result_color += (Ld + Ls);
    }
    Eigen::Vector3f La = ka.cwiseProduct(amb_light_intensity);
    result_color+=La;
    return result_color;
}

void FragmentShader::setAmbientL(float i)
{
    ambientL=i;
}

void FragmentShader::setLightL(float i)
{
    lightL=i;
}

void FragmentShader::setTex(std::string a)
{
    texFile=a;
}

Eigen::Vector3f FragmentShader::textureShader(payload &pl)
{
    Eigen::Vector3f ka = Eigen::Vector3f(0.005, 0.005, 0.005);
    Eigen::Vector3f kd = colorFromTex(pl.texCoord.x(),pl.texCoord.y());
    Eigen::Vector3f ks = Eigen::Vector3f(0.7937, 0.7937, 0.7937);

    auto l1 = light{{20, 20, 20}, {lightL, lightL, lightL}};
    auto l2 = light{{-20, 20, 0}, {lightL, lightL, lightL}};

    std::vector<light> lights = {l1, l2};
    Eigen::Vector3f amb_light_intensity{ambientL, ambientL, ambientL};
    Eigen::Vector3f eye_pos{0, 0, 10};

    float p = 150;

    Eigen::Vector3f color = pl.color;
    Eigen::Vector3f point = pl.shadingCoord;
    Eigen::Vector3f normal = pl.normal;

    Eigen::Vector3f result_color = {0, 0, 0};
    for (auto& light : lights)
    {
        Eigen::Vector3f l = (light.position - point).normalized();      // 光
        Eigen::Vector3f v = (eye_pos - point).normalized();		        // 眼
        Eigen::Vector3f h = (l + v).normalized();                       // 半程向量
        double r_2 = (light.position - point).dot(light.position - point);
        Eigen::Vector3f Ld = kd.cwiseProduct(light.intensity / r_2) * std::max(0.0f, normal.dot(l));    //cwiseProduct()函数允许Matrix直接进行点对点乘法,而不用转换至Array。
        Eigen::Vector3f Ls = ks.cwiseProduct(light.intensity / r_2) * std::pow(std::max(0.0f, normal.dot(h)), p);
        result_color += (Ld + Ls);
    }
    Eigen::Vector3f La = ka.cwiseProduct(amb_light_intensity);
    result_color+=La;
    return result_color;
}

void FragmentShader::LoadTex()
{
    QString file=QString::fromStdString(texFile);
    Tex= new QImage(file);
    Tex->convertToFormat(QImage::Format_RGB32);
}
Eigen::Vector3f FragmentShader::colorFromTex(float u,float v)
{
    int u_img=int(u*Tex->width());
    int v_img=int((1-v)*Tex->height());
    if(u_img<0) u_img+=Tex->width();
    if(v_img<0) v_img+=Tex->height();
    QColor color=Tex->pixelColor(u_img,v_img);
    int r=color.red(),g=color.green(),b=color.blue();
    return Eigen::Vector3f(r*1.0,g*1.0,b*1.0);
}


void FragmentShader::showImage()
{
    for(int i=0;i<Tex->width();i++)
        for(int j=0;j<Tex->height();j++)
        {
            QColor color=Tex->pixel(i,j);
            int r,g,b;
            color.getRgb(&r,&g,&b);
            std::cout<<r<<" "<<g<<" "<<b<<std::endl;;
        }

}
