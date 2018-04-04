#include "initc.h"
#include "scan.h"
#include "settings.h"
#include <QThread>

Initc::Initc()
{
    Worker *worker1 = new Worker;
            worker1->moveToThread(&workerThread1);
    Worker *worker2 = new Worker;
            worker2->moveToThread(&workerThread1);
            connect(&workerThread1, &QThread::finished, worker1, &QObject::deleteLater);
            connect(this, &Initc::operateSelfTest, worker1, &Worker::doSelfTest);
            connect(worker1, &Worker::resultReady, this, &Initc::handleSTResults);

            connect(&workerThread2, &QThread::finished, worker2, &QObject::deleteLater);
            connect(this, &Initc::operateFirstScan, worker2, &Worker::doFirstScan);
            connect(this, &Initc::prepareFirstScan, worker2, &Worker::initFirstScan);
            connect(worker2, &Worker::resultReady, this, &Initc::handleFSResults);

            connect(this, &Initc::firstScanClear, this, &Initc::cleanset);
            workerThread1.start();
            workerThread2.start();
}



void Worker::doSelfTest(){
    //初始化内容
    qDebug("st1");

    QThread::sleep(2);//call selftest proc
    qDebug("st2");
    emit resultReady(1);
}

void Worker::doFirstScan(){
    //初始化内容
    qDebug("fs");


    QThread::sleep(2);//call firstscan proc
    emit resultReady(1);
}
void Worker::initFirstScan()
{

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
