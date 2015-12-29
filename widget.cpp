#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    qDebug().noquote() << "Widget" << this->thread()->currentThreadId();
    qRegisterMetaType< ListAddress >("ListAddress");
    qRegisterMetaType< Address >("Address");
//    qDebug().noquote() << "Q_DECLARE_METATYPE(ListAddress)" << qMetaTypeId<ListAddress>();
    ui->lineEditFileName->setText("base_All.csv");
//    ui->lineEditFileName->setText("base_f10_orig.csv");
//    ui->lineEditFileName->setText("base_f10_orig2.csv");
//    ui->lineEditFileName->setText("base_1000.csv");
    ui->comboBox->addItem("base_All.csv");
    ui->comboBox->addItem("base_f10_orig.csv");
    ui->comboBox->addItem("base_f10_orig2.csv");
    ui->comboBox->addItem("base_1000.csv");

    QObject::connect(&_futureWatcher, SIGNAL(finished()),
                     this, SLOT(onProcessOfOpenFinished()));
    connect(ui->comboBox, SIGNAL(currentIndexChanged(QString)),
            ui->lineEditFileName, SLOT(setText(QString)));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
//    runThread();

    runConcurrent2();
}

void Widget::onToDebug(QString obj, QString mes)
{
    QString s =ui->plainTextEdit->toPlainText();
    QString currTime=QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss.zzz");
    QString append = obj+": "+mes;
    s.append("***\n")
     .append("["+currTime+"]\n"+append+"\n");
    qDebug().noquote() << append;
    ui->plainTextEdit->setPlainText(s);
}

void Widget::runConcurrent2()
{
    qDebug().noquote() << "runThread" << this->thread()->currentThreadId();
    QString currTime=QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss.zzz");
    qDebug().noquote() << "BEGIN"
             << currTime
             << this->thread()->currentThreadId();

    QString openFilename(ui->lineEditFileName->text());
    QString name = QFileInfo(openFilename).fileName();

    QProgressDialog *dialog = new QProgressDialog;
    dialog->setWindowTitle(trUtf8("Открываю файл..."));
    dialog->setLabelText(trUtf8("Открывается файл \"%1\". Ожидайте ...")
                         .arg(name));
    dialog->setCancelButtonText(trUtf8("Отмена"));
    QObject::connect(dialog, SIGNAL(canceled()),
                     &_futureWatcher, SLOT(cancel()));
    QObject::connect(&_futureWatcher, SIGNAL(progressRangeChanged(int,int)),
                     dialog, SLOT(setRange(int,int)));
    QObject::connect(&_futureWatcher, SIGNAL(progressValueChanged(int)),
                     dialog, SLOT(setValue(int)));
    QObject::connect(&_futureWatcher, SIGNAL(finished()),
                     dialog, SLOT(deleteLater()));

    CsvWorker *csv = new CsvWorker;
    QObject::connect(&_futureWatcher, SIGNAL(finished()),
                     csv, SLOT(deleteLater()));

    QFuture<ListAddress> f1 = QtConcurrent::run(csv,
                                         &CsvWorker::readFile,
                                         openFilename,
                                         0);
    // Start the computation.
    _futureWatcher.setFuture(f1);
    dialog->exec();

    currTime=QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss.zzz");
    qDebug().noquote() << "END"
             << currTime
             << this->thread()->currentThreadId();
}

void Widget::runSimple()
{
    qDebug().noquote() << "runThread" << this->thread()->currentThreadId();
    QString currTime=QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss.zzz");
    qDebug().noquote() << "BEGIN"
             << currTime
             << this->thread()->currentThreadId();

    QString openFilename(ui->lineEditFileName->text());

    CsvWorker *csvWorker = new CsvWorker;
    csvWorker->setFileName(openFilename);
    csvWorker->setMaxCountRead(0);

    Database *db = new Database;
    db->setBaseName("Base1.db");
    db->createConnection();
    db->dropTable();
    db->createTable();

    connect(csvWorker, SIGNAL(rowReaded(Address)),
            db, SLOT(insertAddress(Address)));
    connect(db, SIGNAL(toDebug(QString,QString)),
            this, SLOT(onToDebug(QString,QString)));

    connect(csvWorker, SIGNAL(finished()),
            db, SLOT(deleteLater()));
    connect(csvWorker, SIGNAL(finished()),
            csvWorker, SLOT(deleteLater()));

    csvWorker->process();

    currTime=QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss.zzz");
    qDebug().noquote() << "END"
             << currTime
             << this->thread()->currentThreadId();
}

