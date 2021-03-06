/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp -m -a src/applicationadaptor -c ApplicationAdaptor -i fullscreenframe.h -l FullScreenFrame /home/panda/Projects/panda-launcher/src/org.panda.launcher.xml
 *
 * qdbusxml2cpp is Copyright (C) 2019 The Qt Company Ltd.
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#include "applicationadaptor.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/*
 * Implementation of adaptor class ApplicationAdaptor
 */

ApplicationAdaptor::ApplicationAdaptor(FullScreenFrame *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

ApplicationAdaptor::~ApplicationAdaptor()
{
    // destructor
}

void ApplicationAdaptor::hideLauncher()
{
    // handle method call org.panda.Launcher.hideLauncher
    parent()->hideLauncher();
}

void ApplicationAdaptor::showLauncher()
{
    // handle method call org.panda.Launcher.showLauncher
    parent()->showLauncher();
}

void ApplicationAdaptor::toggleLauncher()
{
    // handle method call org.panda.Launcher.toggleLauncher
    parent()->toggleLauncher();
}
