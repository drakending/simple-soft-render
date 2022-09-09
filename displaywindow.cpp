#include "displaywindow.h"
#include <QPainter>
#include "QDebug"
#include "iostream"
#include <algorithm>
#include <math.h>
#include <QDebug>
#include <fragmentshader.h>
#define PI 3.1415926

DisplayWindow::DisplayWindow(QWidget *parent) : QWidget(parent)
{
    width=700,height=700;
    pixelBuffer=new QColor[700*700];
    zBuffer=new float[700*700];
    eye_fov=45;
    aspect_ratio=1;
    zNear=0.1;
    size=3;
    zFar=50;
    xOffset=yOffset=0;
    updateProjectionMatrix();
    modelMatrix=Eigen::Matrix4f::Identity();
    mode=DisplayWindow::MODE::color;
}
static void computeBarycentric2D(float x, float y, const Eigen::Vector4f* v,float &a,float &b,float &c)
{
    a = (x*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*y + v[1].x()*v[2].y() - v[2].x()*v[1].y()) / (v[0].x()*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*v[0].y() + v[1].x()*v[2].y() - v[2].x()*v[1].y());
    b = (x*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*y + v[2].x()*v[0].y() - v[0].x()*v[2].y()) / (v[1].x()*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*v[1].y() + v[2].x()*v[0].y() - v[0].x()*v[2].y());
    c = (x*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*y + v[0].x()*v[1].y() - v[1].x()*v[0].y()) / (v[2].x()*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*v[2].y() + v[0].x()*v[1].y() - v[1].x()*v[0].y());
}

void DisplayWindow::clear()
{
    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
        {
            pixelBuffer[i*height+j]=Qt::white;
            zBuffer[i*height+j]=-10000;
        }
    }
}

void DisplayWindow::drawPoint(Eigen::Vector3f &position, QColor color)
{
    pixelBuffer[int(position.x()*height+position.y())]=color;
    update();
}

void DisplayWindow::drawPoint(Eigen::Vector2f &position, QColor color)
{
    pixelBuffer[int(position.x()*height+position.y())]=color;
    update();
}

void DisplayWindow::drawLine(Eigen::Vector3f &pointA, Eigen::Vector3f &pointB, QColor color)
{
    auto x1 = pointA.x();
    auto y1 = pointA.y();
    auto x2 = pointB.x();
    auto y2 = pointB.y();

    QColor lineColor=color;


    int x,y,dx,dy,dx1,dy1,px,py,xe,ye,i;

    dx=x2-x1;
    dy=y2-y1;
    dx1=fabs(dx);
    dy1=fabs(dy);
    px=2*dy1-dx1;
    py=2*dx1-dy1;

    if(dy1<=dx1)
    {
        if(dx>=0)
        {
            x=x1;
            y=y1;
            xe=x2;
        }
        else
        {
            x=x2;
            y=y2;
            xe=x1;
        }
        Eigen::Vector3f point = Eigen::Vector3f(x, y, 1.0f);
        drawPoint(point,lineColor);
        for(i=0;x<xe;i++)
        {
            x=x+1;
            if(px<0)
            {
                px=px+2*dy1;
            }
            else
            {
                if((dx<0 && dy<0) || (dx>0 && dy>0))
                {
                    y=y+1;
                }
                else
                {
                    y=y-1;
                }
                px=px+2*(dy1-dx1);
            }
//            delay(0);
            Eigen::Vector3f point = Eigen::Vector3f(x, y, 1.0f);
            drawPoint(point,lineColor);
        }
    }
    else
    {
        if(dy>=0)
        {
            x=x1;
            y=y1;
            ye=y2;
        }
        else
        {
            x=x2;
            y=y2;
            ye=y1;
        }
        Eigen::Vector3f point = Eigen::Vector3f(x, y, 1.0f);
        drawPoint(point,lineColor);
        for(i=0;y<ye;i++)
        {
            y=y+1;
            if(py<=0)
            {
                py=py+2*dx1;
            }
            else
            {
                if((dx<0 && dy<0) || (dx>0 && dy>0))
                {
                    x=x+1;
                }
                else
                {
                    x=x-1;
                }
                py=py+2*(dx1-dy1);
            }
            Eigen::Vector3f point = Eigen::Vector3f(x, y, 1.0f);
            drawPoint(point,lineColor);
        }
    }
}

