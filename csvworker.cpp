#include "csvworker.h"

CsvWorker::CsvWorker(QObject *parent) :
    QObject(parent),
    _maxCount(0)
{
    qDebug() << "CsvWorker constructor" << this->thread()->currentThreadId();
}


CsvWorker::~CsvWorker()
{
    qDebug() << "CsvWorker destructor" << this->thread()->currentThreadId();
}


void CsvWorker::setFileName(QString fname)
{
    _fname = fname;
}

void CsvWorker::setMaxCountRead(int maxCount)
{
    if(maxCount>0)
        _maxCount=maxCount;
}

void CsvWorker::process()
{
    process(_fname, _maxCount);
}

Address CsvWorker::parsingLine(QString &line)
{
    Address a;
    a.setRawAddress(line);
    a.setCorrect(false);
    int n1=line.indexOf('(');
    if (n1>0 && (line.indexOf(')',n1)>0))
    {
        int n2=line.indexOf(')', n1);
        int n3=n2-n1;
        a.setAdditional(line.mid(n1+1, n3-1));
        line.remove(n1, n3+1);
    }

    QRegExp reg(BaseRegPattern, Qt::CaseInsensitive);
    int pos=reg.indexIn(line);
    if(pos==-1)
    {
        a.setCorrect(false);
    }
    else
    {
        a.setCorrect(true);
        QStringList caps = reg.capturedTexts();
        a.setStreetId(caps.at(1));
        a.setTypeOfFSubj(caps.at(4));
        a.setFsubj(caps.at(3));
        a.setDistrict(caps.at(7));
        a.setTypeOfCity1(caps.at(9));
        a.setCity1(caps.at(10));
        a.setTypeOfCity2(caps.at(12));
        a.setCity2(caps.at(13));
        a.setStreet(caps.at(16));
        a.setTypeOfStreet(caps.at(17));
        a.setBuildId(caps.at(19));
        a.setBuild(caps.at(23));
        a.setKorp(caps.at(28));
        a.setLitera(caps.at(26)+caps.at(29));
    }
    return a;
}

void CsvWorker::parsingAddress(Address &a)
{
    QString line = a.getRawAddressString();
    a=parsingLine(line);
}

ListAddress CsvWorker::parsingList(QStringList &rows)
{
    qDebug().noquote() << "CsvWorker parsing. Count=" << rows.count() << QThread::currentThreadId();
    ListAddress res;
    foreach (QString line, rows) {
        res.append(parsingLine(line));
    }
    return res;
}

ListAddress CsvWorker::readFile(QString fname, int maxCount)
{
    qDebug().noquote() << "CsvWorker readFile BEGIN" << QThread::currentThreadId();
    QFile file1(fname);
    ListAddress addrs;
    if (!file1.open(QIODevice::ReadOnly))
    {
        qDebug().noquote() << "Ошибка открытия для чтения";
        return addrs;
    }

    QTextStream in(&file1);
    QTextCodec *defaultTextCodec = QTextCodec::codecForName("Windows-1251");
    if (defaultTextCodec)
      in.setCodec(defaultTextCodec);
    int nRow=0;
//    QStringList res;
    while (!in.atEnd())
    {
        QString line = in.readLine();
        if(!line.isEmpty())
        {
            if(nRow==0)
            {
//                qDebug().noquote() << "H:" << line;
            }
            else
            {
//                res.append(line);
                Address a;
                a.setRawAddress(line);
                addrs.append(a);
            }
            nRow++;
        }
        //оставливаем обработку если получено нужное количество строк
        if(maxCount>0 && nRow >= maxCount)
            break;
    }
    file1.close();
//    return QVariant::fromValue(res);
    qDebug().noquote() << "CsvWorker readFile END" << QThread::currentThreadId();
    return addrs;
}

void CsvWorker::process(QString fname, int maxCount)
{
    qDebug().noquote() << "CsvWorker process" << QThread::currentThreadId();
    QFile file1(fname);
    if (!file1.open(QIODevice::ReadOnly))
    {
        qDebug().noquote() << "Ошибка открытия для чтения";
        return;
    }
    int approxRows = file1.size()/90; // 90байт - столько примерно занимает одна строка в файле
    if(maxCount>0)
        emit countRows(maxCount);
    else
        emit countRows(approxRows);

    QTextStream in(&file1);
    QTextCodec *defaultTextCodec = QTextCodec::codecForName("Windows-1251");
    if (defaultTextCodec)
      in.setCodec(defaultTextCodec);
    int nRow=0;
    while (!in.atEnd())
    {
        QString line = in.readLine();
//        QThread::msleep(500);

        if(!line.isEmpty())
        {
            if(nRow==0)
            {
//                qDebug().noquote() << "H:" << line;
                emit headReaded(line);
            }
            else
            {
                Address a;
                a.setRawAddress(line);
                a.setCorrect(false);
                int n1=line.indexOf('(');
                if (n1>0 && (line.indexOf(')',n1)>0))
                {
                    int n2=line.indexOf(')', n1);
                    int n3=n2-n1;
                    a.setAdditional(line.mid(n1+1, n3-1));
                    line.remove(n1, n3+1);
                }
//                qDebug().noquote() << "R:" << a.toString(PARSED);
//                assert(0);

                QRegExp reg(BaseRegPattern, Qt::CaseInsensitive);
                int pos=reg.indexIn(line);
                if(pos==-1)
                {
//                    qDebug().noquote() << "![" << nRow << "]:" << line;
                    a.setCorrect(false);
                }
                else
                {
//                    qDebug().noquote() << "+[" << nRow << "]:" << line;
                    a.setCorrect(true);
                    QStringList caps = reg.capturedTexts();
//                    int i=0;
//                    foreach (QString s, caps) {
//                        qDebug().noquote() << "+[" << nRow << "]:" << i << ":" << s;
//                        i++;
//                    }
                    a.setStreetId(caps.at(1));
                    a.setTypeOfFSubj(caps.at(4));
                    a.setFsubj(caps.at(3));
                    a.setDistrict(caps.at(7));
                    a.setTypeOfCity1(caps.at(9));
                    a.setCity1(caps.at(10));
                    a.setTypeOfCity2(caps.at(12));
                    a.setCity2(caps.at(13));
                    a.setStreet(caps.at(16));
                    a.setTypeOfStreet(caps.at(17));
                    a.setBuildId(caps.at(19));
                    a.setBuild(caps.at(23));
                    a.setKorp(caps.at(28));
                    a.setLitera(caps.at(26)+caps.at(29));
                }


//                qDebug().noquote() << "R:" << a.toString(PARSED);
                emit rowReaded(a);
                emit rowReaded(nRow-1, line);
            }
            if(nRow%10000==0)
                qDebug().noquote() << "P:" << nRow;
            nRow++;
        }
        //оставливаем обработку если получено нужное количество строк
        if(maxCount>0 && nRow >= maxCount)
            break;
    }
    file1.close();
    emit rowsReaded(nRow-1); //-1 - 1-я строка шапка (нумерация с 0)
    emit finished();
    return;
}
