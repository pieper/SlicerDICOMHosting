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

#ifndef __qSlicerHostedApplicationModuleWidget_h
#define __qSlicerHostedApplicationModuleWidget_h

// SlicerQt includes
#include "qSlicerAbstractModuleWidget.h"

#include "qSlicerHostedApplicationModuleExport.h"

class qSlicerHostedApplicationModuleWidgetPrivate;
class vtkMRMLNode;

/// \ingroup Slicer_QtModules_ExtensionTemplate
class Q_SLICER_QTMODULES_HOSTEDAPPLICATION_EXPORT qSlicerHostedApplicationModuleWidget :
  public qSlicerAbstractModuleWidget
{
  Q_OBJECT

public:

  typedef qSlicerAbstractModuleWidget Superclass;
  qSlicerHostedApplicationModuleWidget(QWidget *parent=0);
  virtual ~qSlicerHostedApplicationModuleWidget();

  // utilies available for processing data from the host
  bool loadDICOMSeriesAsVolume(QString seriesUID);
  void addDICOMFileToDatabase(QString filePath);

public slots:
  void onDataAvailable();
  void onFinished();

protected:
  QScopedPointer<qSlicerHostedApplicationModuleWidgetPrivate> d_ptr;
  
  virtual void setup();

private:
  Q_DECLARE_PRIVATE(qSlicerHostedApplicationModuleWidget);
  Q_DISABLE_COPY(qSlicerHostedApplicationModuleWidget);
};

#endif
