#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTimer>

#include "Skin.h"
#include "WsConn.h"
#include "MainWindow.h"

namespace {
    WsConn* conn;
}

WsConn::WsConn(QObject* parent) : QObject(parent)
{
}

WsConn::~WsConn()
{
    qDebug() << "wsConn dispose";
}

void WsConn::startConnect()
{
    auto arguments = QCoreApplication::arguments();
    QString port{ "56620" };
    for (int i = 0; i < arguments.length(); i++)
    {
        if (arguments[i].startsWith("EmbedCalendar_")) {
            port = arguments[i].split("_")[1];
            break;
        }
    }
    QString serverName = "ws://127.0.0.1:%1/EmbedCalendar";
    serverName = serverName.arg(port);
    qDebug() << serverName;
    auto url = QUrl(serverName);
    wsClient = new QWebSocket("EmbedCalendar", QWebSocketProtocol::VersionLatest, this);
    connect(wsClient, &QWebSocket::connected, this, &WsConn::wsConnected);
    connect(wsClient, &QWebSocket::disconnected, this, &WsConn::wsDisconnected);
    connect(wsClient, &QWebSocket::textMessageReceived, this, &WsConn::wsMsgReceived);
    connect(wsClient, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), this, &WsConn::wsError);
    wsClient->open(url);
}

void WsConn::init()
{
    conn = new WsConn(qApp);
    conn->startConnect();
}

WsConn* WsConn::get()
{
    return conn;
}

void WsConn::wsMsgReceived(const QString& message)
{
    qDebug() << "receive one msg:" << message;
    QJsonParseError err;
    QJsonDocument jd = QJsonDocument::fromJson(message.toUtf8(), &err);
    if (err.error != QJsonParseError::NoError || jd.isNull())
    {
        qWarning() << "json parse error" << err.errorString();
        qWarning() << message;
        return;
    }
    data = jd.object()["data"].toObject();
    Skin::init();
    MainWindow::init();
}

void WsConn::wsError(QAbstractSocket::SocketError error)
{
    qDebug() << "webSocketError" << error;
}

void WsConn::sendMsg(const QString& message)
{
    //qDebug() << "WsConn onSendMsg" << message;
    wsClient->sendBinaryMessage(message.toUtf8());
    wsClient->flush();
}

void WsConn::wsDisconnected()
{
    qDebug() << "WsConn wsDisconnected";
    //qApp->quit();
}

void WsConn::wsConnected()
{
    qDebug() << "WsConn wsConnected";
}
