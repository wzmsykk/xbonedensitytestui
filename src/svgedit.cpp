#include "svgedit.h"
#include <QObject>
#include <QFile>
#include <QRegExp>
#include <QCoreApplication>
svgEdit::svgEdit(QObject *parent) : QObject(parent)
{

}
void svgEdit::changeIcon(QString color)
{
    int i;
    QStringList k;
    QString apppath="./";
    QString filename;
    QString currentFileName;
    QRegExp rx("stroke=\"#010101\"");
    QFile work,op;
    QByteArray byteText;
    QString text;
    k<<":/icons/00.svg"<<":/icons/01.svg";
    for(i=0;i<2;i++){


    filename="0"+QString::number(i)+".svg";
    currentFileName=apppath+filename;

    bool ok= QFile::remove(currentFileName);

    qDebug("ok==%d",ok);
    work.setFileName(k.at(i));
    work.open(QIODevice::ReadOnly);
    byteText=work.readAll();

    text=QString(byteText);
    text.replace(rx,"stroke=\""+color.toUtf8()+"\"");
    op.setFileName(currentFileName);
    ok=op.open(QIODevice::ReadWrite | QIODevice::Text);
    qDebug("ok==%d",ok);

    op.seek(0);
    op.write(text.toUtf8());
    op.close();
    work.close();
}
}
