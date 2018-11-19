# EE397
Our EE397 project

Control UI on Computer

---------

## Environment

Python3.6

Pyqt5



## LOG

### 11.3

重写了UI的结构

self.sock 改用Python自带的Socket模块而不是QTCPSocket



### 11.4

8:45

Qdatastream 是否适用，在本项目中该怎么用？

Qtcpsocket 传输数据的方法: https://blog.csdn.net/dwb1015/article/details/21173625

QTCPSocket 传输数据： 注意encode('utf-8')方法

 10:05

https://www.cnblogs.com/A-FM/p/5691468.html 字符串处理

10:25

 更新了cmd的写命令

https://blog.csdn.net/jia666666/article/details/81534588 QSlider各个模块

10:55

  完成了 Slider 和 Edit 的交互



11:55

连接上了服务器并且打印出了信息，接下来要做的是定协议，实现上位机和小车之间的WIFI通信





### 11.19

  Web [HTML] 可以利用表格显示小车参数

```html
<table border="1">
<tr>
<td>row 1, cell 1</td>
<td>row 1, cell 2</td>
</tr>
<tr>
<td>row 2, cell 1</td>
<td>row 2, cell 2</td>
</tr>
</table>
```

