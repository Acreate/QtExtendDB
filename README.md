# QtExtendHttpNetWork

#### 介绍
基于 qt 实现的 get 请求库

#### 使用说明

– 请求一个页面

```c++
cylHttpNetWork::RequestConnect requestConnect;
cylHttpNetWork::NetworkAccessManager networkAccessManager;
cylHttpNetWork::NetworkRequest networkRequest;
cylHttpNetWork::Request request( &networkAccessManager, &requestConnect );

QObject::connect( &requestConnect, &cylHttpNetWork::RequestConnect::networkReplyFinished,
	[]( cylHttpNetWork::RequestConnect *request_connect ) {
		auto networkReply = request_connect->getNetworkReply( );
		auto byteArray = networkReply->readAll( );
		QString msg( byteArray );
		qDebug( ) << "==================================";
		qDebug( ) << msg;
		qDebug( ) << "==================================";
	} );
networkRequest.setHeader( QNetworkRequest::UserAgentHeader, networkRequest.getRandomUserAgentHeader( ) );
request.netGetWork( QUrl( "https://www.121ds.cc/" ), networkRequest );
```

-使用请求等待

```c++
#include <QApplication>
#include <HttpNetWork/NetworkRequest.h>
#include <HttpNetWork/NetworkAccessManager.h>
#include <HttpNetWork/Request.h>
#include <HttpNetWork/RequestConnect.h>

void testRequest( cylHttpNetWork::NetworkAccessManager &networkAccessManager, QUrl qUrl ) {
	// 请求信号转发对象
	cylHttpNetWork::RequestConnect *requestConnect = new cylHttpNetWork::RequestConnect;
	// 请求发送
	cylHttpNetWork::Request *request = new cylHttpNetWork::Request( &networkAccessManager, requestConnect );
	// 响应
	auto time = std::chrono::system_clock::now( ).time_since_epoch( );
	QObject::connect( requestConnect, &cylHttpNetWork::RequestConnect::networkReplyFinished, [=]( cylHttpNetWork::RequestConnect *request_connect ) {
		auto subTime = time - std::chrono::system_clock::now( ).time_since_epoch( );
		auto timeCount = std::chrono::duration_cast< std::chrono::milliseconds >( subTime ).count( );
		qDebug( ) << "over : " << timeCount << "\t" << request;
		requestConnect->deleteLater( );
		request->deleteLater( );
	} );
	// 开始请求
	request->netGetWork( qUrl );
}
void testRequest( cylHttpNetWork::NetworkAccessManager &networkAccessManager, QUrl qUrl, size_t mill ) {
	// 请求信号转发对象
	cylHttpNetWork::RequestConnect *requestConnect = new cylHttpNetWork::RequestConnect;
	// 请求发送
	cylHttpNetWork::Request *request = new cylHttpNetWork::Request( &networkAccessManager, requestConnect );
	// 响应
	auto time = std::chrono::system_clock::now( ).time_since_epoch( );
	QObject::connect( requestConnect, &cylHttpNetWork::RequestConnect::networkReplyFinished, [=]( cylHttpNetWork::RequestConnect *request_connect ) {
		auto subTime = time - std::chrono::system_clock::now( ).time_since_epoch( );
		auto timeCount = std::chrono::duration_cast< std::chrono::milliseconds >( subTime ).count( );
		qDebug( ) << "over : " << timeCount << "\t" << request;
		requestConnect->deleteLater( );
		request->deleteLater( );
	} );
	// 开始请求，但是使用自定义的时间，而并非已经定义的时间间隔
	request->netGetWork( qUrl, mill );
}
int main( int argc, char *argv[ ] ) {
	QApplication app( argc, argv );
	// 请求管理
	cylHttpNetWork::NetworkAccessManager networkAccessManager;
	// 请求体
	cylHttpNetWork::NetworkRequest qNetworkRequest;
	// 配置 url
	QUrl qUrl( "https://www.121ds.cc/" );
	// 设置 url
	qNetworkRequest.setUrl( qUrl );
	// 设置请求头
	qNetworkRequest.setHeader( QNetworkRequest::UserAgentHeader, cylHttpNetWork::NetworkRequest::getRandomUserAgentHeader( ) );
	// 初始化等待功能
	cylHttpNetWork::NetworkRequest::init( );
	// 设置 url 等待 15000 毫秒
	cylHttpNetWork::NetworkRequest::setHostUrlRequestInterval( qUrl, 15000 );

	// 使用默认等待时间
	testRequest( networkAccessManager, qUrl );
	// 使用请求等待时间-使用已经设定的时间
	testRequest( networkAccessManager, qUrl );
	// 使用请求等待时间-但是自定义
	testRequest( networkAccessManager, qUrl, 8000 );
	qDebug( ) << "\n\n\n\n=============\n" << "int main( int argc, char *argv[ ] ) ";
	qDebug( ) << "return app.exec( );";
	return app.exec( );
}

```

显示:

