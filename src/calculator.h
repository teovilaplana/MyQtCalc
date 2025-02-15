#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QObject>
#include <QMetaObject>
#include <QProperty>
#include <QVariant>
#include <QRegularExpression>
#include <math.h>
#include <QtLogging>

class calculator : public QObject
{
    Q_OBJECT
public:
    explicit calculator(QObject *parent = nullptr);

    Q_INVOKABLE void digitPressed(QVariant op, QObject *display);
    Q_INVOKABLE void operatorPressed(QVariant op, QObject *display);
    Q_INVOKABLE bool isOperationDisabled(QVariant op, QObject *display);

signals:

private:
    float m_curVal = 0;
    QVariant m_previousOperator = "";
    QString m_lastOp = "";
    QString m_digits = "";
    const QRegularExpression re_blank_space{R"(\.)"};
    const QRegularExpression re_minus_sign{R"(-)"};

};

#endif // CALCULATOR_H
