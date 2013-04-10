#ifndef AUTHENTIFICATIONRESPONSEACTION_H
#define AUTHENTIFICATIONRESPONSEACTION_H

#include "controleraction.h"

class AuthentificationResponseAction : public ControlerAction
{
	Q_OBJECT
public:
	AuthentificationResponseAction(bool authentificated);
	virtual ~AuthentificationResponseAction();
	virtual QByteArray toByteArray();

public:
	bool m_bAuthentificated;
};

#endif //AUTHENTIFICATIONRESPONSEACTION_H