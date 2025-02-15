# Lessons learned
I did this project to learn to integrate Qt with C++, I already had some knowledge of using C++ but had never done anything with a GUI. Here's what I learned through this project. It is based on the [Calqlatr](https://doc.qt.io/qt-6/qtdoc-demos-calqlatr-example.html) example, it basically uses the same GUI, but the logic is implemented in C++. I also learned a lot about QML while implementing the UI, like objects, functions and attributes.
If you somehow come accross this and find this helpful, yay! :)
# Integrating a C++ class in a Qt Quick applitation
There are a couple of things you have to do in order to be able to call C++ functions/methods from a QML file. 

The first one is obviosly generating the C++ files, in this case a class. When using Qt Creator, marking it as an QObject, this will generate two files, a **source** file and a **header** file. In the header file, declare all methods that will be used, here, as in all C++ header files, you should show the "shape" of your class (members, methods, etc).
One thing to keep in mind when declaring members, is that for the methods you want to be able to call from QML you have to prepend the macro *Q_INVOKABLE*  to the method declaration, like this:
```c++
Q_INVOKABLE void digitPressed(QVariant op, QObject *display);
```

Second, is actually exposing the class as a QML object to the rest of the project, to do this, in the *main.cpp* file, import the class header file like (as a relative import) like you would in any other C++ project. Then register it, by first instatiating your class, and then setting the *context property* with the name you will use to call your classes methods from within the Quick modules, like this:
```c++
#include "src/calculator.h"
int main(int argc, char *argv[]) {
	...
	calculator calculator;
	QQmlApplicationEngine engine;
	engine.rootContext()->setContextProperty("CalcEngine", &calculator);
	...
}
```

Now you are ready to call methods C++ methods from within the QML files, but, you may be wondering, what type should I expect to receive when designing the signature of these methods, it's quite simple: QVariant or QObject. Specially when retrieving object properties, the return value will most likely be of type QVariant. These can be easily cast to different types (like string, or different number types).

How about the other way around, what if I want to call a QML Item Function from within my C++ code, or retrieve a specific property. 
One is simpler than the other: retrieving, so let's look at the example where we want to retrieve the property "maxDigits" of our Display object, we send a pointer to the Display object as an argument when calling the C++ method, this uses the type *QObject* and it has a handy method called *property* , we know that the property "maxDigits" contains a number, but, since the property could be of any type, it is returned as *QVariant*, since we know what is inside of it, we can simply call *.toInt()* on it later and just unpack it as a basic integer. Like this:
```c++
void calculator::digitPressed(QVariant op, QObject *display) {
...
	QVariant max_digits = display->property("maxDigits");
	...
	m_digits.truncate(max_digits.toInt() - 1);
	...
}
```

Now, the one that's a bit more tricky, is calling a Function, this is done via the *QMetaObject* method *invokeMethod*, we just have to provide the Object pointer where the Function is defined, phrased differently, if we want to call a function of the Display object, we have to give it a pointer to the this display object. We also need the name of the function. This suffices for a call with no arguments and no returns:
```c++
QMetaObject::invokeMethod(display, "backspace"); // calls the function "backspace" defined in Display
```
If we want to have a return value, we will use the macro *Q_RETURN_ARG* with an return type (usually QVariant) and a variable to assign it to:
```c++
QMetaObject::invokeMethod(display, "isDisplayEmpty", Q_RETURN_ARG(QVariant, is_display_empty));
```
For passing arguments, it's similar to the return value, only with the macro *Q_ARG* instead (can be used multiple times):
```c++
QMetaObject::invokeMethod(display, "newLine", Q_ARG(QVariant, "="), Q_ARG(QVariant, m_digits.toFloat()));
```

And that's it, that covers the very basic way I learned to send information back and forward between QML and C++.
