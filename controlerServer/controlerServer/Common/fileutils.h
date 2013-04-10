#ifndef FILEUTILS_H
#define FILEUTILS_H
#include <QObject>
#include <QStringList>
#include <QFileInfo>

class FileUtils : public QObject
{
	Q_OBJECT
public:
	Q_INVOKABLE static bool exists(const QString& fileName);

public:
	FileUtils();
	virtual ~FileUtils();
	static QString getAppInstancePath() ;	
	static QString readAllText(const QString &fileName);
	static QString getFullPath(const QString& fileName);
	static QString getFileVersion(const QString& fileName);
	static QString getProductName(const QString& fileName);
	static QString getFileDescription(const QString& fileName);
	static QString getFileInfo(const QString& fileName, const QString& whatInfo);
	static QByteArray readLocalFile(const QString& path);

	static bool writeText(const QString& fileName, QString& content);
	static bool existsFullPath(const QString& fileName);
	static bool makeDir(const QString& strDir);
	static bool makePath(const QString& strPath);
	static void rmPath(const QString& strDir);
	static void rmPath(const QString& strDir, const QString& exceptDir);
	static bool copyFile(const QString& srcFile, const QString& destFile);
	static bool saveFile(const QByteArray* content, const QString& fileName, bool bCover = true);
	static bool renameFile(const QString& fileName, const QString& newName );
	static bool createFile(const QString& fileName, int initSize = 0, bool bForce = false);
	static bool rmFile(const QString& fileName);
	static bool isAbsolute(const QString& filePath);
	
	static int fileCount(const QString& filePath);
	static QStringList getFileList(const QString& dir);
private:
	static void delDir(QFileInfo fileList);
	static void delDir(QFileInfo rmFile, const QString& exceptDir);
	static void fileCounter(const QString& filePath, int* pCounter);
	static QString readAllTextFullPath(const QString& fileName);
};


#endif // FILEUTILS_H
