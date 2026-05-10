# 算法测试体系建设 — 实施计划

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** 将 7 个图像处理算法从 ProcessorThread 中提取为独立函数，用 QtTest 建立单元测试。

**Architecture:** 新增 `algorithms.h/cpp` 作为纯函数库（命名空间 `Algorithms`），ProcessorThread 改为调用该库。测试子项目 `tests/` 直接链接算法源码，用 QtTest 框架验证每个算法。

**Tech Stack:** C++11, Qt5, QtTest, qmake, MSVC x64

---

### Task 1: 创建 algorithms.h

**Files:**
- Create: `algorithms.h`

- [ ] **Step 1: Write algorithms.h**

```cpp
#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <QImage>

namespace Algorithms {

QImage algoCool(const QImage &origin, int delta);
QImage algoWarm(const QImage &origin, int delta);
QImage algoLight(const QImage &origin, int delta);
QImage algoGreyScale(const QImage &origin, int delta);
QImage algoSaturation(const QImage &origin, int delta);
QImage algoBlur(const QImage &origin, int delta);
QImage algoSharpen(const QImage &origin, int delta);

} // namespace Algorithms

#endif // ALGORITHMS_H
```

### Task 2: 创建 algorithms.cpp

**Files:**
- Create: `algorithms.cpp`

- [ ] **Step 1: Write algorithms.cpp — 从 ProcessorThread 提取全部 7 个算法实现，修复 bug，改为值返回**

```cpp
#include "algorithms.h"

#include <QColor>

namespace Algorithms {

QImage algoCool(const QImage &origin, int delta)
{
    QImage newImage(origin.width(), origin.height(), QImage::Format_ARGB32);
    for (int x = 0; x < newImage.width(); x++) {
        for (int y = 0; y < newImage.height(); y++) {
            QColor oldColor(origin.pixel(x, y));
            int r = oldColor.red();
            int g = oldColor.green();
            int b = qBound(0, oldColor.blue() + delta, 255);
            newImage.setPixel(x, y, qRgb(r, g, b));
        }
    }
    return newImage;
}

QImage algoWarm(const QImage &origin, int delta)
{
    QImage newImage(origin.width(), origin.height(), QImage::Format_ARGB32);
    for (int x = 0; x < newImage.width(); x++) {
        for (int y = 0; y < newImage.height(); y++) {
            QColor oldColor(origin.pixel(x, y));
            int r = qBound(0, oldColor.red() + delta, 255);
            int g = qBound(0, oldColor.green() + delta, 255);
            int b = oldColor.blue();
            newImage.setPixel(x, y, qRgb(r, g, b));
        }
    }
    return newImage;
}

QImage algoLight(const QImage &origin, int delta)
{
    QImage newImage(origin.width(), origin.height(), QImage::Format_ARGB32);
    for (int x = 0; x < newImage.width(); x++) {
        for (int y = 0; y < newImage.height(); y++) {
            QColor oldColor(origin.pixel(x, y));
            int r = qBound(0, oldColor.red() + delta, 255);
            int g = qBound(0, oldColor.green() + delta, 255);
            int b = qBound(0, oldColor.blue() + delta, 255);
            newImage.setPixel(x, y, qRgb(r, g, b));
        }
    }
    return newImage;
}

QImage algoGreyScale(const QImage &origin, int delta)
{
    QImage newImage(origin.width(), origin.height(), QImage::Format_ARGB32);
    for (int y = 0; y < newImage.height(); y++) {
        const QRgb *line = reinterpret_cast<const QRgb *>(origin.constScanLine(y));
        for (int x = 0; x < newImage.width(); x++) {
            int average = (qRed(line[x]) + qGreen(line[x]) + qBlue(line[x])) / 3;
            average = qBound(0, average + delta, 255);
            newImage.setPixel(x, y, qRgb(average, average, average));
        }
    }
    return newImage;
}

QImage algoSaturation(const QImage &origin, int delta)
{
    QImage newImage(origin.width(), origin.height(), QImage::Format_ARGB32);
    for (int x = 0; x < newImage.width(); x++) {
        for (int y = 0; y < newImage.height(); y++) {
            QColor oldColor(origin.pixel(x, y));
            QColor hslColor = oldColor.toHsl();
            int h = hslColor.hue();
            int s = qBound(0, hslColor.saturation() + delta, 255);
            int l = hslColor.lightness();
            hslColor.setHsl(h, s, l);
            newImage.setPixel(x, y, qRgb(hslColor.red(), hslColor.green(), hslColor.blue()));
        }
    }
    return newImage;
}

QImage algoBlur(const QImage &origin, int /*delta*/)
{
    QImage newImage(origin);
    int kernel[5][5] = {
        {0, 0, 1, 0, 0},
        {0, 1, 3, 1, 0},
        {1, 3, 7, 3, 1},
        {0, 1, 3, 1, 0},
        {0, 0, 1, 0, 0}
    };
    int kernelSize = 5;
    int sumKernel = 27;

    for (int x = kernelSize / 2; x < newImage.width() - (kernelSize / 2); x++) {
        for (int y = kernelSize / 2; y < newImage.height() - (kernelSize / 2); y++) {
            int r = 0, g = 0, b = 0;
            for (int i = -kernelSize / 2; i <= kernelSize / 2; i++) {
                for (int j = -kernelSize / 2; j <= kernelSize / 2; j++) {
                    QColor color(origin.pixel(x + i, y + j));
                    int k = kernel[kernelSize / 2 + i][kernelSize / 2 + j];
                    r += color.red() * k;
                    g += color.green() * k;
                    b += color.blue() * k;
                }
            }
            r = qBound(0, r / sumKernel, 255);
            g = qBound(0, g / sumKernel, 255);
            b = qBound(0, b / sumKernel, 255);
            newImage.setPixel(x, y, qRgb(r, g, b));
        }
    }
    return newImage;
}

QImage algoSharpen(const QImage &origin, int /*delta*/)
{
    QImage newImage(origin);
    int kernel[5][5] = {
        { 0,  0, -1,  0,  0},
        { 0, -1, -2, -1,  0},
        {-1, -2, 16, -2, -1},
        { 0, -1, -2, -1,  0},
        { 0,  0, -1,  0,  0}
    };
    int kernelSize = 5;
    int sumKernel = 1;

    for (int x = kernelSize / 2; x < newImage.width() - (kernelSize / 2); x++) {
        for (int y = kernelSize / 2; y < newImage.height() - (kernelSize / 2); y++) {
            int r = 0, g = 0, b = 0;
            for (int i = -kernelSize / 2; i <= kernelSize / 2; i++) {
                for (int j = -kernelSize / 2; j <= kernelSize / 2; j++) {
                    QColor color(origin.pixel(x + i, y + j));
                    int k = kernel[kernelSize / 2 + i][kernelSize / 2 + j];
                    r += color.red() * k;
                    g += color.green() * k;
                    b += color.blue() * k;
                }
            }
            r = qBound(0, r / sumKernel, 255);
            g = qBound(0, g / sumKernel, 255);
            b = qBound(0, b / sumKernel, 255);
            newImage.setPixel(x, y, qRgb(r, g, b));
        }
    }
    return newImage;
}

} // namespace Algorithms
```

