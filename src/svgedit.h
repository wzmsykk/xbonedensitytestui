#ifndef SVGEDIT_H
#define SVGEDIT_H

#include <QObject>
#include <QFile>
class svgEdit : public QObject
{
    Q_OBJECT
public:
    explicit svgEdit(QObject *parent = nullptr);
    Q_INVOKABLE void changeIcon(QString color);
    Q_INVOKABLE void changeLCD(int fingers);
signals:

public slots:
};

#endif // SVGEDIT_H
