1. ������������ʾ�ӿڵ�ʹ�÷���������QT5.5.0������ʹ��֮ǰ���Ȱ�װQT5.5.0�����ϰ汾��

2. ������ʹ��MV Viewer�汾Ϊ2.1.2_Build20180323��ʹ��֮ǰ���Ȱ�װ2.1.2_Build20180323�汾MV Viewer

3. �����̲������޸������IP��ʹ��֮ǰ������MV Viewer�޸������IP���������ڵ����Ρ�

4. Ŀ¼�ṹ����

   /ShowImage
     �� Bin      �� ImageConvert.dll����ͼ���ʽת����VideoRender.dll������ʾͼ��
     ��
     �� Include  �� ͷ�ļ�
     ��   �� Media
     ��   �� GenICam     : SDK���ͷ�ļ������������MV Viewer�汾���Դ���ͷ�ļ���(MV Viewer��װĿ¼��\MV Viewer\Development\Include��)
     ��   �� Infra       ��SDK���ͷ�ļ������������MV Viewer�汾���Դ���ͷ�ļ���(MV Viewer��װĿ¼��\MV Viewer\Development\Include��)
     ��   �� Memory      ��SDK���ͷ�ļ������������MV Viewer�汾���Դ���ͷ�ļ���(MV Viewer��װĿ¼��\MV Viewer\Development\Include��)
     ��
     �� Depends  ��  ����ʱ������
     ��   �� ImageConvert.lib
     ��   �� VideoRender.lib  
     ��   �� MVSDKmd.lib     �� SDK�⡣���������MV Viewer�汾���Դ���ͷ�ļ���(MV Viewer��װĿ¼��\MV Viewer\Development\Lib��)
     ��
     �� Src
     ��   �� Dahua.h         �� �ͻ���Ҫ�������ӿ���
     ��   �� dahua.cpp       �� �ͻ���Ҫ�������ӿ���
     ��   �� MessageQue.h    �� ֡����������ݽṹ�����裡��
     ��   �� main.cpp        �� <����>Ϊ����ʾ�ӿڵ�ʹ�÷������ṩ�ģ�ʵ��ʹ�ýӿ�ʱ����Ҫ����
     ��   �� form.h          �� <����>Ϊ����ʾ�ӿڵ�ʹ�÷������ṩ�ģ�ʵ��ʹ�ýӿ�ʱ����Ҫ����
     ��   �� form.cpp        �� <����>Ϊ����ʾ�ӿڵ�ʹ�÷������ṩ�ģ�ʵ��ʹ�ýӿ�ʱ����Ҫ����
     ��   �� form.ui         �� <����>Ϊ����ʾ�ӿڵ�ʹ�÷������ṩ�ģ�ʵ��ʹ�ýӿ�ʱ����Ҫ����
     ��   ��cammerwidget.ui  �� <����>Ϊ����ʾ�ӿڵ�ʹ�÷������ṩ�ģ�ʵ��ʹ�ýӿ�ʱ����Ҫ����
     ��
     �� ShowQImage.pro


5.�ӿ�ʹ��ʱ��Ҫע��˳��

��1���ȷ��������CameraCheck���������������CameraOpen������������Ժ�ſ����޸��ع�����桢�ı䴥����ʽ��
��2����ʼ�ɼ���CameraStart���Ժ�ſ�����ʾͼ�񣬷��򻺴����Ϊ�ա�

6.ע�����

��1��Ĭ�����ӵ��Ƿ����б��еĵ�һ�������
��2��ShowImage�ӿ�û�л�ȡͼ���ܣ�ͼ���ڻص������л�ȡ����ֻ����ʾ�ã���Ҫ��������һ��ָ��pRgbFrameBuf����ָ��ָ��Ҫ��ʾ��ͼ�����ݡ�
��3) Ŀǰ����ֻ֧��Mono8��RGB24ͼ������ת����QImage��
     Mono8����Ҫת�����ֱ������QImage,�����ʽͨ��ImageConvert����ת����RGB24������QImage
 (4) Sample��ͨ��QLabel��ʾͼ��QLabelֻ֧��QPixmap�������Ƚ�QImageת��ΪQPixmap��ת�������ͼ����ʾ�ӳ١�
 (5) �������QImage�����dahua.cpp�е�ShowImage������ ��mono8��ʽת��ΪRGB24�����dahua.cpp�е�DisplayThreadProc����

                                                                             - END -