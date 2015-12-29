#ifndef CSVWORKER_H
#define CSVWORKER_H

#include <QObject>
#include <QFile>
#include <QTextCodec>
#include <QString>
#include <QStringList>
#include "defines.h"
#include "address.h"

typedef QList< Address > ListAddress;

class CsvWorker : public QObject
{
    Q_OBJECT
public:
    explicit CsvWorker(QObject *parent = 0);
    ~CsvWorker();
    void setFileName(QString fname);
    void setMaxCountRead(int maxCount);

signals:
    void rowReaded(int rowNumber, QString row);
    void rowReaded(Address a);
    void rowReaded(int rowNumber, QStringList row);
    void rowsReaded(int count);
    void countRows(int count);
    void headReaded(QStringList head);
    void headReaded(QString head);
    void finished();

public slots:
    void process();
    void process(QString fname, int maxCount);
//    void stringToAddressList(QStringList &rows, ListAddress &addrs);
//    QStringList readFile2(QString fname, int maxCount);
    ListAddress readFile(QString fname, int maxCount);
    ListAddress parsingList(QStringList &rows);
    Address parsingLine(QString &line);
    void parsingAddress(Address &a);

private:
    QString _fname;
    int _maxCount;

};

#endif // CSVWORKER_H
