#ifndef MATHM_H
#define MATHM_H
#include <QOpenGLContext>
#include <GL/glu.h>
#include "GL/glu.h"


struct Vert{
    float x,
          y,
          z;
};

class mathM
{
public:
    mathM();
    mathM(float x, float y, float z);

    void ConvertCoords(QVector2D mouse_pos, float height, float width);
    void TrackIntersect(float v1, float v2, float v3, float width);
    void Testpreobr(float nSca, QVector3D cordes, float height, float width);

    QVector<Vert> detectUp;
    QVector<Vert> detectDown;
    QVector<Vert> VectConv;

    QVector<Vert> track;
    QVector<Vert> newPoint;

private:
    float convx(float x, float width);
    float convy(float y, float height);

    const double maxAcceptableDistanceSqr = 0.1f;
};
#endif // MATHM_H
