# EmbedCalendar

得到桌面壁纸
附加自己UI
绘制到SHELLDLL_DefView的父窗口上
非常快的定时器检查壁纸的变化

窗口x,y,w,h屏幕坐标转化为窗口坐标
为SysListView32窗口的指定区域拍照：Photo1；
Photo2 = 窗口像素+Photo1
把Photo2绘制到SHELLDLL_DefView窗口上
定时重绘


可以给SHELLDLL_DefView绘图
但绘制的内容会盖住SysListView32的图标
此时再给SysListView32拍照的话，拍到的是窗口像素，而不是图标图像