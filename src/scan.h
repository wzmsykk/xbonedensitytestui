#ifndef SCAN_H
#define SCAN_H
#include <QObject>
#include<QThread>
class ScanWorker :public QObject

{
    Q_OBJECT
public slots:
    void doScan();
    void prepareScan();
    //void stopScan();
    //void stopPreparation();
signals:
    void scanPreperationOK(int result);
    void scanOK(int result);
};

class Scan:public QObject
{
    Q_OBJECT
    QThread workerThread1;
    QThread workerThread2;
public:
    Scan();
    ~Scan();
public slots:
    void cancelScan();
    void cancelPerperation();
signals:
    void handleScanPreperationResults(int result);
    void handleScanResults(int result);
    void prepareScan();
    void operateScan();


};

#endif // SCAN_H
