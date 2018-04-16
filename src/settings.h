#ifndef SETTINGS_H
#define SETTINGS_H
#include <QObject>
#include <QDateTime>
#include <QSettings>

class Settings:public QObject
{
    Q_OBJECT
    Q_ENUMS(Dpis)

public:
    Settings(QObject *parent=0);
    ~Settings();
    Q_PROPERTY(int copies READ copies WRITE setCopies NOTIFY statusChanged)
    Q_PROPERTY(Dpis dpi READ dpi WRITE setDpi)
    Q_PROPERTY(bool color READ color WRITE setColor NOTIFY statusChanged)
    Q_PROPERTY(int password READ password WRITE setPassword NOTIFY statusChanged)
     Q_PROPERTY(bool isPswExist READ isPswExist WRITE setPswExist NOTIFY statusChanged)


    enum Dpis{ LowDpi,//Important! using enums like ClassName.MyEnum
      HighDpi};       //Never use mySet.MyEnum

    int copies();
    void setCopies(int cpy);
    Dpis dpi();
    void setDpi(Dpis dpi);

    bool color();
    void setColor(bool col);

    int password();
    void setPassword(int password);
    bool isPswExist();
    void setPswExist(bool bol);

    Q_INVOKABLE void writeSettings();
    Q_INVOKABLE void readSettings();
    Q_INVOKABLE int changeTime(QDateTime dateTime);
signals:
    void statusChanged();
private:
    int m_copies;
    Dpis m_dpi;

    bool m_color;
    int m_password;
    bool m_pswExist;
};

#endif // SETTINGS_H
