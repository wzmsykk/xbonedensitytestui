#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QtQuick>
#include "initc.h"
#include "settings.h"
#include "scan.h"
#include "svgedit.h"
void langzh(){

}
int main(int argc, char *argv[])
{
    svgEdit ss;
    ss.changeIcon("#000000");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QTranslator zh;
    zh.load("zh_cn.qm",":/");

    qmlRegisterType<Initc>("com.kp.initc",1,0,"Initc");
    qmlRegisterType<Scan>("com.kp.scan",1,0,"Scan");
    qmlRegisterType<Settings>("com.kp.settings",1,0,"Settings");
    Settings mySet;

    QGuiApplication app(argc, argv);
    app.installTranslator(&zh);
    QFont font;
        font.setPointSize(16);
        font.setFamily("微软雅黑");

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
