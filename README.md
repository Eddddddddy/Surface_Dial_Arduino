# Surface_Dial_Arduino

Use attiny85 to achieve surface dial function

将这个添加到你的Arduino IDE开发板中：http://digistump.com/package_digistump_index.json

/bom.xlsx ：元器件清单

/lib ：arduino库

/src ：arduino代码（包含融丝恢复器代码）

/tools ：编程器上位机软件

/stl model ：3D打印模型

/PCB_with_gerber ：pcb文件

【点个Star！】谢谢！

在[这里](https://www.bilibili.com/video/BV17K411W78w/)可以查看视频介绍

------

|                              | Pro                                  | Lite                              |
| ---------------------------- | ------------------------------------ | --------------------------------- |
| **O&M**                      | @Eddddddddy            | @Eddddddddy, @leostudiooo         |
| **Basic Functions**          | Y                                    | Y                                 |
| **Cost**                     | Higher                               | Lower                             |
| **Vibration(Taptic Engine)** | Y                                    | N                                 |
| **Encoder**                  | EC11E1834403*(ALPS, without detent)* | EC11*(With detent)*               |
| **Arduino File**             | /src/Surface_Dial_attiny.ino         | /src/Surface_Dial_attiny_Lite.ino |
| **PCB File**                 | /PCB_with_gerber/Pro | /PCB_with_gerber/Lite |
| **Fuse Reset**               | Needed                               | Unnecessary                       |