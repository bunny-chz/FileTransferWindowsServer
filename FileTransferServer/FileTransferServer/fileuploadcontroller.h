/**
 * @brief 客户端上传文件处理类
 * @author Bunny GitHub: bunny-chz
 * @date 2022/10/13
 */
#ifndef FILEUPLOADCONTROLLER_H
#define FILEUPLOADCONTROLLER_H

#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"


using namespace stefanfrings;

/**
 * @brief 客户端上传文件处理类
 */

class FileUploadController : public HttpRequestHandler {
    Q_OBJECT
    Q_DISABLE_COPY(FileUploadController)
public:

    /** 构造函数 */
    FileUploadController();

    /** 请求处理，响应 */
    void service(HttpRequest& request, HttpResponse& response);
    /** 保存文件函数，把临时文件复制到指定路径 */
    bool saveFile(QString &fileName, QTemporaryFile* tmpFile, QString& strFileName);
    /** 操作成功页面提示 */
    void operationResultHTML(QByteArray str, HttpResponse& response);
    /** 保存上传文件记录日志 */
    bool saveText(QString strContent,QString fileName);
};

#endif // FILEUPLOADCONTROLLER_H
