#include "mytcpsocket.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

MyTcpSocket::MyTcpSocket(QObject *parent) : QObject(parent)
{    
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                ipAddressesList.at(i).toIPv4Address())
            {
                  ipAddress = ipAddressesList.at(i).toString();
                  break;
            }
    }

    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
}

void MyTcpSocket::doConnect()
{

    struct sockaddr_in server_addr, local_addr;
    int sock_fd = ::socket(AF_INET, SOCK_STREAM, 0);

    if (sock_fd < 0)
        qDebug() << "Error in client creating\n";
    else
        qDebug() << "Client Created\n";

    qsocket = new QTcpSocket(this);

    // binding client with specific port
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY;
    local_addr.sin_port = htons(20);
    local_addr.sin_addr.s_addr = inet_addr("192.168.0.101");

    //Server Address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8890);
    server_addr.sin_addr.s_addr = inet_addr("192.168.0.41");

    if (::bind(sock_fd, (struct sockaddr*)&local_addr, sizeof(struct sockaddr_in)) == 0)
        qDebug() << "Binded Correctly\n";
    else
        qDebug() << "Unable to bind\n";

    connect(qsocket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(qsocket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(qsocket, SIGNAL(readyRead()),this, SLOT(readyRead()));

    if (::connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
          qDebug() << "Error in Connection\n";
    }
    else
    {
         qDebug() << "Client Connected\n";
    }

     qsocket->setSocketDescriptor(sock_fd);

}

void MyTcpSocket::connected()
{
    qDebug() << "connected...";
}

void MyTcpSocket::disconnected()
{
    qDebug() << "disconnected...";
}

void MyTcpSocket::bytesWritten(qint64 bytes)
{
    qDebug() << bytes << " bytes written...";
}

void MyTcpSocket::readyRead()
{
    qDebug() << "reading...";
}
