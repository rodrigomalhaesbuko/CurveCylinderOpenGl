#include "mainwindow.h"
#include "renderwidget.h"
#include <QSurfaceFormat>
#include <QApplication>
#include "polygonalmoulder.h"
#include <QStyle>
#include "glm/glm.hpp"
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main(int argc, char *argv[])
{

    //Necessario em MacOS
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setVersion( 3, 3 ); //versao OpenGL
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);
    QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

    QApplication a(argc, argv);

    RenderWidget renderWidget;

    renderWidget.show();

    return a.exec();
}





























