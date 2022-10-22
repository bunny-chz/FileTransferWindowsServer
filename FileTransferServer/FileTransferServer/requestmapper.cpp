/**
 * @brief 请求URL映射器类
 * @author Bunny GitHub: bunny-chz
 * @date 2022/10/13
 */
#include "requestmapper.h"
#include <QLabel>
#include <QList>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QStandardPaths>
#include <QList>
#include <QPushButton>
#include <QFileDialog>
#include <QTime>

RequestMapper::RequestMapper(QObject* parent)
    : HttpRequestHandler(parent) {

}

void RequestMapper::service(HttpRequest& request, HttpResponse& response) {
    //获取客户端或者网页端请求的URL
    QByteArray path = request.getPath();
    qDebug("RequestMapper: path ---> %s",path.data());
    //连接测试
    if (path=="/testLink") {
        serviceTestLink(response);
    }
    //GET获取主页
    else if (path=="/") {
        mainPageHTML(response);
    }
    //GET获取发送文本页面，textCtoS 即是 text from Client to Server的缩写
    else if (path=="/textCtoSPage") {
        textCToSHTML(response);
    }
    //POST文本到服务器， textCtoS 即是 text from Client to Server的缩写
    else if (path=="/textCtoS") {
        postString.service(request, response);
    }
    //GET获取发送文件页面，fileCtoS 即是 file from Client to Server的缩写
    else if (path=="/fileCtoSPage") {
        fileCToSHTML(response);
    }
    //POST文件到服务器，fileCtoS 即是 file from Client to Server的缩写
    else if (path=="/fileCtoS") {
        fileUploadController.service(request, response);
    }
    //手机客户端GET服务器发送的文本信息， textStoC 即是 text from Server to Client
    else if (path=="/textStoC") {
        getTextFromServer(false,response);
    }
    //网页端GET服务器发送的文本信息，textStoC 即是 text from Server to Client，Web是指网页端特定的URL
    else if (path=="/textStoCWeb") {
        getTextFromServer(true,response);
    }
    //网页端GET服务器发送的文件，fileStoC 即是 file from Server to Client，Web是指网页端特定的URL
    else if (path=="/fileStoCWeb") {
        getFileFromServer(true,response);
    }
    //手机客户端GET服务器发送的文件，fileStoC 即是 file from Server to Client
    else if (path=="/fileStoC") {
        getFileFromServer(false,response);
    }
    //    else if (path=="/files/upload") { //多文件上传

    //    }
    //404 NotFound
    else {
        response.setStatus(404,"Not found");
        responseNotFound(response);
    }
    qDebug("RequestMapper: finished request");
}
void RequestMapper::mainPageHTML(HttpResponse& response){
    response.setHeader("Content-Type", "text/html; charset=UTF-8");
    response.write("<!DOCTYPE html>");
    response.write("<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"><title>文件快传网页端首页</title></head>");
    response.write("<body><h1>文件快传网页端首页</h1>");
    response.write("<ul><h2>上传文件到服务器</h2>");
    response.write("<li><a href=\"./fileCtoSPage\">上传文件到服务器</a></li>");
    response.write("<h2>从服务器下载文件</h2>");
    response.write("<li><a href=\"./fileStoCWeb\">从服务器下载文件</a></li>");
    response.write("<h2>发送文本给服务器</h2>");
    response.write("<li><a href=\"./textCtoSPage\">发送文本给服务器</a></li>");
    response.write("<h2>接收服务器的文本</h2>");
    response.write("<li><a href=\"./textStoCWeb\">接收服务器的文本</a></li></ul>");
    response.write("</body></html>",true);
}
void RequestMapper::serviceTestLink(HttpResponse& response){
    response.setHeader("Content-Type", "text/html; charset=UTF-8");
    response.write("<!DOCTYPE html>");
    response.write("<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">"
                   "<title>连接测试</title></head>");
    response.write("<h1>连接测试，收到此信息即是连接成功，状态正常！</h1>");
    response.write("</body></html>",true);
}
void RequestMapper::textCToSHTML(HttpResponse& response){
    response.setHeader("Content-Type", "text/html; charset=UTF-8");
    response.write("<!DOCTYPE html>");
    response.write("<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"><title>发送文本</title></head>");
    response.write("<body><h1>发送文本</h1>");
    response.write("<form action=\"./textCtoS\" method=\"POST\">");
    response.write("<h2>输入文本</h2>");
    response.write("<input  type = \"text\" name=\"text\"/>");
    response.write("<input type=\"submit\" value=\"发送\">");
    response.write("</form>");
    response.write("</body></html>",true);
}
void RequestMapper::fileCToSHTML(HttpResponse& response){
    response.setHeader("Content-Type", "text/html; charset=UTF-8");
    response.write("<!DOCTYPE html>");
    response.write("<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">"
                   "<title>上传文件</title></head>");
    response.write("<body><h1>上传文件</h1>");
    response.write("<form method=\"POST\" enctype=\"multipart/form-data\" action=\"./fileCtoS\">");
    response.write("<input type=\"hidden\" name=\"upload\" value=\"uploadFile\">");
    response.write("<h2>选择文件</h2>");
    response.write("<input type=\"file\" name=\"file\"><br>");
    response.write("<input type=\"submit\" value=\"上传文件\">");
    response.write("</form>");
    response.write("</body></html>",true);
}
void RequestMapper::responseNotFound(HttpResponse& response){
    response.setHeader("Content-Type", "text/html; charset=UTF-8");
    response.write("<!DOCTYPE html>");
    response.write("<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">"
                   "<title>404 Not Found</title></head>");
    response.write("<h1>404 Not Found</h1>");
    response.write("<h2>该站点不存在！</h2>");
    response.write("</body></html>",true);
}
void RequestMapper::responseNoSetFile(HttpResponse& response){
    response.setHeader("Content-Type", "text/html; charset=UTF-8");
    response.write("<!DOCTYPE html>");
    response.write("<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">"
                   "<title>服务器提示</title></head>");
    response.write("<h1>服务器没有设置文件分享</h1>");
    response.write("</body></html>",true);
}
void RequestMapper::responseNotSetText(HttpResponse& response){
    response.setHeader("Content-Type", "text/html; charset=UTF-8");
    response.write("<!DOCTYPE html>");
    response.write("<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">"
                   "<title>服务器提示</title></head>");
    response.write("<h1>服务器没有发送文本信息</h1>");
    response.write("</body></html>",true);
}
void RequestMapper::getTextFromServer(bool isWeb,HttpResponse& response){
    //QtWebApp每个Handler内不允许开其它内存，和UI无法匹配，采取读文件的形式接收手机发来的信息
    QString dirName = "C:/FileTransfer Files/AppConfig/";
    QString str = "";
    QDir dir(dirName);
    if(!dir.exists())
    {
        dir.mkdir(dirName);
        qDebug() << "dir.mkdir ---> " <<"创建成功";
    }
    QFile *file = new QFile;
    QDir::setCurrent(dirName);
    //设置打开的路径
    file->setFileName("TextStoC.txt");
    //只读
    bool openOK = file->open(QIODevice::ReadOnly);

    if(openOK)
    {
        //问件指针关联文本流
        QTextStream stream(file);
        //UTF-8
        stream.setCodec("UTF-8");
        str = stream.readAll();

        //关闭文件
        file->close();
        delete file;
        if(str.isEmpty()){
            responseNotSetText(response);
        }else{
            if(isWeb){
                QStringList strList = str.split("#-#-#");
                QString strTime = strList[0];
                QString strText = strList[1];

                response.setHeader("Content-Type", "text/html; charset=UTF-8");
                response.write("<!DOCTYPE html>");
                response.write("<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">"
                               "<title>服务器发送的文本</title></head>");
                response.write("<h1>服务器发送的文本</h1>");
                response.write("<h2>" + strTime.toUtf8() + "</h2>");
                response.write("<h3>" + strText.toUtf8() + "</h3>");
                response.write("</body></html>",true);
            }else {
                response.setHeader("Content-Type", "text/plain; charset=UTF-8");
                response.write(str.toUtf8(),true);
            }
        }
    }else{
        responseNotSetText(response);
    }
}
void RequestMapper::getFileFromServer(bool isWeb, HttpResponse& response){
    //QtWebApp每个Handler内不允许开其它内存，和UI无法匹配，采取读文件的形式接收手机发来的信息
    QString dirName = "C:/FileTransfer Files/AppConfig/";
    QDir dir(dirName);
    if(!dir.exists())
    {
        dir.mkdir(dirName);
        qDebug() << "dir.mkdir ---> " <<"创建成功";
    }
    QFile *file = new QFile;
    QDir::setCurrent(dirName);
    //设置打开的路径
    file->setFileName("FileStoC.txt");
    QString str = "";
    QString fileMimeType = "text/plain";
    //只读
    bool openOK = file->open(QIODevice::ReadOnly);

    if(openOK)
    {
        //问件指针关联文本流
        QTextStream stream(file);
        //UTF-8
        stream.setCodec("UTF-8");
        str = stream.readAll();
        //关闭文件
        file->close();
        delete file;
    }
    if(str.isEmpty()){
        responseNoSetFile(response);
    }else{
        qDebug() << "str ---> " << str;
        QStringList strListTxt =  str.split("#-#-#");
        QString filePath = strListTxt[0];
        qDebug() << "filePath ---> " << filePath;
        fileMimeType = strListTxt[1];
        qDebug() << "fileMimeType ---> " << fileMimeType;
        QStringList strList =  filePath.split("/");
        QString fileName = strList[strList.length() - 1];
        qDebug() << "fileName ---> " << fileName;
        //ByteDate 为字节流
        QFile File(filePath);
        File.open(QIODevice::ReadOnly);
        QByteArray fileByteArray = File.readAll();
        if(isWeb){
            response.setHeader("Content-Type", fileMimeType.toUtf8());
            response.write(fileByteArray,true);
        }
        else{
            response.setHeader("Content-Type", fileName.toUtf8());
            response.write(fileByteArray,true);
        }
    }
}
