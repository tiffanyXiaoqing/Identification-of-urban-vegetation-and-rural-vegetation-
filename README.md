# 基于高分辨率遥感影像的城市植被和乡村植被的判别 
## 简介 Introduction
该算法是基于城市植被和乡村植被的周围环境特征进行区分。简要来说，乡村植被区域往往是一个开放的绿色区域，城市植被区域则是被房屋，道路所环绕。当然，我们还必须考虑像素混淆等一些细节问题。更加详细的设计原理可以参考`城市和乡村绿地的判别原理.pdf` 这个文档。<br />
根据这个基本原理，我将这个项目的实现分为以下几个步骤：<br />
**1.影像的预处理** <br />
**2.城市密度模型建立**<br />
**3.基于FloodFill算法的分类划分**<br />
**4.代码的优化：回溯算法实现FloodFill算法，多线程并行运行**<br />

The algorithm is based on the surrounding environmental characteristics of urban vegetation and rural vegetation. In short, the rural vegetation area is often an open green area, while the urban vegetation area is surrounded by houses and roads. Of course, we must also consider some details such as pixel obfuscation. For more detailed design principles, please refer to the document "Principle of Urban and Rural Greenbelt Discrimination.pdf". <br />
According to this basic principle, I divide the implementation of this project into the following steps:<br />
**1.Image preprocessing** <br />
**2.Building urban density model** <br />
**3.Classfying urban vegetation and rural vegetation based on FloodFill algorithm**<br />
**4.Code optimization：comabining FloodFill algorithm with backtracking and Multithreading**<br />

## 影像的预处理 Image preprocessing
原始图像的每个像素点都有255个信息位，直接使用会加大分类难度。为了有效提取城市植被和乡村植被的信息，将图像信息划分为植被、房屋建筑、裸地（包括道路）， 阴影（包括无法识别的区域）四大类。<br />
![原始图片](F:\UG\results\原始图片.jpg) ![预处理后](F:\UG\results\预处理后.png) <br />


## More information about how to realize it
If you are familar with English, you can refer to the `Mathematical principle and explaination file`. For Chinese, you can refer to `城市和乡村绿地的判别原理` file.
## Ahaaa
Please throw a start if you feel useful.
