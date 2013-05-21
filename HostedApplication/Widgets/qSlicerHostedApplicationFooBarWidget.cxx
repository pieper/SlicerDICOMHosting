/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// FooBar Widgets includes
#include "qSlicerHostedApplicationFooBarWidget.h"
#include "ui_qSlicerHostedApplicationFooBarWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_HostedApplication
class qSlicerHostedApplicationFooBarWidgetPrivate
  : public Ui_qSlicerHostedApplicationFooBarWidget
{
  Q_DECLARE_PUBLIC(qSlicerHostedApplicationFooBarWidget);
protected:
  qSlicerHostedApplicationFooBarWidget* const q_ptr;

public:
  qSlicerHostedApplicationFooBarWidgetPrivate(
    qSlicerHostedApplicationFooBarWidget& object);
  virtual void setupUi(qSlicerHostedApplicationFooBarWidget*);
};

// --------------------------------------------------------------------------
qSlicerHostedApplicationFooBarWidgetPrivate
::qSlicerHostedApplicationFooBarWidgetPrivate(
  qSlicerHostedApplicationFooBarWidget& object)
  : q_ptr(&object)
{
}

// --------------------------------------------------------------------------
void qSlicerHostedApplicationFooBarWidgetPrivate
::setupUi(qSlicerHostedApplicationFooBarWidget* widget)
{
  this->Ui_qSlicerHostedApplicationFooBarWidget::setupUi(widget);
}

//-----------------------------------------------------------------------------
// qSlicerHostedApplicationFooBarWidget methods

//-----------------------------------------------------------------------------
qSlicerHostedApplicationFooBarWidget
::qSlicerHostedApplicationFooBarWidget(QWidget* parentWidget)
  : Superclass( parentWidget )
  , d_ptr( new qSlicerHostedApplicationFooBarWidgetPrivate(*this) )
{
  Q_D(qSlicerHostedApplicationFooBarWidget);
  d->setupUi(this);
}

//-----------------------------------------------------------------------------
qSlicerHostedApplicationFooBarWidget
::~qSlicerHostedApplicationFooBarWidget()
{
}
