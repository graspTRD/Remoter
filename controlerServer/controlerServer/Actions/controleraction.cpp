#include "stdafx.h"
#include "controleraction.h"
#include "mousemoveaction.h"
#include "mouseclickaction.h"
#include "mousewheelaction.h"
#include "keyboardaction.h"
#include "authentificationaction.h"
#include "fileexplorerequestaction.h"
#include "textaction.h"
#include "shortcutgroupaction.h"

ControlerAction::ControlerAction(QObject* parent /*= 0*/)
	:QObject(parent), m_bValid(false), m_type(-1)
{

}

ControlerAction::~ControlerAction()
{
}

bool ControlerAction::isActionValid() const
{
	return this->m_bValid;
}

ControlerAction* ControlerAction::parseAction(const QByteArray& data )
{
	byte type = static_cast<byte>(data.at(0));
	switch (type)
	{
	case MOUSE_MOVE:
		return MouseMoveAction::parse(data);
	case MOUSE_CLICK:
		return MouseClickAction::parse(data);
	case MOUSE_WHEEL:
		return MouseWheelAction::parse(data);
	case KEYBOARD:
		return KeyboardAction::parse(data);
	case AUTHENTIFICATION:
		return AuthentificationAction::parse(data);
 	case FILE_EXPLORE_REQUEST:
 		return FileExploreRequestAction::parse(data);
	case TEXT:
		return TextAction::parse(data);
	case SHORTCUTGROUP:
		return ShortcutGroupAction::parse(data);
		// 	case SCREEN_CAPTURE_REQUEST:
		// 		return ScreenCaptureRequestAction.parse(data);
		// 	case AUTHENTIFICATION_RESPONSE:
		// 		return AuthentificationResponseAction.parse(data);
		// 	case SCREEN_CAPTURE_RESPONSE:
		// 		return ScreenCaptureResponseAction.parse(data);
		// 	case FILE_EXPLORE_RESPONSE:
		// 		return FileExploreResponseAction.parse(data);
		// 	default:
		// 		throw new ProtocolException();
	default :
		return NULL;
	}
}

byte ControlerAction::getActionType() const
{
	return m_type;
}


QByteArray ControlerAction::toByteArray()
{
	return QByteArray();
}

QString ControlerAction::readStringFromData(QByteArray* data)
{
	Q_ASSERT(data);
	ushort size = GetDataFromStream<ushort>(&data->left(2));
	QString str = QString::fromUtf8(data->mid(2, size));
	*data = data->right(data->size() - 2 - size);
	return str;
}
