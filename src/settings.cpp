#include "settings.h"
#include <QTime>
#include<QtDebug>
#include <time.h>
Settings::Settings(QObject *parent):
    QObject(parent)
    ,m_copies(1)
    ,m_dpi(Settings::HighDpi)
    ,m_color(0)
  ,m_password(0)
  ,m_pswExist(false)
  ,m_byteUsed(0)
  ,m_gain(0)
  ,m_expcount(0)
  ,m_accmu(0)
  ,m_serial("123456")
{

}
Settings::~Settings(){

}
void Settings::writeSettings()

{
    QSettings settings("Software Inc.","BDT");
    settings.setValue("copies",m_copies);
    settings.setValue("dpi",int(m_dpi));
    settings.setValue("colored",m_color);
    settings.setValue("password",m_password);
    settings.setValue("pswExist",m_pswExist);

}
void Settings::readSettings()

{
    QSettings settings("Software Inc.","BDT");


    m_copies=settings.value("copies").toInt();
    m_dpi=Dpis(settings.value("dpi").toInt());
    m_password=settings.value("password").toInt();
    m_pswExist=settings.value("pswExist").toBool();
    m_color=settings.value("colored").toBool();
}
int Settings::changeTime(QDateTime dateTime){
    long _t=dateTime.toSecsSinceEpoch();
    //int err=stime(&_t);
    int err=-1;
    if(err==-1) qDebug("Need Previlige");
    return err;

}
int Settings::copies(){
    return m_copies;
}

void Settings::setCopies(int cpy){
    m_copies=cpy;
    emit statusChanged();
}
Settings::Dpis Settings::dpi(){
    return m_dpi;
}
void Settings::setDpi(Dpis dpi)
{
    m_dpi=dpi;
    emit statusChanged();
}

bool Settings::color(){
    return m_color;
}
void Settings::setColor(bool col){
    m_color=col;
    emit statusChanged();
}


int Settings::password(){
    return m_password;
}
void Settings::setPassword(int pass){
    m_password=pass;
    emit statusChanged();
}

bool Settings::isPswExist(){
    return m_pswExist;
}
void Settings::setPswExist(bool bol){
    m_pswExist=bol;
    emit statusChanged();
}



int Settings::byteUsed(){return 0;}

int Settings::checkSum(){return 0;}
int Settings::PMTValue(){return 0;}
int Settings::zoffset(){return 0;}
int Settings::toffset(){return 0;}
int Settings::gain(){return m_gain;}
void Settings::setGain(int gain){m_gain=gain;}
int Settings::offset(){return 0;}
int Settings::avgmu(){return 0;}

int Settings::expcount(){return m_expcount;}
void Settings::setExpcount(int expcount){m_expcount=expcount;}
int Settings::accmu(){return m_accmu;}
void Settings::setAccmu(int accmu){m_accmu=accmu;}
QString Settings::serial(){return m_serial;}
void Settings::setSerial(QString serial){m_serial=serial;}
QString Settings::sver(){
    QString sver="v0.01";
    return sver;}
QString Settings::dver(){
    QString dver="v2.0";
    return dver;
}
float Settings::bmd1(){
    return 0;
}
float Settings::bmd2(){return 0;}
float Settings::bmd3(){return 0;}
float Settings::snr(){return 0;}
float Settings::mu(){return 0;}




