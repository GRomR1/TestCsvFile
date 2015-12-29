#ifndef DEFINES_H
#define DEFINES_H

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
#include <QTextStream>
#include <assert.h>

const QString TableName = "base1";

const int MAX_OPEN_ROWS=0;
//const int MAX_OPEN_IN_ROWS=0; //moved in ExcelWidget

const QString BaseRegPattern =
        //old <23.12.2015
//        "\"([0-9]*)\";\"(г*\\.* *[^ ]*( обл\\.| АО\\.| Респ\\.))*(, )*((.*) р-н)*(, )*((п\\/о\\.) ([^,]*))*,* *((д\\.|ж\\/д_рзд\\.|рп\\.|ж\\/д_ст\\.|ст\\.|высел\\.|х\\.|казарма\\.|м\\.|жилрайон\\.|нп\\.|снт\\.|г\\.|дп\\.|снт\\.|п\\.|пст\\.|п\\/ст\\.|пгт\\.|п\\/гт\\.|с\\.|тер\\.|массив\\.) ([^,]*))*(, )*((д\\.|ж\\/д_рзд\\.снт\\.|г\\.|дп\\.|нп\\.|мкр\\.|жилзона\\.|снт\\.|п\\.|пст\\.|п\\/ст\\.|пгт\\.|п\\/гт\\.|с\\.|тер\\.|массив\\.)( [^,]*))*(, )*((.* )(ул\\.|пер\\.|наб\\.|ш\\.|линия\\.|казарма\\.|б-р\\.|тер\\.|уч-к\\.|пр-кт\\.|сад\\.|остров\\.|канал\\.|дор\\.|км\\.|городок\\.|тракт\\.|гск\\.|просек\\.|переезд\\.|заезд\\.|парк\\.|площадь|проезд\\.|туп\\.|м\\.|проток\\.|ст\\.|вал\\.|п\\.|спуск\\.|пл-ка\\.|д\\.|коса\\.|п\\/ст\\.|пр\\.|пл\\.|мкр\\.|аллея\\.|кв-л\\.|ж\\/д_будка\\.|х\\.|рзд\\.|тск\\.|сквер\\.|стр\\.|снт\\.|мост\\.|метро\\.))*(, )*\";\"([0-9]*)\";\"((д\\.|ая\\.|ая|нетр\\.|нетр|а\\\\я\\.|а\\\\я|а\\/я\\.|а\\/я)*(([0-9Х][-\\\\\\/0-9]*)([\\\\\\/-]([А-Яа-я])*)*))*([^\"]*)\";\"(([0-9]*)([А-Яа-я]*))*\"";

