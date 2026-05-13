# CLAUDE.md

此文件为 Claude Code (claude.ai/code) 在此仓库中工作时提供指导。

## 构建

这是一个使用 qmake + C++17 的 Qt6 Widgets 项目。在 Windows 上使用 MSVC 2022 x64 工具链。

```bash
qmake EditPic.pro
nmake
```

清理构建产物：
```bash
Qt-clear.bat
```

## 架构

EditPic 是一个简单的图像处理应用。界面左侧为按钮面板（打开文件、7 个滤镜按钮），中间有一个控制滤镜强度的水平滑块，右侧是用于显示处理后图像的 QGraphicsView 画布。

**数据流：** MainWindow 加载图片 → 发送 IMAGE_INFO（图片 + 算法 ID + 强度值）到 ProcessorThread → ProcessorThread 在后台 QThread 中执行像素处理算法 → 发出 `ShowPicInDebugDlg` 信号 → MainWindow 接收处理后的图像，通过 ImageWidget 渲染到 QGraphicsView 上。

**核心类：**

- `MainWindow` — 管理一切。持有已加载图片列表（`m_qlistLoadImgs`），追踪当前图片索引（`m_nCrtIndex`）和当前算法（`m_nCrtAlgoID`）。每个滤镜按钮设置 `m_nCrtAlgoID`（1-7），并将当前图片分派到处理线程。算法 ID：1=冷色调，2=暖色调，3=亮度，4=灰度，5=饱和度，6=模糊，7=锐化。
- `ImageWidget`（QGraphicsItem）— 以原点为中心渲染 QPixmap。支持缩放（鼠标滚轮，最大 50x）、平移（左键拖拽）和重置（右键）。加载时自动缩放以适应 QGraphicsView。
- `ProcessorThread`（QThread）— 生产者-消费者循环：每 10ms 轮询 `m_qlistImageInfo`（QMutex 保护），处理最后一项，并将结果信号发回。线程内每张图片的处理是同步的。
- `IMAGE_INFO`（typedef.h）— `{ QImage qImage; int nAlgoID; int nDelta; }`

**处理算法**（均在 ProcessorThread 中）：除模糊和锐化使用 5x5 卷积核外，其余均为逐像素颜色通道操作。每个算法返回堆上分配的 `QImage*`，被拷贝到 `IMAGE_INFO::qImage` 中（存在潜在内存泄漏——返回的 QImage 从未被释放）。滑块的 `delta` 值用于调整滤镜强度。

**UI 布局**（mainwindow.ui）：左侧面板（QVBoxLayout，8 个按钮 + 标签），右侧面板（QSlider + 名为 `ImageGraphic` 的 QGraphicsView）。窗口默认大小 1280×720。

## 注意事项

- 项目中无单元测试。
- 滑块移动处理函数被故意注释掉了（滑块每像素移动都触发处理，开销过大）。
- `AlgoGreyScale` 存在 bug——在计算平均值时重复使用了 `qRed(line[x])`，而非分别取 `qRed`、`qGreen`、`qBlue`。
- 线程关闭使用 bool 标志（`m_bStarted`），而非 `QThread::requestInterruption()`。
