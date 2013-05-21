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
#include <QtPlugin>

// HostedApplication Logic includes
#include <vtkSlicerHostedApplicationLogic.h>

// HostedApplication includes
#include "qSlicerHostedApplicationModule.h"
#include "qSlicerHostedApplicationModuleWidget.h"

//-----------------------------------------------------------------------------
Q_EXPORT_PLUGIN2(qSlicerHostedApplicationModule, qSlicerHostedApplicationModule);

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerHostedApplicationModulePrivate
{
public:
  qSlicerHostedApplicationModulePrivate();
};

//-----------------------------------------------------------------------------
// qSlicerHostedApplicationModulePrivate methods

//-----------------------------------------------------------------------------
qSlicerHostedApplicationModulePrivate
::qSlicerHostedApplicationModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerHostedApplicationModule methods

//-----------------------------------------------------------------------------
qSlicerHostedApplicationModule
::qSlicerHostedApplicationModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerHostedApplicationModulePrivate)
{
}

//-----------------------------------------------------------------------------
qSlicerHostedApplicationModule::~qSlicerHostedApplicationModule()
{
}

//-----------------------------------------------------------------------------
QString qSlicerHostedApplicationModule::helpText()const
{
  return "This uses a loadable module bundled in an extension to make slicer behave as a hosted application according to the DICOM Application Hosting specification.";
}

//-----------------------------------------------------------------------------
QString qSlicerHostedApplicationModule::acknowledgementText()const
{
  return "This work was was partially funded by NIH grant 3P41RR013218-12S1";
}

//-----------------------------------------------------------------------------
QStringList qSlicerHostedApplicationModule::contributors()const
{
  QStringList moduleContributors;
  moduleContributors << QString("Steve Pieper (Isomics), Ivo Wolf (DKFZ), Lawrence Tarbox (Washington University, St. Louis)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qSlicerHostedApplicationModule::icon()const
{
  return QIcon(":/Icons/HostedApplication.png");
}

//-----------------------------------------------------------------------------
QStringList qSlicerHostedApplicationModule::categories() const
{
  return QStringList() << "Servers";
}

//-----------------------------------------------------------------------------
QStringList qSlicerHostedApplicationModule::dependencies() const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
void qSlicerHostedApplicationModule::setup()
{
  this->Superclass::setup();
}

//-----------------------------------------------------------------------------
qSlicerAbstractModuleRepresentation * qSlicerHostedApplicationModule
::createWidgetRepresentation()
{
  return new qSlicerHostedApplicationModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qSlicerHostedApplicationModule::createLogic()
{
  return vtkSlicerHostedApplicationLogic::New();
}
