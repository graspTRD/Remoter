#ifndef CONTROLERSERVER_H
#define CONTROLERSERVER_H

#include<QObject>
#include<QHostAddress>
#include <QUdpSocket>

class QTcpServer;

class ControlerServer : public QObject
{
	Q_OBJECT
public:
	ControlerServer(QObject* parent = 0);
	virtual ~ControlerServer();
	bool listen(const QHostAddress &address = QHostAddress::Any, quint16 port = 0);
	bool listen(quint16 port);
	bool isServerRunning() const;
	bool hasConnected() const;
	void close();

private slots:
	void newConnection();
	void newDatagrams();
	void onConnectionReleased();
	void onProcessDatagrams(const QByteArray& data, const QHostAddress& address, qint16 port);
	

private:
	QTcpServer *m_tcpServer;
	QUdpSocket *m_udpSocket;
	bool m_bConnected;
};

#endif