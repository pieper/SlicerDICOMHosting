/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

// Qt includes
#include <QDebug>
#include <QDockWidget>
#include <QPushButton>

// SlicerQt includes
#include "qSlicerHostedApplicationModuleWidget.h"
#include "ui_qSlicerHostedApplicationModuleWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerHostedApplicationModuleWidgetPrivate: public Ui_qSlicerHostedApplicationModuleWidget
{
public:
  qSlicerHostedApplicationModuleWidgetPrivate();

  QDockWidget *Dock;
  QPushButton *FinishedButton;
};

//-----------------------------------------------------------------------------
// qSlicerHostedApplicationModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicerHostedApplicationModuleWidgetPrivate::qSlicerHostedApplicationModuleWidgetPrivate()
{
  this->Dock = 0;
  this->FinishedButton = 0;
}

//-----------------------------------------------------------------------------
// qSlicerHostedApplicationModuleWidget methods

//-----------------------------------------------------------------------------
qSlicerHostedApplicationModuleWidget::qSlicerHostedApplicationModuleWidget(QWidget* _parent)
  : Superclass( _parent )
  , d_ptr( new qSlicerHostedApplicationModuleWidgetPrivate )
{
}

//-----------------------------------------------------------------------------
qSlicerHostedApplicationModuleWidget::~qSlicerHostedApplicationModuleWidget()
{
}

//-----------------------------------------------------------------------------
void qSlicerHostedApplicationModuleWidget::setup()
{
  Q_D(qSlicerHostedApplicationModuleWidget);
  d->setupUi(this);

  // Create a floating 
  d->Dock = new QDockWidget();
  d->FinishedButton = new QPushButton(d->Dock);
  d->FinishedButton->setText("Finalize Hosted Application Task\n(push this when finished with operation)");
  d->Dock->setWidget(d->FinishedButton);
  d->Dock->setFloating(true);
  d->Dock->show();
  this->Superclass::setup();
}

