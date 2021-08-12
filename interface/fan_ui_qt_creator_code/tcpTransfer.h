#ifndef TCPTRANSFER_H
#define TCPTRANSFER_H

#include <QTcpServer>

class QTcpSocket;

class LocalServer : public QTcpServer{
    Q_OBJECT
public:
    explicit LocalServer(QObject* parent = 0);

public:
    QTcpSocket *mSocket;
};
#endif // TCPTRANSFER_H