void Widget::runThread()
{
    qDebug().noquote() << "runThread" << this->thread()->currentThreadId();
    QString currTime=QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss.zzz");
    qDebug().noquote() << "BEGIN"
             << currTime
             << this->thread()->currentThreadId();


    QString openFilename(ui->lineEditFileName->text());
    QString name = QFileInfo(openFilename).fileName();

    QProgressDialog *dialog = new QProgressDialog;
    dialog->setWindowTitle(trUtf8("Открываю файл..."));
    dialog->setLabelText(trUtf8("Открывается файл \"%1\". Ожидайте ...")
                         .arg(name));
    dialog->setCancelButtonText(trUtf8("Отмена"));

    CsvWorker *csvWorker = new CsvWorker;
    csvWorker->setFileName(openFilename);
    csvWorker->setMaxCountRead(0);

    Database *db = new Database;
    db->setBaseName("Base2.db");
    db->createConnection();
    db->dropTable();
    db->createTable();

    connect(csvWorker, SIGNAL(rowReaded(Address)),
            db, SLOT(insertAddressLater(Address)));
    connect(db, SIGNAL(toDebug(QString,QString)),
            this, SLOT(onToDebug(QString,QString)));

    QThread *thread = new QThread;
    csvWorker->moveToThread(thread);
    db->moveToThread(thread);
    connect(thread, SIGNAL(started()),
            csvWorker, SLOT(process()));
    connect(csvWorker, SIGNAL(finished()),
            thread, SLOT(quit()));
    connect(dialog, SIGNAL(canceled()),
            thread, SLOT(quit()));

    connect(thread, SIGNAL(finished()),
            db, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()),
            csvWorker, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()),
            thread, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()),
            dialog, SLOT(deleteLater()));

    thread->start();
    dialog->exec();

    currTime=QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss.zzz");
    qDebug().noquote() << "END"
             << currTime
             << this->thread()->currentThreadId();

}

void Widget::run()
{
    qDebug().noquote() << "run" << this->thread()->currentThreadId();
    QString currTime=QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss.zzz");
    qDebug().noquote() << "BEGIN"
             << currTime
             << this->thread()->currentThreadId();

    QString openFilename(ui->lineEditFileName->text());
    QString name = QFileInfo(openFilename).fileName();

    QProgressDialog *dialog = new QProgressDialog;
    dialog->setWindowTitle(trUtf8("Открываю файл..."));
    dialog->setLabelText(trUtf8("Открывается файл \"%1\". Ожидайте ...")
                         .arg(name));
    dialog->setCancelButtonText(trUtf8("Отмена"));
    QObject::connect(dialog, SIGNAL(canceled()),
                     &_futureWatcher, SLOT(cancel()));
    QObject::connect(&_futureWatcher, SIGNAL(progressRangeChanged(int,int)),
                     dialog, SLOT(setRange(int,int)));
    QObject::connect(&_futureWatcher, SIGNAL(progressValueChanged(int)),
                     dialog, SLOT(setValue(int)));
    QObject::connect(&_futureWatcher, SIGNAL(finished()),
                     dialog, SLOT(deleteLater()));
    CsvWorker *csv = new CsvWorker;
    QObject::connect(&_futureWatcher, SIGNAL(finished()),
                     csv, SLOT(deleteLater()));

//    QFuture<ListAddress> f1 = QtConcurrent::run(readAndParseDB,
//                                             openFilename,
//                                             0);
    QFuture<void> f1 = QtConcurrent::run(csv,
                                         &CsvWorker::process,
                                         openFilename,
                                         0);
    // Start the computation.
//    _futureWatcher.setFuture(f1);
    dialog->exec();

    currTime=QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss.zzz");
    qDebug().noquote() << "END"
             << currTime
             << this->thread()->currentThreadId();
}

