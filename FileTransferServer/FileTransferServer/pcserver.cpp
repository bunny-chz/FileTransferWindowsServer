/**
 * @brief 界面类
 * @author Bunny GitHub: bunny-chz
 * @date 2022/10/13
 */
#include "pcserver.h"
#include "ui_pcserver.h"
#include <QSettings>
#include <QFile>
#include <QHostAddress>
#include <QDir>
#include <QNetworkInterface>
#include <QFileDialog>
#include<QMimeType>
#include<QMimeDatabase>
#include<QPixmap>
#include<QPainter>
#include<QMouseEvent>
#include<QObject>

#include "requestmapper.h"
#include "httplistener.h"

PcServer::PcServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PcServer)
{
    ui->setupUi(this);
    this->setWindowTitle("文件快传服务器端");
    setWindowFlags(Qt::FramelessWindowHint);
    //设置图标
    this->setWindowIcon(QIcon(":/res/app_icon.png"));
    QString configFileName = searchConfigFile();
    QSettings* listenerSettings = new QSettings(configFileName, QSettings::IniFormat, this);
    listenerSettings->beginGroup("listener");
    // 启动HTTP服务器
    new HttpListener(listenerSettings, new RequestMapper(this), this);
    qDebug("config file loaded");

    QString currentAddress = getDeviceIP();
    ui->label->setText("本机IP为 --> " + currentAddress + ", 设置的端口为 --> 9102");
    ui->label_2->setText("安卓手机或者浏览器地址请填入 " + currentAddress + ":9102");
}