### Task 3: 修改 processorthread.h — 移除私有算法方法

**Files:**
- Modify: `processorthread.h`

- [ ] **Step 1: 删除 7 个私有算法方法声明**

将 processorthread.h 中第 19-26 行的以下内容删除：
```cpp
    QImage      *AlgoCool(int delta, QImage * origin);
    QImage      *AlgoWarm(int delta, QImage * origin);
    QImage      *AlgoGreyScale(int delta,QImage * origin);
    QImage      *AlgoLight(int delta,QImage * origin);
    QImage      *AlgoSaturation(int delta, QImage * origin);
    QImage      *AlgoBlur(int delta,QImage * origin);
    QImage      *AlgoSharpen(int delta,QImage * origin);
```

修改后 processorthread.h 应为：

```cpp
#ifndef PROCESSORTHREAD_H
#define PROCESSORTHREAD_H
#include <QObject>
#include<QThread>
#include<QMutex>
#include"typedef.h"
class ProcessorThread:public QThread
{
        Q_OBJECT
public:
    ProcessorThread();
    ~ProcessorThread();
    void        run();
    void        InsertImg(IMAGE_INFO &imgInfo);
    void        ProcessImg(IMAGE_INFO *imgInfo);
public:
    bool                m_bStarted;
    QList<IMAGE_INFO>   m_qlistImageInfo;
private:
    QMutex      m_qMutex;
signals:
    void        ShowPicInDebugDlg(QImage img);
};

#endif // PROCESSORTHREAD_H
```

### Task 4: 修改 processorthread.cpp — 调用 Algorithms 命名空间

