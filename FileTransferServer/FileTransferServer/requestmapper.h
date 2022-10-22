/**
 * @brief 请求URL映射器类
 * @author Bunny GitHub: bunny-chz
 * @date 2022/10/13
 */
#ifndef REQUESTMAPPER_H
#define REQUESTMAPPER_H

#include "httprequesthandler.h"
#include "fileuploadcontroller.h"
#include "poststring.h"


using namespace stefanfrings;

class RequestMapper : public HttpRequestHandler {
    Q_OBJECT
public:
    RequestMapper(QObject* parent=0);

    FileUploadController fileUploadController;
    PostString postString;

    /** 请求处理，响应 */
    void service(HttpRequest& request, HttpResponse& response);
    /** 响应函数，返回网页端主页 */
    void mainPageHTML(HttpResponse& response);
    /** 连接测试提示页面 */
    void serviceTestLink(HttpResponse& response);
    /** 客户端发送文本操作页面 */
    void textCToSHTML(HttpResponse& response);
    /** 客户端发送文件操作页面 */
    void fileCToSHTML(HttpResponse& response);
    /** 404 NotFound */
    void responseNotFound(HttpResponse& response);
    /** 服务器没有设置分享文件提示页面 */
    void responseNoSetFile(HttpResponse& response);
    /** 服务器没有发送文本提示页面 */
    void responseNotSetText(HttpResponse& response);
    /** 接收网页端或者手机客户端的信息 */
    void getTextFromServer(bool isWeb,HttpResponse& response);
     /** 下载网页端或者手机客户端的文件 */
    void getFileFromServer(bool isWeb, HttpResponse& response);
};

#endif // REQUESTMAPPER_H
