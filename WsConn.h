#pragma once

#include <QObject>
#include <QWebSocket>
#include <QJsonObject>

class WsConn : public QObject
{
	Q_OBJECT
public:
	WsConn(QObject* parent=nullptr);
	~WsConn();
	void startConnect();
signals:
	void sendMsg(QString message);
private:
	void wsMsgReceived(const QString& message);
	void wsDisconnected();
	void wsConnected();
	void wsError(QAbstractSocket::SocketError error);
	void onSendMsg(QString message);
private:
	QWebSocket* wsClient;
};

