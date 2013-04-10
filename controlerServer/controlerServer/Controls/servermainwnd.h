#ifndef SERVERMAINWND_H
#define SERVERMAINWND_H
#include <QtGui/QMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPoint>

class ConfigWnd : public QMainWindow
{
	Q_OBJECT

public:
	ConfigWnd(QWidget *parent = 0, Qt::WFlags flags = 0);
	~ConfigWnd();

signals:
	void windowHided();

protected:
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

private:
	void initialize();
	void closeEvent(QCloseEvent * event);
	QLineEdit* m_passwordEdit;
	QLabel* m_stateLabel;
	QPoint mouseMovePos;
};


#endif //SERVERMAINWND_H