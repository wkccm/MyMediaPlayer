# MyMeidaPlayer
C++实现多画面播放器

# 踩坑经验
- read access voilation at 0x0，很可能是变量没有初始化
- 一定要注意静态库、动态库的配置
- 加上声音功能，结果发现关闭文件或文件播放结束后程序崩溃