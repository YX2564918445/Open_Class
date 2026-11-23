# 科创扬帆系列公开课

西安理工大学自动化与信息工程学院学生科学与技术协会「科创扬帆系列公开课」配套资料仓库，包含课程讲义与可直接编译下载的示例工程。

本仓库面向零基础同学，从 51 单片机入门开始，后续将陆续补充 **嘉立创 EDA** 与 **STM32** 相关课程。

## 仓库结构

```
Open_Class/
├── 51_Course/          # 51 单片机课程讲义（Markdown）
│   ├── assets/         # 讲义图片与 logo
│   ├── pdf/            # 本地生成的 PDF（已被 .gitignore 忽略）
│   ├── build_pdf.py    # 讲义转 PDF 脚本
│   └── pdf-style.css   # PDF 样式
├── 51_Project/         # 51 单片机配套例程（Keil C51 工程）
├── .gitignore
└── README.md
```

| 目录 | 说明 |
| --- | --- |
| [51_Course/](51_Course/) | 51 单片机培训讲义，统一使用 Markdown（`.md`）编写，讲义目录见 [51_Course/README.md](51_Course/README.md) |
| [51_Project/](51_Project/) | 51 单片机例程，按讲次编号，每个子目录为一个独立 Keil 工程 |
| `JLC_EDA_Course/`、`STM32_Course/` | 规划中，后续课程资料将按同样方式组织 |

## 51 单片机培训

- **开发板**：普中 A2 开发板
- **主控芯片**：STC89C52RC（工程按 AT89C52 配置，指令与寄存器兼容）
- **晶振频率**：11.0592 MHz
- **开发环境**：Keil uVision（C51）
- **下载工具**：STC-ISP

### 课程与例程

例程编号与课程讲次对应，每个工程均可单独打开、编译和下载。

| 序号 | 例程 | 讲义 | 内容 | 主要知识点 |
| --- | --- | --- | --- | --- |
| 1 | [1First_LED](51_Project/1First_LED/) | [第 1 讲](51_Course/01_Keil新建工程与程序下载.md) | 点亮 LED、LED 交替亮灭 | GPIO 输出、共阳极接法、`sbit` 位定义 |
| 2 | [2Waterfall_Light](51_Project/2Waterfall_Light/) | [第 2 讲](51_Course/02_流水灯.md) | 流水灯 | 移位运算、端口整体赋值、延时函数 |
| 3 | [3Key_LED](51_Project/3Key_LED/) | [第 3 讲](51_Course/03_按键控制LED.md) | 按键控制 LED | GPIO 输入、按键扫描、软件消抖 |
| 4 | [4Nixie](51_Project/4Nixie/) | [第 4 讲](51_Course/04_数码管.md) | 8 位数码管显示 | 共阴段码、74HC138 位选、动态扫描与消隐 |
| 5 | [5Buzzer](51_Project/5Buzzer/) | [第 5 讲](51_Course/05_蜂鸣器.md) | 按键控制蜂鸣器 | 蜂鸣器驱动、频率控制、多文件模块化编程 |
| 6 | [6Motor](51_Project/6Motor/) | [第 6 讲](51_Course/06_直流电机.md) | 按键控制直流电机转速 | 电机驱动、PWM 与占空比、`Delay_10us` 软件 PWM |
| 7 | [7MatrixKey](51_Project/7MatrixKey/) | [第 7 讲](51_Course/07_矩阵按键.md) | 矩阵按键输入、数码管显示 | 4×4 矩阵键盘、逐行扫描、显示缓冲区左移溢出、键值映射 |
| 8 | [8ExternalINT0](51_Project/8ExternalINT0/) | [第 8 讲](51_Course/08_外部中断0.md) | 外部中断打断流水灯 | 中断概念、TCON/IE 配置、下降沿触发、ISR 编写 |
| 9 | [9ExternalINT1](51_Project/9ExternalINT1/) | [第 9 讲](51_Course/09_外部中断1.md) | 数码管秒表 + 按住闪灯 | 低电平触发、反复进入 ISR、动态扫描计时、ISR 阻塞主循环 |
| 10 | [10TimerStopwatch](51_Project/10TimerStopwatch/) | [第 10 讲](51_Course/10_定时器秒表.md) | 定时器 0 数码管秒表 | 定时器原理、10 ms 初值、模式 1 软件重装、软件分频、`volatile`、临界区 |
| 11 | [11BreathingLED](51_Project/11BreathingLED/) | [第 11 讲](51_Course/11_定时器PWM与呼吸灯.md) | 定时器 PWM 呼吸灯 | 100 µs 节拍、占空比比较、中断时间预算、一个定时器多节拍 |
| 12 | [12SerialLED](51_Project/12SerialLED/) | [第 12 讲](51_Course/12_串口通信.md) | 串口控制 LED 并回显 | 波特率与初值、定时器 1 模式 2、串口中断收发共用、判帧、临界区 |

