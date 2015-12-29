#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QObject>
#include <QString>
#include <QVector>
#include <QList>
#include <QThread>
#include <QMutex>
#include <QMap>
#include <QSet>
#include <QDateTime>
#include <QTextCodec>
#include <QTextStream>
#include <QFileDialog>
#include <QFile>
#include <QtConcurrentRun>
#include <QtConcurrentMap>
#include <QFutureWatcher>
#include <QFuture>
#include <QProgressDialog>
#include <QMessageBox>
#include <assert.h>
#include "address.h"
#include "csvworker.h"
#include "database.h"

typedef QList< Address > ListAddress;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void run();
    void runThread();
    void runSimple();
    void runConcurrent2();
    void onToDebug(QString obj, QString mes);

private slots:
    void on_pushButton_clicked();
    void onProcessOfOpenFinished();
    void onProcessOfParsingFinished();

private:
    Ui::Widget *ui;
    QFutureWatcher<ListAddress>     _futureWatcher;
    QFutureWatcher<void>     _futureWatcher2;
    ListAddress _addrs;
//    CsvWorker _csv;
//    Database _db;
};

#endif // WIDGET_H
