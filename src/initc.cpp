#include "initc.h"
#include "scan.h"
#include "settings.h"
#include <QThread>

Initc::Initc()
{
    InitWorker *worker1 = new InitWorker;
            worker1->moveToThread(&workerThread1);
    InitWorker *worker2 = new InitWorker;
            worker2->moveToThread(&workerThread2);
            connect(&workerThread1, &QThread::finished, worker1, &QObject::deleteLater);

            connect(&workerThread2, &QThread::finished, worker2, &QObject::deleteLater);

            connect(this, &Initc::operateSelfTest, worker1, &InitWorker::doSelfTest);
            connect(worker1, &InitWorker::selfTestResultReady, this, &Initc::handleSelfTestResults);

            connect(this, &Initc::operateFirstScan, worker1, &InitWorker::doFirstScan);
            connect(this, &Initc::prepareFirstScan, worker1, &InitWorker::initFirstScan);
            connect(worker1, &InitWorker::firstScanResultReady, this, &Initc::handleFirstScanResults);
            connect(worker1, &InitWorker::firstScanPreperationReady, this, &Initc::firstScanReady);

            workerThread1.start();
            workerThread2.start();
}



void InitWorker::doSelfTest(){
    //初始化内容
    qDebug("st1");

    QThread::sleep(2);//call selftest proc
    qDebug("st2");
    emit selfTestResultReady(1);
}

void InitWorker::doFirstScan(){
    //初始化内容
    qDebug("fs");


    QThread::sleep(2);//call firstscan proc

    emit firstScanResultReady(1);
}
void InitWorker::initFirstScan()
{
    qDebug("initFS");
    QThread::sleep(1);
    emit firstScanPreperationReady(1);
}
void Initc::cleanset(){
    return;
}

Initc::~Initc() {
        workerThread1.quit();
        workerThread1.wait();
        workerThread2.quit();
        workerThread2.wait();
}
