#ifndef SVGEDIT_H
#define SVGEDIT_H

#include <QObject>
#include <QFile>
class svgEdit : public QObject
{
    Q_OBJECT
public:
    explicit svgEdit(QObject *parent = nullptr);
    void changeIcon(QString color);
signals:

public slots:
};

#endif // SVGEDIT_H
