1. 本例程用于演示接口的使用方法，基于QT5.5.0开发，使用之前请先安装QT5.5.0或以上版本。

2. 本例程使用MV Viewer版本为2.1.2_Build20180323，使用之前请先安装2.1.2_Build20180323版本MV Viewer

3. 本例程不包含修改相机的IP，使用之前请先用MV Viewer修改相机的IP到网卡所在的网段。

4. 目录结构如下

	```
	   /ShowImage
		 ├ Bin      ： ImageConvert.dll用于图像格式转换，VideoRender.dll用来显示图像。
		 │
		 ├ Include  ： 头文件
		 │   ├ Media
		 │   ├ GenICam     : SDK库的头文件。请配合所用MV Viewer版本里自带的头文件。(MV Viewer安装目录下\MV Viewer\Development\Include中)
		 │   ├ Infra       ：SDK库的头文件。请配合所用MV Viewer版本里自带的头文件。(MV Viewer安装目录下\MV Viewer\Development\Include中)
		 │   └ Memory      ：SDK库的头文件。请配合所用MV Viewer版本里自带的头文件。(MV Viewer安装目录下\MV Viewer\Development\Include中)
		 │
		 ├ Depends  ：  编译时依赖库
		 │   ├ ImageConvert.lib
		 │   ├ VideoRender.lib  
		 │   └ MVSDKmd.lib     ： SDK库。请配合所用MV Viewer版本里自带的头文件。(MV Viewer安装目录下\MV Viewer\Development\Lib中)
		 │
		 ├ Src
		 │   ├ Dahua.h         ： 客户需要的适配层接口类
		 │   ├ dahua.cpp       ： 客户需要的适配层接口类
		 │   ├ MessageQue.h    ： 帧缓存队列数据结构（必需！）
		 │   ├ main.cpp        ： <例程>为了演示接口的使用方法而提供的，实际使用接口时不需要包含
		 │   ├ form.h          ： <例程>为了演示接口的使用方法而提供的，实际使用接口时不需要包含
		 │   ├ form.cpp        ： <例程>为了演示接口的使用方法而提供的，实际使用接口时不需要包含
		 │   ├ form.ui         ： <例程>为了演示接口的使用方法而提供的，实际使用接口时不需要包含
		 │   └cammerwidget.ui  ： <例程>为了演示接口的使用方法而提供的，实际使用接口时不需要包含
		 │
		 └ ShowQImage.pro

	```
5. 接口使用时需要注意顺序。

	1. 先发现相机（CameraCheck），再连接相机（CameraOpen），连接相机以后才可以修改曝光和增益、改变触发方式。
	2. 开始采集（CameraStart）以后才可以显示图像，否则缓存队列为空。

6. 注意事项。
	1. 默认连接的是发现列表中的第一个相机。
	2. ShowImage接口没有获取图像功能（图像在回调函数中获取），只做显示用，需要给它传入一个指针pRgbFrameBuf，该指针指向要显示的图像数据。
	3. 目前工程只支持Mono8和RGB24图像数据转化成QImage，Mono8不需要转码可以直接生成QImage,其余格式通过ImageConvert库先转换成RGB24再生成QImage
	4. Sample中通过QLabel显示图像，QLabel只支持QPixmap。所以先将QImage转换为QPixmap，转换会造成图像显示延迟。
	5. 如何生成QImage请参照dahua.cpp中的ShowImage函数， 非mono8格式转码为RGB24请参照dahua.cpp中的DisplayThreadProc函数

<center> END </center>

----

[========]

### 2020.03.14 总结一下已经实现的工作：打开相机、显示图像这些功能已经在原始的代码中实现。
- 新添加的功能有：将相机改为硬件除法、并将图像显示在label上，label_2显示图书部分，Label_3显示是否有误。
- 添加了识别部分的代码，已经测试功能正常。
- 新加了LCD显示屏显示数量，但是效果不好，想着改为label显示
- 添加了数据库，不知道能否正确实现增删改查功能，不知道如何查看

### 20200316周：要实现自动化训练部分所需要的脚本代码，能实现一键训练功能。具体的功能如下：
~~1. 手工拍照完成之后，要修改文件的名称，统一文件名称；~~
~~2. 修改完文件名称之后要对所有的图像进行裁剪~~
~~3. 裁剪完成之后要对图像进行标记（手工），标记完成之后要复制N的标记文件，与图像文件名一一对应~~
~~4. 将图像文件和标记文件移动到指定的文件夹中~~
~~5. 训练模型。修改训练配置文件中的参数等~~
~~6. 训练完成后将得到的模型移动到指定的文件夹中~~

- [ ] 1. 新书拍照。在菜单中添加拍照的菜单，实现将实时视频流保存为图片。
- [x] 2. 新书截图。借助之前的画模板的程序（已实现在程序中调用），在新书图片截取图书，并将图书位置等相关信息输出。（已实现）
- [x] 3. 图书标记。调用打标签的程序（已实现），将标签和图书图片进行批量的复制和调整。标签文件批量复制，图片文件进行处理调整亮度实现多张照片的效果。
- [ ] 4. 训练数据文件的产生和移动，训练配置文件的修改
- [ ] 5. 训练模型
- [x] 6. 得到的模型移动到指定位置？或者采用final weight？
 <u>**训练模块完成**</u>