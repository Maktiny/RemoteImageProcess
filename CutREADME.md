#  Remote Image processing

简单抠像

1.目的：

该程序利用GDAL图像处理库，简单的从一张图像中把一个人物图像抠出来，融合到另一张图像中。

两张源图：

superman.jpg

![](http://ww1.sinaimg.cn/large/0075ZKFJly1fwiep3lp2hj30hs0dcn23.jpg)

earth.jpg

![](http://ww1.sinaimg.cn/large/0075ZKFJly1fwieqi5uigj30hs0dcju4.jpg)

程序把superman.jpg中的人物图像抠出来加到earth.jpg中。

实现思路：

由于superman.jpg中人物背景为绿幕，绿幕的三个像素通道值10<r<160; 100<g<220; 10<b<110，所以把两张图像的每个通道值都读入缓存中，如果superman.jpg图像的通道1的r值在10 - 160之间，则为绿幕背景，把相应的earth.jpg图像的通道1的r值赋给superman.jpg图像的通道1的r值，依次对其他两个通道执行类似的操作即可。

结果：在项目文件夹Image中（不知道为啥，处理过的图片上传微博图床失败）