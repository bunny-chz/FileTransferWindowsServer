/**
 * @brief 客户端上传文件处理类
 * @author Bunny GitHub: bunny-chz
 * @date 2022/10/13
 */

#include "fileuploadcontroller.h"
#include <QStandardPaths>
#include <QMessageBox>
#include <QDir>
#include <QLabel>
#include <QTime>
#include <QDate>

FileUploadController::FileUploadController()
{

}
void FileUploadController::service(HttpRequest& request, HttpResponse& response)
{
    //提取上传内容中头部为"file"的信息
    QTemporaryFile* tmpFile = request.getUploadedFile("file");
    // 设置为不自动删除
    tmpFile->setAutoRemove(false);
    if (tmpFile->open())
    {
        tmpFile->close();
        QString strFileTemplate = tmpFile->fileTemplate();
        QString strFileName = tmpFile->fileName();

        qDebug() << "fileTemplate ---> " << strFileTemplate;
        qDebug() << "strFileName ---> " << strFileName;

        QByteArray  param = request.getParameter("file");
        qDebug() << "param ---> " << param.data();
        QString fileName = QString(param);
        fileName.replace("\"","");
        qDebug() << "fileName ---> " << fileName;
        bool saveResult = saveFile(fileName,tmpFile,strFileName);
        QString srtContent = "时间: " + QDate::currentDate().toString("yyyy/MM/dd") + " " + QTime::currentTime().toString("HH:mm:ss") + "\n" + fileName + "\n";
        saveText(srtContent,"FileCtoSLog.txt");
        if(saveResult){
            operationResultHTML("上传文件操作成功，且保存成功！",response);
        }
        else{
            operationResultHTML("上传文件操作成功，但保存失败，请勿重复上传！",response);
        }
    }
    else
    {
        operationResultHTML("上传文件操作失败！",response);
    }
}
bool FileUploadController::saveFile(QString &fileName, QTemporaryFile* tmpFile, QString& strFileName){
    //QString desktop_path = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString dirName = "C:/FileTransfer Files/Upload/";
    QDir dir(dirName);
    if(!dir.exists())
    {
        dir.mkdir(dirName);
        qDebug() << "dir.mkdir ---> " <<"创建成功";
    }
    bool copyOK = tmpFile->copy(strFileName,  dirName + fileName);
    if(copyOK){
        qDebug() << "copyOK ---> " << copyOK;
        tmpFile->remove();
        return true;
    }
    qDebug() << "copyOK ---> " << copyOK;
    tmpFile->remove();
    return false;
}
bool FileUploadController::saveText(QString strContent,QString fileName)
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
    bool openOK = file->open(QIODevice::Append);
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
void FileUploadController::operationResultHTML(QByteArray str, HttpResponse& response){
    response.setHeader("Content-Type", "text/html; charset=UTF-8");
    response.write("<!DOCTYPE html>");
    response.write("<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">"
                   "<title>服务器提示</title></head>");
    response.write("<h2>" + str + "</h2>");
    response.write("</body></html>",true);
}
