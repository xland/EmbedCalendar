#pragma once

#include <QObject>
#include <QWebSocket>
#include <QJsonObject>

class WsConn : public QObject
{
	Q_OBJECT
public:
	~WsConn();
	void startConnect();
	static WsConn* init();
	static WsConn* get();
	void sendMsg(const QString& message);
signals:
	void onData(const QJsonObject& data);
	void onToast(const QJsonObject& data);
public:
private:
	WsConn(QObject* parent = nullptr);
	void wsMsgReceived(const QString& message);
	void wsDisconnected();
	void wsConnected();
	void wsError(QAbstractSocket::SocketError error);
private:
	QWebSocket* wsClient;
};

