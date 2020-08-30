# 二合一版本未经本人测试

# 关于版本选择和焊接 Version Choice & Soldering

## **！建议使用/PCB_with_gerber/Pro/...attiny v2 github.zip！**

## 版本选择 Version Chioce

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

## PCB

你可以选择原版Pro或Lite，也可以尝试新出来的二合一版本。
You can choose the original Pro or Lite, or the 2 in 1 ver.

## 焊接 Soldering (for 2 in 1 ver.)

如果你选择二合一版本，那么你有升级的机会。
If you choose the 2 in 1(AIO) version, you have a chance to upgrade to Pro.

- 如果你选择Pro，请连接板子背面标注SW Pro的两个焊盘。
If you choose Pro, plz connect the two solders with "SW Pro" on the bottom.

- 如果你选择Lite，请连接板子正面标注SW Lite的两个焊盘。
If you choose Lite, plz connect the two solders with "SW Lite" on the top.
