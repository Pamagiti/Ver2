#include "model.h"

model::model()
{

}

void model::loadObj(QString filename)
{
    QFile object(filename);

    if(!object.open(QIODevice::ReadOnly))
        return;

    while (!object.atEnd())
        record(object.readLine());

    object.close();
}

void model::record(const QString &line)
{
    if (line.at(0) == 'v' && line.at(1) == ' ')
        readVerticex(get_corret_line(line));
    else if (line.at(0) == 'v' && line.at(1) == 't')
        readTextureCord(get_corret_line(line));
    else if (line.at(0) == 'v' && line.at(1) == 'n')
        readNormal(get_corret_line(line));
    else if (line.at(0) == 'v' && line.at(1) == 'p')
        readSpaceVert(get_corret_line(line));
    else if (line.at(0) == 'f')
        readFace(get_corret_line(line));

}

void model::readVerticex(const QString &line)
{
    int p = 0;

    QStringList list;

    list = line.split(QRegExp(" "));

    vert.x = list.at(1).toFloat();
    vert.y = list.at(2).toFloat();
    vert.z = list.at(3).toFloat();

    list_vertices.push_back(vert);
    p++;
}

void model::readTextureCord(const QString &line)
{
    m_texture_coord = true;

    QStringList list;


    list = line.split(QRegExp(" "));

    tc.u = list.at(1).toFloat();
//    qDebug() << tc.u;
    tc.v = list.at(2).toFloat();
//    qDebug() << tc.v;
    list_texture.push_back(tc);
}

void model::readNormal(const QString &line)
{
    m_normal = true;

    QStringList list;

    list = line.split(QRegExp(" "));

    norm.x = list.at(1).toFloat();
    norm.y = list.at(2).toFloat();
    norm.z = list.at(3).toFloat();

//    qDebug() << norm.x;

    list_normal.push_back(norm);
}

void model::readSpaceVert(const QString &line)
{
    QStringList list;
    SpaceV sv;

    list = line.split(QRegExp(" "));

    sv.u = list.at(1).toFloat();

    if (list.size() > 2)
        sv.v = list.at(2).toFloat();

    if (list.size() > 3)
        sv.v = list.at(3).toFloat();

    space_vert.push_back(sv);
}

void model::readFace(const QString &line)
{
    QStringList list;
    Face f;

    // f.id_mtl = usemtl.size();

    list = line.split(QRegExp(" "));
    //qDebug() << list;

    if (list.last() == "\r\n"){
        countFace = list.size() - 2;
        list.removeAt(list.indexOf(list.last()));} else countFace = list.size() - 1;
    //qDebug() << countFace;
    QStringList list_value;
    if (m_texture_coord && !m_normal)
    {

        // f 1/1 2/2 3/3
        for(int i = 1; i < countFace + 1; ++i)
        {
            list_value = list.at(i).split(QRegExp("/"));
            //qDebug() << list_value;
            if (list_value.size() != 0)
            {
                f.id_vertices = list_value.at(0).toInt() - 1;
                f.id_textur_coordinat = list_value.at(1).toInt() - 1;
                f.id_normal = 0;
                count++;
            }
            face.push_back(f);
        }
    } else if (!m_texture_coord && m_normal)
    {
        for(int i = 1; i < countFace + 1; ++i)
        {

            // f 1//1 2//2 3//3
            list_value = list.at(i).split(QRegExp("/"));
            if (list_value.size() != 0)
            {
                f.id_vertices = list_value.at(0).toInt() - 1;
                f.id_textur_coordinat = 0;
                f.id_normal = list_value.at(2).toInt() - 1;
                count++;
            }
            face.push_back(f);
        }
    } else if (m_texture_coord && m_normal)
    {
        for(int i = 1; i < countFace + 1; ++i)
        {
            // f 1/1/1 2/2/2 3/3/3
            //qDebug() << "ну вот все в сборе";
            list_value = list.at(i).split(QRegExp("/"));

            if (list_value.size() != 1)
            {
                f.id_vertices = list_value.at(0).toInt() - 1;
                f.id_textur_coordinat = list_value.at(1).toInt() - 1;
                f.id_normal = list_value.at(2).toInt() - 1;
                count++;
            }
            face.push_back(f);
        }
    }
}


QString model::get_corret_line(const QString &line)
{
    QString new_line;

    for (int i = 0; i < line.size(); ++i)
    {
        if(isblank(line.at(i)))
        {
            new_line.push_back(' ');
            while(isblank(line.at(i)) && isblank(line.at(i + 1)))
                ++i;
        } else
            new_line.push_back(line.at(i));
    }
    return  new_line;

}

bool model::isblank(const QChar &ch)
{
    return (ch == ' ' || ch == '\t');
}

