1. 下载源码
git clone git@github.com:godotengine/godot.git
git checkout 4.4

2. Windows
安装 Visual Studio 2022，包含 Desktop development with C++ 工作负载
安装 Python 3.x（SCons 需要）
安装 SCons：
pip install scons
如果需要导出 Android/iOS/HTML5，还需要安装对应 SDK/NDK/emscripten

3. 编译 Godot

Godot 可以直接用 scons 命令编译：

# 编译默认平台（Windows/Linux/macOS 桌面）
scons platform=windows   # Windows
scons platform=linuxbsd  # Linux
scons platform=osx       # macOS


常用参数：

tools=yes ：带编辑器

target=editor ：开发版，带调试信息

target=release ：正式版

use_lto=yes ：启用 LTO 优化

例如 Windows 编译带编辑器：

scons platform=windows tools=yes target=editor -j8


-j8 表示使用 8 个 CPU 核心并行编译，加快速度。

4. tiled
正常安装, tmx 资源应该在 godot 的工程下, 不然导不出来