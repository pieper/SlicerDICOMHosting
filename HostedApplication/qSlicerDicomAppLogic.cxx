/*=============================================================================

  Library: MITK

  Copyright (c) German Cancer Research Center,
    Division of Medical and Biological Informatics

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

=============================================================================*/

// Qt includes
#include <QtPlugin>
#include <QRect>
#include <QDebug>
#include <QPushButton>
#include <QApplication>
#include <QLabel>
#include <QRect>
#include <QStringList>
#include <QDir>
#include <QTemporaryFile>
#include <QPainter>

// CTK includes
#include <ctkDicomAppHostingTypes.h>
#include <ctkDicomHostInterface.h>
#include "ctkDicomAvailableDataHelper.h"

// DCMTK includes
#include <dcmimage.h>

#include "qSlicerDicomAppLogic.h"

//----------------------------------------------------------------------------
qSlicerDicomAppLogic::qSlicerDicomAppLogic(ctkPluginContext* context):
ctkDicomAbstractApp(context), Context(context), AppWidget(0)
{
  connect(this, SIGNAL(startProgress()), this, SLOT(onStartProgress()), Qt::QueuedConnection);
  connect(this, SIGNAL(resumeProgress()), this, SLOT(onResumeProgress()), Qt::QueuedConnection);
  connect(this, SIGNAL(suspendProgress()), this, SLOT(onSuspendProgress()), Qt::QueuedConnection);
  connect(this, SIGNAL(cancelProgress()), this, SLOT(onCancelProgress()), Qt::QueuedConnection);
  connect(this, SIGNAL(exitHostedApp()), this, SLOT(onExitHostedApp()), Qt::QueuedConnection);

  //notify Host we are ready.
  try {
    getHostInterface()->notifyStateChanged(ctkDicomAppHosting::IDLE);
  }
  catch(...)
  {
    qDebug() << "qSlicerDicomAppLogic: Could not getHostInterface()";
  }
}

//----------------------------------------------------------------------------
qSlicerDicomAppLogic::~qSlicerDicomAppLogic()
{
  QList <QSharedPointer<ctkPlugin> > plugins = Context->getPlugins();
  for (int i = 0; i < plugins.size(); ++i)
  {
    qDebug() << plugins.at(i)->getSymbolicName ();
  }
}

//----------------------------------------------------------------------------
bool qSlicerDicomAppLogic::bringToFront(const QRect& requestedScreenArea)
{
  if(this->AppWidget!=NULL)
  {
    this->AppWidget->move(requestedScreenArea.topLeft());
    this->AppWidget->resize(requestedScreenArea.size());
    this->AppWidget->activateWindow();
    this->AppWidget->show();
    //this->AppWidget->raise();
  }
  return true;
}

//----------------------------------------------------------------------------
void qSlicerDicomAppLogic::onStartProgress()
{
  setInternalState(ctkDicomAppHosting::INPROGRESS);

  getHostInterface()->notifyStateChanged(ctkDicomAppHosting::INPROGRESS);

  if(this->AppWidget)
  {
    this->AppWidget->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->AppWidget->show();
  }
}

//----------------------------------------------------------------------------
void qSlicerDicomAppLogic::onResumeProgress()
{
  //reclame all resources.

  //notify state changed
  setInternalState(ctkDicomAppHosting::INPROGRESS);
  getHostInterface()->notifyStateChanged(ctkDicomAppHosting::INPROGRESS);
  if(this->AppWidget)
  {
    this->AppWidget->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->AppWidget->show();
  }
  //we're rolling
  //do something else normally, but this is an example
  //xxxui.LoadDataButton->setEnabled(true);
}

//----------------------------------------------------------------------------
void qSlicerDicomAppLogic::onSuspendProgress()
{
  //release resources it can reclame later to resume work
  //ui.LoadDataButton->setEnabled(false);
  //notify state changed
  setInternalState(ctkDicomAppHosting::SUSPENDED);
  getHostInterface()->notifyStateChanged(ctkDicomAppHosting::SUSPENDED);

  if(this->AppWidget)
  {
    this->AppWidget->show();
  }
}

//----------------------------------------------------------------------------
void qSlicerDicomAppLogic::onCancelProgress()
{
  //release all resources
  onReleaseResources();
  //update state
  setInternalState(ctkDicomAppHosting::IDLE);
  getHostInterface()->notifyStateChanged(ctkDicomAppHosting::IDLE);
  if(this->AppWidget)
  {
    this->AppWidget->hide();
  }
}

//----------------------------------------------------------------------------
void qSlicerDicomAppLogic::onExitHostedApp()
{
  //useless move, but correct:
  setInternalState(ctkDicomAppHosting::EXIT);
  getHostInterface()->notifyStateChanged(ctkDicomAppHosting::EXIT);
  qDebug() << "Exiting";
  //die
  qApp->exit(0);
}

//----------------------------------------------------------------------------
void qSlicerDicomAppLogic::onReleaseResources()
{
  this->AppWidget->hide();
}

void qSlicerDicomAppLogic::setAppWidget(QWidget * widget)
{
  this->AppWidget = widget;

  // hide widget (and set widget falgs) if we are really running as a hosted app
  if(Context->getProperty("dah.appURL").toString().isEmpty()==false)
  {
    this->AppWidget->hide();
    this->AppWidget->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
  }
}
