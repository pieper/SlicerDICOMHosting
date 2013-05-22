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


#ifndef __qSlicerDicomAppLogic_h
#define __qSlicerDicomAppLogic_h

// Qt includes
#include <QUuid>

// CTK includes
#include <ctkDicomAbstractApp.h>
#include <ctkDicomAppInterface.h>

#include <ctkServiceTracker.h>

struct ctkDicomAppInterface;

class QWidget;

class qSlicerDicomAppLogic : public ctkDicomAbstractApp
{
  Q_OBJECT
  Q_INTERFACES(ctkDicomAppInterface)

public:

  qSlicerDicomAppLogic(ctkPluginContext* context);
  virtual ~qSlicerDicomAppLogic();

  // ctkDicomAppInterface

  /**
   * Method triggered by the host. By calling this method, the Hosting System is asking the Hosted Application to take whatever steps are
   * needed to make its GUI visible as the topmost window, and to gain focus.
   * \return TRUE if the Hosted Application received the request and will act on it. Otherwise it returns FALSE
   */
  virtual bool bringToFront(const QRect& requestedScreenArea);

  void setAppWidget(QWidget * widget);

protected Q_SLOTS:

  void onStartProgress();
  void onResumeProgress();
  void onSuspendProgress();
  void onCancelProgress();
  void onExitHostedApp();
  void onReleaseResources();

private:
  QWidget * AppWidget;
  ctkPluginContext* Context;
}; // qSlicerDicomAppLogic


#endif
