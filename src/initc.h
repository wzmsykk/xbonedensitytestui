#ifndef initc_H
#define initc_H
#include <QObject>
#include<QThread>
//
class Worker : public QObject
{
    Q_OBJECT

public slots:
    void doSelfTest();
    void doFirstScan();
    void initFirstScan();
signals:
    void resultReady(int result);
};





class Initc:public QObject
{
    Q_OBJECT
    QThread workerThread1;
    QThread workerThread2;
public:
    Initc();
    ~Initc();
    enum status{
        succeed=1,
        failed=0,
        status_n
    };
    Q_ENUM(status)

public slots:
     void cleanset();
signals:
    void handleSTResults(int result);
    void handleFSResults(int result);
    void operateSelfTest();
    void prepareFirstScan();
    void operateFirstScan();
    void firstScanClear();
};

#endif // initc_H
