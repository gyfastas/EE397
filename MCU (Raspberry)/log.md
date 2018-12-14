1已经完成蓝牙的关闭，cmdline.txt，config.txt文件的修改，导入了pi3-miniuart-bt-overlay.dtb

2/lib/systemd/system/hciuart.server的修改出现问题

3关于串口调试工具，cutecom,mincom,pyserial均以安装完成，推荐使用cutecom

4关于如何打开串口，
https://blog.csdn.net/qq_30968657/article/details/52044876
http://shumeipai.nxez.com/2017/04/29/raspberry-pi-using-the-serial-debugging-tool-xgcom.html
http://shumeipai.nxez.com/2016/08/08/solution-raspberry-pi3-serial-uart-use-issues.html
有用https://blog.csdn.net/wren1997/article/details/79273477
minicom操作https://blog.csdn.net/bird67/article/details/2127235

现在完成串口通信;
ttyAMA0,115200,cutecom

2018/11/25 23:29:05 
关于uart通信，使用pyseria库，在之前的工作中已经安装，但是未检测能否使用，如果不能，参照https://www.cnblogs.com/cj2014/p/3818675.html

已写实验用的python文件uart-1，和图像处理代码ima-1,

python图像处理参考网页
cv2:
https://blog.csdn.net/Lxingmo/article/details/73825081
https://blog.csdn.net/what_lei/article/details/49159655
uart:
https://www.cnblogs.com/cj2014/p/3818675.html

下一步目标：
识别黑线，想办法将偏转角得出，要考虑提前量
