#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "src/calculator.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    //qmlRegisterType<calculator>("CalcEngine", 1, 0, "CalcEngine");

    calculator calculator;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("CalcEngine", &calculator);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("MyCalc", "Main");

    return app.exec();
}
