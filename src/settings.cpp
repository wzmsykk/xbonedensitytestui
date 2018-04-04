#include "settings.h"
#include <QTime>
#include<QtDebug>
Settings::Settings()
{

}
void Settings::getTime(){
    currentTime.setTime(QTime::currentTime());
    qDebug()<<QTime::currentTime().toString(Qt::ISODate);
}
