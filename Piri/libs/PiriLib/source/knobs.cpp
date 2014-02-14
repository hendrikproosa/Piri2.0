#include "knobcallback.h"

#include <QString>
#include <QtDebug>
#include <QtWidgets>
#include <QObject>

#include "knobs.h"

/*!
 * \brief Adds values to last widget in callback.
 *
 * Adds new values to last widget, for example combobox. Can be comma separated
 * list of multiple values.
 * \param f Knob Callback that has the widget.
 * \param str List of values to be added
 */
void ADD_VALUES(KnobCallback *f, QString str)
{
    QWidget *last = f->getLayout()->itemAt(f->getLayout()->rowCount()*2-1)->widget();
    for (int i = 1; i <= f->getLayout()->rowCount(); i++)
    {
        qDebug() << "Item in callback: " << f->getLayout()->itemAt(i*2-1)->widget();
    }
    qDebug() << "Last widget in callback: " << last;
    if (qobject_cast<ComboBoxKnob*>(last))
    {
        ComboBoxKnob *combo = qobject_cast<ComboBoxKnob*>(last);
        combo->addItems(str.split(","));
        combo->setMaximumWidth(str.length() * 5);
    }
}

/*!
 * \brief Updates value list in callback widget.
 *
 * Updates the widget with new values. Clears all old values! Values can be
 * in comma separated list.
 * \param f Knob callback that has the widget.
 * \param widget Widget to be updated.
 * \param str List of values to be added.
 */
void UPDATE_VALUES(KnobCallback *f, QWidget* widget, QString str)
{
    if (qobject_cast<ComboBoxKnob*>(widget))
    {
        ComboBoxKnob *combo = qobject_cast<ComboBoxKnob*>(widget);
        combo->clear();
        combo->addItems(str.split(","));
        combo->setMaximumWidth(str.length() * 5);
    }
}

/*!
 * \brief Updates widget and sets current index.
 *
 * This function updates widget and sets current index to 'str'.
 * Str must be index number! Most useful with comboboxes.
 * \param f Knob callback that has the widget.
 * \param widget Widget to be updated.
 * \param str Index to be set.
 */
void UPDATE(KnobCallback *f, QWidget* widget, QString str)
{
    if (qobject_cast<ComboBoxKnob*>(widget))
    {
        ComboBoxKnob *combo = qobject_cast<ComboBoxKnob*>(widget);
        combo->setCurrentIndex(str.toInt());
    }
}


KnobBase::KnobBase()
{
    hash();
}

void KnobBase::hash()
{
    _myHash = "tere";
}


/*
----------------------------------------------------------------------
STRING
----------------------------------------------------------------------
*/

/*!
 * \brief Creates a new string knob.
 * \param f Knob callback that will hold the knob.
 * \param value Value that will be connected to knob.
 * \param label Knob label.
 * \return New StringKnob object.
 */
StringKnob* String_knob(KnobCallback *f, QString *value, QString label)
{
    //qDebug() << "String knob: " << value << *value << label << value->toString();
    StringKnob *knob = new StringKnob(f, value, label);
    f->getLayout()->addRow(label, knob);
    f->addKnob(knob, label, (QVariant*)value);
    knob->setToolTip(label.replace(":", "").toLower());
    return knob;
}


/*!
 * \brief StringKnob initializer.
 * \param f Knob callback that will hold the knob.
 * \param value Value that will be connected to knob.
 * \param label Knob label.
 */
StringKnob::StringKnob(KnobCallback *f, QString* value, QString label) :
    _myValue(value)
{
    this->setText(*value);
    connect(this, SIGNAL(editingFinished()), this, SLOT(updateValue()));
    connect(this, SIGNAL(editingFinished()), f, SLOT(valueChanged()));
    updateHash();
}

/*!
 * \brief Get node hash string.
 * \return Hash string.
 */
QString StringKnob::getHash()
{
    return _myHash;
}

void StringKnob::updateHash()
{
    _myHash = generateHash(QString(*_myValue));
}

/*!
 * \brief Updates variable associated with knob.
 */
void StringKnob::updateValue()
{

    *_myValue = this->text();
    updateHash();
    //qDebug() << "StringKnob hash updated: " << _myHash;
}


/*
----------------------------------------------------------------------
INTEGER_KNOB
----------------------------------------------------------------------
*/

/*!
 * \brief Creates a new integerknob.
 * \param f Knob callback that will hold the knob.
 * \param value Value that will be connected to knob.
 * \param label Knob label.
 * \return New IntegerKnob object.
 */
IntegerKnob *Integer_knob(KnobCallback *f, int *value, QString label)
{
    qDebug() << "Integer knob: " << value << *value << label;
    IntegerKnob *knob = new IntegerKnob(f, value, label);
    f->getLayout()->addRow(label, knob);
    //f->addKnob(knob, label, QString("%1").arg(*value, 0, 'g', 30));
    f->addKnob(knob, label, (QVariant*)value);
    knob->setToolTip(label.replace(":", "").toLower());
    return knob;
}


/*!
 * \brief Main IntegerKnob initializer.
 * \param f Knob callback that will hold the knob.
 * \param value Value that will be connected to knob.
 * \param label Knob label.
 */
IntegerKnob::IntegerKnob(KnobCallback *f, int *value, QString label) :
    _myValue(value)
{
    this->setValue(*value);
    this->setMaximumWidth(70);
    this->setMinimumHeight(20);
    this->setRange(-100000000, 100000000);
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(updateValue(int)));
    connect(this, SIGNAL(valueChanged(int)), f, SLOT(valueChanged()));
    updateHash();
}