**Files:**
- Modify: `processorthread.cpp`

- [ ] **Step 1: 添加 include，修改 ProcessImg 为调用 Algorithms:: 函数，删除 7 个私有方法实现**

在文件顶部添加：
```cpp
#include "algorithms.h"
```

将 `ProcessImg` 方法（第 44-86 行）替换为：

```cpp
void ProcessorThread::ProcessImg(IMAGE_INFO *imgInfo)
{
    switch (imgInfo->nAlgoID) {
    case 1: imgInfo->qImage = Algorithms::algoCool(imgInfo->qImage, imgInfo->nDelta); break;
    case 2: imgInfo->qImage = Algorithms::algoWarm(imgInfo->qImage, imgInfo->nDelta); break;
    case 3: imgInfo->qImage = Algorithms::algoLight(imgInfo->qImage, imgInfo->nDelta); break;
    case 4: imgInfo->qImage = Algorithms::algoGreyScale(imgInfo->qImage, imgInfo->nDelta); break;
    case 5: imgInfo->qImage = Algorithms::algoSaturation(imgInfo->qImage, imgInfo->nDelta); break;
    case 6: imgInfo->qImage = Algorithms::algoBlur(imgInfo->qImage, imgInfo->nDelta); break;
    case 7: imgInfo->qImage = Algorithms::algoSharpen(imgInfo->qImage, imgInfo->nDelta); break;
    }
}
```

删除文件中所有 7 个算法私有方法实现（第 88-314 行：AlgoCool, AlgoWarm, AlgoGreyScale, AlgoLight, AlgoSaturation, AlgoBlur, AlgoSharpen 的函数体全部删除）。

最终的 processorthread.cpp 应为：

```cpp
#include "processorthread.h"
#include "algorithms.h"

ProcessorThread::ProcessorThread():m_bStarted(true)
{

}

ProcessorThread::~ProcessorThread()
{

}

void ProcessorThread::run()
{

    while(m_bStarted)
    {
        m_qMutex.lock();
        if(!m_qlistImageInfo.isEmpty())
        {
            IMAGE_INFO imgInfo;
            imgInfo = m_qlistImageInfo.takeLast();
            m_qMutex.unlock();
            ProcessImg(&imgInfo);

            emit ShowPicInDebugDlg(imgInfo.qImage);
        }
        else
        {
            m_qMutex.unlock();
        }

        QThread::msleep(10);
    }
}

void ProcessorThread::InsertImg(IMAGE_INFO &imgInfo)
{
    m_qMutex.lock();
    m_qlistImageInfo.push_front(imgInfo);
    m_qMutex.unlock();
}

void ProcessorThread::ProcessImg(IMAGE_INFO *imgInfo)
{
    switch (imgInfo->nAlgoID) {
    case 1: imgInfo->qImage = Algorithms::algoCool(imgInfo->qImage, imgInfo->nDelta); break;
    case 2: imgInfo->qImage = Algorithms::algoWarm(imgInfo->qImage, imgInfo->nDelta); break;
    case 3: imgInfo->qImage = Algorithms::algoLight(imgInfo->qImage, imgInfo->nDelta); break;
    case 4: imgInfo->qImage = Algorithms::algoGreyScale(imgInfo->qImage, imgInfo->nDelta); break;
    case 5: imgInfo->qImage = Algorithms::algoSaturation(imgInfo->qImage, imgInfo->nDelta); break;
    case 6: imgInfo->qImage = Algorithms::algoBlur(imgInfo->qImage, imgInfo->nDelta); break;
    case 7: imgInfo->qImage = Algorithms::algoSharpen(imgInfo->qImage, imgInfo->nDelta); break;
    }
}
```

### Task 5: 修改 EditPic.pro — 加入 algorithms 文件

**Files:**
- Modify: `EditPic.pro`

- [ ] **Step 1: 在 SOURCES 和 HEADERS 中添加 algorithms 文件**

在 SOURCES 列表末尾添加 `algorithms.cpp`：
```
SOURCES += \
        main.cpp \
        mainwindow.cpp \
    imagewidget.cpp \
    processorthread.cpp \
    algorithms.cpp
```

在 HEADERS 列表末尾添加 `algorithms.h`：
```
HEADERS += \
        mainwindow.h \
    imagewidget.h \
    processorthread.h \
    typedef.h \
    algorithms.h
```

### Task 6: 构建主项目验证编译

