#include "scan.h"
#include <QThread>

/*signals:
    void handleScanPreperationResults(int result);
    void handleScanResults(int result);
    void prepareScan();
    void operateScan();
public slots:
    void cancelScan();
    void cancelPerperation();
public slots:
    void doScan();
    void prepareScan();
    void stopScan();
    void stopPreparation();
signals:
    void scanPreperationOK(int result);
    void scanOK(int result);
    */
Scan::Scan()
{
    ScanWorker *worker1 = new ScanWorker;
            worker1->moveToThread(&workerThread1);
    ScanWorker *worker2 = new ScanWorker;
            worker2->moveToThread(&workerThread2);
            connect(&workerThread1, &QThread::finished, worker1, &QObject::deleteLater);

            connect(&workerThread2, &QThread::finished, worker2, &QObject::deleteLater);
            connect(this,&Scan::operateScan,worker1,&ScanWorker::doScan);
            connect(this,&Scan::prepareScan,worker1,&ScanWorker::prepareScan);
            //connect(this,&Scan::cancelScan,worker2,&ScanWorker::stopScan);
           // connect(this,&Scan::cancelPerperation,worker2,&ScanWorker::stopPreparation);
            connect(worker1,&ScanWorker::scanPreperationOK,this,&Scan::handleScanPreperationResults);
            connect(worker1,&ScanWorker::scanOK,this,&Scan::handleScanResults);

           // connect(this,Scan::handleScanPreperationResults,worker2,ScanWorker::scanPreperationOK);
           // connect(this,Scan::handleScanResults,worker2,ScanWorker::scanOK);
            workerThread1.start();
            workerThread2.start();

}
Scan::~Scan(){
        workerThread1.quit();
        workerThread1.wait();
        workerThread2.quit();
        workerThread2.wait();
}

void  ScanWorker::doScan(){

    QThread::sleep(1);
     qDebug("doingScan");
    emit scanOK(1);//stage1
       QThread::sleep(1);
      emit scanOK(2);//stage2
         QThread::sleep(1);
      emit scanOK(3);//stage3
         QThread::sleep(1);
      emit scanOK(4);//stage4
         QThread::sleep(1);
      emit scanOK(5);//stage5
}
void ScanWorker::prepareScan(){

    QThread::sleep(1);
     qDebug("PrepScan");
    emit scanPreperationOK(1);
}
void Scan::cancelScan(){
    qDebug("cancelScan");
    //workerThread1.terminate();
    emit handleScanResults(0);
}
void Scan::cancelPerperation(){
    qDebug("cancelPrep");
    //workerThread1.terminate();

    emit handleScanPreperationResults(0);
}



