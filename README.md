# FDiskThief
U盘小偷 MFC版

# 起源
这是我受小破站某个视频的启发，写的一个程序，目的就是趁老师不注意自动偷取老师U盘的文件。  
正好以这个程序为契机，学习一下MFC；所以我是刚刚学习MFC，有写的不好的地方请指出！

# 功能
- 复制u盘文件到指定目录
- 检测到u盘的根目录下有3141314.txt时，他会认为是我的u盘，就会把指定目录下的文件剪切到u盘里
- 开机自启
- 托盘图标，右键显隐窗口，左键彩蛋
- 防止发现，启动程序自动隐藏窗口
- 恶搞老师U盘，往里面放一些奇怪的文件，详见[恶搞如何自定义](https://github.com/Gingmzmzx/FDiskThief#%E6%81%B6%E6%90%9E%E5%A6%82%E4%BD%95%E8%87%AA%E5%AE%9A%E4%B9%89)
- 会有一些提示性的气泡通知（该功能存在问题，见：[存在问题](https://github.com/Gingmzmzx/FDiskThief#%E5%AD%98%E5%9C%A8%E9%97%AE%E9%A2%98)中的第一条

# 使用方法：
双击打开exe就可以了。  
右键托盘的图标打开主窗口。  
如果想结束程序请从任务管理器中结束。

# 解释
### 应用名
关于为啥程序要叫`鸿合U盘大师`？  
那是因为我们班电脑牌子是鸿合的qwq
### 判定详细解释
如果U盘的根目录下没有`3141314.txt`这个文件，他就会认为这是老师的U盘，就会将此U盘中的文件复制到指定的本地文件夹中  
相反，如果有这个文件，他就会认为是我的U盘，就会将本地目录中的文件剪切到我的U盘中
### 为什么不写一些与服务器通信的东西
那是因为我们班里的电脑不输入教职工密码无法上网。
### 恶搞如何自定义
`FDiskThiefDlg.cpp`的第**21、22**行，`filesTitle`为文件标题（带后缀）`filesContent`为文件内容，注意与上面的`filesTitle`一一对应！
**请注意内容必须是使用_T("")将字符转换为宽字节的Unicode编码！**

# 存在问题
- 实锤气泡通知（右下角）目前不适用于Windows11。不过既然我们班电脑是win10，就不管了，有需求的可以帮忙改了交个`Pull requests`

# 开放源代码许可
本仓库使用`Apache License`许可证，您必须在遵守`Apache License`的同时在用户使用的明显处（例如主界面中）标注原作者信息和原仓库，例如`原作者Gingmzmzx 仓库Gingmzmzx/FDiskThief`
