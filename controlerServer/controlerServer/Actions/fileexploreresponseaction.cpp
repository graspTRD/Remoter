#include "stdafx.h"
#include "fileexploreresponseaction.h"

FileExploreResponseAction::FileExploreResponseAction( QString& directory, const QStringList& contents )
	:m_directory(directory)
{
	m_type = FILE_EXPLORE_RESPONSE;
	m_contents.append(contents);
}

FileExploreResponseAction::~FileExploreResponseAction()
{
}

QByteArray FileExploreResponseAction::toByteArray()
{
	QByteArray outData;
	QDataStream os(&outData, QIODevice::ReadWrite);

	QByteArray utfData = m_directory.toUtf8();
	os<<m_type<<(ushort)utfData.size();
	os.writeRawData(utfData, utfData.length());
	os<<m_contents.length();

	foreach(QString item, m_contents )
	{
		utfData = item.toUtf8();
		os<<(ushort)utfData.size();
		os.writeRawData(utfData, utfData.length());
	}
	return outData;
}

FileExploreResponseAction* FileExploreResponseAction::parse( const QByteArray& data )
{
	QStringList files;
	QByteArray iData = data.right(data.size() - 1);
	QString directory = readStringFromData(&iData);//iData 发生了改变
	int filesSize = GetDataFromStream<int>(&iData.left(2));

   QByteArray curData = iData.right(iData.size() - 2);
	for (int i = 0; i < filesSize; i++)
	{
		QString curStr = readStringFromData(&curData);
		files.append(curStr);
	}

	return new FileExploreResponseAction(directory, files);
}
