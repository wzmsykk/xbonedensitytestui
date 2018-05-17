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


    Q_PROPERTY(int byteUsed READ byteUsed)
    Q_PROPERTY(int checkSum READ checkSum)
    Q_PROPERTY(int PMTValue READ PMTValue)
    Q_PROPERTY(int zoffset READ zoffset)
    Q_PROPERTY(int toffset READ toffset)
    Q_PROPERTY(int gain READ gain WRITE setGain)
    Q_PROPERTY(int offset READ offset)
    Q_PROPERTY(int avgmu READ avgmu)
    Q_PROPERTY(int expcount READ expcount)
    Q_PROPERTY(int accmu READ accmu WRITE setAccmu)
    Q_PROPERTY(QString serial READ serial WRITE setSerial)
    Q_PROPERTY(QString sver READ sver)
    Q_PROPERTY(QString dver READ dver)
    Q_PROPERTY(float bmd1 READ bmd1)
    Q_PROPERTY(float bmd2 READ bmd2)
    Q_PROPERTY(float bmd3 READ bmd3)
    Q_PROPERTY(float snr READ snr)
    Q_PROPERTY(float mu READ mu)






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

    int byteUsed();
    int checkSum();
    int PMTValue();
    int zoffset();
    int toffset();
    int gain();
    void setGain(int gain);
    int offset();
    int avgmu();

    int expcount();
    void setExpcount(int expcount);
    int accmu();
     void setAccmu(int accmu);
    QString serial();
    void setSerial(QString serial);
    QString sver();
    QString dver();
    float bmd1();
    float bmd2();
    float bmd3();
    float snr();
    float mu();

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

    int m_byteUsed;
    int m_gain;
    int m_expcount;
    int m_accmu;
    QString m_serial;
};

#endif // SETTINGS_H