/*!
 * \brief IntegerKnob initializer with min and max.
 * \param f Knob callback that will hold the knob.
 * \param value Value that will be connected to knob.
 * \param label Knob label.
 */
IntegerKnob::IntegerKnob(KnobCallback *f, int *value, QString label, int min, int max) :
    _myValue(value)
{
    this->setValue(*value);
    this->setMaximumWidth(70);
    this->setMinimumHeight(20);
    this->setRange(min, max);
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(updateValue(int)));
    connect(this, SIGNAL(valueChanged(int)), f, SLOT(valueChanged()));
}

/*!
 * \brief Get node hash string.
 * \return Hash string.
 */
QString IntegerKnob::getHash()
{
    return _myHash;
}

/*!
 * \brief Update knob hash after changes.
 */
void IntegerKnob::updateHash()
{
    _myHash = generateHash(QString(*_myValue).toLatin1());
}


/*!
 * \brief Updates variable connected to knob.
 * \param v
 */
void IntegerKnob::updateValue(int v)
{
    *_myValue = this->value();
    updateHash();
}


/*
----------------------------------------------------------------------
CHECKBOX
----------------------------------------------------------------------
*/

CheckBoxKnob *CheckBox_knob(KnobCallback *f, bool *value, QString label)
{
    qDebug() << "Checkbox knob: " << value << *value << label;
    CheckBoxKnob *knob = new CheckBoxKnob(f, value, label);
    f->getLayout()->addRow(label, knob);
    f->addKnob(knob, label, (QVariant*)value);
    knob->setToolTip(label.replace(":", "").toLower());
    return knob;
}


CheckBoxKnob::CheckBoxKnob(KnobCallback *f, bool* value, QString label) :
    _myValue(value)
{
    this->setTristate(false);
    this->setChecked(*value);
    this->setMaximumSize(15, 15);
    connect(this, SIGNAL(stateChanged(int)), this, SLOT(updateValue(int)));
    connect(this, SIGNAL(clicked()), f, SLOT(valueChanged()));
    updateHash();
}

int CheckBoxKnob::value()
{
    return *_myValue;
}

void CheckBoxKnob::updateValue(int v)
{
    *_myValue = this->isChecked();
    updateHash();
}

void CheckBoxKnob::valueChanged(bool v)
{
    int val = 0;
    if (v) {val = 1;}
    emit updateValue(val);
}

void CheckBoxKnob::updateHash()
{
    _myHash = generateHash(QString(*_myValue));
}

QString CheckBoxKnob::getHash()
{
    return _myHash;
}

/*
----------------------------------------------------------------------
COMBOBOX
----------------------------------------------------------------------
*/

ComboBoxKnob* ComboBox_knob(KnobCallback *f, int *value, QString label, QString valueName)
{
    qDebug() << "Combobox knob: " << value << *value << label;
    ComboBoxKnob *knob = new ComboBoxKnob(f, value);
    f->getLayout()->addRow(label, knob);
    f->addKnob(knob, label, (QVariant*)value);
    knob->setToolTip(valueName);
    return knob;
}


ComboBoxKnob::ComboBoxKnob(KnobCallback *f, int* value) :
    _myValue(value)
{
    this->setMinimumWidth(70);
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(updateValue(int)));
    connect(this, SIGNAL(currentIndexChanged(int)), f, SLOT(valueChanged()));
    updateHash();
}

void ComboBoxKnob::updateValue(int v)
{
    *_myValue = this->currentIndex();
    updateHash();
}

void ComboBoxKnob::updateHash()
{
    _myHash = generateHash(QString(*_myValue));
}

QString ComboBoxKnob::getHash()
{
    return _myHash;
}

/*
----------------------------------------------------------------------
FILEOPENDIALOG
----------------------------------------------------------------------
*/

FileDialogKnob* FileDialog_knob(KnobCallback *f, QString *value, QString label)
{
    FileDialogKnob *knob = new FileDialogKnob(f, value);
    f->getLayout()->addRow(label, knob);
    //f->addKnob(knob, label, *value);
    f->addKnob(knob, label, (QVariant*)value);
    knob->setToolTip(label.replace(":", "").toLower());
    knob->updateValueFromDialog(*value);
    return knob;
}


FileDialogKnob::FileDialogKnob(KnobCallback *f, QString* value) :
    _myValue(value)
{

    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->setMargin(0);
    this->setLayout(hLayout);

    QPushButton *dlgButton = new QPushButton();
    QLineEdit *nameknob = new QLineEdit();
    nameknob->setText(*value);

    hLayout->addWidget(nameknob);
    hLayout->addWidget(dlgButton);

    QObject::connect(dlgButton, SIGNAL(clicked()), this, SLOT(getFileName()));
    QObject::connect(this, SIGNAL(valueUpdated(QString)), nameknob, SLOT(setText(QString)));
    QObject::connect(this, SIGNAL(valueUpdated(QString)), this, SLOT(updateHash(QString)));
    QObject::connect(nameknob, SIGNAL(textChanged(QString)), f, SLOT(valueChanged()));
    updateHash();
}


void FileDialogKnob::updateValueFromDialog(QString s)
{
    *_myValue = s;
    updateHash();
}

void FileDialogKnob::getFileName()
{
    QString fname = QFileDialog::getOpenFileName(this, tr("Open File"), "C:/");
    *_myValue = fname;
    emit valueUpdated(fname);
}

void FileDialogKnob::updateHash(QString text)
{
    _myHash = generateHash(text);

}

void FileDialogKnob::updateHash()
{
    _myHash = generateHash(*_myValue);
}

QString FileDialogKnob::getHash()
{
    return _myHash;
}