void Widget::onProcessOfParsingFinished()
{
    if(_futureWatcher2.isFinished()
            && !_futureWatcher2.isCanceled())
    {
        qDebug().noquote() << "finish parsing good"
                           << _addrs.size();

        QString currTime=QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss.zzz");
        qDebug().noquote() << "Insert in Base BEGIN"
                 << currTime
                 << this->thread()->currentThreadId();

        Database *db = new Database;
        db->setBaseName("Base2.db");
        db->createConnection();
        db->dropTable();
        db->createTable();

        ListAddress::iterator it=_addrs.begin();
        for(;it!=_addrs.end();it++)
        {
            db->insertAddressWithCheck(*it);
        }

        currTime=QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss.zzz");
        qDebug().noquote() << "Insert in Base END"
                 << currTime
                 << this->thread()->currentThreadId();

    }
    else
    {
        qDebug().noquote() << "finish bad";

    }
}
void parsingAddress(Address &a)
{
    QString line = a.getRawAddressString();
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
        a.setBuildId(caps.at(19));
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
        a.setBuild(caps.at(23));
        a.setKorp(caps.at(28));
        a.setLitera(caps.at(25)+caps.at(26)+caps.at(29));

//        if(a.getBuildId()==14042969)
//        {
//            int n=0;
//            foreach (QString s, caps) {
//                qDebug().noquote() << "[" << n << "]=" << s;
//                n++;
//            }
//            qDebug().noquote() << a.toString(PARSED);
//            assert(0);
//        }
    }

}

void Widget::onProcessOfOpenFinished()
{
    if(_futureWatcher.isFinished()
            && !_futureWatcher.isCanceled())
    {
        qDebug().noquote() << "finish good";
        _addrs = _futureWatcher.future().result();
        qDebug().noquote() << "convert good"
                            << _addrs.size();
        if(!_addrs.isEmpty())
        {
                QString currTime=QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss.zzz");
                qDebug().noquote() << "onProcessOfOpenFinished BEGIN"
                         << currTime
                         << this->thread()->currentThreadId();

                QProgressDialog *dialog = new QProgressDialog;
                dialog->setWindowTitle(trUtf8("Обрабатываем файл..."));
                dialog->setLabelText(trUtf8("Обрабатывается файл. Строк: \"%1\". Ожидайте ...")
                                     .arg(_addrs.size()));
                dialog->setCancelButtonText(trUtf8("Отмена"));
                QObject::connect(dialog, SIGNAL(canceled()),
                                 &_futureWatcher2, SLOT(cancel()));
                QObject::connect(&_futureWatcher2, SIGNAL(progressRangeChanged(int,int)),
                                 dialog, SLOT(setRange(int,int)));
                QObject::connect(&_futureWatcher2, SIGNAL(progressValueChanged(int)),
                                 dialog, SLOT(setValue(int)));
                QObject::connect(&_futureWatcher2, SIGNAL(finished()),
                                 dialog, SLOT(deleteLater()));
//                CsvWorker *csv = new CsvWorker;
//                QObject::connect(&_futureWatcher2, SIGNAL(finished()),
//                                 csv, SLOT(deleteLater()));

                QObject::connect(&_futureWatcher2, SIGNAL(finished()),
                                 this, SLOT(onProcessOfParsingFinished()));

                QFuture<void> f1 = QtConcurrent::map(_addrs,
                                                     parsingAddress
                                                     );

//                QFuture<Address> f1 = QtConcurrent::mapped(rows.begin(), rows.end(),
//                                                     &CsvWorker::parsingLine);
                // Start the computation.
                _futureWatcher2.setFuture(f1);
                dialog->exec();

                currTime=QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss.zzz");
                qDebug().noquote() << "onProcessOfOpenFinished END"
                         << currTime
                         << this->thread()->currentThreadId();
        }
    }
    else
        qDebug().noquote() << "finish bad";
}