> 讲义统一位于 [51_Course/](51_Course/)，讲次编号与例程编号一一对应。修改例程时请同步更新对应讲义。

### 中期考核

第 9 讲课后布置中期考核，题目为 **用普中 A2 开发板做一台简易计算器**（第 10~12 讲照常推进）：

- 矩阵键盘输入 `0~9` 与运算符，数码管显示数字、负号和小数点；
- 实现多位整数的加、减、乘、除，结果正确；
- 支持清零、连续计算、除零提示与溢出提示；
- 提交时间：**11 月 29 日 20:00 前**；
- 提交邮箱：**2086446841@qq.com**，邮件主题 `中期考核+班级+姓名`；
- 参考评分：键盘输入 20 分、数码管显示 20 分、四则运算 40 分、辅助功能与异常处理 20 分。

> 提交物形式与评分细则以课堂公布为准。

### 快速开始

1. 克隆仓库：

   ```bash
   git clone https://github.com/YX2564918445/Open_Class.git
   ```

2. 用 Keil uVision 打开任一例程目录下的 `.uvproj` 工程文件（如 `51_Project/1First_LED/First_LED.uvproj`）。
3. 点击 **Build**（F7）编译工程，确认无报错。
4. 使用 STC-ISP 将生成的 `.hex` 文件下载到开发板（详细步骤见[第 1 讲](51_Course/01_Keil新建工程与程序下载.md)）。

> 编译生成的 `Objects/`、`Listings/`、`.hex` 等文件属于中间产物，已被 `.gitignore` 忽略，请勿提交。

## 讲义 PDF

讲义正文只维护 Markdown，PDF 由脚本在本地生成，**不提交进仓库**（`51_Course/pdf/` 已被忽略）。

依赖：

- Python 3 + [PyMuPDF](https://pymupdf.readthedocs.io/)（用于逐页盖 logo）
- Node.js（脚本通过 `npx md-to-pdf` 渲染，首次运行会自动拉取依赖）
- 本机安装的 Chrome（路径写死在 `51_Course/build_pdf.py` 的 `LAUNCH_OPTIONS` 中，如不同请自行修改）

用法：

```bash
cd 51_Course
python build_pdf.py --all                 # 全部重新生成
python build_pdf.py 07_矩阵按键.md         # 只生成指定讲义
```

生成的 PDF 输出到 `51_Course/pdf/`，样式定义在 `pdf-style.css`，与已发布的前六份保持同一版式（页边距、字号、逐页 logo、目录内链可点击）。

> 用 Chrome 自带的 `--print-to-pdf` 或 DevTools 的 `Page.printToPDF` 导出的 PDF 会丢掉目录内链，这也是这里改用 `md-to-pdf` 的原因。

## 文档规范

为保证资料在 GitHub、编辑器与各类笔记工具中都能正常阅读和检索，课程文档统一遵循以下约定：

- **讲义只使用 Markdown（`.md`）格式**，不提交 `.doc`、`.docx`、`.pdf`、`.ppt`、`.pptx` 等二进制文档；
- 文件命名使用 `讲次_主题.md`，例如 `01_Keil新建工程与程序下载.md`；
- 文档中的图片放在同级 `assets/` 或 `images/` 目录中，使用相对路径引用；
- 代码块请标注语言类型（如 `c`、`bash`），便于高亮显示；
- 一个讲次对应一个 Markdown 文件，内容较长时可拆分为多个文件，并在讲次目录下提供索引。

## 贡献指南

欢迎协会成员共同完善课程资料：

1. 新建分支进行修改，避免直接在主分支上开发；
2. 提交前确认工程能正常编译，且未提交编译产物；
3. 提交信息简明说明改动内容，例如 `docs: 补充流水灯讲义`、`feat: 新增蜂鸣器例程`；
4. 通过 Pull Request 合并。

## 维护者

本仓库由西安理工大学自动化与信息工程学院学生科学与技术协会维护。

| 姓名 | 班级 | 角色 |
| --- | --- | --- |
| 尹祥 | 自卓241班 | 第35届执行主席 |

## 后续计划

- [x] 51 单片机基础例程
- [x] 51 单片机课程讲义第 1~12 讲（含矩阵按键、外部中断、定时器、串口）
- [x] 讲义转 PDF 工具链（样式对齐既有六份）
- [ ] 中期考核讲评与优秀作品整理
- [ ] 嘉立创 EDA 电路设计与 PCB 教程
- [ ] STM32 入门与综合实践教程