void DisplayWindow::drawTriangle(Triangle &t)
{
    float f1 = (50 - 0.1) / 2.0;
    float f2 = (50 + 0.1) / 2.0;

    if(mode==DisplayWindow::MODE::grid)
    {
        Eigen::Matrix4f trans=projectionMatrix*viewMatrix*modelMatrix;
        Eigen::Vector4f v[]=
        {
            trans*toVec4(t.getX()),
            trans*toVec4(t.getY()),
            trans*toVec4(t.getZ())
        };
        for(auto& vec:v)
        {
            vec/=vec.w();
        }
        for(auto& vert:v)
        {
            vert.x()=0.5*width*(vert.x()+1.0);
            vert.y()=0.5*height*(vert.y()+1.0);
            vert.z() = vert.z() * f1 + f2;
        }
        Eigen::Vector3f v2[]=
        {
            toVec3(v[0]),
            toVec3(v[1]),
            toVec3(v[2])
        };
        drawLine(v2[0],v2[1]);
        drawLine(v2[1],v2[2]);
        drawLine(v2[2],v2[0]);
    }
    else
    {
        Eigen::Matrix4f trans=projectionMatrix*viewMatrix*modelMatrix;
        Eigen::Matrix4f mv=viewMatrix*modelMatrix;
        Eigen::Vector4f v[]=
        {
            trans*toVec4(t.getX()),
            trans*toVec4(t.getY()),
            trans*toVec4(t.getZ())
        };
        for(auto& vec:v)
        {
            vec/=vec.w();
        }
        for(auto& vert:v)
        {
            vert.x()=0.5*width*(vert.x()+1.0);
            vert.y()=0.5*height*(vert.y()+1.0);
            vert.z() = vert.z() * f1 + f2;
        }

        std::array<Eigen::Vector3f,3> viewSpace_pos;
        for(int i=0;i<3;i++)
        {
            Eigen::Vector4f p=mv*toVec4(t.getX());
            viewSpace_pos[i]=toVec3(p);
        }

        Eigen::Matrix4f inv_trans=(viewMatrix*modelMatrix).inverse().transpose();
        Eigen::Vector4f n[]={
            inv_trans*toVec4(t.getNormal(0),0),
            inv_trans*toVec4(t.getNormal(1),0),
            inv_trans*toVec4(t.getNormal(2),0)
        };
        Eigen::Vector3f a(148.0,121.0,92.0);
        for(int i=0;i<3;i++)
        {
            t.setColor(a,i);
        }

        float l=std::min(std::min(v[0].x(),v[1].x()),v[2].x());
        float r=std::max(std::max(v[0].x(),v[1].x()),v[2].x());
        float b=std::min(std::min(v[0].y(),v[1].y()),v[2].y());
        float u=std::max(std::max(v[0].y(),v[1].y()),v[2].y());
        for(int x=(int)l;x<=ceil(r);++x)
        {
            for(int y=(int)b;y<=ceil(u);++y)
            {
                float alpha,beta,gamma;
                computeBarycentric2D(x+0.5, y+0.5, v,alpha,beta,gamma);
                float w_reciprocal = 1.0/(alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
                float z_interpolated = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
                z_interpolated *= w_reciprocal;
                if(insideTriangle(v,x+0.5,y+0.5)&&z_interpolated>=zBuffer[x*height+y])
                {
                    Eigen::Vector3f col=alpha*t.getColor(0)+beta*t.getColor(1)+gamma*t.getColor(2);
                    Eigen::Vector4f normal4=(alpha*n[0]+beta*n[1]+gamma*n[2]).normalized();
                    Eigen::Vector3f shadingCoord=alpha*viewSpace_pos[0]+beta*viewSpace_pos[1]+gamma*viewSpace_pos[2];
                    Eigen::Vector2f texCoord=alpha*t.getTexCoord(0)+beta*t.getTexCoord(1)+gamma*t.getTexCoord(2);
                    Eigen::Vector3f normal3=toVec3(normal4);
                    normal3=normal3.normalized();
                    zBuffer[x*height+y]=z_interpolated;
                    Eigen::Vector2f pos(x,y);
                    payload pl(shadingCoord,normal3,col,pos,texCoord);
                    Eigen::Vector3f color=fragmentShader(pl);
                    QColor renCol=vec2Col(color);
                    drawPoint(pos,renCol);
                }
            }
        }
    }
    return;
}

void DisplayWindow::setEyePosition(Eigen::Vector3f &a)
{
    eyePosition=a;
}

void DisplayWindow::setView(Eigen::Vector3f &a)
{
    view=a.normalized();
}

void DisplayWindow::setViewUp(Eigen::Vector3f &a)
{
    viewUp=a;
}

void DisplayWindow::setMode(DisplayWindow::MODE in_mode)
{
    mode=in_mode;
}

void DisplayWindow::setFragmentShader(std::function<Eigen::Vector3f (payload&)> _fragmentShader)
{
    fragmentShader=_fragmentShader;
}

void DisplayWindow::setSize(float i)
{
    size=i;
}

void DisplayWindow::loadPosition(const std::vector<Eigen::Vector3f> &positions)
{
    for(int i=0;i<positions.size();i++)
    {
        position.push_back(positions.at(i));
    }
}

void DisplayWindow::loadIndices(const std::vector<Eigen::Vector3f> &indices)
{
    for(int i=0;i<indices.size();i++)
    {
        this->indices.push_back(indices.at(i));
    }
}

void DisplayWindow::loadColors(const std::vector<Eigen::Vector3f> &col)
{
    for(int i=0;i<col.size();i++)
    {
        colors.push_back(col.at(i));
    }
}

void DisplayWindow::updateViewMatrix()
{

    viewMatrix << 1,0,0,-eyePosition[0],
                 0,1,0,-eyePosition[1],
                 0,0,1,-eyePosition[2],
                 0,0,0,1;

}

void DisplayWindow::updateProjectionMatrix()
{

    float f, n, l, r, b, t, fov;
    fov = eye_fov / 180 * PI;
    n = -zNear; //znear是正值
    f = -zFar;
    t = tan(fov / 2) * zNear;
    b = -t;
    r = t * aspect_ratio;
    l = -r;
    //透视->正交 perspective->orthographic
    Eigen::Matrix4f pertoorth;
    pertoorth <<
        n, 0, 0, 0,
        0, n, 0, 0,
        0, 0, n + f, -n * f,
        0, 0, 1, 0;
    //正交——移动
    Eigen::Matrix4f orth1;
    orth1 <<
        1, 0, 0, -(r + l) / 2,
        0, 1, 0, -(t + b) / 2,
        0, 0, 1, -(n + f) / 2,
        0, 0, 0, 1;
    //正交——缩放
    Eigen::Matrix4f orth2;
    orth2 <<
        2 / (r - l), 0, 0, 0,
        0, 2 / (t - b), 0, 0,
        0, 0, 2 / (n - f), 0,
        0, 0, 0, 1;
    //确保图象是正的，要对z轴进行反转

    projectionMatrix = orth2 * orth1 * pertoorth;//注意矩阵顺序，变换从右往左依次进行
}

void DisplayWindow::updateModelMatrix()
{
    Eigen::Matrix4f rotationY,rotationX;
    float y = yAngle * PI / 180.f;
    float x = xAngle * PI /180.f;
    rotationY << cos(y), 0, sin(y), 0,
                0, 1, 0, 0,
                -sin(y), 0, cos(y), 0,
                0, 0, 0, 1;
    rotationX << 1,0,0,0,
                 0,cos(x),-sin(x),0,
                 0,sin(x),cos(x),0,
                 0,0,0,1;

    Eigen::Matrix4f scale;
    scale << size, 0, 0, 0,
              0, size, 0, 0,
              0, 0, size, 0,
              0, 0, 0, 1;

    Eigen::Matrix4f translate;
    translate << 1, 0, 0, xOffset,
            0, 1, 0, yOffset,
            0, 0, 1, 0,
            0, 0, 0, 1;

    modelMatrix= translate * rotationX* rotationY * scale;
}

void DisplayWindow::render()
{
    clear();
    for(auto t:triangleList)
    {
        drawTriangle(*t);
    }
    update();
}

void DisplayWindow::setRenderMode(DisplayWindow::MODE in_mode)
{
    mode=in_mode;
}


void DisplayWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);// set the background to white.
    QPen pen;
    pen.setStyle(Qt::NoPen);
    pen.setColor(Qt::white);
    painter.setPen(pen);
    for(int x=0;x<width;x++)
    {
        for(int y=0;y<height;y++)
        {
            painter.setBrush(pixelBuffer[x*width+y]);
            painter.drawRect(x,height-y-1,1,1);
        }
    }
}

QColor vec2Col(Eigen::Vector3f &col)
{
    QColor color;
    for(int i=0;i<3;i++)
    {
        if(col(i)<0) col(i)=0;
        else if(col[i]>1) col(i)=1;
    }
    color.setRgb(int(col.x()*255),int(col.y()*255),int(col.z()*255));
    return color;
}
