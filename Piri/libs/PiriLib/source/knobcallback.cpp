#include "knobcallback.h"
#include "node.h"
#include "nodegraph.h"
#include "mainwindow.h"
#include "knobs.h"

#include <QtDebug>


KnobCallback::KnobCallback(Node *parent)
{
    myParent = parent;
    myHash = "no_hash";
    setFocusPolicy(Qt::WheelFocus);

    makeKnobs();
}


void KnobCallback::makeKnobs()
{

    // Header bar
    QLineEdit *nodeName = new QLineEdit(myParent->getName());
    QPushButton *buttonh = new QPushButton("?");
    QPushButton *buttonx = new QPushButton("✕");

    buttonh->setFixedWidth(25);
    buttonh->setToolTip(myParent->getDesc());
    buttonx->setFixedWidth(25);
    connect(buttonx, SIGNAL(clicked()), this, SLOT(hide()));
    connect(nodeName, SIGNAL(textChanged(QString)), this, SLOT(nodeNameChanged(QString)));

    QWidget *header = new QWidget();
    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->setMargin(0);
    header->setLayout(hLayout);
    hLayout->addWidget(nodeName);
    hLayout->addWidget(buttonh);
    hLayout->addWidget(buttonx);

    // Widgetid, mis kirjeldavad erinevaid knobide tab'e
    QWidget *knobArea = new QWidget();
    QWidget *notesArea = new QWidget();
    QWidget *metadataArea = new QWidget();
    QWidget *userKnobArea = new QWidget();

    // Knobide ala layout - vormilayout, mis jaotab üksteise alla label-widget põhimõttel
    QFormLayout *layout = new QFormLayout;
    layout->setMargin(20);
    knobArea->setLayout(layout);
    myLayout = layout;

    // Tabwidget mis hoiab erinevaid knobide tab'e
    QTabWidget *propTab = new QTabWidget;
    //propTab->addTab(vGroupBox, "Knobs");
    propTab->addTab(knobArea, "Knobs");
    propTab->addTab(metadataArea, "Metadata");
    propTab->addTab(notesArea, "Notes");
    propTab->addTab(userKnobArea, "User");
    propTab->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    // Kogu ühe sõlme andmete hoidjawidget ja selle layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(header);
    mainLayout->addWidget(propTab);
    //mainLayout->setSizePolicy(QSizePolicy::Expanding);
    this->setLayout(mainLayout);

}

/*!
 * \brief Called when node name is changed in callback.
 *
 * Updates node name so that it is drawn on top of node properly.
 * \param name New name as string.
 */
void KnobCallback::nodeNameChanged(QString name)
{
    myParent->setName(name);
}

/*!
 * \brief Get callback layout that holds knobs.
 * \return Layout as QFormLayout
 */
QFormLayout *KnobCallback::getLayout()
{
    return myLayout;
}

/*!
 * \brief Get callback parent node.
 * \return Parent node
 */
Node* KnobCallback::getParent()
{
    return myParent;
}

/*!
 * \brief Creates new knob structure and adds it to knob list "knobs"
 * \param knob Knob widget
 * \param label Knob label
 * \param value Knob value as QVariant
 */
void KnobCallback::addKnob(QWidget* knob, QString label, QVariant* value)
{
    KnobStruct *kn = new KnobStruct;
    kn->widget = knob;
    kn->label = label;
    kn->value = value;
    knobs << kn;
}


/*!
 * \brief Get knob structure for knob with label knobLabel
 * \param knobLabel Label of knob searched
 * \return Knob structure if found, 0 otherwise
 */
KnobStruct* KnobCallback::getKnob(QString knobLabel)
{
    foreach (KnobStruct *kn, knobs) {
        if (kn->label == knobLabel)
            return kn;
    }
    return 0;
}

/*!
 * \brief Get callback hash
 *
 * Calls all knobs and adds their hash values. In the end calculates
 * new hash from this.
 * \return Callback hash
 */
QString KnobCallback::getHash()
{
    QString hashString;
    foreach (KnobStruct *ks, knobs) {
        QWidget* w = ks->widget;

        KnobBase* kb = dynamic_cast<KnobBase*>(w);
        if (kb)
        {
            hashString += kb->getHash();
        }

    }
    myHash = QString(QCryptographicHash::hash(hashString.toLatin1(),QCryptographicHash::Md5).toHex());
    return myHash;
}

/*!
 * \brief Called when knob value is changed. Calls evaluate on nodegraph.
 */
void KnobCallback::valueChanged()
{
    myParent->getParent()->evaluate();
}

/*!
 * \brief Updates callback knobs. Not working at the moment!!!
 */
void KnobCallback::updateKnobs()
{
    //myParent->getOp()->make_knobs(this);
}

/*!
 * \brief Show error on statusbar and add it to messagelog
 * \param msg Message as Qstring
 */
void KnobCallback::showError(QString msg)
{
    myParent->getParent()->getParent()->logMessage(msg);
    myParent->getParent()->getParent()->statusBar()->showMessage(msg);
}
