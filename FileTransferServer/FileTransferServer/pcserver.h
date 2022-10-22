/**
 * @brief 界面类
 * @author Bunny GitHub: bunny-chz
 * @date 2022/10/13
 */
#ifndef PCSERVER_H
#define PCSERVER_H

#include <QWidget>

namespace Ui {
class PcServer;
}

class PcServer : public QWidget
{
    Q_OBJECT

public:
    explicit PcServer(QWidget *parent = nullptr);
    ~PcServer();

    //实例化一个QPoint对象z,记录鼠标点击处到窗口左上角的点坐标
    QPoint z;

    /** 在目录中逐一寻找QTWebApp配置文件 */
    QString searchConfigFile();
    /** 获取本机IP */
    QString getDeviceIP();
    /** 鼠标按下事件 */
    void mousePressEvent(QMouseEvent *event);
    /** 鼠标移动事件 */
    void mouseMoveEvent(QMouseEvent *event);
    /** 鼠标释放事件 */
    void mouseReleaseEvent(QMouseEvent *);
private slots:
    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

    void on_toolButton_3_clicked();

    void on_toolButton_4_clicked();

    void on_toolButton_5_clicked();

    void on_closeBtn_2_clicked();

private:
    Ui::PcServer *ui;
};

#endif // PCSERVER_H
