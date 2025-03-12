#pragma once

#include <QObject>
#include <QWebSocket>
#include <QJsonObject>

class WsConn : public QObject
{
	Q_OBJECT
public:
	~WsConn();
	static void init();
	static WsConn* get();
	void sendMsg(const QString& message);
public:
	QJsonObject data;
private:
	WsConn(QObject* parent = nullptr);
	void startConnect();
	void wsMsgReceived(const QString& message);
	void wsDisconnected();
	void wsConnected();
	void wsError(QAbstractSocket::SocketError error);
private:
	QWebSocket* wsClient;
};

