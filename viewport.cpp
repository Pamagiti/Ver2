#include "viewport.h"
#include <QOpenGLContext>
#include <QDir>
#include "model.h"
#include <GL/internal/glcore.h>
#include <QtGui>
#include <QtOpenGL>
#include <GL/glu.h>


viewport::viewport(QWidget *parent)
    : QOpenGLWidget(parent)
{
    nSca = 1;
    xT = 0;
    yT = 0;
}

//viewport::viewport(QString p)
//{
//    this->path = p;
//}

viewport::~viewport()
{

}

void viewport::initializeGL()
{
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    QStringList pathToFiles = QFileDialog::getOpenFileNames(nullptr, "Open Dialog", "", "*.obj");
    QString path;
    foreach(QString str, pathToFiles) path.append(str);
    QDir(path).absolutePath();

    obj.loadObj(path);
    initGeom();
}

void viewport::resizeGL(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w*500, h*500);
    glOrtho(-50, 50, -50, 50, -50, 50);
}

void viewport::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

//    glBegin(GL_POINTS);
//        glPointSize(5);
//        glColor3f(0.0f, 0.0f, 0.0f);
//        for (int i = 0; i < list_vertices.size(); i++){
//            GLfloat x = list_vertices.at(i).x;
//            GLfloat y = list_vertices.at(i).y;
//            GLfloat z = list_vertices.at(i).z;
//            glVertex3f(x, y, z);
//        }
//    glEnd();
    glRotatef(angle_x, 1,0,0);
    glRotatef(angle_y, 0,1,0);

    glScaled(nSca, nSca, nSca);

    glTranslatef(xT, 0, 0);
    glTranslatef(yT, 0, 0);

    glPointSize(5);
    glBegin(GL_POINTS);
        glColor3f(0.0f, 0.0f, 0.0f);
        for (int i = 0; i < vertexes.size(); i ++)
        {
            uint tmp = indexes.at(i);
            float x = vertexes.at(tmp).position.x();
            float y = vertexes.at(tmp).position.y();
            float z = vertexes.at(tmp).position.z();
            glVertex3f(x, y, z);
        }
    glEnd();

    if (obj.countFace == 3){
        glBegin(GL_TRIANGLES);
            glColor3f(0.0f, 0.0f, 1.0f);
            for (int i = 0; i < vertexes.size(); i ++)
            {
                uint tmp = indexes.at(i);
                float x = vertexes.at(tmp).position.x();
                float y = vertexes.at(tmp).position.y();
                float z = vertexes.at(tmp).position.z();
                glVertex3f(x, y, z);
            }
        glEnd();
    } else if (obj.countFace == 4){
        glBegin(GL_QUADS);
            glColor3f(0.0f, 0.0f, 1.0f);
            for (int i = 0; i < vertexes.size(); i ++)
            {
                uint tmp = indexes.at(i);
                float x = vertexes.at(tmp).position.x();
                float y = vertexes.at(tmp).position.y();
                float z = vertexes.at(tmp).position.z();
                glVertex3f(x, y, z);
            }
        glEnd();
    }
    if (flag){
    glPointSize(5);
    glBegin(GL_POINTS);
        glColor3f(1.0f, 0.0f, 0.0f);
        for (int i = 0; i < matan.newPoint.size(); i ++)
        {
            float x =  matan.newPoint.at(i).x;
            float y =  matan.newPoint.at(i).y;
            float z =  matan.newPoint.at(i).z;
            glVertex3f(x, y, z);
        }
    glEnd();
    }
}

void viewport::mouseMoveEvent(QMouseEvent *event)
{
    QVector2D diff = QVector2D(event->localPos()) - m_mousePosition;
    m_mousePosition = QVector2D(event->localPos());

    float angleX = diff.x() / 2.0f;
    float angleY = diff.y() / 2.0f;

    angle_x += angleY;// * angle_x;
    angle_y += angleX;// * angle_y;
    update();


//    m_rotationX = QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, angleX) * m_rotationX;
//    m_rotationY = QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, angleY) * m_rotationY;

//    float X = 180 * (GLfloat)(event->y() - m_mousePosition.y()) / (2 * width());
//    float Y = 180 * (GLfloat)(event->x() - m_mousePosition.x()) / (2 * height());
//    angle_x -= X / 20;
//    angle_y -= Y / 20;

}

