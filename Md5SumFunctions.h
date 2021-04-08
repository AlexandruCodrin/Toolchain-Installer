#pragma once
#include <QDesktopServices>
#include <QUrl>
#include <QProcess>
#include <QtWidgets/QMainWindow>
#include <QString>
#include <QFile>
#include <QDebug>
#include <QtCore>
#include <QCryptographicHash>
#include <QCheckBox>
#include <QProgressBar>
#include <QObject>
#include <QtWidgets/QMainWindow>
#include "ui_ces_tool.h"

class Md5SumFunctions : public QObject
{
public:
	Md5SumFunctions();
	Q_OBJECT
public slots:
	QString md5sum(QString path) 
	{
		qDebug() << "md5sum function started";
		qDebug() << "function is in" << QThread::currentThread()<<" thread";
		
		QString dir(path);
		QByteArray hash;
		QDirIterator iterator(dir, QDirIterator::Subdirectories);
		QCryptographicHash crypto(QCryptographicHash::Keccak_512);
		while (iterator.hasNext())
		{
			QFile file(iterator.next());
			if (file.open(QIODevice::ReadOnly))
			{
				//	qDebug() << "Opened:" << file.fileName() << endl;
				while (!file.atEnd())
				{
					crypto.addData(file.read(4096));
					//crypto.addData(file.readAll());
				}

			}
			else
			{
				//	qDebug() << "Can't open" << file.fileName() << file.errorString() << endl;
			}
		}
		hash.append(crypto.result());
		QString DataAsString = QTextCodec::codecForMib(1015)->toUnicode(hash);
		//	qDebug() << DataAsString;
		//	qDebug() << hash.toHex();
		//	qDebug() << qPrintable(hash);
		//qDebug() << hash.toHex();
		//return hash.toHex(); //this is what you need
		/*char* da = hash.data();
		while (*da)
		{
			qDebug() << "[" << *da << "]" << endl;
			da++;
		}*/
		QString result = hash.toHex();
		return result;
		//emit resultReady(result);
	}
signals:
	void resultReady(QString& result);
};
class Controller : public QObject
{
	Q_OBJECT
	QThread workerThread;
public:
	Controller()
	{
		Md5SumFunctions* worker = new Md5SumFunctions;
		worker->moveToThread(&workerThread);
		connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
		connect(this, & Controller::operate, worker, &Md5SumFunctions::md5sum);
		//connect(worker, &Md5SumFunctions::resultReady, this, &Controller::handleResults);
	}
	~Controller()
	{
		workerThread.quit();
		workerThread.wait();
	}
public slots:
	//void handleResults(QString &);
signals:
	void operate(QString);
};

