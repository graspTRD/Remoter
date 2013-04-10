#pragma once

#include "controleraction.h"
class FileExploreRequestAction : public ControlerAction
{
	Q_OBJECT
public:
	FileExploreRequestAction(QString directory, QString file);
	~FileExploreRequestAction(void);
	virtual QByteArray toByteArray();
	static FileExploreRequestAction* parse(const QByteArray& data);

public:
	 QString m_directory;
	 QString m_file;
};

