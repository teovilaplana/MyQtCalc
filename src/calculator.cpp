#include "calculator.h"

calculator::calculator(QObject *parent)
    : QObject{parent}

{ }

bool calculator::isOperationDisabled(QVariant op, QObject *display) {
    if (m_digits != "" && m_lastOp != "=" && (op.toString() == "π" || op.toString() == "e"))
        return true;
    if (m_digits == "" && !((op.toString() >= "0" && op.toString() <= "9") || op.toString() == "π" || op.toString() == "e" || op.toString() == "AC"))
        return true;
    QVariant is_operand_empty;
    QMetaObject::invokeMethod(display, "isOperandEmpty", Q_RETURN_ARG(QVariant, is_operand_empty));


    if (op == "foobar" && ( is_operand_empty.toBool() || !((m_lastOp >= "0" && m_lastOp <= "9") || m_lastOp == "π" || m_lastOp == "e" || m_lastOp == ".")))
        return true;
    if (op == "=" && m_previousOperator.toString().length() != 1)
        return true;
    if (op == "." && m_digits.contains(re_blank_space))
        return true;
    if (op == "√" &&  m_digits.contains(re_minus_sign))
        return true;
    QVariant is_display_empty;
    QMetaObject::invokeMethod(display, "isDisplayEmpty", Q_RETURN_ARG(QVariant, is_display_empty));
    if (op.toString() == "AC" && is_display_empty.toBool())
        return true;

    return false;
}

void calculator::digitPressed(QVariant op, QObject *display) {

    if (isOperationDisabled(op, display))
        return;
    if (m_lastOp == "π" || m_lastOp == "e")
        return;
    // handle mathematical constants
    auto max_digits = display->property("maxDigits");
    qDebug("Max Display digits %d", max_digits.toInt());
    if (op == "π"){
        m_lastOp = op.toString();
        m_digits = QVariant(M_PI).toString();
        qDebug("Pi: %f", m_digits.toFloat());
        m_digits.truncate(max_digits.toInt() - 1);
        qDebug("Pi truncated: %f", m_digits.toFloat());
        QMetaObject::invokeMethod(display, "appendDigit", Q_ARG(QVariant, m_digits));
        return;
    }
    if (op == "e") {
        m_lastOp = op.toString();
        m_digits = QVariant(exp(1)).toString();
        m_digits.truncate(max_digits.toInt() - 1);
        QMetaObject::invokeMethod(display, "appendDigit", Q_ARG(QVariant, m_digits));
        return;
    }
    // append a digit to another digit for decimal point
    if (m_lastOp.length() == 1 && ((m_lastOp >= "0" && m_lastOp <= "9") || m_lastOp == ".")) {
        qDebug("Appending to end");
        if(m_digits.length() >= max_digits.toInt())
            return;
        m_digits.append(op.toString());
        QMetaObject::invokeMethod(display, "appendDigit", Q_ARG(QVariant, op));
    // else just write a single digit to display
    }
    else {
        m_digits = op.toString();
        // QVariant new_digit = m_digits;
        QMetaObject::invokeMethod(display, "appendDigit", Q_ARG(QVariant, m_digits.toFloat()));
    }
    m_lastOp = op.toString();
    qDebug("%f", m_digits.toFloat());
}

