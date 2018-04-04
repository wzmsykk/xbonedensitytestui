#ifndef SETTINGS_H
#define SETTINGS_H
#include <QObject>
#include <QDateTime>
class Settings:public QObject
{
    Q_OBJECT
public:
    Settings();
    void getTime();
    QDateTime currentTime;

};

#endif // SETTINGS_H
