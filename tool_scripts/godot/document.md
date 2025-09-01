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


1,いっか，一家
2，ありがたい,有難い
3,むきあう、向き合う
4。つくづく。
5。やしなう。養う
6。やくめ。役目
7、あまえる、甘える
8、せけんしらず、世間知らず
9、しつけ、
10、いいつける、言いつける
11、じりつ、自立
12、いいだす、言い出す
13、いし、意思
14、そんちょう、尊重
15、せっとく、説得
16、なっとく、納得
17、さからう、逆らう
18、はんせい、反省
19、ほうっておく、放っておく
20、かいご、介護
21、ホームヘルパー
22、せだい、世代
23、つぐ、継ぐ
24、にんしん、妊娠
25、しゅっさん、出産
26、うむ、生む
27、ゆうじん、友人
28、なか、仲
29、えん、縁
30、ニックネーム
31、たいはん、大半
32、
