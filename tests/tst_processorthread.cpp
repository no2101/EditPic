#include <QtTest>
#include "processorthread.h"

class TestProcessorThread : public QObject
{
    Q_OBJECT

private:
    // 生成指定大小的纯色测试图片
    QImage createSolidImage(int width, int height, QColor color)
    {
        QImage img(width, height, QImage::Format_ARGB32);
        img.fill(color);
        return img;
    }

private slots:
    void testProcessImgCool()
    {
        ProcessorThread processor;
        QImage original = createSolidImage(10, 10, QColor(100, 100, 100));
        IMAGE_INFO info;
        info.qImage = original;
        info.nAlgoID = 1; // 冷色调
        info.nDelta = 30;

        processor.ProcessImg(&info);

        // 冷色调只增加蓝色通道，红绿不变
        QColor result = info.qImage.pixelColor(5, 5);
        QCOMPARE(result.red(), 100);
        QCOMPARE(result.green(), 100);
        QCOMPARE(result.blue(), 130); // 100 + 30
    }

    void testProcessImgWarm()
    {
        ProcessorThread processor;
        QImage original = createSolidImage(10, 10, QColor(100, 100, 100));
        IMAGE_INFO info;
        info.qImage = original;
        info.nAlgoID = 2; // 暖色调
        info.nDelta = 30;

        processor.ProcessImg(&info);

        // 暖色调增加红绿通道，蓝色不变
        QColor result = info.qImage.pixelColor(5, 5);
        QCOMPARE(result.red(), 130);
        QCOMPARE(result.green(), 130);
        QCOMPARE(result.blue(), 100);
    }

    void testProcessImgLight()
    {
        ProcessorThread processor;
        QImage original = createSolidImage(10, 10, QColor(100, 100, 100));
        IMAGE_INFO info;
        info.qImage = original;
        info.nAlgoID = 3; // 亮度
        info.nDelta = 50;

        processor.ProcessImg(&info);

        // 亮度同时增加三个通道
        QColor result = info.qImage.pixelColor(5, 5);
        QCOMPARE(result.red(), 150);
        QCOMPARE(result.green(), 150);
        QCOMPARE(result.blue(), 150);
    }

    void testProcessImgGreyScale()
    {
        ProcessorThread processor;
        QImage original = createSolidImage(10, 10, QColor(120, 180, 60));
        IMAGE_INFO info;
        info.qImage = original;
        info.nAlgoID = 4; // 灰度
        info.nDelta = 0;

        processor.ProcessImg(&info);

        // 灰度图每个通道值应相同
        QColor result = info.qImage.pixelColor(5, 5);
        QCOMPARE(result.red(), result.green());
        QCOMPARE(result.green(), result.blue());
    }

    void testProcessImgAlgo0()
    {
        ProcessorThread processor;
        QImage original = createSolidImage(10, 10, QColor(100, 100, 100));
        IMAGE_INFO info;
        info.qImage = original;
        info.nAlgoID = 0; // 无操作
        info.nDelta = 0;

        processor.ProcessImg(&info);

        // 算法 ID 为 0 时不做任何处理
        QColor result = info.qImage.pixelColor(5, 5);
        QCOMPARE(result.red(), 100);
        QCOMPARE(result.green(), 100);
        QCOMPARE(result.blue(), 100);
    }

    void testProcessImgClamp()
    {
        ProcessorThread processor;
        QImage original = createSolidImage(10, 10, QColor(250, 250, 250));
        IMAGE_INFO info;
        info.qImage = original;
        info.nAlgoID = 3; // 亮度
        info.nDelta = 100;

        processor.ProcessImg(&info);

        // 通道值不应超过 255
        QColor result = info.qImage.pixelColor(5, 5);
        QCOMPARE(result.red(), 255);
        QCOMPARE(result.green(), 255);
        QCOMPARE(result.blue(), 255);
    }

    void testInsertImg()
    {
        ProcessorThread processor;
        QImage img = createSolidImage(5, 5, Qt::red);
        IMAGE_INFO info;
        info.qImage = img;
        info.nAlgoID = 1;
        info.nDelta = 10;

        processor.InsertImg(info);

        QCOMPARE(processor.m_qlistImageInfo.size(), 1);
        QCOMPARE(processor.m_qlistImageInfo.first().nAlgoID, 1);
        QCOMPARE(processor.m_qlistImageInfo.first().nDelta, 10);
    }
};

QTEST_MAIN(TestProcessorThread)
#include "tst_processorthread.moc"
