#ifndef TEXTACTION_H
#define TEXTACTION_H

#include "controleraction.h"

class TextAction : public ControlerAction
{
	Q_OBJECT
public:
	TextAction(const QString& passWord);
	virtual ~TextAction();
	static TextAction* parse(const QByteArray& data);

public:
	QString m_text;
};

#endif //TEXTACTION_H



