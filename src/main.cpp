#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QtQuick>
#include "initc.h"
#include "settings.h"
#include "scan.h"
#include "svgedit.h"
int main(int argc, char *argv[])
{
    svgEdit ss;
    ss.changeIcon("#000000");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    qmlRegisterType<Initc>("com.kp.initc",1,0,"Initc");
    qmlRegisterType<Scan>("com.kp.scan",1,0,"Scan");
    qmlRegisterType<Settings>("com.kp.settings",1,0,"Settings");
    Settings mySet;

    QGuiApplication app(argc, argv);
    QFont font;
        font.setPointSize(16);
        font.setFamily("tahoma");

        app.setFont(font);
    QQmlApplicationEngine engine;
     engine.rootContext()->setContextProperty("mySet",&mySet);
     engine.rootContext()->setContextProperty("applicationDirPath",QGuiApplication::applicationDirPath());
    engine.addImportPath(QCoreApplication::applicationDirPath() + QDir::separator() + QLatin1String("qml"));
    engine.load(QUrl(QStringLiteral("qrc:/newMain.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;


    return app.exec();
}
