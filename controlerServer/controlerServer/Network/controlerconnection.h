#ifndef CONTROLERCONNECTION_H
#define CONTROLERCONNECTION_H

#include<QObject>
#include <QFileInfo>
#include <QTimer>
#include <QPoint>

class QTcpSocket;
class ControlerAction;
class AuthentificationAction;
class FileExploreRequestAction;
class ShortcutGroupAction;
class KeyboardAction;
class MouseClickAction;
class MouseMoveAction;
class MouseWheelAction;
class TextAction;

class ControlerConnection : public QObject
{
	Q_OBJECT
public:
	ControlerConnection(QTcpSocket *socket, QObject *parent = 0);
	virtual ~ControlerConnection();

signals:
	void disconnected();

private slots:
	void slotDataRecevied();

private: 
	void dealWithActions(QByteArray& data);
	void onAction(ControlerAction* action);
	void doMoveMouse(MouseMoveAction* action);
	void doMouseClick(MouseClickAction* action);
	void doMouseWheel(MouseWheelAction* action);
	void doPressKeyboard(KeyboardAction* action);
	void doText(TextAction *action);
	void doAuthentification(AuthentificationAction* action);
	void doFileExploreRequestAction(FileExploreRequestAction* action);
	void doShortcutGroupAction(ShortcutGroupAction* action);
	void doFileExplore(QString& filefullName);
	void doFileExploreRoots();
	void doCloseActivedWindow();

	void sendAction(ControlerAction* action);
	void sendFileExploreResponse(QString& filePath, QFileInfoList& infos);

	int getDataLength(const QByteArray& data);
	qint64 sendData(const QByteArray& data);

private:
	QTcpSocket* m_pSocket;
	bool m_authentificated;
};

#endif