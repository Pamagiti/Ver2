#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "viewport.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    viewport3D = new viewport();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete viewport3D;
}


void MainWindow::on_actionLoad_triggered()
{
//    QStringList pathToFiles = QFileDialog::getOpenFileNames(nullptr, "Open Dialog", "", "*.obj");
//    QString paths;
//    foreach(QString str, pathToFiles ) paths.append(str);
//    viewport3D = new viewport(paths);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setMargin(0);
    layout->addWidget(viewport3D);
    ui->openGLWidget->setLayout(layout);
}
