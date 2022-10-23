# FileTransferWindowsServer
基于C++ QT的文件快传Windows HTTP 服务器端，

用到了QtWebApp，可以与客户端实现数据收发。

------------------------------

这个是自定义文件传输软件系统的一部分---电脑的服务器端，

可以在局域网内基于HTTP协议，实现电脑，安卓，网页浏览器三端数据互通


电脑客户端请看这里（用到了QNetworkAccessManager类进行GET,POST请求）

https://github.com/bunny-chz/FileTransferWindowsClient

手机端服务器端和客户端（用到了NanoHTTPD,Retrofit）

https://github.com/bunny-chz/FileTransferAndroid


**开发环境**

Windows QT 5.14.2

本应用可以实现客户端和服务器端数据交换

用到了QT的QtWebApp

https://github.com/fffaraz/QtWebApp

具体如何导入QtWebApp到项目中请看这篇文章

https://blog.csdn.net/weixin_41001497/article/details/108317463?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522166650854016800186534827%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=166650854016800186534827&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduend~default-1-108317463-null-null.142^v59^pc_rank_34_2,201^v3^control_1&utm_term=qtwebapp&spm=1018.2226.3001.4187

**自定义的服务器接口**

说明：下面的IP和Port为设备所属网络环境的IP和端口，只支持HTTP

textCtoS 即是 text from Client to Server的缩写，其他以此类推

http://IP:Port/ 网页端主页，客户端GET获取到一个网页

http://IP:Port/testLink 客户端GET请求，连接测试

http://IP:Port/textCtoSPage 客户端GET，获取到一个发送文本给服务器的操作网页

http://IP:Port/textCtoS 链接中带参数，客户端POST请求后，会给服务器发送文本信息，键值值为"text=???"

http://IP:Port/textStoC 客户端GET获取到服务器发送的信息

http://IP:Port/textStoCWeb 网页端GET获取到服务器发送的信息

http://IP:Port/fileCtoS 客户端POST请求后，会给服务器发送文件，POST请求内容标记为"file"

http://IP:Port/fileCtoSPage 网页端GET获取到一个发送文件给服务器的操作网页

http://IP:Port/fileStoCWeb 网页端GET服务器发送的文件

http://IP:Port/fileStoC 客户端GET服务器发送的文件


**界面预览**

![image](https://user-images.githubusercontent.com/57706599/197378840-9ed8ab29-19e6-49aa-b74e-8610ccd9e924.png)

