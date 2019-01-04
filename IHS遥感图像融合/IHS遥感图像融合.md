## IHS遥感图像融合

常用的颜色模型处了RGB三原色模型之外，另外一种广泛采用的颜色模型是亮度Intensity、色调Hue、饱和度Saturation（IHS颜色模型）

![](http://ww1.sinaimg.cn/large/0070vHShly1fxtybmctudj30sg0d0jxd.jpg)

亮度表示光谱的整体亮度大小，对应于图像的空间信息属性；

色调描述纯色的属性，决定于光谱的主波长；

饱和度表征光谱的主波长在强度中的比例，色调和饱和度代表图像的光谱分辨率。

IHS变换图像融合就是建立在IHS空间模型的基础上，其基本思想就是在IHS空间中，将低空间分辨率的多光谱图像的亮度分量用高空间分辨率的灰度图象替换。

![](http://ww1.sinaimg.cn/large/0070vHShly1fxtycx6ynlj30hv08ngms.jpg)

算法描述：

1) 将多光谱影像重采样到与全色影像具有相同的分辨率；

(2) 将多光谱图像的Ｒ、Ｇ、Ｂ三个波段转换到IHS空间，得到Ｉ、Ｈ、Ｓ三个分量；

(3) 以Ｉ分量为参考，对全色影像进行直方图匹配

(4) 用全色影像替代Ｉ分量，然后同Ｈ、Ｓ分量一起逆变换到RGB空间，从而得到融合图像。

RGB与IHS变换的基本公式：

![](http://ww1.sinaimg.cn/large/0070vHShly1fxtym2vb50j30i707777d.jpg)

多光谱图像

[]: https://www.jianguoyun.com/p/DbgnnwwQgtCdBxixvIoB

全色图像：

[https://www.jianguoyun.com/p/DYdVTi0QgtCdBxiwvIoB]: 

处理结果：

[]: https://www.jianguoyun.com/p/DY7Z8kUQgtCdBxi1vIoB

