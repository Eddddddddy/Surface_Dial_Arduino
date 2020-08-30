# Surface_Dial_Arduino

Use attiny85 to achieve surface dial function

## **！注意！建议使用/PCB_with_gerber/Pro/...attiny v2 github.zip！其他pcb工程（包含立创EDA的开源工程）未经本人测试，不做可靠性保证。**

将这个添加到你的Arduino IDE开发板中：http://digistump.com/package_digistump_index.json

/bom.xlsx ：元器件清单

/lib ：arduino库

/src ：arduino代码（包含融丝恢复器代码）

/tools ：编程器上位机软件

/stl model ：3D打印模型

/PCB_with_gerber ：pcb文件

立创EDA开源文件在[这里](https://oshwhub.com/Lingling0408/project-dial-pcb)

【点个Star！】谢谢！

在[这里](https://www.bilibili.com/video/BV17K411W78w/)可以查看视频介绍

------

|                              | Pro                                  | Lite                              |
| ---------------------------- | ------------------------------------ | --------------------------------- |
| **O&M**                      | @Eddddddddy            | @Eddddddddy, @leostudiooo         |
| **Basic Functions**          | Y                                    | Y                                 |
| **Cost**                     | Higher                               | Lower                             |
| **Vibration(Taptic Engine)** | Y                                    | N                                 |
| **Encoder**                  | EC11E1834403 _(ALPS, without detent)_ | EC11*(With detent)*               |
| **Arduino File**             | /src/Surface_Dial_attiny.ino         | /src/Surface_Dial_attiny_Lite.ino |
| **PCB File** _(Now All-in-One)_                 | /PCB_with_gerber/Pro | /PCB_with_gerber/Lite |
| **Fuse Reset**               | Needed                               | Unnecessary                       |

------

你需要做什么？

- 想好购买的元器件是Pro还是Lite *（虽然后期也可以升级）*

- 在你信任的地方购买所需元器件

- 焊接（见/PCB_with_Gerber/README.md)

- 刷入程序

- 开始你的Dial之旅
