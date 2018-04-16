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
    int err=stime(&_t);
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







