# STM32_BootLoader_HAL
[![LICENSE](https://img.shields.io/badge/license-Anti%20996-blue.svg)](https://github.com/996icu/996.ICU/blob/master/LICENSE)

## 介绍
基于官方STM32F4系列单片机uart-bootloader示例修改的STM32 bootloader程序，同时参考了GitHub以及相关论坛的开源资源。使用CubeMX创建工程，方便移植，简单易用，后续计划制作USB传输以及上位机。

## 硬件
* STM32F407VET6核心板(自制)
* 中景圆电子1.3寸TFT全彩显示屏(240*240)
* STLINK v2.1(nucleo板子上掰的)

## 软件
* Keil MDK v5
* [hyperterminal](https://github.com/JassyL/STM32_BootLoader_HAL/tree/master/hyperterminal_WIN10)

## 使用方法
TODO

## 演示
* 进入bootloader
![](https://raw.githubusercontent.com/JassyL/STM32_BootLoader_HAL/master/resources/1.gif)

* 打开bin文件
![](https://raw.githubusercontent.com/JassyL/STM32_BootLoader_HAL/master/resources/2.gif)

* 传输
![](https://raw.githubusercontent.com/JassyL/STM32_BootLoader_HAL/master/resources/3.gif)

* 运行APP代码
![](https://raw.githubusercontent.com/JassyL/STM32_BootLoader_HAL/master/resources/4.gif)

## 实物
![](https://raw.githubusercontent.com/JassyL/STM32_BootLoader_HAL/master/resources/Board.jpg)