```
run code :  0
over :  -6011    cylHttpNetWork::Request(0x1fa5d94efb0)
run code :  0
over :  -15260   cylHttpNetWork::Request(0x1fa5f9a8ed0)
run code :  -2




=============
 int main( int argc, char *argv[ ] )
return app.exec( );
over :  -8274    cylHttpNetWork::Request(0x1fa5f9a8c90)

```

## 更多的说明

### NetworkAccessManager

网络对象管理类

​	实现自 QNetworkAccessManager

### NetworkRequest

网络请求体类

​	实现自 QNetworkRequest

​	内部包含多种工具，使用这些工具必须先初始化

```c++
static void NetworkRequest::initTools( );
```

​	其他工具

```c++
/// <summary>
/// 初始化
/// </summary>
static void initTools( );
/// <summary>
/// 设置域请求间隔-毫秒
/// </summary>
/// <param name="q_url">域url</param>
/// <param name="microseconds">时间</param>
static void setHostUrlRequestInterval( const QUrl &q_url, const size_t &microseconds );
/// <summary>
/// 获取域的休眠间隔时间-毫秒。默认返回 defaultHostRequestIntervalMilliseconds
/// </summary>
/// <param name="q_url">域</param>
static size_t getHostUrlRequestInterval( const QUrl &q_url );
/// <summary>
/// 获取当前时间的时间戳
/// </summary>
/// <returns></returns>
static NetworkRequest::Time_Duration getNowTimeDuration( );
/// <summary>
/// 时间戳转换到毫秒
/// </summary>
/// <param name="time_duration">时间戳</param>
/// <returns>毫秒</returns>
static long long getTimeDurationToMilliseconds(const NetworkRequest::Time_Duration& time_duration);
/// <summary>
/// 获取域请求时间
/// </summary>
/// <param name="q_url">域链接</param>
/// <param name="result">返回的时间</param>
/// <returns>成功返回 true，flase 表示不存在</returns>
static bool getHostLastRequestTime( const QUrl &q_url, NetworkRequest::Time_Duration &result );
/// <summary>
/// 设置域请求时间
/// </summary>
/// <param name="q_url">设置的域</param>
/// <param name="time_duration">请求的时间</param>
static void setHostLastRequestTime( const QUrl &q_url, const Time_Duration &time_duration );
/// <summary>
/// 设置当前时间到域时间请求上
/// </summary>
/// <param name="q_url">设置的域</param>
static void updateCurrentTimeToHostLastRequestTime( const QUrl &q_url );
/// <summary>
/// 获取已经库存的 UserAgentHeader
/// </summary>
/// <returns>所有的 UserAgentHeader</returns>
static QStringList getUserAgentHeaders( );
/// <summary>
/// 获取一个随机的 UserAgentHeader
/// </summary>
/// <returns>随机的 UserAgentHeader</returns>
static QString getRandomUserAgentHeader( );
```

### Request

发送请求实用体。

继承自 QObject，内部包含 RequestConnect 与 NetworkAccessManager，并且自身用于操作  RequestConnect 与 NetworkAccessManager，实现内部消息的转发。

#### get 请求

使用 Request::netGetWork 可以实现 get 请求。