//        "\"([0-9]*)\";\"( *([^ ]*)( обл\\.| АО\\.| Респ\\.))*(, )*((.*) р-н)*,* *((п\\/о\\.|д\\.|ж\\/д_рзд\\.|рп\\.|ж\\/д_ст\\.|ст\\.|высел\\.|х\\.|казарма\\.|м\\.|жилрайон\\.|нп\\.|снт\\.|г\\.|дп\\.|снт\\.|п\\.|пст\\.|п\\/ст\\.|пгт\\.|п\\/гт\\.|с\\.|тер\\.|массив\\.) ([^,]*))*,* *((п\\/о\\.|д\\.|ж\\/д_рзд\\.снт\\.|г\\.|дп\\.|нп\\.|мкр\\.|жилзона\\.|снт\\.|п\\.|пст\\.|п\\/ст\\.|пгт\\.|п\\/гт\\.|с\\.|тер\\.|массив\\.)( [^,]*))*(, )*((.* )(ул\\.|пер\\.|наб\\.|ш\\.|линия\\.|казарма\\.|б-р\\.|тер\\.|уч-к\\.|пр-кт\\.|сад\\.|остров\\.|канал\\.|дор\\.|км\\.|городок\\.|тракт\\.|гск\\.|просек\\.|переезд\\.|заезд\\.|парк\\.|площадь|проезд\\.|туп\\.|м\\.|проток\\.|ст\\.|вал\\.|п\\.|спуск\\.|пл-ка\\.|д\\.|коса\\.|п\\/ст\\.|пр\\.|пл\\.|мкр\\.|аллея\\.|кв-л\\.|ж\\/д_будка\\.|х\\.|рзд\\.|тск\\.|сквер\\.|стр\\.|снт\\.|мост\\.|метро\\.))*(, )*\";\"([0-9]*)\";\"((д\\.|ая\\.|ая|нетр\\.|нетр|а\\\\я\\.|а\\\\я|а\\/я\\.|а\\/я)*(([0-9Х][-\\\\\\/0-9]*)([\\\\\\/-]([А-Яа-я])*)*))*([^\"]*)\";\"(([0-9]*)([А-Яа-я]*))*\"";
//to lower case ONLY:
          "\"([0-9]*)\";\"( *([^ ]*)( обл\\.| ао\\.| респ\\.))*(, )*((.*) р-н)*,* *((п\\/о\\.|д\\.|ж\\/д_рзд\\.|рп\\.|ж\\/д_ст\\.|ст\\.|высел\\.|х\\.|казарма\\.|м\\.|жилрайон\\.|нп\\.|снт\\.|г\\.|дп\\.|снт\\.|п\\.|пст\\.|п\\/ст\\.|пгт\\.|п\\/гт\\.|с\\.|тер\\.|массив\\.) ([^,]*))*,* *((п\\/о\\.|д\\.|ж\\/д_рзд\\.снт\\.|г\\.|дп\\.|нп\\.|мкр\\.|жилзона\\.|снт\\.|п\\.|пст\\.|п\\/ст\\.|пгт\\.|п\\/гт\\.|с\\.|тер\\.|массив\\.)( [^,]*))*(, )*((.* )(ул\\.|пер\\.|наб\\.|ш\\.|линия\\.|казарма\\.|б-р\\.|тер\\.|уч-к\\.|пр-кт\\.|сад\\.|остров\\.|канал\\.|дор\\.|км\\.|городок\\.|тракт\\.|гск\\.|просек\\.|переезд\\.|заезд\\.|парк\\.|площадь|проезд\\.|туп\\.|м\\.|проток\\.|ст\\.|вал\\.|п\\.|спуск\\.|пл-ка\\.|д\\.|коса\\.|п\\/ст\\.|пр\\.|пл\\.|мкр\\.|аллея\\.|кв-л\\.|ж\\/д_будка\\.|х\\.|рзд\\.|тск\\.|сквер\\.|стр\\.|снт\\.|мост\\.|метро\\.))*(, )*\";\"([0-9]*)\";\"((д\\.|ая\\.|ая|нетр\\.|нетр|а\\\\я\\.|а\\\\я|а\\/я\\.|а\\/я)*(([0-9х][-\\\\\\/0-9]*)([\\\\\\/-]([а-я])*)*))*([^\"]*)\";\"(([0-9]*)([а-я]*))*\"";

enum TypeOfRow
{
    BASE_TYPE = 1,
    INPUT_TYPE = 2
};

//https://en.wikipedia.org/wiki/Federal_subjects_of_Russia
enum TypeOfFederalSubject
{
    INCORRECT_SUBJ = -1,
    REPUBLIC = 1,
    KRAY = 2,
    OBLAST = 3,
    CITY_OF_FED_SIGN = 4,
    AUTONOM_OBLAST = 5,
    AUTONOM_OKRUG = 6
};

enum AddressElements
{
    STREET=1,      ///< название ул. (пр., ш., бул. и пр.)
    STREET_ID,     ///< id ул.
    KORP,          ///< корпус
    BUILD,         ///< дом
    BUILD_ID,      ///< id дома
    ADDITIONAL,    ///< допольнительная информация

    TYPE_OF_CITY1,
    CITY1,
    TYPE_OF_CITY2,
    CITY2,
    TYPE_OF_STREET,///< (ул., пр., ш., бул. и пр.)
    LITERA,
    DISTRICT,      ///< район
    TYPE_OF_FSUBJ,
    FSUBJ,         ///< federal subject (федеральный субъект) (название области, края, республики и пр.)
    RAW_ADDR,      ///< "сырой" адрес (тот который был прочитан из файла сsv)
    CORRECT,
};

const QList<AddressElements> ListMandatoryAddressElements {
    STREET, STREET_ID, BUILD_ID
};

