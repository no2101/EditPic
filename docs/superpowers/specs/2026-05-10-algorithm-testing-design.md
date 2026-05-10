# 图像处理算法测试 — 设计文档

**日期：** 2026-05-10  
**范围：** 为核心图像处理算法建立单元测试体系，同时小幅重构以改善可测试性。

## 目标

- 为 7 个滤镜算法建立可靠的单元测试
- 解耦算法函数与 ProcessorThread，改善代码结构
- 修复过程中发现的已知 bug
- 引入与主项目一致的 QtTest 测试框架（qmake + MSVC x64）

## 文件结构

```text
EditPic/
├── algorithms.h          # 新增 — 7 个算法函数声明
├── algorithms.cpp        # 新增 — 7 个算法函数实现（从 ProcessorThread 提取）
├── tests/
│   ├── tests.pro         # 新增 — 测试子项目
│   └── test_algorithms.cpp # 新增 — QtTest 测试用例
├── processorthread.cpp   # 修改 — 改为调用 algorithms 中的函数
├── processorthread.h     # 修改 — 移除私有算法方法声明
└── EditPic.pro           # 修改 — SOURCES/HEADERS 中加入 algorithms 文件
```

## 算法提取

### 命名空间

所有算法函数放入 `Algorithms` 命名空间。

### 函数签名

原签名（返回堆指针，存在内存泄漏）：
```cpp
QImage *AlgoCool(int delta, QImage *origin);
```

新签名（值返回，利用 Qt 隐式共享）：
```cpp
QImage algoCool(const QImage &origin, int delta);
```

全部 7 个函数：
- `algoCool(const QImage &origin, int delta)`
- `algoWarm(const QImage &origin, int delta)`
- `algoLight(const QImage &origin, int delta)`
- `algoGreyScale(const QImage &origin, int delta)`
- `algoSaturation(const QImage &origin, int delta)`
- `algoBlur(const QImage &origin, int delta)`
- `algoSharpen(const QImage &origin, int delta)`

### ProcessorThread 调整

`ProcessImg()` 中直接调用 `Algorithms::algoXxx()`，不再持有私有算法方法。原有 `new QImage` 堆分配逻辑一并消除。

## Bug 修复

### algoGreyScale — 红色通道重复使用

原代码：
```cpp
int average = (qRed(line[x]) + qGreen(line[x]) + qRed(line[x])) / 3;
```

修复为：
```cpp
int average = (qRed(line[x]) + qGreen(line[x]) + qBlue(line[x])) / 3;
```

## 测试用例

### 框架

- QtTest（`QTEST_APPLESS_MAIN`，无需 GUI）
- 测试项目：`tests/tests.pro`，独立于主项目构建

### 测试覆盖

| 算法 | 测试点 |
|---|---|
| `algoCool` | delta=0 无变化；delta>0 仅蓝通道增加；delta=255 蓝通道封顶 |
| `algoWarm` | delta=0 无变化；delta>0 仅红绿通道增加；边界值封顶 |
| `algoLight` | delta=0 无变化；delta>0 三通道等量增加；delta<0 变暗 |
| `algoGreyScale` | 输出 R=G=B=三通道平均值（验证 bug 修复） |
| `algoSaturation` | delta=0 无变化；delta>0 饱和度增加 |
| `algoBlur` | 输出尺寸不变；5x5 卷积正确；delta 参数当前未生效（记录为已知问题） |
| `algoSharpen` | 输出尺寸不变；5x5 卷积正确；delta 参数当前未生效（同上） |

### 通用验证

- 所有算法输出尺寸与输入一致
- 所有算法输出保持 ARGB32 格式
- 极端 delta 值（-255、+255）不崩溃

## 构建与运行

```bash
# 主项目（不变）
qmake EditPic.pro && nmake

# 测试
cd tests && qmake tests.pro && nmake
tests.exe
```

## 不纳入范围

- CI/CD — 单人项目，后续需要再引入
- 代码覆盖率 — 先有测试再考虑
- 性能 benchmark — 当前不需要
- GUI / 线程 / ImageWidget 测试 — 本轮只覆盖算法层