**Files:**
- (验证 — 不修改文件)

- [ ] **Step 1: 构建主项目**

```bash
cd d:/Qt/EditPic && qmake EditPic.pro && nmake
```

Expected: 编译通过，无错误。`release/EditPic.exe` 生成成功。

### Task 7: 创建测试项目配置

**Files:**
- Create: `tests/tests.pro`

- [ ] **Step 1: 创建 tests/tests.pro**

```bash
mkdir -p d:/Qt/EditPic/tests
```

```qmake
QT += testlib
TARGET = tests
TEMPLATE = app
CONFIG += c++11 console
CONFIG -= app_bundle

SOURCES += test_algorithms.cpp \
    ../algorithms.cpp

HEADERS += ../algorithms.h
```

### Task 8: 创建测试用例

**Files:**
- Create: `tests/test_algorithms.cpp`

- [ ] **Step 1: 编写完整测试文件**

```cpp
#include <QtTest>
#include "../algorithms.h"

class TestAlgorithms : public QObject
{
    Q_OBJECT

private:
    QImage createTestImage(int w, int h)
    {
        QImage img(w, h, QImage::Format_ARGB32);
        // 填充已知颜色：每个像素 R=x*50, G=y*50, B=100
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                img.setPixel(x, y, qRgb(x * 50, y * 50, 100));
            }
        }
        return img;
    }

private slots:
    // ---- algoCool ----
    void algoCool_deltaZero_shouldNotChange()
    {
        QImage src = createTestImage(4, 4);
        QImage result = Algorithms::algoCool(src, 0);
        QCOMPARE(result.size(), src.size());
        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                QCOMPARE(result.pixel(x, y), src.pixel(x, y));
            }
        }
    }

    void algoCool_shouldIncreaseBlueOnly()
    {
        QImage src(1, 1, QImage::Format_ARGB32);
        src.setPixel(0, 0, qRgb(100, 150, 50));
        QImage result = Algorithms::algoCool(src, 30);
        QColor c(result.pixel(0, 0));
        QCOMPARE(c.red(),   100);
        QCOMPARE(c.green(), 150);
        QCOMPARE(c.blue(),  80); // 50+30
    }

    void algoCool_blueChannelShouldClampAt255()
    {
        QImage src(1, 1, QImage::Format_ARGB32);
        src.setPixel(0, 0, qRgb(10, 20, 240));
        QImage result = Algorithms::algoCool(src, 30);
        QColor c(result.pixel(0, 0));
        QCOMPARE(c.blue(), 255);
    }

    // ---- algoWarm ----
    void algoWarm_deltaZero_shouldNotChange()
    {
        QImage src = createTestImage(4, 4);
        QImage result = Algorithms::algoWarm(src, 0);
        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                QCOMPARE(result.pixel(x, y), src.pixel(x, y));
            }
        }
    }

    void algoWarm_shouldIncreaseRedGreenOnly()
    {
        QImage src(1, 1, QImage::Format_ARGB32);
        src.setPixel(0, 0, qRgb(100, 150, 200));
        QImage result = Algorithms::algoWarm(src, 20);
        QColor c(result.pixel(0, 0));
        QCOMPARE(c.red(),   120);
        QCOMPARE(c.green(), 170);
        QCOMPARE(c.blue(),  200);
    }

    void algoWarm_redGreenShouldClampAt255()
    {
        QImage src(1, 1, QImage::Format_ARGB32);
        src.setPixel(0, 0, qRgb(245, 250, 100));
        QImage result = Algorithms::algoWarm(src, 20);
        QColor c(result.pixel(0, 0));
        QCOMPARE(c.red(),   255);
        QCOMPARE(c.green(), 255);
    }

    // ---- algoLight ----
    void algoLight_deltaZero_shouldNotChange()
    {
        QImage src = createTestImage(4, 4);
        QImage result = Algorithms::algoLight(src, 0);
        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                QCOMPARE(result.pixel(x, y), src.pixel(x, y));
            }
        }
    }

    void algoLight_shouldIncreaseAllChannels()
    {
        QImage src(1, 1, QImage::Format_ARGB32);
        src.setPixel(0, 0, qRgb(50, 100, 150));
        QImage result = Algorithms::algoLight(src, 10);
        QColor c(result.pixel(0, 0));
        QCOMPARE(c.red(),   60);
        QCOMPARE(c.green(), 110);
        QCOMPARE(c.blue(),  160);
    }

    void algoLight_negativeDelta_shouldDarken()
    {
        QImage src(1, 1, QImage::Format_ARGB32);
        src.setPixel(0, 0, qRgb(50, 100, 150));
        QImage result = Algorithms::algoLight(src, -30);
        QColor c(result.pixel(0, 0));
        QCOMPARE(c.red(),   20);
        QCOMPARE(c.green(), 70);
        QCOMPARE(c.blue(),  120);
    }

    // ---- algoGreyScale ----
    void algoGreyScale_shouldAverageThreeChannels()
    {
        QImage src(1, 1, QImage::Format_ARGB32);
        src.setPixel(0, 0, qRgb(30, 60, 90)); // average = 60
        QImage result = Algorithms::algoGreyScale(src, 0);
        QColor c(result.pixel(0, 0));
        QCOMPARE(c.red(),   60);
        QCOMPARE(c.green(), 60);
        QCOMPARE(c.blue(),  60);
    }

    void algoGreyScale_withDelta_shouldApplyDelta()
    {
        QImage src(1, 1, QImage::Format_ARGB32);
        src.setPixel(0, 0, qRgb(30, 60, 90)); // average = 60, +10 = 70
        QImage result = Algorithms::algoGreyScale(src, 10);
        QColor c(result.pixel(0, 0));
        QCOMPARE(c.red(),   70);
        QCOMPARE(c.green(), 70);
        QCOMPARE(c.blue(),  70);
    }

    // ---- algoSaturation ----
    void algoSaturation_deltaZero_shouldNotChange()
    {
        QImage src = createTestImage(4, 4);
        QImage result = Algorithms::algoSaturation(src, 0);
        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                QCOMPARE(result.pixel(x, y), src.pixel(x, y));
            }
        }
    }

    void algoSaturation_shouldIncreaseSaturation()
    {
        QImage src(1, 1, QImage::Format_ARGB32);
        src.setPixel(0, 0, qRgb(200, 100, 100)); // 有色彩的像素
        QColor srcHsl = QColor(src.pixel(0, 0)).toHsl();
        int srcSat = srcHsl.saturation();

        QImage result = Algorithms::algoSaturation(src, 50);
        QColor resHsl = QColor(result.pixel(0, 0)).toHsl();
        QVERIFY(resHsl.saturation() > srcSat);
    }

    void algoSaturation_saturationShouldClamp()
    {
        QImage src(1, 1, QImage::Format_ARGB32);
        src.setPixel(0, 0, qRgb(200, 100, 100));
        QImage result = Algorithms::algoSaturation(src, 300); // 极端值
        QColor resHsl = QColor(result.pixel(0, 0)).toHsl();
        QVERIFY(resHsl.saturation() >= 0);
        QVERIFY(resHsl.saturation() <= 255);
    }

    // ---- algoBlur ----
    void algoBlur_shouldPreserveSize()
    {
        QImage src = createTestImage(8, 8);
        QImage result = Algorithms::algoBlur(src, 0);
        QCOMPARE(result.size(), src.size());
    }

    void algoBlur_shouldProduceBlurredOutput()
    {
        // 创建一个边缘清晰的图片：左边是红(255,0,0)，右边是蓝(0,0,255)
        QImage src(6, 6, QImage::Format_ARGB32);
        for (int x = 0; x < 6; x++) {
            for (int y = 0; y < 6; y++) {
                if (x < 3)
                    src.setPixel(x, y, qRgb(255, 0, 0));
                else
                    src.setPixel(x, y, qRgb(0, 0, 255));
            }
        }
        QImage result = Algorithms::algoBlur(src, 0);

        // 边界像素 (x=2) 在模糊后应该不再是纯红
        QColor boundaryPixel(result.pixel(2, 2));
        QVERIFY(boundaryPixel.red() < 255); // 红色受蓝色区域影响而降低
        QVERIFY(boundaryPixel.blue() > 0);  // 蓝色通道有渗透
    }

    void algoBlur_edgesUnchanged()
    {
        QImage src = createTestImage(8, 8);
        QImage result = Algorithms::algoBlur(src, 0);
        // 边缘像素 (0,0) 应保持不变（kernel 无法覆盖）
        QCOMPARE(result.pixel(0, 0), src.pixel(0, 0));
    }

    // ---- algoSharpen ----
    void algoSharpen_shouldPreserveSize()
    {
        QImage src = createTestImage(8, 8);
        QImage result = Algorithms::algoSharpen(src, 0);
        QCOMPARE(result.size(), src.size());
    }

    void algoSharpen_shouldIncreaseEdgeContrast()
    {
        // 边缘图片：左边暗(10,10,10)，右边亮(240,240,240)
        QImage src(6, 6, QImage::Format_ARGB32);
        for (int x = 0; x < 6; x++) {
            for (int y = 0; y < 6; y++) {
                if (x < 3)
                    src.setPixel(x, y, qRgb(10, 10, 10));
                else
                    src.setPixel(x, y, qRgb(240, 240, 240));
            }
        }
        QImage result = Algorithms::algoSharpen(src, 0);

        // 边界处暗侧像素应被推得更暗（锐化效果）
        QColor darkSidePixel(result.pixel(2, 2));
        QVERIFY2(darkSidePixel.red() <= 10,
                 QString("Sharpened dark side should be <= 10, got %1").arg(darkSidePixel.red()));
    }

    // ---- 通用测试 ----
    void all_shouldPreserveSize()
    {
        QImage src = createTestImage(5, 7);
        QCOMPARE(Algorithms::algoCool(src, 10).size(), src.size());
        QCOMPARE(Algorithms::algoWarm(src, 10).size(), src.size());
        QCOMPARE(Algorithms::algoLight(src, 10).size(), src.size());
        QCOMPARE(Algorithms::algoGreyScale(src, 10).size(), src.size());
        QCOMPARE(Algorithms::algoSaturation(src, 10).size(), src.size());
        QCOMPARE(Algorithms::algoBlur(src, 10).size(), src.size());
        QCOMPARE(Algorithms::algoSharpen(src, 10).size(), src.size());
    }

    void all_shouldPreserveFormat()
    {
        QImage src(3, 3, QImage::Format_ARGB32);
        QCOMPARE(Algorithms::algoCool(src, 10).format(), QImage::Format_ARGB32);
        QCOMPARE(Algorithms::algoWarm(src, 10).format(), QImage::Format_ARGB32);
        QCOMPARE(Algorithms::algoLight(src, 10).format(), QImage::Format_ARGB32);
        QCOMPARE(Algorithms::algoGreyScale(src, 10).format(), QImage::Format_ARGB32);
        QCOMPARE(Algorithms::algoSaturation(src, 10).format(), QImage::Format_ARGB32);
        QCOMPARE(Algorithms::algoBlur(src, 10).format(), QImage::Format_ARGB32);
        QCOMPARE(Algorithms::algoSharpen(src, 10).format(), QImage::Format_ARGB32);
    }

    void all_extremeDelta_shouldNotCrash()
    {
        QImage src = createTestImage(4, 4);
        QVERIFY(!Algorithms::algoCool(src, -255).isNull());
        QVERIFY(!Algorithms::algoWarm(src, 255).isNull());
        QVERIFY(!Algorithms::algoLight(src, -255).isNull());
        QVERIFY(!Algorithms::algoGreyScale(src, 255).isNull());
    }
};

QTEST_APPLESS_MAIN(TestAlgorithms)
#include "test_algorithms.moc"
```

