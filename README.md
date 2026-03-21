# 飞机大战

电子科技大学信软学院，大一上，程序设计与算法基础课程，PBLF 小组项目。

--- 

12.13 upd:

正式版 v1.0 发布！

将 [Releases](https://github.com/c1ampy/galaxy/releases) 中的 `.zip` 压缩文件下载后解压，直接运行 `galaxy.exe` 即可游玩。

后续可推出动态难度系统、道具系统等功能。

---

组内组外人员想要 playtest 的可以参考如下环境配置步骤：

- 使用 Visual Studio 2022 **及以下**版本。

- 使用 EasyX 的自动安装程序为 Visual Studio 部署 EasyX。（详见 EasyX 官网的[这个教程](https://easyx.cn/setup)）

- 将 `source` 文件夹下的所有文件（包括 `assets` 文件夹和其中所有贴图，但不含 `source` 文件夹本身）都放在用 Visual Studio 创建的同一个项目目录下，并向该项目添加所有头文件和源文件。（详见[这个教程](https://zhuanlan.zhihu.com/p/476486522)）

- 调出 Visual Studio 的「高级保存选项」菜单（详见[这个教程](https://jishuzhan.net/article/1923749980226048002)），将编码设置为「简体中文(GB2312) - 代码页 936」，并将所有源文件与头文件都按这个编码格式保存一遍。

- Ctrl + F5 编译运行 `main.cpp` 即可。

---

**CMake 构建**

构建需求：
- Windows
- CMake 3.20+
- 编译器（MinGW 或 MSVC）
- EasyX
  - 使用 MinGW 时，需要自行下载 EasyX 的 MinGW 版本，并放到项目根目录的 `easyx` 文件夹下（包含 `include` 和 `lib64/lib32`）。
  - 使用 MSVC 时，需要 EasyX 的 `.lib` 版本（`EasyXw.lib` 或 `EasyX.lib`），同样放到 `easyx` 目录下。

MinGW 快速构建指令（推荐）：
```powershell
cmake -S . -B build-mingw -G "MinGW Makefiles" -DCMAKE_C_COMPILER=C:\mingw64\bin\gcc.exe -DCMAKE_CXX_COMPILER=C:\mingw64\bin\g++.exe
cmake --build build-mingw
```
