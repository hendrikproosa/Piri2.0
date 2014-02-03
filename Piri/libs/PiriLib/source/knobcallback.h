#ifndef KNOBCALLBACK_H
#define KNOBCALLBACK_H

#include <QtWidgets>

#include "pirilib.h"

struct KnobStruct {
    QWidget* widget;
    QString label;
    QVariant* value;
    QString* hash;
};

class Node;

class PIRILIBSHARED_EXPORT KnobCallback : public QWidget
{
    Q_OBJECT
public:
    KnobCallback(Node* parent);

    void makeKnobs();
    QFormLayout *getLayout();
    Node* getParent();

    void addKnob(QWidget* knob, QString label, QVariant *value);
    KnobStruct* getKnob(QString knobLabel);

    QString getHash();

    void showError(QString msg);

public slots:
    void valueChanged(); /*! Called when value of any knob is changed */
    void updateKnobs(); /*! Updates all knobs */
    void nodeNameChanged(QString name); /*! Called when node name is changed */

private:
    QFormLayout* myLayout; /*! Callback layout that holds knobs */
    Node* myParent; /*! Parent node of this callback */
    QList<KnobStruct*> knobs; /*! List of knobs in this callback */
    QString myHash; /*! Hash of this callback, calculated from knob values*/
    QString topHash; /*! Hash of whole node tree above parent node of this callback */

};

#endif // KNOBCALLBACK_H