PcServer::~PcServer()
{
    delete ui;

}
QString PcServer::searchConfigFile() {
    QString binDir=QCoreApplication::applicationDirPath();
    QString appName=QCoreApplication::applicationName();
    QFile file;
    file.setFileName(binDir+"/webapp1.ini");
    if (!file.exists()) {
        file.setFileName(binDir+"/etc/webapp1.ini");
        if (!file.exists()) {
            file.setFileName(binDir+"/../etc/webapp1.ini");
            if (!file.exists()) {
                file.setFileName(binDir+"/../"+appName+"/etc/webapp1.ini");
                if (!file.exists()) {
                    file.setFileName(binDir+"/../../"+appName+"/etc/webapp1.ini");
                    if (!file.exists()) {
                        file.setFileName(binDir+"/../../../../../"+appName+"/etc/webapp1.ini");
                        if (!file.exists()) {
                            file.setFileName(QDir::rootPath()+"etc/webapp1.ini");
                        }
                    }
                }
            }
        }
    }
    if (file.exists()) {
        QString configFileName=QDir(file.fileName()).canonicalPath();	//将相对路径换成绝对路径
        qDebug("using config file %s", qPrintable(configFileName));
        return configFileName;
    }
    else {
        qFatal("config file not found");	//找不到该文件，则应用程序将输出一条错误消息，并中止程序
    }
}
QString PcServer::getDeviceIP()
{
    QString ip_address;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses(); // 获取所有ip
    for (int i = 0; i < ipAddressesList.size(); ++i)
    {

        QHostAddress ipAddr = ipAddressesList.at(i);
        if ((ipAddr.protocol() == QAbstractSocket::IPv4Protocol))// 筛选出ipv4
        {
            qDebug() << "ipv4 --> " << ipAddr.toString();
            if (!ipAddr.toString().startsWith("169"))// 过滤掉网卡，路由器等的ip
            {
                ip_address = ipAddr.toString();
                break;
            }
        }
    }
    if (ip_address.isEmpty())
        ip_address = QHostAddress(QHostAddress::LocalHost).toString();
    return ip_address;
}
//读取发给服务器的文本信息
void PcServer::on_toolButton_clicked()
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
    file->setFileName("TextCtoS.txt");
    //只读
    bool openOK = file->open(QIODevice::ReadOnly);

    if(openOK)
    {
        //问件指针关联文本流
        QTextStream stream(file);
        //UTF-8
        stream.setCodec("UTF-8");
        ui->textEdit_4->setText(stream.readAll());
        //关闭文件
        file->close();
        delete file;

        //获取当前光标的位置
        QTextCursor cursor = ui->textEdit_4->textCursor();
        //将光标移动到末尾
        cursor.movePosition(QTextCursor::End);
        //将设置好的光标放到edit中
        ui->textEdit_4->setTextCursor(cursor);
    }

}
//写入发给客户端的文字
void PcServer::on_toolButton_2_clicked()
{
    //QtWebApp每个Handler内不允许开其它内存，和UI无法匹配，采取读写文件的形式处理文本
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
    file->setFileName("TextStoC.txt");
    //只写
    bool openOK = file->open(QIODevice::WriteOnly);
    if(openOK)
    {
        //问件指针关联文本流
        QTextStream stream(file);
        //UTF-8
        stream.setCodec("UTF-8");
        if(!ui->textEdit_3->toPlainText().isEmpty()){
            stream<<"时间: " + QDate::currentDate().toString("yyyy/MM/dd") + " " + QTime::currentTime().toString("HH:mm:ss") + "#-#-#" + ui->textEdit_3->toPlainText();
        }
        else{
            stream<<"";
        }
        //关闭文件
        file->close();
        delete file;
    }

}
//发送文件给客户端
void PcServer::on_toolButton_3_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,tr("选择文件分享给客户端"),"",tr("*"));
    if (filePath.isEmpty())
    {
        return;
    }
    QMimeDatabase db;
    QMimeType subFileMimeType = db.mimeTypeForFile(filePath);  //根据前面定义的文件名（含后缀）
    QString fileMimeType = subFileMimeType.name();  //使用name()将MimeType类型转为字符串类型
    qDebug() << "fileMimeType ---> " << fileMimeType;
    ui->label_6->setText("已选择的文件: " + filePath);
    //QtWebApp每个Handler内不允许开其它内存，和UI无法匹配，采取读写文件的形式处理文本
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
    //只写
    bool openOK = file->open(QIODevice::WriteOnly);
    if(openOK)
    {
        //问件指针关联文本流
        QTextStream stream(file);
        //UTF-8
        stream.setCodec("UTF-8");
        stream<<filePath + "#-#-#" + fileMimeType;
        //关闭文件
        file->close();
        delete file;
    }
}
//读取上传文件日志文件
void PcServer::on_toolButton_4_clicked()
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
    file->setFileName("FileCtoSLog.txt");
    //只读
    bool openOK = file->open(QIODevice::ReadOnly);

    if(openOK)
    {
        //问件指针关联文本流
        QTextStream stream(file);
        //UTF-8
        stream.setCodec("UTF-8");
        QString str = stream.readAll();
        if(str.isEmpty()){
            ui->textBrowser->setText("无上传文件记录");
        }else{
            ui->textBrowser->setText(str);
        }
        //关闭文件
        file->close();
        delete file;
    }else{
        ui->textBrowser->setText("无上传文件记录");
    }
}
//清除上传文件日志文件
void PcServer::on_toolButton_5_clicked()
{
    ui->textBrowser->setText("");
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
    file->setFileName("FileCtoSLog.txt");
    //只写
    bool openOK = file->open(QIODevice::WriteOnly);
    if(openOK)
    {
        //问件指针关联文本流
        QTextStream stream(file);
        //UTF-8
        stream.setCodec("UTF-8");
        stream<<"";
        //关闭文件
        file->close();
        delete file;
    }
}
//鼠标按下事件
void PcServer::mousePressEvent(QMouseEvent *event)
{
    QPoint y = event->globalPos();//获取鼠标点击处的全局坐标
    QPoint x = this->geometry().topLeft();//获取窗口左上角到屏幕左上角的坐标
    this->z = y - x;//相减即为z坐标，鼠标点击处到窗口左上角的点坐标
}
//鼠标移动事件
void PcServer::mouseMoveEvent(QMouseEvent *event)
{
    QPoint y = event->globalPos();//获取鼠标点击处的全局坐标
    QPoint x = y - z;
    move(x);//鼠标移动，窗口就移动
}
//鼠标释放事件
void PcServer::mouseReleaseEvent(QMouseEvent *){
    this->z = QPoint();//鼠标释放后让z坐标重置为0
}
void PcServer::on_closeBtn_2_clicked()
{
    this->close();
}
