# NPU_RTSP_DEMO

1. 安装依赖

   包括但不限于opencv 4.5、live555、freeglut3、glog、显卡硬解库、目标检测库.

   上述opencv4.5可自行编译安装，live555、freeglut3、glog麒麟源中有提供。

   ```shell
   sudo cp libs/*.so /usr/lib/aarch64-linux-gnu/
   ```

2. 初始化NPU驱动

   ```shell
   cd npu_driver
   make 
   sudo mkdir /lib/firmware/cambricon
   sudo cp -r ./lib/firmware/* /lib/firmware/cambricon
   cd ..
   ```

3. 加载NPU驱动

   每次重启后，需要重新加载NPU驱动

   ```shell
   cd npu_driver
   sudo ./load
   cd ..
   ```

   

4. 编译程序

   ```shell
   mkdir build
   cd build
   cmake ..
   make -j
   ```

   

5. 运行

   ```shell
   # 全功能正常运行
   gltest rtsp://192.168.50.203/live/av0
   # 功能关闭选项
   # --nouse_npu: 加入此选项后，将不会进行目标检测的工作，npu不会工作。卸载npu的驱动同样可以达到相同的效果
   # --nouse_rtsp: 加入此选项后，将不使用显卡进行解码，使用测试图像(./video/0750.bmp)进行目标检测
   # --nouse_rtsp_for_npu: 加入此选项后，显卡解码后的图像不作为目标检测的来源，即不进行显存复制到内存的操作，使用测试图像(./video/0750.bmp)进行目标检测。当nouse_rtsp启用时，该选项不生效
   # --nodisplay_rtsp: 加入此选项后，显卡解码后的图像不会进行纹理贴图
   # --nodisplay_detect: 加入此选项后，目标检测后的结果图像不会进行纹理贴图
   ```

   