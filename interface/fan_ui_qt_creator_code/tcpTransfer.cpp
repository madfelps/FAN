#include <tcpTransfer.h>
#include <QTcpSocket>
#include <QTextStream>

LocalServer::LocalServer(QObject* parent)
    :QTcpServer(parent)
{
    mSocket = nullptr;

    connect(this, &LocalServer::newConnection, [&](){
        mSocket = nextPendingConnection();
    });
}
