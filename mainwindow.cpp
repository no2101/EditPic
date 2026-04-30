#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_nCrtIndex(0),
    m_nCrtAlgoID(1)
{
    ui->setupUi(this);
    QObject::connect(&m_processThread,&ProcessorThread::ShowPicInDebugDlg,this,&MainWindow::recvShowPicSignal);
    m_processThread.start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::recvShowPicSignal(QImage image)
{
    QPixmap ConvertPixmap=QPixmap::fromImage(image);//The QPixmap class is an off-screen image representation that can be used as a paint device
    QGraphicsScene  *qgraphicsScene = new QGraphicsScene;//要用QGraphicsView就必须要有QGraphicsScene搭配着用
    m_Image = new ImageWidget(&ConvertPixmap);//实例化类ImageWidget的对象m_Image，该类继承自QGraphicsItem，是自己写的类
    int nwith = ui->ImageGraphic->width();//获取界面控件Graphics View的宽度
    int nheight = ui->ImageGraphic->height();//获取界面控件Graphics View的高度
    m_Image->setQGraphicsViewWH(nwith,nheight);//将界面控件Graphics View的width和height传进类m_Image中
    qgraphicsScene->addItem(m_Image);//将QGraphicsItem类对象放进QGraphicsScene中
    ui->ImageGraphic->setSceneRect(QRectF(-(nwith/2),-(nheight/2),nwith,nheight));//使视窗的大小固定在原始大小，不会随图片的放大而放大（默认状态下图片放大的时候视窗两边会自动出现滚动条，并且视窗内的视野会变大），防止图片放大后重新缩小的时候视窗太大而不方便观察图片
    ui->ImageGraphic->setScene(qgraphicsScene);//Sets the current scene to scene. If scene is already being viewed, this function does nothing.
    ui->ImageGraphic->setFocus();//将界面的焦点设置到当前Graphics View控件
}
void MainWindow::on_pushButton_open_clicked()
{
    QFileDialog *fileDlg = new QFileDialog(this);
    fileDlg->setWindowTitle("Choose Pictures");
    QStringList qstrFilters;
    qstrFilters<<"Image files(*.bmp *.jpg *.pbm *.pgm *.png *.ppm *.xbm *.xpm)";
    qstrFilters<<"Any files (*)";
    fileDlg->setNameFilters(qstrFilters);//设置文件过滤器
    fileDlg->setFileMode(QFileDialog::ExistingFiles);//设置能选择多个文件，如果是单个文件就写成QFileDialog::ExistingFile
    if(fileDlg->exec() == QDialog::Accepted)
    {
        QStringList strPathList = fileDlg->selectedFiles();
        if(!m_qlistLoadImgs.empty())
        {
            m_qlistLoadImgs.clear();
        }
        QPixmap pixmapTemp;
        for(int i = 0;i<strPathList.count();i++)//导入一张461kb图片后，该循环增加内存占用8972kb
        {
            pixmapTemp.load(strPathList[i]);
            m_qlistLoadImgs.push_back(pixmapTemp.toImage());
        }
        if(!m_qlistLoadImgs.isEmpty())
        {
            m_nCrtIndex = 0;
            recvShowPicSignal(m_qlistLoadImgs.at(m_nCrtIndex));
        }
    }
    else
    {
        QMessageBox mesg;
        mesg.warning(this,"警告","打开图片失败!");
    }
    fileDlg->close();
    delete fileDlg;
    fileDlg = nullptr;
}

void MainWindow::on_pushButton_1cold_clicked()
{
    m_nCrtAlgoID = 1;
    if(!m_qlistLoadImgs.empty())
    {
        IMAGE_INFO imageInfo;
        imageInfo.qImage = m_qlistLoadImgs.at(m_nCrtIndex);
        imageInfo.nDelta = ui->horizontalSlider->value();
        imageInfo.nAlgoID = m_nCrtAlgoID;
        m_processThread.InsertImg(imageInfo);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    m_processThread.m_bStarted = false;
    m_processThread.wait();
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)//每次slider滑动一格都会触发该函数
{
//    if(!m_qlistLoadImgs.empty())
//    {
//        IMAGE_INFO imageInfo;
//        imageInfo.qImage = m_qlistLoadImgs.at(m_nCrtIndex);
//        imageInfo.nDelta = position;//ui->horizontalSlider->value();
//        imageInfo.nAlgoID = m_nCrtAlgoID;
//        m_processThread.InsertImg(imageInfo);
//    }
}

void MainWindow::on_horizontalSlider_sliderReleased()
{
    if(!m_qlistLoadImgs.empty())
    {
        IMAGE_INFO imageInfo;
        imageInfo.qImage = m_qlistLoadImgs.at(m_nCrtIndex);
        imageInfo.nDelta = ui->horizontalSlider->value();
        imageInfo.nAlgoID = m_nCrtAlgoID;
        m_processThread.InsertImg(imageInfo);
    }
}

void MainWindow::on_pushButton_2worm_clicked()//暖色调调节
{
    m_nCrtAlgoID = 2;
    if(!m_qlistLoadImgs.empty())
    {
        IMAGE_INFO imageInfo;
        imageInfo.qImage = m_qlistLoadImgs.at(m_nCrtIndex);
        imageInfo.nDelta = ui->horizontalSlider->value();
        imageInfo.nAlgoID = m_nCrtAlgoID;
        m_processThread.InsertImg(imageInfo);
    }
}

void MainWindow::on_pushButton_3light_clicked()//亮度调节
{
    m_nCrtAlgoID = 3;
    if(!m_qlistLoadImgs.empty())
    {
        IMAGE_INFO imageInfo;
        imageInfo.qImage = m_qlistLoadImgs.at(m_nCrtIndex);
        imageInfo.nDelta = ui->horizontalSlider->value();
        imageInfo.nAlgoID = m_nCrtAlgoID;
        m_processThread.InsertImg(imageInfo);
    }
}

void MainWindow::on_pushButton_4grey_clicked()
{
    m_nCrtAlgoID = 4;
    if(!m_qlistLoadImgs.empty())
    {
        IMAGE_INFO imageInfo;
        imageInfo.qImage = m_qlistLoadImgs.at(m_nCrtIndex);
        imageInfo.nDelta = ui->horizontalSlider->value();
        imageInfo.nAlgoID = m_nCrtAlgoID;
        m_processThread.InsertImg(imageInfo);
    }
}

void MainWindow::on_pushButton_5saturation_clicked()
{
    m_nCrtAlgoID = 5;
    if(!m_qlistLoadImgs.empty())
    {
        IMAGE_INFO imageInfo;
        imageInfo.qImage = m_qlistLoadImgs.at(m_nCrtIndex);
        imageInfo.nDelta = ui->horizontalSlider->value();
        imageInfo.nAlgoID = m_nCrtAlgoID;
        m_processThread.InsertImg(imageInfo);
    }
}

void MainWindow::on_pushButton_6blur_clicked()
{
    m_nCrtAlgoID = 6;
    if(!m_qlistLoadImgs.empty())
    {
        IMAGE_INFO imageInfo;
        imageInfo.qImage = m_qlistLoadImgs.at(m_nCrtIndex);
        imageInfo.nDelta = ui->horizontalSlider->value();
        imageInfo.nAlgoID = m_nCrtAlgoID;
        m_processThread.InsertImg(imageInfo);
    }
}

void MainWindow::on_pushButton_7sharpen_clicked()
{
    m_nCrtAlgoID = 7;
    if(!m_qlistLoadImgs.empty())
    {
        IMAGE_INFO imageInfo;
        imageInfo.qImage = m_qlistLoadImgs.at(m_nCrtIndex);
        imageInfo.nDelta = ui->horizontalSlider->value();
        imageInfo.nAlgoID = m_nCrtAlgoID;
        m_processThread.InsertImg(imageInfo);
    }
}
