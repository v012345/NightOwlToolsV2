## 目录结构
+ assets_ext
    > 打包的时候, 把 assets_ext 里的文件, 全部复制到 assets 里
+ assets
    > 就是 Closers.cocos 里的客户端 Resources 中的文件, 只不过进行了一些预处理. 比如, 文本文件的加密, 图片转成 .pkm 文件
+ libs
    > 打包的时候, 把 Closers.android\\.libs 中的库文件复制到 libs 里, 目前这几个文件的作用还不是很了解, 之后补充
    + libclosers.so
    + android-async-http-1.4.9.jar
    + android-support-compat.jar
    + aonesdklib.jar
    + bugly_agent.jar
    + bugly_crash_release.jar
    + com.android.vending.expansion.zipfile.jar
    + cputils.jar
    + httpclient-4.4.1.1.jar
    + SSWrapper.jar
