/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp -m -a src/applicationadaptor -c ApplicationAdaptor -i fullscreenframe.h -l FullScreenFrame /home/panda/Projects/panda-launcher/src/org.panda.launcher.xml
 *
 * qdbusxml2cpp is Copyright (C) 2019 The Qt Company Ltd.
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#ifndef APPLICATIONADAPTOR_H
#define APPLICATIONADAPTOR_H

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
#include "fullscreenframe.h"
QT_BEGIN_NAMESPACE
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;
QT_END_NAMESPACE

/*
 * Adaptor class for interface org.panda.Launcher
 */
class ApplicationAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.panda.Launcher")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"org.panda.Launcher\">\n"
"    <method name=\"showLauncher\"/>\n"
"    <method name=\"hideLauncher\"/>\n"
"    <method name=\"toggleLauncher\"/>\n"
"  </interface>\n"
        "")
public:
    ApplicationAdaptor(FullScreenFrame *parent);
    virtual ~ApplicationAdaptor();

    inline FullScreenFrame *parent() const
    { return static_cast<FullScreenFrame *>(QObject::parent()); }

public: // PROPERTIES
public Q_SLOTS: // METHODS
    void hideLauncher();
    void showLauncher();
    void toggleLauncher();
Q_SIGNALS: // SIGNALS
};

#endif
