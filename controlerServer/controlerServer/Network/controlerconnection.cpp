#include "stdafx.h"
#include "controlerconnection.h"
#include "Actions/controleraction.h"
#include "Actions/mousemoveaction.h"
#include "Actions/mouseclickaction.h"
#include "Actions/mousewheelaction.h"
#include "Actions/keyboardaction.h"
#include "Actions/authentificationaction.h"
#include "Actions/authentificationresponseaction.h"
#include "Actions/fileexploreresponseaction.h"
#include "Actions/fileexplorerequestaction.h"
#include "Actions/shortcutgroupaction.h"
#include "Actions/textaction.h"
#include "Common/keycodeconverter.h"
#include "Common/uiutils.h"

#include <QTcpSocket>
#include <QHostAddress>
#include <QDir>

static QHash<byte, int> g_actionSize;


ControlerConnection::ControlerConnection( QTcpSocket *socket, QObject *parent /*= 0*/ )
	:QObject(parent)
	,m_pSocket(socket), m_authentificated(false)
{
	Q_ASSERT(socket);
	g_actionSize.insert(ControlerAction::MOUSE_MOVE,	5);
	g_actionSize.insert(ControlerAction::MOUSE_CLICK,	3);
	g_actionSize.insert(ControlerAction::MOUSE_WHEEL,	2);
	g_actionSize.insert(ControlerAction::KEYBOARD,		5);
	g_actionSize.insert(ControlerAction::SHORTCUTGROUP, 13);
	g_actionSize.insert(ControlerAction::AUTHENTIFICATION, -1);
	g_actionSize.insert(ControlerAction::TEXT, -1);
	
	qDebug() << "新连接：" << socket->peerAddress() << socket->peerPort();
	connect(m_pSocket, SIGNAL(readyRead()), this, SLOT(slotDataRecevied()));
	connect(m_pSocket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
	connect(m_pSocket, SIGNAL(disconnected()), this, SLOT(deleteLater()));

	//mouseTimer = new QTimer();
	//mouseTimer->start();
	//mouseTimer->setInterval(5);
	//connect(mouseTimer, SIGNAL(timeout()), this, SLOT(mouseMoveTimeout()));
}

ControlerConnection::~ControlerConnection()
{
	qDebug()<<"连接释放！";
}

qint64 ControlerConnection::sendData(const QByteArray& data )
{
	if(!m_pSocket) return -1;
	return m_pSocket->write(data);
}

void ControlerConnection::sendAction(ControlerAction* action )
{
	sendData(action->toByteArray());
}

int ControlerConnection::getDataLength( const QByteArray& data )
{
	int dataSize = data.size();

	byte actionID = data.at(0);
	int nLen = -1;
	if(actionID == ControlerAction::AUTHENTIFICATION || actionID == ControlerAction::TEXT)
	{
		if(dataSize >= 3)
		{
			QByteArray wData = data.mid(1, 2);
			nLen = GetDataFromStream<short>(&wData) + 3;
		}
	}
	else if(actionID == ControlerAction::FILE_EXPLORE_REQUEST)
	{
		if(dataSize >= 5)
		{
			short sizeStr1 = GetDataFromStream<short>(&data.mid(1, 2));
			short sizeStr2 = GetDataFromStream<short>(&data.mid(sizeStr1 + 3, 2));
			nLen = sizeStr1 + sizeStr2 + 5;
		}
	}
	else
	{
		nLen = g_actionSize.value(actionID, -1);
	}

	return nLen;
}

void ControlerConnection::slotDataRecevied()
{
	while(m_pSocket->bytesAvailable())
	{
		QByteArray arr = m_pSocket->readAll();

		if( arr.size() <= 0 ) 
		{
			qDebug()<<"错误！未收到数据.";
		}
		else
		{
			dealWithActions(arr);
		}
	}
}

void ControlerConnection::dealWithActions( QByteArray& data )
{
	//分包
	int dataSize = data.size();
	while(dataSize > 0)
	{
		int nLen = getDataLength(data);

		if(nLen == -1 || dataSize < nLen)
		{
			qDebug()<<"数据错误！ID:"<<data.at(0);
			return;
		}

		QByteArray curData = data.left(nLen);
		ControlerAction* action = ControlerAction::parseAction(curData);
		onAction(action);
		data = data.right(dataSize - nLen);
		dataSize = data.size();
	}
}

void ControlerConnection::onAction( ControlerAction* action )
{
	byte type = action->getActionType();

	if(m_authentificated)
	{
		switch (type)
		{
		case ControlerAction::MOUSE_MOVE:
			doMoveMouse((MouseMoveAction*)action);
			break;
		case ControlerAction::MOUSE_CLICK:
			doMouseClick((MouseClickAction*)action);
			break;
		case ControlerAction::MOUSE_WHEEL:
			doMouseWheel((MouseWheelAction*)action);
			break;
		case ControlerAction::KEYBOARD:
			doPressKeyboard((KeyboardAction*)action);
			break;
		case ControlerAction::FILE_EXPLORE_REQUEST:
			doFileExploreRequestAction((FileExploreRequestAction*)action);
			break;
		case ControlerAction::TEXT:
			doText((TextAction *)action);
			break;
		case  ControlerAction::SHORTCUTGROUP:
			doShortcutGroupAction((ShortcutGroupAction*)action);
			break;
			// 	case FILE_EXPLORE_RESPONSE:
			// 		return FileExploreResponseAction.parse(data);
			// 	case SCREEN_CAPTURE_REQUEST:
			// 		return ScreenCaptureRequestAction.parse(data);
			// 	case SCREEN_CAPTURE_RESPONSE:
			// 		return ScreenCaptureResponseAction.parse(data);

			// 	default:
			// 		throw new ProtocolException();
		default : break;
		}
	}
	else
	{
		if(type == ControlerAction::AUTHENTIFICATION)
			doAuthentification((AuthentificationAction*)action);
		if(!m_authentificated)
		{
			m_pSocket->disconnectFromHost();
			qDebug()<<"服务器主动断开连接!";
		}
	}

	action->deleteLater();

	// 	if (this.authentificated)
	// 	{
	// 		if (action instanceof ScreenCaptureRequestAction)
	// 		{
	// 			this.screenCapture((ScreenCaptureRequestAction) action);
	// 		}
	// 	}
}

/*
void ControlerConnection::mouseMoveTimeout()
{
	return;
/ *
	if (mousePos.x() == 0 && mousePos.y() == 0) 
	{
		mouseTimer->stop();
		return;
	}
	int moveX = 0;
	int moveY = 0;
	if (mousePos.x())
	{
		mousePos.x() > 0 ? moveX = 1 : moveX = -1;
	}
	if (mousePos.y())
	{
		mousePos.y() > 0 ? moveY = 1 : moveY = -1;
	}
	mousePos.setX(mousePos.x() - moveX);
	mousePos.setY(mousePos.y() - moveY);
	//moveX == 0 ? mousePos.setX(mousePos.x() - moveX) : 0;
	//moveY == 0 ? mousePos.setY(mousePos.y() - moveY) : 0;
	mouse_event(MOUSEEVENTF_MOVE, moveX, moveY, 0, 0);* /
}*/

void ControlerConnection::doMoveMouse( MouseMoveAction* action )
{
	if(!action) return;
/*
	int moveX = qAbs(action->m_moveX);
	int moveY = qAbs(action->m_moveY);

	qDebug()<<action->m_moveX<<action->m_moveY;

	qreal rateX = (8.0 * moveX) / 200.0 + 0.2;
	qreal rateY = (8.0 *moveY ) / 200.0 + 0.2;*/

/*
	if (moveX < 100 && moveY < 100)
	{
		mousePos.setX(action->m_moveX);
		mousePos.setY(action->m_moveY);
		if (!mouseTimer->isActive()) mouseTimer->start();
		return;
	}*/
/*
	if (moveX < 5) action->m_moveX = 0;
	else if (moveX < 10) action->m_moveX > 0 ? action->m_moveX = 1 : action->m_moveX = -1;

	if (moveY < 5) action->m_moveY = 0;
	else if (moveY < 10) action->m_moveY > 0 ? action->m_moveY = 1 : action->m_moveY = -1;*/
	
	mouse_event(MOUSEEVENTF_MOVE, action->m_moveX, action->m_moveY, 0, 0);
}

void ControlerConnection::doMouseClick( MouseClickAction* action )
{
	if(!action) return;
	qDebug()<<"MouseClickEvent:";
	qDebug()<<"Button:"<<action->m_button<<"State:"<<action->m_state;
	DWORD msFlag;

	switch (action->m_button)
	{
	case MouseClickAction::BUTTON_LEFT:
		msFlag = action->m_state ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_LEFTUP;
		break;
	case MouseClickAction::BUTTON_RIGHT:
		msFlag = action->m_state ?  MOUSEEVENTF_RIGHTDOWN : MOUSEEVENTF_RIGHTUP;
		break;
	case MouseClickAction::BUTTON_MIDDLE:
		msFlag = action->m_state ?  MOUSEEVENTF_MIDDLEDOWN : MOUSEEVENTF_MIDDLEUP;
		break;
	default:
		return;
	}
	mouse_event(msFlag, 0, 0, 0, 0);
}

void ControlerConnection::doMouseWheel( MouseWheelAction* action )
{
	if(!action) return;
	char amount = (char)(action->m_amount);
	qDebug()<<"MouseWheelEvent:";
	qDebug()<<"account:"<<(int)amount;
	mouse_event(MOUSEEVENTF_WHEEL, 0, 0, amount * WHEEL_DELTA, 0);
}

void ControlerConnection::doPressKeyboard( KeyboardAction* action )
{
	if(!action) return;
	int vkCode = KeyCodeConverter::convert(action->m_unicode);
	qDebug()<<"KeyPressedEvent";
	qDebug()<<"key:"<<vkCode;

	if(vkCode == VK_CLOSE)
	{
		doCloseActivedWindow();
	}
	else
	{
		keybd_event(vkCode, 0, 0, 0);
		keybd_event(vkCode, 0, KEYEVENTF_KEYUP, 0); 
	}
}

void ControlerConnection::doText(TextAction *action)
{
	if (!action) return;
	QString text = action->m_text;
	qDebug()<<text;
	for (int i = 0; i < text.size(); i++)
	{
		WORD wscan = text.at(i).unicode();
		
		KEYBDINPUT kDown;
		KEYBDINPUT kUp;

		kDown.wVk = kUp.wVk = 0;
		kDown.wScan = kUp.wScan = wscan;

		kDown.dwFlags = KEYEVENTF_UNICODE;
		kUp.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
		//kDown.time = kUp.time = 0;
		//kDown.dwExtraInfo = kUp.dwExtraInfo = (WORD)::GetMessageExtraInfo();

		INPUT inputKeyDown;
		INPUT inputKeyUp;

		inputKeyDown.type = inputKeyUp.type = INPUT_KEYBOARD;
		inputKeyDown.ki = kDown;
		inputKeyUp.ki = kUp;

		INPUT input[2];
		memset(input, 0, 2 * sizeof(INPUT));
		input[0] = inputKeyDown;
		input[1] = inputKeyUp;
		::SendInput(1, input, sizeof(INPUT));
		::SendInput(1, input + 1, sizeof(INPUT));
	}
}

void ControlerConnection::doAuthentification( AuthentificationAction* action )
{
	if(!action) return;

	qDebug()<<"密码验证:";
	qDebug()<<"passWord: "<<action->m_passWord;
	if(action->m_passWord == "aa12")
	{
		m_authentificated = true;
		qDebug()<<"验证成功!";
	}
	else
	{
		qDebug()<<"密码错误！验证失败";
	}
	sendAction(&AuthentificationResponseAction(m_authentificated));
}

void ControlerConnection::doFileExploreRequestAction( FileExploreRequestAction* action )
{	
	if(!action) return;
	QString directory = QDir::toNativeSeparators(action->m_directory);
	QString fileName = QDir::toNativeSeparators(action->m_file);
	qDebug()<<"文件浏览请求:";
	qDebug()<<"dict: "<<directory<<"file:"<<fileName;

	if(directory.isEmpty() && fileName.isEmpty())
	{
		doFileExploreRoots();
		return;
	}
	
	if(directory.isEmpty())
	{
		doFileExplore(fileName);
	}
	else
	{
		QString newDir = directory.endsWith('/') ? directory + fileName : directory + QDir::separator() + fileName;
		doFileExplore(newDir);
	}
}

void ControlerConnection::doFileExploreRoots()
{
	QString empty;
	sendFileExploreResponse(empty, QDir::drives());
}

void ControlerConnection::doFileExplore( QString& filefullName )
{
	if(filefullName.endsWith(".."))
	{
		QString tmp = filefullName.left(filefullName.size() - 2);
		if(QFileInfo(tmp).isRoot())
		{
			doFileExploreRoots();
			return;
		}
	}

	QString dstFullPath = QFileInfo(filefullName).absoluteFilePath();
	QFileInfo file(dstFullPath);

	if (file.exists())
	{
		if (file.isDir())
		{
			sendFileExploreResponse(dstFullPath, file.dir().entryInfoList());
		}
		else
		{
			std::wstring str = QDir::toNativeSeparators(dstFullPath).toStdWString();
			::ShellExecute(NULL, QString("open").toStdWString().c_str(), str.c_str(), NULL, NULL, SW_NORMAL);
		}
	}
	else
	{
		doFileExploreRoots();
	}
}

void ControlerConnection::sendFileExploreResponse(QString& filePath, QFileInfoList& infos )
{
	QStringList contents;

	//如果是根
	QFileInfo cntInfo(filePath);
	if(cntInfo.isRoot()) 
		contents.append("..");

	foreach(QFileInfo curInfo, infos)
	{
		QString name = curInfo.fileName();
		if(name == ".") continue;
		if(name == "..")
		{
			contents.append(name);
			continue;
		}

		QString fullPath = curInfo.absoluteFilePath();
		if(!name.isEmpty())
		{
			if(curInfo.isDir()) 
			{
				if(!QDir::toNativeSeparators(fullPath).endsWith('\\'))
				{
					name += QDir::separator();
				}
			}
		}
		else
		{
			name = fullPath;
		}

		contents.append(QDir::toNativeSeparators(name));
	}

	sendAction(&FileExploreResponseAction(filePath, contents));
}

void ControlerConnection::doCloseActivedWindow()
{
	keybd_event(VK_MENU, 0, 0, 0);
	keybd_event(VK_F4, 0, 0, 0);
	keybd_event(VK_F4, 0, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);
}


void ControlerConnection::doShortcutGroupAction( ShortcutGroupAction* action )
{
	if(!action) return;
	qDebug()<<"ShortcutGroupEvent:";
	int mode = action->m_mode;
	int key1 = KeyCodeConverter::convert(action->m_firstKey);
	int key2 = KeyCodeConverter::convert(action->m_lastKey);
	qDebug()<<"mode:"<<mode<<"Key1:"<<key1<<"Key2"<<key2;

	if(mode == ShortcutGroupAction::SHORTCUT_START)
	{
		keybd_event(key1, 0, 0, 0);
		keybd_event(key2, 0, 0, 0);
		keybd_event(key2, 0, KEYEVENTF_KEYUP, 0);
		if(key1 == VK_LWIN && (key2 == VK_OEM_PLUS || key2 == VK_OEM_MINUS))
		{
			Uiutils::hideZoom();
		}
	}
	else if(mode == ShortcutGroupAction::SHORTCUT_END)
	{
		keybd_event(key1, 0, KEYEVENTF_KEYUP, 0);
		if(key1 == VK_LWIN && (key2 == VK_OEM_PLUS || key2 == VK_OEM_MINUS))
		{
			Uiutils::killZoom();
		}
	}
}
