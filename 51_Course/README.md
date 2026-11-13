# 51 单片机课程讲义

本目录存放「科创扬帆系列公开课」51 单片机培训的配套讲义，所有文档均为 Markdown（`.md`）格式。

## 课程安排

| 讲次 | 讲义 | 配套例程 | 主要内容 |
| --- | --- | --- | --- |
| 第 1 讲 | [Keil 新建工程与程序下载](01_Keil新建工程与程序下载.md) | [1First_LED](../51_Project/1First_LED/) | 开发板与 Keil 环境、新建工程、编译、STC-ISP 下载、点亮 LED |
| 第 2 讲 | [流水灯](02_流水灯.md) | [2Waterfall_Light](../51_Project/2Waterfall_Light/) | 数制与位运算、LED 电路、移位流水灯、延时函数 |
| 第 3 讲 | [按键控制 LED](03_按键控制LED.md) | [3Key_LED](../51_Project/3Key_LED/) | 准双向口、按键电路、软件消抖、状态翻转 |
| 第 4 讲 | [数码管](04_数码管.md) | [4Nixie](../51_Project/4Nixie/) | 共阴数码管、段码、74HC138 位选、动态扫描与消隐、多文件工程 |
| 第 5 讲 | [蜂鸣器](05_蜂鸣器.md) | [5Buzzer](../51_Project/5Buzzer/) | 多文件模块化、无源蜂鸣器、方波与频率、`Delay_10us` |
| 第 6 讲 | [直流电机](06_直流电机.md) | [6Motor](../51_Project/6Motor/) | ULN2003 驱动、PWM 调速、占空比、`static` 状态保持 |
| 第 7 讲 | [矩阵按键](07_矩阵按键.md) | [7MatrixKey](../51_Project/7MatrixKey/) | 4×4 矩阵键盘、行扫描、数码管显示缓冲区、左移溢出、键值映射 |
| 第 8 讲 | [外部中断 0](08_外部中断0.md) | [8ExternalINT0](../51_Project/8ExternalINT0/) | 中断概念、TCON/IE、下降沿触发、ISR 编写、流水灯 |
| 第 9 讲 | [外部中断 1](09_外部中断1.md) | [9ExternalINT1](../51_Project/9ExternalINT1/) | 低电平触发、反复进入 ISR、秒表、数码管计时、ISR 冻结 main |

## 编写约定

- 文件命名：`讲次_主题.md`，例如 `01_点亮第一个LED.md`；
- 图片存放于 `assets/` 目录，使用相对路径引用，例如 `![接线图](assets/led-接线图.png)`；
- 代码块标注语言：` ```c `；
- 讲义中的代码应与 [51_Project/](../51_Project/) 中对应例程保持一致，修改例程时同步更新讲义。

## 生成 PDF

讲义正文只维护 Markdown，PDF 在本地生成、不提交进仓库：

```bash
cd 51_Course
python build_pdf.py --all              # 全部重新生成
python build_pdf.py 07_矩阵按键.md      # 只生成指定讲义
```

产物输出到 `pdf/`，样式定义在 [pdf-style.css](pdf-style.css)，与已发布的前六份保持同一版式。依赖与用法详见[根目录 README](../README.md#讲义-pdf)。
