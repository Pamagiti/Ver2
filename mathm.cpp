#include "mathm.h"
#include <QOpenGLContext>
#include <GL/glu.h>
#include "GL/glu.h"
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <math.h>
#include <QMatrix4x4>


mathM::mathM()
{

}

void mathM::TrackIntersect(float v1, float v2, float v3, float width)
{
    float xu = detectUp.at(0).x;
    float yu = detectUp.at(0).y;
    float zu = detectUp.at(0).z;
    QVector3D P1 = QVector3D(xu, yu, zu);
    QVector3D V = QVector3D(v1, v2, v3);
    float x = v1;
//    qDebug() << test << x;
    float xd = detectDown.at(0).x;
    float yd = detectDown.at(0).y;
    float zd = detectDown.at(0).z;
    QVector3D P2 = QVector3D(xd, yd, zd);

    float r1 = sqrt(pow((P1.x() - V.x()), 2) + pow((P1.y() - V.y()), 2) + pow((P1.z() - V.z()), 2));
    float r2 = sqrt(pow((P2.x() - V.x()), 2) + pow((P2.y() - V.y()), 2));// + pow((P2.z() - V.z()), 2)

    Vert newp;
    if (r2 < maxAcceptableDistanceSqr){
        newp.x = xd;
        newp.y = yd;
        newp.z = zd;
    }
    newPoint.append(newp);
}

void mathM::Testpreobr(float nSca, QVector3D cordes, float height, float width)
{
    float mouse_x = cordes.x();
    float mouse_y = cordes.y();

    GLint viewport[4];
    GLdouble projection[16];

    GLdouble modelview[16];
    GLdouble vx, vy, vz;

    Vert det;

    GLdouble wx, wy, wz;

    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);

    vx = (2.0f * 50*nSca* mouse_x)/(width) - 1.0f;
    vy = (-2.0f * 50*nSca*  mouse_y)/(height)+ 1.0f;
    qDebug() << vx << vy << "NewConvertSYKA";
    //дальний конец
    vz = 1;
    gluUnProject(vx, vy, vz, modelview, projection, viewport, &wx, &wy, &wz);
    det.x = wx;
    det.y = wy;
    det.z = wz;
    VectConv.append(det);
    //qDebug() << "X" << wx << " Y" << wy << " Z" << wz;
}


void mathM::ConvertCoords(QVector2D mouse_pos, float height, float width)
{
    float mouse_x = mouse_pos.x();
    float mouse_y = mouse_pos.y();

    GLint viewport[4];
    GLdouble projection[16];

    GLdouble modelview[16];
    GLdouble vx, vy, vz;
    Vert detUp;
    Vert detDown;

    GLdouble wx, wy, wz;

    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);

//    QMatrix4x4 proj((float)projection[0], (float)projection[1], (float)projection[2], (float)projection[3], (float)projection[4], (float)projection[5],
//            (float)projection[6], (float)projection[7], (float)projection[8], (float)projection[9], (float)projection[10], (float)projection[11],
//            (float)projection[12], (float)projection[13], (float)projection[14], (float)projection[15]);

//    QMatrix4x4 model((float)modelview[0], (float)modelview[1], (float)modelview[2], (float)modelview[3], (float)modelview[4], (float)modelview[5],
//            (float)modelview[6], (float)modelview[7], (float)modelview[8], (float)modelview[9], (float)modelview[10], (float)modelview[11],
//            (float)modelview[12], (float)modelview[13], (float)modelview[14], (float)modelview[15]);

//    QVector4D tmp(2.0f * mouse_pos.x() / width - 1.0f, -2.0f * mouse_pos.y() / height +1, -1.0f, 1.0f);
//    QVector4D iTmp((proj.inverted() * tmp).toVector2D(), -1.0f, 0.0f);
//    QVector3D Direction((model.inverted() * iTmp).toVector3D());

//    QVector3D camPos(model.inverted() * QVector4D(0.0f, 0.0f, 0.0f, 1.0f).toVector3D());

//    QVector3D N(0.0f, 0.0f, 1.0f);

//    float t = -QVector3D::dotProduct(mouse_pos, N) / QVector3D::dotProduct(Direction, N);
//    QVector3D result = camPos + Direction * t;
//    float x = result.x();
//    float y = result.y();
//    float z = result.z();
//    qDebug() << "X" << x << " Y" << y << " Z" << z;

    vx = 2.0f * mouse_x / width - 1.0f;
    vy = -2.0f * mouse_y / height + 1.0f;
    qDebug() << vx << vy << "NewConvertBLYAT";
    //ближний конец
    vz = -1;
    gluUnProject(vx, vy, vz, modelview, projection, viewport, &wx, &wy, &wz);
    detUp.x = wx;
    detUp.y = wy;
    detUp.z = wz;
    //qDebug() << "X" << wx << " Y" << wy << " Z" << wz;
    detectUp.append(detUp);
    //дальний конец
    vz = 1;
    gluUnProject(vx, vy, vz, modelview, projection, viewport, &wx, &wy, &wz);
    detDown.x = wx;
    detDown.y = wy;
    detDown.z = wz;
    detectDown.append(detDown);
    //qDebug() << "X" << wx << " Y" << wy << " Z" << wz;
}

