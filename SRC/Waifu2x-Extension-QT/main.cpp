﻿/*
    Copyright (C) 2020  Aaron Feng

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    My Github homepage: https://github.com/AaronFeng753
*/

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);//高分辨率屏幕支持
    QApplication a(argc,argv);
    a.setQuitOnLastWindowClosed(false);//隐藏无窗口时保持运行
    MainWindow *w = new MainWindow;
    w->show();
    return a.exec();
}

/*
v2.56.52:
- 新特性:可启用[替换原文件]以直接用处理后的文件替换原文件.(附加设置,可选,默认禁用)
- 性能优化.
- 更新FFmpeg&FFprobe,ImageMagick.
- 修复bug.
--------------------------------------------------------------------------
To do:
- 移植到Linux.
---------------------------------------------------------------------------
Integrated component:
- gifsicle version 1.92(release)
- SoX 14.4.2-win32(release)
- waifu2x-caffe 1.2.0.4
- Waifu2x-converter e7de04d
- waifu2x-ncnn-vulkan 20200818
- SRMD-ncnn-Vulkan 20200818
- realsr-ncnn-vulkan 20200818
- ImageMagick 7.0.10-30
- Anime4KCPP v2.3.0
- FFmpeg&FFprobe 2020-09-20-git-ef29e5bf42
- NirCmd v2.86
---
Icons made by : Freepik (https://www.flaticon.com/authors/freepik) From Flaticon : https://www.flaticon.com/
*/
