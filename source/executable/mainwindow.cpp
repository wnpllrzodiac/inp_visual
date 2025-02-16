#include "mainwindow.h"
#include "widgets/configwidget.h"
#include "widgets/vtkwidget.h"
#include <QFileDialog>
#include <QHBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QSplitter>

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
{
    initUI();
    setupConnections();
    retranslateUI();
}

MainWindow::~MainWindow() { }

void MainWindow::initUI()
{
    resize(1200, 800);

    QMenuBar *menu_bar = new QMenuBar(this);
    QMenu *file_menu = menu_bar->addMenu("");

    open_abaqus_action_ = new QAction(this);
    open_gmsh_action_ = new QAction(this);
    open_vtu_action_ = new QAction(this);
    open_vtk_action_ = new QAction(this);
    save_action_ = new QAction(this);

    file_menu->addAction(open_abaqus_action_);
    file_menu->addAction(open_gmsh_action_);
    // file_menu->addAction(open_vtu_action_);
    // file_menu->addAction(open_vtk_action_);
    // file_menu->addAction(save_action_);

    setMenuBar(menu_bar);

    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal, this);

    mainSplitter->setChildrenCollapsible(false);
    mainSplitter->setHandleWidth(1);

    configWidget_ = new ConfigWidget(this);
    vtkWidget_ = new VtkWidget(this);

    mainSplitter->addWidget(configWidget_);
    mainSplitter->addWidget(vtkWidget_);

    QList<int> sizes;
    sizes << width() * 0.25 << width() * 0.75;
    mainSplitter->setSizes(sizes);

    mainSplitter->setStretchFactor(0, 1);
    mainSplitter->setStretchFactor(1, 3);

    setCentralWidget(mainSplitter);
}

void MainWindow::setupConnections()
{
    connect(open_abaqus_action_, &QAction::triggered, this, &MainWindow::openAbaqusFile);
    connect(open_gmsh_action_, &QAction::triggered, this, &MainWindow::openGmshFile);
}

void MainWindow::retranslateUI()
{
    setWindowTitle(tr("INP文件可视化分析器"));
    menuBar()->findChild<QMenu *>("")->setTitle(tr("文件(&F)"));
    open_abaqus_action_->setText(tr("打开ABAQUS文件(&O)"));
    open_gmsh_action_->setText(tr("打开Gmsh文件(&O)"));
    open_vtu_action_->setText(tr("打开VTU文件(&O)"));
    open_vtk_action_->setText(tr("打开VTK文件(&O)"));
    save_action_->setText(tr("另存为(&S)"));
}

void MainWindow::openAbaqusFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开ABAQUS文件"), "", tr("ABAQUS文件 (*.inp)"));

    if (!fileName.isEmpty())
    {
        vtkWidget_->openAbaqusFile(fileName);
    }
}

void MainWindow::openGmshFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开Gmsh文件"), "", tr("Gmsh文件 (*.inp)"));

    if (!fileName.isEmpty())
    {
        vtkWidget_->openGmshFile(fileName);
    }
}