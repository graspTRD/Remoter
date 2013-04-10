#pragma once

#include "controleraction.h"
#include <QStringList>

class FileExploreResponseAction : public ControlerAction
{
	Q_OBJECT
public:
	FileExploreResponseAction(QString& directory, const QStringList& contents);
	virtual ~FileExploreResponseAction();
	virtual QByteArray toByteArray();
	static FileExploreResponseAction* parse(const QByteArray& data);

public:
	QString m_directory;
	QStringList m_contents;
};