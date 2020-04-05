# 基于高分辨率遥感影像的城市植被和乡村植被的判别 
## 1.简介 Introduction
该算法是基于城市植被和乡村植被的周围环境特征进行区分。简要来说，乡村植被区域往往是一个开放的绿色区域，城市植被区域则是被房屋，道路所环绕。当然，我们还必须考虑像素混淆等一些细节问题。更加详细的设计原理可以参考`城市和乡村绿地的判别原理.pdf` 这个文档。<br />
根据这个基本原理，我将这个项目的实现分为以下几个步骤：<br />
**1.影像的预处理** <br />
**2.基于圆形窗口的城市密度模型建立**<br />
**3.基于FloodFill算法的分类划分**<br />
**4.代码的优化：回溯算法实现FloodFill算法，多线程并行运行**<br />

The algorithm is based on the surrounding environmental characteristics of urban vegetation and rural vegetation. In short, the rural vegetation area is often an open green area, while the urban vegetation area is surrounded by houses and roads. Of course, we must also consider some details such as pixel obfuscation. For more detailed design principles, please refer to the document "Principle of Urban and Rural Greenbelt Discrimination.pdf". <br />
According to this basic principle, I divide the implementation of this project into the following steps:<br />
**1.Image preprocessing** <br />
**2.Building urban density model** <br />
**3.Classfying urban vegetation and rural vegetation based on FloodFill algorithm**<br />
**4.Code optimization：comabining FloodFill algorithm with backtracking and Multithreading**<br />

## 2.影像的预处理 Image preprocessing
原始图像的每个像素点都有255个信息位，直接使用会加大分类难度。为了有效提取城市植被和乡村植被的信息，利用ERDAS软件的监督学习方法将图像信息划分为植被、房屋建筑、裸地（包括道路）， 阴影（包括无法识别的区域）四大类。<br />
<img src="https://github.com/tiffanyXiaoqing/Identification-of-urban-vegetation-and-rural-vegetation-/blob/master/images/%E5%8E%9F%E5%A7%8B%E5%9B%BE%E7%89%87.jpg" width = "300" height = "200" alt="原始图片" align=center />  <img src="https://github.com/tiffanyXiaoqing/Identification-of-urban-vegetation-and-rural-vegetation-/blob/master/images/%E9%A2%84%E5%A4%84%E7%90%86%E5%90%8E.PNG" width = "300" height = "200" alt="预处理后图片" align=center />
<br />

## 3.城市密度模型建立 Building urban density model
在房屋建筑密集的地方，城市的像素点非常集中，城市密度模型中的像素值越高。相反，在植被出现多的地方，城市的像素点稀疏，城市密度模型中的像素值越低。利用这个基本原理，利用城市像素信息建立城市密度模型。<br />
城市的像素点被用具有一定半径的圆圈来表示，圆圈覆盖面积之内的像素值为 1。重叠圆圈的像素值进行叠加，得到整幅图像进行像素值，以此计算来获得城市密度模型。这个过程如下图所示。 <br />
圆圈如何用算法来实现？实现起来也不难，在二维数组中，设半径为r，行为x，高为y，利用关系式<a href="https://www.codecogs.com/eqnedit.php?latex=\left&space;(&space;r-x&space;\right&space;)^{2}&plus;\left&space;(&space;r-y&space;\right&space;)^{2}&space;=&space;r^{2}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\left&space;(&space;r-x&space;\right&space;)^{2}&plus;\left&space;(&space;r-y&space;\right&space;)^{2}&space;=&space;r^{2}" title="\left ( r-x \right )^{2}+\left ( r-y \right )^{2} = r^{2}" /></a>即可得到一个包含圆形窗口的二维数组。

## More information about how to realize it
If you are familar with English, you can refer to the `Mathematical principle and explaination file`. For Chinese, you can refer to `城市和乡村绿地的判别原理` file.
## Ahaaa
Please throw a start if you feel useful.