const QList<AddressElements> ListAddressElements {
    STREET, STREET_ID, KORP, BUILD, BUILD_ID, ADDITIONAL,
            DISTRICT, FSUBJ,
            TYPE_OF_CITY1,
            CITY1,
            TYPE_OF_CITY2,
            CITY2,
            TYPE_OF_STREET,
            LITERA,
            CORRECT,
            TYPE_OF_FSUBJ,
};

typedef QMap<AddressElements, int> MapAddressElementPosition;
typedef QMap<AddressElements, QString> MapAEValue;

//key of map - sheet name, value - list of rows, row - string list
typedef QList< QStringList > ExcelSheet;
typedef QMap<QString, ExcelSheet > ExcelDocument;

typedef QVector<QString> VectorOfString;

const QMap<AddressElements, QString> MapColumnNames {
    {STREET, "STREET"},
    {STREET_ID, "STREET_ID"},
    {KORP, "KORP"},
    {BUILD, "BUILD"},
    {BUILD_ID, "BUILD_ID"},
    {ADDITIONAL, "ADDITIONAL"},
    {DISTRICT, "DISTRICT"},
    {TYPE_OF_FSUBJ, "TYPE_OF_FSUBJ"},
    {FSUBJ, "FSUBJ"},
    {RAW_ADDR, "RAW"},
    {TYPE_OF_CITY1, "TYPE_OF_CITY1"},
    {CITY1,	"CITY1"},
    {TYPE_OF_CITY2, "TYPE_OF_CITY2"},
    {CITY2, "CITY2"},
    {TYPE_OF_STREET, "TYPE_OF_STREET"},
    {LITERA, "LITERA"},
    {CORRECT, "CORRECT"},
};

const QMap<AddressElements, QString> MapColumnParsedNames {
    {STREET, "!P_STREET"},
    {STREET_ID, "!P_STREET_ID"},
    {BUILD, "!P_BUILD"},
    {BUILD_ID, "!P_BUILD_ID"},
    {TYPE_OF_STREET, "!P_TYPE_OF_STREET"},
    {ADDITIONAL, "!P_ADDITIONAL"},
    {TYPE_OF_CITY1, "!P_TYPE_OF_CITY1"},
    {CITY1, "!P_CITY1"},
    {TYPE_OF_CITY2, "!P_TYPE_OF_CITY2"},
    {CITY2, "!P_CITY2"},
    {TYPE_OF_FSUBJ, "!P_TYPE_OF_FSUBJ"},
    {FSUBJ, "!P_FSUBJ"},
    {DISTRICT, "!P_DISTRICT"},
    {TYPE_OF_STREET, "!P_TYPE_OF_STREET"},
    {STREET, "!P_STREET"},
    {KORP, "!P_KORP"},
    {LITERA, "!P_LITERA"},
    {ADDITIONAL, "!P_ADDITIONAL"},

};

const QMap<QString, TypeOfFederalSubject> MapStringFSubj {
    {"республика", REPUBLIC},
    {"респ", REPUBLIC},                 //main REPUBLIC
//    {"р-ка", REPUBLIC},
//    {"р", REPUBLIC},
    {"край", KRAY},                     //main KRAY
    {"кр", KRAY},
    {"область", OBLAST},
    {"обл", OBLAST},                    //main OBLAST
//    {OBLAST, "о", OBLAST},
    {"автономная область", AUTONOM_OBLAST},
    {"автономная обл", AUTONOM_OBLAST},
    {"авт.обл", AUTONOM_OBLAST},
    {"аобл", AUTONOM_OBLAST},           //main AUTONOM_OBLAST
    {"автономный округ", AUTONOM_OKRUG},
    {"а.о", AUTONOM_OKRUG},
    {"ао", AUTONOM_OKRUG},              //main AUTONOM_OKRUG
};

const QMap<TypeOfFederalSubject, QString> MapFSubjString {
    {REPUBLIC, "респ"},
    {KRAY, "край"},
    {OBLAST, "обл"},
    {AUTONOM_OBLAST, "аобл"},
    {AUTONOM_OKRUG, "ао"},
    {INCORRECT_SUBJ, "inc"}
};

enum Errors
{
    XLS_WORKER_ERROR=1,
};

#endif // DEFINES_H
