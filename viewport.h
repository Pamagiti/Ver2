#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QQuaternion>
#include <QDir>
#include <QOpenGLBuffer>
#include "model.h"
#include "mathm.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <QtGui>
#include <QtOpenGL>

struct VertexData
{
    VertexData() //Конструктор по умолчанию не принимающий аргументы
    {
    }
    VertexData(QVector3D p, QVector2D t, QVector3D n):
        position(p), textCoord(t), normal(n)
    {//Конструктор с списком инициализации
    }
    QVector3D position; //позиция
    QVector2D textCoord; //текстурные координаты
    QVector3D normal; //нормали
};

class viewport : public QOpenGLWidget
{
    Q_OBJECT

public:
    viewport(QWidget *parent = nullptr);
    viewport(QString p);
    ~viewport();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    void initGeom();
    void track();
    void ConvertCoords(float mouse_x, float mouse_y, float height);

    QString path;
    QVector<VertexData> vertexes;
    QVector<GLuint> indexes;

    float m_mouse_x;
    float m_mouse_y;

protected:
    void paintGL();
    void initializeGL();
    void resizeGL(int w, int h);

private:
    GLfloat angle_x;
    GLfloat angle_y;
    GLfloat xT;
    GLfloat yT;
    QVector2D m_mousePosition;
    QVector2D m_mouseConvert;
    QQuaternion m_rotationX;
    QQuaternion m_rotationY;

    float nSca;
    int k;
    bool flag = false;

    model obj;
    mathM matan;
};

#endif // VIEWPORT_H
