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
public:
	QJsonObject data;
signals:
	void sendMsg(QString message);
private:
	WsConn(QObject* parent = nullptr);
	void startConnect();
	void wsMsgReceived(const QString& message);
	void wsDisconnected();
	void wsConnected();
	void wsError(QAbstractSocket::SocketError error);
	void onSendMsg(QString message);
private:
	QWebSocket* wsClient;
};