void calculator::operatorPressed(QVariant op, QObject *display) {
    if (isOperationDisabled(op, display))
        return;

    if (op == "±") {
        m_digits = QString::number(m_digits.toFloat() * -1);
        QMetaObject::invokeMethod(display, "setDigit", Q_ARG(QVariant, m_digits));
        return;
    }

    if (op == "foobar") {
        qDebug("Erasing");
        m_digits = m_digits.slice(0, m_digits.size()-1);
        if (m_digits == "-")
            m_digits = "";
        QMetaObject::invokeMethod(display, "backspace");
        return;
    }

    m_lastOp = op.toString();
    qDebug("%s", m_lastOp.toStdString().c_str());
    if (m_previousOperator == "+") {
        qDebug("Adding");
        m_digits = QString::number(m_curVal + m_digits.toFloat());
        qDebug("%f", m_digits.toFloat());
    }
    else if (m_previousOperator == "-") {
        qDebug("Minus?");
        m_digits = QString::number(m_curVal - m_digits.toFloat());
    }
    else if (m_previousOperator == "×") {
        m_digits = QString::number(m_curVal * m_digits.toFloat());
    }
    else if (m_previousOperator == "÷") {
        m_digits = QString::number(m_curVal / m_digits.toFloat());
    }

    if (op == "+" || op == "-" || op == "×" || op == "÷") {
        m_previousOperator = op;
        m_curVal = m_digits.toFloat();
        m_digits = "";
        QMetaObject::invokeMethod(display, "displayOperator", Q_ARG(QVariant, m_previousOperator));
        return;
    }

    m_curVal = 0;
    m_previousOperator = "";

    if (op == "=") {
        QMetaObject::invokeMethod(display, "newLine", Q_ARG(QVariant, "="), Q_ARG(QVariant, m_digits.toFloat()));
    }
    if (op == "√") {
        m_digits = QString::number(sqrt(m_digits.toFloat()));
        QMetaObject::invokeMethod(display, "newLine", Q_ARG(QVariant,"√"), Q_ARG(QVariant, m_digits.toFloat()));
    }
    else if (op== "⅟x") {
        m_digits = QString::number(1 / m_digits.toFloat());
        QMetaObject::invokeMethod(display, "newLine", Q_ARG(QVariant,"⅟x"), Q_ARG(QVariant, m_digits.toFloat()));
    }
    else if (op == "x²") {
        m_digits = QString::number(m_digits.toFloat() * m_digits.toFloat());
        QMetaObject::invokeMethod(display, "newLine", Q_ARG(QVariant,"x²"), Q_ARG(QVariant, m_digits.toFloat()));
    }
    else if (op == "x³") {
        m_digits = QString::number(m_digits.toFloat() * m_digits.toFloat() * m_digits.toFloat());
        QMetaObject::invokeMethod(display, "newLine", Q_ARG(QVariant,"x³"), Q_ARG(QVariant, m_digits.toFloat()));
    }
    else if (op == "|x|") {
        m_digits = QString::number(abs(m_digits.toFloat()));
        QMetaObject::invokeMethod(display, "newLine", Q_ARG(QVariant,"|x|"), Q_ARG(QVariant, m_digits.toFloat()));
    }
    else if (op == "⌊x⌋") {
        m_digits = QString::number(floor(m_digits.toFloat()));
        QMetaObject::invokeMethod(display, "newLine", Q_ARG(QVariant,"⌊x⌋"), Q_ARG(QVariant, m_digits.toFloat()));
    }
    else if (op == "sin") {
        m_digits = QString::number(sin(m_digits.toFloat()));
        QMetaObject::invokeMethod(display, "newLine", Q_ARG(QVariant,"sin"), Q_ARG(QVariant, m_digits.toFloat()));
    }
    else if (op == "cos") {
        m_digits = QString::number(cos(m_digits.toFloat()));
        QMetaObject::invokeMethod(display, "newLine", Q_ARG(QVariant,"cos"), Q_ARG(QVariant, m_digits.toFloat()));
    }
    else if (op == "tan") {
        m_digits = QString::number(tan(m_digits.toFloat()));
        QMetaObject::invokeMethod(display, "newLine", Q_ARG(QVariant,"tan"), Q_ARG(QVariant, m_digits.toFloat()));
    }
    else if (op == "log") {
        m_digits = QString::number(log10(m_digits.toFloat()));
        QMetaObject::invokeMethod(display, "newLine", Q_ARG(QVariant,"log"), Q_ARG(QVariant, m_digits.toFloat()));
    }
    else if (op == "ln") {
        m_digits = QString::number(log(m_digits.toFloat()));
        QMetaObject::invokeMethod(display, "newLine", Q_ARG(QVariant,"ln"), Q_ARG(QVariant, m_digits.toFloat()));
    }

    if (op == "AC") {
        QMetaObject::invokeMethod(display, "allClear");
        m_curVal = 0;
        m_lastOp = "";
        m_digits = "";
        m_previousOperator = "";
    }
}