### Task 9: 构建并运行测试

**Files:**
- (验证 — 不修改文件)

- [ ] **Step 1: 构建测试项目**

```bash
cd d:/Qt/EditPic/tests && qmake tests.pro && nmake
```

Expected: 编译通过，无错误。

- [ ] **Step 2: 运行测试**

```bash
cd d:/Qt/EditPic/tests && release/tests.exe
# 或 debug/tests.exe
```

Expected: 全部测试 PASS（约 18 个测试用例）。

### Task 10: 提交所有更改

**Files:**
- (Git 操作 — 不修改代码文件)

- [ ] **Step 1: 暂存并提交**

```bash
cd d:/Qt/EditPic
git add algorithms.h algorithms.cpp
git add processorthread.h processorthread.cpp EditPic.pro
git add tests/tests.pro tests/test_algorithms.cpp
git commit -m "$(cat <<'EOF'
feat: 提取图像处理算法为独立库并添加单元测试

- 新增 algorithms.h/cpp，7个算法放入 Algorithms 命名空间
- 修复 AlgoGreyScale 中红色通道重复使用的 bug
- ProcessorThread 改为调用 Algorithms 命名空间
- 新增 tests/ 子项目，使用 QtTest 框架覆盖全部 7 个算法
EOF
)"
```
