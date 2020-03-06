1. 本例程用于演示接口的使用方法，基于QT5.5.0开发，使用之前请先安装QT5.5.0或以上版本。

2. 本例程使用MV Viewer版本为2.1.2_Build20180323，使用之前请先安装2.1.2_Build20180323版本MV Viewer

3. 本例程不包含修改相机的IP，使用之前请先用MV Viewer修改相机的IP到网卡所在的网段。

4. 目录结构如下

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


5.接口使用时需要注意顺序。

（1）先发现相机（CameraCheck），再连接相机（CameraOpen），连接相机以后才可以修改曝光和增益、改变触发方式。
（2）开始采集（CameraStart）以后才可以显示图像，否则缓存队列为空。

6.注意事项。

（1）默认连接的是发现列表中的第一个相机。
（2）ShowImage接口没有获取图像功能（图像在回调函数中获取），只做显示用，需要给它传入一个指针pRgbFrameBuf，该指针指向要显示的图像数据。
（3) 目前工程只支持Mono8和RGB24图像数据转化成QImage，
     Mono8不需要转码可以直接生成QImage,其余格式通过ImageConvert库先转换成RGB24再生成QImage
 (4) Sample中通过QLabel显示图像，QLabel只支持QPixmap。所以先将QImage转换为QPixmap，转换会造成图像显示延迟。
 (5) 如何生成QImage请参照dahua.cpp中的ShowImage函数， 非mono8格式转码为RGB24请参照dahua.cpp中的DisplayThreadProc函数

                                                                             - END -