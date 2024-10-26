#include "server.h"

Server::Server() {

    if(this->listen(QHostAddress::Any, 1919)){
        qDebug()<<"start";
    }
    else{
        qDebug()<<"error";
    }
    nextBlockSize = 0;
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead,this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected,this, &Server::socketDiconnect);

    sockets.push_back(socket);
    qDebug()<<"socket connected:"<<socket->socketDescriptor();

}



void Server::slotReadyRead()
{
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_2);
    if(in.status() == QDataStream::Ok)
    {
        qDebug()<<"read...";
        for(;;)
        {
            if(nextBlockSize == 0)
            {
                qDebug()<<"nextBlockSize = 0";
                if(socket->bytesAvailable() < 2){
                    break;
                }
                in>>nextBlockSize;
                qDebug()<<"nextBlockSize =" << nextBlockSize;
            }
            if(socket->bytesAvailable() < nextBlockSize)
            {
                qDebug()<<"Data not full. Break";
                break;
            }

            QString str;
            in>>str;
            nextBlockSize = 0;
            qDebug()<<str;
            sendToClient(str);
            break;
        }
    }
    else
    {
        qDebug()<<"DataStream error";
    }
}

void Server::sendToClient(QString msg)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    out<<qint16(0)<<msg;
    out.device()->seek(0);
    out<<qint16(Data.size() - sizeof(qint16));
    out<<msg;
    //socket->write(Data);
    for(auto& sock:sockets){
        if(sock!=socket)
            sock->write(Data);
    }
}

void Server::socketDiconnect(){
    qDebug()<<"Disconnecting...";
    sockets[sockets.indexOf((QTcpSocket*)sender())]->deleteLater();
    sockets.erase(std::find(sockets.begin(),sockets.end(),(QTcpSocket*)sender()));
    qDebug()<<"Disconnected";
}

