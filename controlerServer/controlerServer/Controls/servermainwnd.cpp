#include "stdafx.h"
#include "servermainwnd.h"
#include "controlerapplication.h"

#include <QLabel>
#include <QCloseEvent>
#include <QHostInfo>
#include <QGraphicsDropShadowEffect>
#include <QPushButton>


ConfigWnd::ConfigWnd(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	initialize();
}

ConfigWnd::~ConfigWnd()
{

}

void ConfigWnd::closeEvent( QCloseEvent * event )
{
	event->ignore();
	hide();
	emit windowHided();
}

void ConfigWnd::initialize()
{
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	this->setAutoFillBackground(true);
	this->setFixedSize(520, 420);
	QPixmap bgPix(520, 420);
	bgPix.fill(Qt::transparent);
	QPalette bgPalette;
	bgPalette.setBrush(QPalette::Window, QBrush(bgPix));
	this->setPalette(bgPalette);

	//设置背景;
	QPixmap pix(500, 400);
	pix.fill(Qt::transparent);
	pix.load(":/Controler/Resources/window.png");

	QWidget* mainWidget = new QWidget();
	mainWidget->setFixedSize(500, 400);

	QPalette palette;
	palette.setBrush(QPalette::Window, QBrush(pix));
	mainWidget->setAutoFillBackground(true);
	mainWidget->setPalette(palette);

	QHBoxLayout *hlayout = new QHBoxLayout;
	QPushButton *cancelButton = new QPushButton();
	cancelButton->setStyleSheet("QPushButton{border-image:url(:/Controler/Resources/cancel.png);}");
	cancelButton->setFixedSize(18, 18);
	cancelButton->setCursor(QCursor(Qt::PointingHandCursor));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(hide()));
	hlayout->addStretch(1);
	hlayout->addWidget(cancelButton);

	QString hostString;

	QHostInfo info=QHostInfo::fromName(QHostInfo::localHostName());

	foreach(QHostAddress address, info.addresses())
	{
		if (address.protocol() == QAbstractSocket::IPv4Protocol)
		{
			hostString.append(address.toString());
			break;
		}
	}
	QLabel *ipLbl = new QLabel();
	ipLbl->setText("本机地址:");
	ipLbl->setStyleSheet("QLabel{font-size:24px;color:#666666;}");

	QLabel *label = new QLabel();
	label->setText(hostString);
	label->setStyleSheet("QLabel{font-size:34px;color:#c77601;}");

	QVBoxLayout* outLay = new QVBoxLayout();
	//outLay->addLayout(hlayout);
	outLay->addWidget(ipLbl, 0, Qt::AlignLeft);
	outLay->addWidget(label, 0, Qt::AlignCenter);
	outLay->setAlignment(Qt::AlignCenter);

	QVBoxLayout *mLayout = new QVBoxLayout();
	mLayout->addLayout(hlayout);
	mLayout->addLayout(outLay);
/*
	QHBoxLayout* stateLay = new QHBoxLayout();
	m_stateLabel = new QLabel("连接状态：无连接.");
	stateLay->addWidget(m_stateLabel);
	stateLay->addStretch();

	QHBoxLayout* psdLay = new QHBoxLayout();
	
	QLabel* passWord = new QLabel(this);
	passWord->setText("连接密码：");
	m_passwordEdit = new QLineEdit(this);
	psdLay->addWidget(passWord);
	psdLay->addWidget(m_passwordEdit);

	outLay->addStretch(1);
	outLay->addLayout(stateLay);
	outLay->addLayout(psdLay);
	outLay->addStretch(1);*/
	mainWidget->setLayout(mLayout);

	this->setCentralWidget(mainWidget);

	QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
	effect->setBlurRadius(20);
	mainWidget->setGraphicsEffect(effect);
}

void ConfigWnd::mouseMoveEvent(QMouseEvent *event)//鼠标按下并移动则移动不规则窗体;
{
	if(mouseMovePos != QPoint(0, 0))
	{
		move(geometry().x() + event->globalPos().x() - mouseMovePos.x(), geometry().y() + event->globalPos().y() - mouseMovePos.y());
		mouseMovePos = event->globalPos();
	}
}

void ConfigWnd::mousePressEvent(QMouseEvent *event)
{
	mouseMovePos = event->globalPos();
}

void ConfigWnd::mouseReleaseEvent(QMouseEvent *event)
{
	mouseMovePos = QPoint(0, 0);
}