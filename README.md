# 飞机大战

电子科技大学信软学院，大一上，程序设计与算法基础课程，PBLF 小组项目。

--- 

12.11 upd:

First demo released!

实现了最最基本的游戏功能，贴图及其生草，但尚未实现游戏选项菜单、暂停/恢复/重玩、最高分存读档、难度调整等一系列功能。

组内组外人员想要 playtest 的可以参考如下环境：

- IDE 使用 Visual Studio 2022 **及以下**版本。

- 使用 EasyX 的自动安装程序为 Visual Studio 部署 EasyX。（详见 EasyX 官网的[这个教程](https://easyx.cn/setup)）

- 手动将 `main.cpp` 中 $89$ 行处的 `render_load_gameplay_textures()` 各个参数修改为贴图在你的计算机上的**绝对**路径。

- 将所有源文件和头文件都放在 Visual Studio 的同一个项目目录下，并向该项目添加这些文件。（详见[这个教程](https://zhuanlan.zhihu.com/p/476486522)）

- 调出 Visual Studio 的「高级保存选项」菜单（详见[这个教程](https://jishuzhan.net/article/1923749980226048002)），将编码设置为「简体中文(GB2312) - 代码页 936」，并将所有源文件与头文件都按这个编码格式保存一遍。

- ctrl + F5 编译运行 `main.cpp` 即可。
