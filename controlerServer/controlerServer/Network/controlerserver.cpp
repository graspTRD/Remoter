#include "stdafx.h"
#include "controlerserver.h"
#include "controlerconnection.h"
#include "Actions/broadcastaction.h"

#include <QTcpServer>
#include <QHostInfo>

ControlerServer::ControlerServer( QObject* parent /*= 0*/ )
	:QObject(parent)
	,m_tcpServer(NULL), m_udpSocket(NULL)
	,m_bConnected(false)
{

}

ControlerServer::~ControlerServer()
{

}

bool ControlerServer::listen( const QHostAddress &address /*= QHostAddress::Any*/, quint16 port/*=0*/ )
{
// 	if(isServerRunning())
// 	{
// 		if(port == m_tcpServer->serverPort() &&
// 			address == m_tcpServer->serverAddress())
// 		{
// 			return m_tcpServer->isListening();
// 		}
// 		else
// 		{
// 			close();
// 		}
// 	}

	m_udpSocket = new QUdpSocket(this);
	if(!m_udpSocket->bind(address, port))
	{
		qDebug()<<"Udp°ó¶¨Ê§°Ü£¡";
		Free_Ptr_Later(m_udpSocket);
		return false;
	}
	connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(newDatagrams()));

	m_tcpServer = new QTcpServer;
	connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
	
	if(!m_tcpServer->listen(address, port))
	{
		qDebug()<<"Tcp¼àÌýÊ§°Ü£¡";
		close();
	}

	return true;
}

bool ControlerServer::listen( quint16 port )
{
	return listen(QHostAddress::Any, port);
}

void ControlerServer::newConnection()
{
	Q_ASSERT(m_tcpServer);
	while(m_tcpServer->hasPendingConnections())
	{
		//if(m_bConnected) continue;
 		ControlerConnection *connection = new ControlerConnection(m_tcpServer->nextPendingConnection(), this);
		connect(connection, SIGNAL(disconnected()), this, SLOT(onConnectionReleased()));
		//m_bConnected = true;
	}
}

void ControlerServer::newDatagrams()
{
	Q_ASSERT(m_udpSocket);
	QByteArray datagrams;
	QHostAddress address;
	quint16 port;

	while(m_udpSocket->hasPendingDatagrams())
	{
		//if(m_bConnected) continue;
		datagrams.resize(m_udpSocket->pendingDatagramSize());
		m_udpSocket->readDatagram(datagrams.data(), datagrams.size(), &address, &port);
		onProcessDatagrams(datagrams, address, port);
	}
}

void ControlerServer::onProcessDatagrams(const QByteArray& data, const QHostAddress& address, qint16 port)
{
	BroadcastAction* request = BroadcastAction::parse(data);
	QString context = request->m_content;
	qDebug()<<"BroadcastAction content is : "<<context;
	if(context.toLower() == "pccontroler") 
	{
		m_udpSocket->writeDatagram(QHostInfo::localHostName().toLocal8Bit(), address, port);	
	}
	Free_Ptr(request);
}

void ControlerServer::onConnectionReleased()
{
	m_bConnected = false;
}

void ControlerServer::close()
{
	if(m_udpSocket)
	{
		m_udpSocket->close();
		Free_Ptr_Later(m_udpSocket);
		if(m_tcpServer)
		{
			m_tcpServer->close();
			Free_Ptr_Later(m_tcpServer);
		}
	}
}

bool ControlerServer::isServerRunning() const
{
	if(!m_udpSocket || !m_tcpServer)
	{
		return false;
	}

	return m_udpSocket->isValid() && m_tcpServer->isListening();
}

bool ControlerServer::hasConnected() const
{
	return m_bConnected;
}



