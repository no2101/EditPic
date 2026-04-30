#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QFileDialog>
#include <QList>
#include <QMessageBox>
#include <QImage>
#include <QGraphicsScene>
#include "imagewidget.h"
#include "typedef.h"
#include "processorthread.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void            recvShowPicSignal(QImage image);
private slots:
    void on_pushButton_open_clicked();

    void on_pushButton_1cold_clicked();

    void on_horizontalSlider_sliderMoved(int position);

    void on_horizontalSlider_sliderReleased();

    void on_pushButton_2worm_clicked();

    void on_pushButton_3light_clicked();

    void on_pushButton_4grey_clicked();

    void on_pushButton_5saturation_clicked();

    void on_pushButton_6blur_clicked();

    void on_pushButton_7sharpen_clicked();

private:
    ImageWidget     *m_Image;
    Ui::MainWindow  *ui;
    QList<QImage>   m_qlistLoadImgs;
    qint32          m_nCrtIndex;
    qint32          m_nCrtAlgoID;
    ProcessorThread m_processThread;
protected:
    void            closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
