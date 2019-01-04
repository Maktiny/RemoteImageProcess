这是一个利用GDAL库对图像处理的例子，旨在认识，理解GDAL库。GDAL是一个能够处理各种栅格地理数据格式的库。

连接：<https://www.gdal.org/>

项目工具：Visual C++ 2010 express版本

工具连接：：<https://ouceducn-my.sharepoint.com/:u:/g/personal/gaofeng_ouc_edu_cn/EeLSAzv5H5tIgXPm3bADAJcBklbVYqMXw-G6G39t4ZGmEg?e=PZSmoS>

GDAL库，GDAL库的使用，需要三个文件，.h文件，.dill文件，.lib文件

所需文件包连接：<https://ouceducn-my.sharepoint.com/:u:/g/personal/gaofeng_ouc_edu_cn/ES8fvuffgxdOs7lSwpTzocYBd4gJFrDde6vNIXFhQyMWeQ?e=7LbJgt>

注意(环境配置）：在建立项目的时候，必须把gdal包和gdal_i.lib文件复制到项目的当前目录下，将gdal18.dll文件复制到Debug或者Release文件目录下即可。

程序目的：将一张普通.jpg格式的图片读入，然后输出成.tif格式。