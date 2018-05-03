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
    QString apppath="./";

    QString filename="00.svg";
    QString currentFileName=apppath+filename;

       bool ok= QFile::remove(currentFileName);

     qDebug("ok==%d",ok);
    QRegExp rx("stroke=\"#010101\"");

    qDebug("ok==%d",ok);
    QFile work(":/icons/action/assessment_copy.svg");
    work.open(QIODevice::ReadOnly);
    QByteArray byteText=work.readAll();

    QString text(byteText);
    text.replace(rx,"stroke=\""+color.toUtf8()+"\"");
    QFile op(currentFileName);
    ok=op.open(QIODevice::ReadWrite | QIODevice::Text);
    qDebug("ok==%d",ok);

    op.seek(0);
    op.write(text.toUtf8());
    op.close();
    work.close();

}