```c++

/// <summary>
/// 发送 get 请求<br/>
/// 返回 -2 时，你仍然可以使用 NetworkAccessManager 信号槽，但作为转发的 RequestConnect 将会失去效果
/// </summary>
/// <param name="url">请求的 rul</param>
/// <param name="connect_type">信号链接类型</param>
/// <returns>执行编码，非0表示失败</returns>
int32_t netGetWork( const QUrl &url, Qt::ConnectionType connect_type = Qt::AutoConnection );
/// <summary>
/// 使用模板发送 get 请求，请求 url 来自参数 url，并非模板 network_request<br/>
/// 返回 -2 时，你仍然可以使用 NetworkAccessManager 信号槽，但作为转发的 RequestConnect 将会失去效果
/// </summary>
/// <param name="url">请求的 rul</param>
/// <param name="network_request">模板</param>
/// <param name="connect_type">信号链接类型</param>
/// <returns>执行编码，非0表示失败</returns>
int32_t netGetWork( const QUrl &url, const NetworkRequest &network_request, Qt::ConnectionType connect_type = Qt::AutoConnection );
/// <summary>
/// 使用模板发送 get 请求，请求 url 来自 network_request 模板<br/>
/// 返回 -2 时，你仍然可以使用 NetworkAccessManager 信号槽，但作为转发的 RequestConnect 将会失去效果
/// </summary>
/// <param name="network_request">模板</param>
/// <param name="connect_type">信号链接类型</param>
/// <returns>执行编码，非0表示失败</returns>
int32_t netGetWork( const NetworkRequest &network_request, Qt::ConnectionType connect_type = Qt::AutoConnection );
/// <summary>
/// 使用模板发送 get 请求，并且等待时间 microseconds 毫秒<br/>
/// 返回 -2 时，你仍然可以使用 NetworkAccessManager 信号槽，但作为转发的 RequestConnect 将会失去效果
/// </summary>
/// <param name="url">请求地址</param>
/// <param name="microseconds">等待毫秒</param>
/// <param name="connect_type">信号链接类型</param>
/// <returns>执行编码，非0表示失败</returns>
int32_t netGetWork( const QUrl &url, size_t microseconds, Qt::ConnectionType connect_type = Qt::AutoConnection );
/// <summary>
/// 使用模板发送 get 请求，并且等待时间 microseconds 毫秒<br/>
/// 使用的请求模板来自 network_request，链接来自参数 url<br/>
/// 返回 -2 时，你仍然可以使用 NetworkAccessManager 信号槽，但作为转发的 RequestConnect 将会失去效果
/// </summary>
/// <param name="url">请求地址</param>
/// <param name="network_request">请求模板</param>
/// <param name="microseconds">等待毫秒</param>
/// <param name="connect_type">信号链接类型</param>
/// <returns>执行编码，非0表示失败</returns>
int32_t netGetWork( const QUrl &url, const NetworkRequest &network_request, size_t microseconds, Qt::ConnectionType connect_type = Qt::AutoConnection );
/// <summary>
/// 使用模板发送 get 请求，并且等待时间 microseconds 毫秒<br/>
/// 使用的请求链接来自 network_request 对象<br/>
/// 返回 -2 时，你仍然可以使用 NetworkAccessManager 信号槽，但作为转发的 RequestConnect 将会失去效果
/// </summary>
/// <param name="network_request">请求模板</param>
/// <param name="microseconds">等待毫秒</param>
/// <param name="connect_type">信号链接类型</param>
/// <returns>执行编码，非0表示失败</returns>
int32_t netGetWork(const NetworkRequest &network_request, size_t microseconds, Qt::ConnectionType connect_type = Qt::AutoConnection );
```

#### 睡眠工具

```c++
/// <summary>
/// 睡眠指定的时间-毫秒
/// </summary>
/// <param name="milliseconds">毫秒</param>
/// <returns>毫秒</returns>
static size_t sleep( const size_t &milliseconds );
```

### RequestConnect

该对象实现信号转发，该对象只能一次性的配置

NetworkAccessManager 与 QNetworkReply 各配置一次

```c++
/// <summary>
/// 设置 NetworkAccessManager 对象转发体
/// </summary>
/// <param name="request">转发对象</param>
/// <param name="connect_type">信号槽链接类型</param>
/// <returns>失败返回 false</returns>
bool setNetworkAccessManager( NetworkAccessManager *request, Qt::ConnectionType connect_type = Qt::AutoConnection );
/// <summary>
/// 设置 QNetworkReply 对象转发体
/// </summary>
/// <param name="networkReply">转发对象</param>
/// <param name="connect_type">信号槽链接类型</param>
/// <returns>失败返回 false</returns>
bool setNetworkReply( QNetworkReply *networkReply, Qt::ConnectionType connect_type = Qt::AutoConnection );
```

实现自 QObject

#### 转发内容

```c++
	Q_SIGNALS: //  QNetworkAccessManager 转发
		void networkAccessManagerAuthenticationRequired( QNetworkReply *reply, QAuthenticator *authenticator );
		void networkAccessManagerEncrypted( QNetworkReply *reply );
		void networkAccessManagerFinished( QNetworkReply *reply );
		void networkAccessManagerPreSharedKeyAuthenticationRequired( QNetworkReply *reply, QSslPreSharedKeyAuthenticator *authenticator );
		void networkAccessManagerProxyAuthenticationRequired( const QNetworkProxy &proxy, QAuthenticator *authenticator );
		void networkAccessManagerSslErrors( QNetworkReply *reply, const QList< QSslError > &errors );
	Q_SIGNALS: //  QNetworkReply 转发
		void networkReplySocketStartedConnecting( );
		void networkReplyRequestSent( );
		void networkReplyMetaDataChanged( );
		void networkReplyMetaDataFinished( );
		void networkReplyFinished( RequestConnect *requestConnect );
		void networkReplyErrorOccurred( QNetworkReply::NetworkError );
#if QT_CONFIG( ssl )
		void networkReplyEncrypted( );
		void networkReplySslErrors( const QList< QSslError > &errors );
		void networkReplyPreSharedKeyAuthenticationRequired( QSslPreSharedKeyAuthenticator *authenticator );
#endif
		void networkReplyRedirected( const QUrl &url );
		void networkReplyRedirectAllowed( );

		void networkReplyUploadProgress( qint64 bytesSent, qint64 bytesTotal );
		void networkReplyDownloadProgress( qint64 bytesReceived, qint64 bytesTotal );
```



其中

```c++
	void networkReplyFinished( RequestConnect *requestConnect );
```

属于二次转发，

```c++
	connect( this, &RequestConnect::networkReplyMetaDataFinished, [=]( ) {
		emit networkReplyFinished( this );
	} );

```







