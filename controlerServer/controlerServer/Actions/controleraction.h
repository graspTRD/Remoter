#ifndef CONTROLERACTION_H
#define CONTROLERACTION_H

#include<QObject>
#include <QByteArray>
#include "stdafx.h"

class ControlerAction : public QObject
{
	Q_OBJECT
public:
	ControlerAction(QObject* parent = 0);
	virtual ~ControlerAction();
	bool isActionValid() const;
	byte getActionType() const;
	virtual QByteArray toByteArray();

	static ControlerAction* parseAction(const QByteArray& data );
	static QString readStringFromData(QByteArray* data);
	//public void toDataOutputStream(DataOutputStream dos);

public:
	static const byte MOUSE_MOVE = 0;
	static const byte MOUSE_CLICK = 1;
	static const byte MOUSE_WHEEL = 2;
	static const byte KEYBOARD = 3;
	static const byte AUTHENTIFICATION = 4;
	static const byte AUTHENTIFICATION_RESPONSE = 5;
	static const byte SCREEN_CAPTURE_REQUEST = 6;
	static const byte SCREEN_CAPTURE_RESPONSE = 7;
	static const byte FILE_EXPLORE_REQUEST = 8;
	static const byte FILE_EXPLORE_RESPONSE = 9;
	static const byte TEXT = 10;
	static const byte SHORTCUTGROUP = 11;

protected:
	bool m_bValid;
	byte m_type;
};



#endif //CONTROLERACTION_H


