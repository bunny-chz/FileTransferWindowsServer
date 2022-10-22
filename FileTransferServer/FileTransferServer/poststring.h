/**
 * @brief 客户端发送信息的处理类
 * @author Bunny GitHub: bunny-chz
 * @date 2022/10/13
 */
#ifndef POSTSTRING_H
#define POSTSTRING_H

#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"


using namespace stefanfrings;

class PostString : public HttpRequestHandler {
    Q_OBJECT
    Q_DISABLE_COPY(PostString)
public:

    /** 构造 */
    PostString();

    /** 请求处理，响应 */
    void service(HttpRequest& request, HttpResponse& response);
    /** 保存客户端发送的信息 */
    bool saveText(QString strContent,QString fileName);
    /** 客户端发送信息操作提示页面 */
    void operationResultHTML(QByteArray str, HttpResponse& response);
};

#endif // POSTSTRING_H
