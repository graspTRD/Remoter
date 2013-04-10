#include "stdafx.h"
#include "fileexplorerequestaction.h"

FileExploreRequestAction::FileExploreRequestAction( QString directory, QString file )
	:m_directory(directory), m_file(file)
{
	m_type = FILE_EXPLORE_REQUEST;
}


FileExploreRequestAction::~FileExploreRequestAction(void)
{

}

FileExploreRequestAction* FileExploreRequestAction::parse( const QByteArray& data )
{

	QByteArray dataStr = data.right(data.size() - 1);

	QString directory = readStringFromData(&dataStr);
	QString file = readStringFromData(&dataStr);
	return new FileExploreRequestAction(directory, file);
}

QByteArray FileExploreRequestAction::toByteArray()
{
	QByteArray outData;
	QDataStream os(&outData, QIODevice::ReadWrite);
	os<<m_type<<m_directory<<m_file;
	return outData;
}

