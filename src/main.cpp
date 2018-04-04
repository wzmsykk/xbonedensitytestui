#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include<QtQuick>
#include "initc.h"
#include "settings.h"
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    qmlRegisterType<Initc>("com.kp.initc",1,0,"Initc");

    QGuiApplication app(argc, argv);
    Settings set;
    set.getTime();

    QQmlApplicationEngine engine;
    engine.addImportPath(QCoreApplication::applicationDirPath() + QDir::separator() + QLatin1String("..") +
                             QDir::separator() + QLatin1String("fluid") + QDir::separator() + QLatin1String("qml"));
    engine.addImportPath(QCoreApplication::applicationDirPath() + QDir::separator() + QLatin1String("qml"));
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;


    return app.exec();
}
