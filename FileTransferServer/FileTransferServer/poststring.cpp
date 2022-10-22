/**
 * @brief 客户端发送信息的处理类
 * @author Bunny GitHub: bunny-chz
 * @date 2022/10/13
 */
#include "poststring.h"
#include <QStandardPaths>
#include <QMessageBox>
#include <QDir>
#include <QLabel>
#include <QTime>

PostString::PostString()
{}

void PostString::service(HttpRequest& request, HttpResponse& response)
{
    QByteArray param = request.getParameter("text");
    qDebug("RequestMapper: param ---> %s",param.data());
    QString str(param);
    str.replace("\"","");
    qDebug() << "str ---> " << str;
    QString currentTime = "*** " + QDate::currentDate().toString("yyyy/MM/dd") + " " + QTime::currentTime().toString("HH:mm:ss") +  " ***" + "\r\n\r\n";
    bool postResult = saveText(currentTime + str,"TextCtoS.txt");
    if(postResult){
        operationResultHTML("发送文本操作成功！",response);
    }
    else{
        operationResultHTML("发送文本操作失败！",response);
    }
}

bool PostString::saveText(QString strContent,QString fileName)
{
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
    file->setFileName(fileName);
    //只写
    bool openOK = file->open(QIODevice::WriteOnly);
    if(openOK)
    {
        //问件指针关联文本流
        QTextStream stream(file);
        //UTF-8
        stream.setCodec("UTF-8");
        stream<<strContent;
        //关闭文件
        file->close();
        delete file;
        return true;
    }
    return false;
}
void PostString::operationResultHTML(QByteArray str, HttpResponse& response){
    response.setHeader("Content-Type", "text/html; charset=UTF-8");
    response.write("<!DOCTYPE html>");
    response.write("<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">"
                   "<title>服务器提示</title></head>");
    response.write("<h2>" + str + "</h2>");
    response.write("</body></html>",true);
}

