#ifndef KNOBS_H
#define KNOBS_H

#include <QString>
#include <QtDebug>
#include <QtWidgets>
#include <QObject>

#include "pirilib.h"
#include "knobcallback.h"


class KnobCallback;
class ComboBoxKnob;

void PIRILIBSHARED_EXPORT ADD_VALUES(KnobCallback *f, QString str);
void PIRILIBSHARED_EXPORT UPDATE_VALUES(KnobCallback *f, QWidget *widget, QString str);
void PIRILIBSHARED_EXPORT UPDATE(KnobCallback *f, QWidget* widget, QString str);

class PIRILIBSHARED_EXPORT KnobBase
{
public:
    KnobBase();
    virtual QString getHash() { return _myHash; }
private:
    void hash();
    QString _myHash;
};

class PIRILIBSHARED_EXPORT StringKnob : public QLineEdit, public KnobBase
{
    Q_OBJECT
public:
    StringKnob(QWidget *parent = 0);
    StringKnob(KnobCallback *f, QString *value, QString label);
    QString getHash();

public slots:
    void updateValue();

private:
    QString* _myValue;
    QString _myHash;
    void updateHash();
};


class PIRILIBSHARED_EXPORT IntegerKnob : public QSpinBox, public KnobBase
{
    Q_OBJECT
public:
    IntegerKnob(QWidget *parent = 0);
    IntegerKnob(KnobCallback *f, int *value, QString label);
    IntegerKnob(KnobCallback *f, int *value, QString label, int min, int max);
    QString getHash();


public slots:
    void updateValue(int v);

private:
    int* _myValue;
    QString _myHash;
    void updateHash();
};

class PIRILIBSHARED_EXPORT CheckBoxKnob : public QCheckBox, public KnobBase
{
    Q_OBJECT
public:
    CheckBoxKnob(QWidget *parent = 0);
    CheckBoxKnob(KnobCallback *f, bool *value, QString label);
    int value();
    QString getHash();

public slots:
    void updateValue(int v);
    void valueChanged(bool v);

private:
    bool* _myValue;
    QString _myHash;
    void updateHash();
};

class PIRILIBSHARED_EXPORT ComboBoxKnob : public QComboBox, public KnobBase
{
    Q_OBJECT
public:
    ComboBoxKnob(QWidget *parent = 0);
    ComboBoxKnob(KnobCallback *f, int *value);
    QString getHash();

public slots:
    void updateValue(int v);

private:
    int* _myValue;
    QString _myHash;
    void updateHash();
};

class PIRILIBSHARED_EXPORT FileDialogKnob : public QWidget, public KnobBase
{
    Q_OBJECT
public:
    FileDialogKnob(QWidget *parent = 0);
    FileDialogKnob(KnobCallback *f, QString *value);
    QString getHash();

public slots:
    void updateValueFromDialog(QString s);
    void getFileName();

signals:
    void valueUpdated(QString fileName);

private:
    QString* _myValue;
    QString _myHash;
    void updateHash();

};


StringKnob* PIRILIBSHARED_EXPORT String_knob(KnobCallback* f, QString *value, QString label);
IntegerKnob* PIRILIBSHARED_EXPORT Integer_knob(KnobCallback* f, int *value, QString label);
CheckBoxKnob* PIRILIBSHARED_EXPORT CheckBox_knob(KnobCallback* f, bool *value, QString label);
ComboBoxKnob* PIRILIBSHARED_EXPORT ComboBox_knob(KnobCallback* f, int *value, QString label, QString valueName);
FileDialogKnob* PIRILIBSHARED_EXPORT FileDialog_knob(KnobCallback* f, QString* value, QString label);

#endif // KNOBS_H