void viewport::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
        m_mousePosition = QVector2D(event->localPos());
        event->accept();
    }
    if (event->button() == Qt::RightButton){
        m_mouseConvert = QVector2D(event->localPos());
        track();
        flag = true;
        event->accept();
        update();
}
}

void viewport::wheelEvent(QWheelEvent *event)
{
    if ((event->delta()) > 0){
        nSca = nSca * 1.1f;
    } else if ((event->delta()) < 0){
        nSca = nSca / 1.1f;
    }
    update();
}

void viewport::initGeom()
{
    QVector<QVector3D> coords;
    QVector<QVector2D> texCoords;
    QVector<QVector3D> normal;

    for (int i = 0; i < obj.list_vertices.size(); i++)
    {
        GLfloat xV = obj.list_vertices.at(i).x;
        GLfloat yV = obj.list_vertices.at(i).y;
        GLfloat zV = obj.list_vertices.at(i).z;
//        qDebug() << "вершины" << xV << yV << zV;
        coords.append(QVector3D(xV, yV, zV));
       // qDebug() << "Успех вершин";
    }
    if (!obj.m_texture_coord)
    {
        for (int i = 0; i < obj.count; i++)
        {
            GLfloat uT = 0;
            GLfloat vT = 0;
//            qDebug() << "текстуры" << uT << vT;
            texCoords.append(QVector2D(0, 1));
        }
    } else if (obj.m_texture_coord){
        for (int i = 0; i < obj.list_texture.size(); i++)
        {
            GLfloat uT = obj.list_texture.at(i).u;
            GLfloat vT = obj.list_texture.at(i).v;
//            qDebug() << "текстуры" << uT << vT;
            texCoords.append(QVector2D(0, 1));
        }
    }
    //qDebug() << "старт заполнения нормалей";
    if (!obj.m_normal)
    {
        for (int i = 0; i < obj.count; i++)
        {
            GLfloat xN = 0;
            GLfloat yN = 0;
            GLfloat zN = 0;
//            qDebug() << "нормали" << xN << yN << zN;
            normal.append(QVector3D(xN, yN, zN));
        }
    } else if (obj.m_normal)
    {
        for (int i = 0; i < obj.list_normal.size(); i++)
        {
            GLfloat xN = obj.list_normal.at(i).x;
            GLfloat yN = obj.list_normal.at(i).y;
            GLfloat zN = obj.list_normal.at(i).z;
            //qDebug() << "нормали" << xN << yN << zN;
            normal.append(QVector3D(xN, yN, zN));
        }
    }
    qDebug() << "Заполнение нормалей   не пошло по пизде";
    for (int i = 0; i < obj.face.size(); i++)
    {

        GLint cord = obj.face.at(i).id_vertices;
        GLint tcord = obj.face.at(i).id_textur_coordinat;
        GLint norma = obj.face.at(i).id_normal;
        //qDebug() << cord << tcord << norma;
        vertexes.append(VertexData(coords[cord], texCoords[tcord], normal[norma]));
        indexes.append(indexes.size());
    }

//    m_arrayBuffer.create();
//    m_arrayBuffer.bind();
//    m_arrayBuffer.allocate(vertexes.constData(), vertexes.size() * sizeof (VertexData));
//    m_arrayBuffer.release();
//    //qDebug() << "бинд буферов - в";
//    m_indexBuffer.create();
//    m_indexBuffer.bind();
//    m_indexBuffer.allocate(indexes.constData(), indexes.size() * sizeof (GLint));
    //    m_indexBuffer.release();
}

void viewport::track()
{
    matan.ConvertCoords(m_mouseConvert, height(), width());

    float k1 = m_mouseConvert.x();
    float k2 = m_mouseConvert.y();
    //qDebug() << k1 << k2;

    for (int i = 0; i < obj.list_vertices.size(); i++)
    {
        float x = obj.list_vertices.at(i).x;
        float y = obj.list_vertices.at(i).y;
        float z = obj.list_vertices.at(i).z;
        qDebug() << "CORDINAT";
        QVector3D keks = QVector3D(x, y, z);
        matan.Testpreobr(nSca, keks, height(), width());
        matan.TrackIntersect(x, y, z, width());
    }
    float x = matan.newPoint.at(0).x;
    float y = matan.newPoint.at(0).y;
    float z = matan.newPoint.at(0).z;
    //qDebug() << x << y << z;
//    float x1 = matan.detectUp.at(0).x;
//    float y1 = matan.detectUp.at(0).y;
//    float z1 = matan.detectUp.at(0).z;
    //qDebug() << x1 << y1 << z1;

}


