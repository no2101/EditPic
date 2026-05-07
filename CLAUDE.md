# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build

This is a Qt5 Widgets project using qmake with C++11. Use a MinGW toolchain on Windows.

```bash
qmake EditPic.pro
make
# or: mingw32-make (depending on your Qt MinGW installation)
```

To clean build artifacts:
```bash
Qt-clear.bat
```

## Architecture

EditPic is a simple image processing application. The UI has a button panel (open file, 7 filter buttons) on the left, a horizontal slider for filter intensity, and a QGraphicsView canvas for displaying the processed image.

**Data flow:** MainWindow loads images → sends IMAGE_INFO (image + algorithm ID + delta) to ProcessorThread → ProcessorThread runs the pixel-processing algorithm in a background QThread → emits `ShowPicInDebugDlg` signal → MainWindow receives processed image and renders it via ImageWidget on the QGraphicsView.

**Key classes:**

- `MainWindow` — Owns everything. Holds the list of loaded images (`m_qlistLoadImgs`), tracks the current image index (`m_nCrtIndex`) and current algorithm (`m_nCrtAlgoID`). Each filter button sets `m_nCrtAlgoID` (1-7) and dispatches the current image to the processing thread. Algorithm IDs: 1=Cool, 2=Warm, 3=Lightness, 4=Grayscale, 5=Saturation, 6=Blur, 7=Sharpen.
- `ImageWidget` (QGraphicsItem) — Renders a QPixmap centered at origin. Supports zoom (mouse wheel, up to 50x), pan (left-click drag), and reset (right-click). Auto-scales to fit the QGraphicsView on load.
- `ProcessorThread` (QThread) — Producer-consumer loop: polls `m_qlistImageInfo` every 10ms (QMutex-protected), processes the last item, and signals the result back. Processing is synchronous per-image within the thread.
- `IMAGE_INFO` (typedef.h) — `{ QImage qImage; int nAlgoID; int nDelta; }`

**Processing algorithms** (all in ProcessorThread): pixel-by-pixel color channel manipulation except Blur and Sharpen which use 5x5 convolution kernels. Each returns a heap-allocated `QImage*` that is copied into `IMAGE_INFO::qImage` (potential memory leak — the returned QImage is never deleted). The `delta` slider value adjusts filter intensity.

**UI layout** (mainwindow.ui): Left panel (QVBoxLayout with 8 buttons + label), right panel (QSlider + QGraphicsView named `ImageGraphic`). Window default size 1280×720.

## Notes

- No unit tests exist in the project.
- The slider-moved handler is intentionally commented out (firing on every pixel of slider movement was too expensive).
- `AlgoGreyScale` has a bug — it uses `qRed(line[x])` twice instead of `qRed`, `qGreen`, `qBlue` for the average calculation.
- Thread shutdown uses a bool flag (`m_bStarted`) rather than `QThread::requestInterruption()`.
