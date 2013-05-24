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
#include <QMessageBox>
#include <QPushButton>

// SlicerQt includes
#include "qSlicerHostedApplicationModuleWidget.h"
#include "ui_qSlicerHostedApplicationModuleWidget.h"

// CTKPluginFramework includes
#include <ctkConfig.h>
#include <ctkPluginFrameworkFactory.h>
#include <ctkPluginFramework.h>
#include <ctkPluginException.h>
#include <ctkPluginContext.h>

// ctkDICOM includes
#include <ctkDICOMDatabase.h>

#include <qSlicerDicomAppLogic.h>

// Qt includes for DAH
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QDirIterator>

// qSlicer includes
#include "qSlicerAbstractCoreModule.h"
#include "qSlicerApplication.h"
#include <qSlicerModuleManager.h>

// vtk includes
#include <vtkSmartPointer.h>
#include <vtkStringArray.h>

// vtkMRML includes
#include <vtkMRMLScalarVolumeNode.h>
#include <vtkMRMLScene.h>
#include <vtkMRMLSelectionNode.h>

// vtkSlicer includes
#include <vtkSlicerApplicationLogic.h>
#include <vtkSlicerVolumesLogic.h>

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerHostedApplicationModuleWidgetPrivate: public Ui_qSlicerHostedApplicationModuleWidget
{
public:
  qSlicerHostedApplicationModuleWidgetPrivate();

  qSlicerDicomAppLogic* AppLogic;

  QDockWidget *Dock;
  QPushButton *FinishedButton;

  QList< vtkSmartPointer<vtkMRMLScalarVolumeNode> > VolumeNodesFromHost;

};

//-----------------------------------------------------------------------------
// qSlicerHostedApplicationModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicerHostedApplicationModuleWidgetPrivate::qSlicerHostedApplicationModuleWidgetPrivate()
{
  this->AppLogic = 0;
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

  // CTK plugin Framework setup
  QString hostURL = "http://localhost:8080/HostInterface";//parsedArgs.value("hostURL").toString();
  QString appURL = "http://localhost:8081/ApplicationInterface";//parsedArgs.value("applicationURL").toString();
  qDebug() << "appURL is: " << appURL << " . Extracted port is: " << QUrl(appURL).port();

  // pass further parameters the plugins
  ctkProperties fwProps;
  fwProps.insert("dah.hostURL", hostURL);
  fwProps.insert("dah.appURL", appURL);
  fwProps.insert(ctkPluginConstants::FRAMEWORK_STORAGE_CLEAN, ctkPluginConstants::FRAMEWORK_STORAGE_CLEAN_ONFIRSTINIT);

  ctkPluginFrameworkFactory fwFactory(fwProps);
  QSharedPointer<ctkPluginFramework> framework = fwFactory.getFramework();
  try
    {
    framework->init();
    }
  catch (const ctkPluginException& exc)
    {
    qCritical() << "Failed to initialize the plug-in framework:" << exc;
    return;
    }

#ifdef CMAKE_INTDIR
  QString pluginPath = CTK_PLUGIN_DIR CMAKE_INTDIR "/";
#else
  QString pluginPath = CTK_PLUGIN_DIR;
#endif

  qApp->addLibraryPath(pluginPath);

  QStringList libFilter;
  libFilter << "*.dll" << "*.so" << "*.dylib";
  QDirIterator dirIter(pluginPath, libFilter, QDir::Files);

  QStringList pluginsToInstall;
  pluginsToInstall << "org_commontk_dah_core." << "org_commontk_dah_hostedapp";

  QList<QSharedPointer<ctkPlugin> > installedPlugins;
  while(dirIter.hasNext())
    {
    try
      {
      QString fileLocation = dirIter.next();
      foreach(QString pluginToInstall, pluginsToInstall)
        {
        if (fileLocation.contains(pluginToInstall))
          {
          QSharedPointer<ctkPlugin> plugin = framework->getPluginContext()->installPlugin(QUrl::fromLocalFile(fileLocation));
          installedPlugins << plugin;
          break;
          }
        }
      }
    catch (const ctkPluginException& e)
      {
      qCritical() << e.what();
      }
    }

  framework->start();

  foreach(QSharedPointer<ctkPlugin> plugin, installedPlugins)
    {
    plugin->start();
    }

  ctkPluginContext* context = fwFactory.getFramework()->getPluginContext();
  d->AppLogic = new qSlicerDicomAppLogic(context);
  context->registerService<ctkDicomAppInterface>(d->AppLogic);

  connect(d->AppLogic, SIGNAL(dataAvailable()), this, SLOT(onDataAvailable()));

  // Create a floating
  d->Dock = new QDockWidget();
  d->FinishedButton = new QPushButton(d->Dock);
  d->FinishedButton->setText("Finalize Hosted Application Task\n(push this when finished with operation)");
  d->Dock->setWidget(d->FinishedButton);
  d->Dock->setFloating(true);
  d->Dock->show();

  connect(d->FinishedButton, SIGNAL(clicked()), this, SLOT(onFinished()));

  this->Superclass::setup();
}


#include <ctkDicomAvailableDataHelper.h>
#include <ctkDicomHostInterface.h>

//-----------------------------------------------------------------------------
bool qSlicerHostedApplicationModuleWidget::loadDICOMSeriesAsVolume(QString seriesUID)
{
  Q_D(qSlicerHostedApplicationModuleWidget);

  // get the slicer volumes logic to use for loading
  qSlicerAbstractCoreModule* volumesModule =
    qSlicerApplication::application()->moduleManager()->module("Volumes");
  vtkSlicerVolumesLogic* volumesLogic =
    vtkSlicerVolumesLogic::SafeDownCast(volumesModule->logic());
  if (!volumesLogic)
    {
    qCritical() << "No volumes logic available in slicer.";
    return false;
    }

  // get the database
  ctkDICOMDatabase* dicomDatabase = qSlicerApplication::application()->dicomDatabase();

  // get the filepaths for each file in the series, and the series description to use as a name
  QStringList seriesFiles = dicomDatabase->filesForSeries(seriesUID);
  if (seriesFiles.size() == 0)
    {
    qCritical() << "There are no files in the database for the series: " << seriesUID;
    return false;
    }
  QString seriesDescription = dicomDatabase->fileValue(seriesFiles[0], "0008,103e");
  qDebug() << "Series Description: " << seriesDescription;

  // get a vtkStringArray of all files that are part of the series
  vtkSmartPointer<vtkStringArray> fileList = vtkSmartPointer<vtkStringArray>::New();
  foreach(const QString &seriesFile, seriesFiles)
    {
    fileList->InsertNextValue(seriesFile.toLatin1().data());
    }

  // Perform the actual data loading
  QString archetypeFilePath = seriesFiles[0];
  const char *archetypeFilePathPointer = archetypeFilePath.toLatin1().data();
  const char *volumeName = seriesDescription.toLatin1().data();
  vtkMRMLScalarVolumeNode *volumeNode = volumesLogic->AddArchetypeScalarVolume(archetypeFilePathPointer,volumeName,0,fileList);
  if (!volumeNode)
    {
    qCritical() << "Failed to load file: " << archetypeFilePath;
    return false;
    }
  volumeNode->SetName(volumeName);
  d->VolumeNodesFromHost << vtkSmartPointer<vtkMRMLScalarVolumeNode>(volumeNode);

  // automatically select the volume to display
  vtkSlicerApplicationLogic *appLogic = qSlicerApplication::application()->applicationLogic();
  vtkMRMLSelectionNode *selNode = appLogic->GetSelectionNode();
  selNode->SetReferenceActiveVolumeID(volumeNode->GetID());
  appLogic->PropagateVolumeSelection();

  return true;
}

//-----------------------------------------------------------------------------
void qSlicerHostedApplicationModuleWidget::addDICOMFileToDatabase(QString filePath)
{
  ctkDICOMDatabase* dicomDatabase = qSlicerApplication::application()->dicomDatabase();
  dicomDatabase->insert(filePath);
}

//-----------------------------------------------------------------------------
void qSlicerHostedApplicationModuleWidget::onDataAvailable()
{
  Q_D(qSlicerHostedApplicationModuleWidget);
  const ctkDicomAppHosting::AvailableData& data = d->AppLogic->getIncomingAvailableData();
  if(data.patients.count()==0)
    return;
  const ctkDicomAppHosting::Patient& firstpatient = *data.patients.begin();
  QList<QUuid> uuidlist = ctkDicomAvailableDataHelper::getAllUuids(firstpatient);

  QString transfersyntax("1.2.840.10008.1.2.1");
  QList<QString> transfersyntaxlist;
  transfersyntaxlist.append(transfersyntax);
  QList<ctkDicomAppHosting::ObjectLocator> locators;
  locators = d->AppLogic->getHostInterface()->getData(uuidlist, transfersyntaxlist, false);
  qDebug() << "got locators! " << QString().setNum(locators.count());

  // go through the available data and add each dicom file into 
  // the slicer local dicom database
  QString s;
  s=s+" loc.count:"+QString().setNum(locators.count());
  foreach(ctkDicomAppHosting::ObjectLocator locator, locators)
  {
    s=s+" URI: "+locator.URI +" locatorUUID: "+locator.locator+" sourceUUID: "+locator.source;
    qDebug() << "URI: " << locator.URI;
    QString filename = locator.URI;
    if(filename.startsWith("file:/",Qt::CaseInsensitive))
      filename=filename.remove(0,8);
    qDebug()<<filename;
    if(QFileInfo(filename).exists())
    {
      try {
        qDebug() << "Everything looks great: now we will load the following file: " << filename;
        this->addDICOMFileToDatabase(filename);
      }
      catch(...)
      {
        qCritical() << "Caught exception while trying to load file" << filename;
      }
    }
    else
    {
      qCritical() << "File does not exist: " << filename;
    }
  }

  // Now the data should go from the database into slicer, with series mapped to volumes
  foreach(ctkDicomAppHosting::Patient patient, data.patients)
    {
    foreach(ctkDicomAppHosting::Study study, patient.studies)
      {
      foreach(ctkDicomAppHosting::Series series, study.series)
        {
        qDebug() << "Now let's try loading the data!" << series.seriesUID;
        bool result = this->loadDICOMSeriesAsVolume(series.seriesUID);
        if (result)
          {
          qDebug() << "Load completed!";
          }
        else
          {
          qCritical() << "Shoot!  Cannot load series: " << series.seriesUID;
          }
        }
      }
    }
}

//-----------------------------------------------------------------------------
void qSlicerHostedApplicationModuleWidget::onFinished()
{
  Q_D(qSlicerHostedApplicationModuleWidget);

  vtkMRMLScene *scene = qSlicerApplication::application()->mrmlScene();
  vtkSmartPointer<vtkMRMLScalarVolumeNode> hostVolumeNode;
  vtkMRMLScalarVolumeNode *slicerVolumeNode;

  int volumeCount = scene->GetNumberOfNodesByClass("vtkMRMLScalarVolumeNode");
  int volumeIndex;
  for (volumeIndex = 0; volumeIndex < volumeCount; ++volumeIndex)
    {
    slicerVolumeNode = vtkMRMLScalarVolumeNode::SafeDownCast(
                          scene->GetNthNodeByClass(volumeIndex, "vtkMRMLScalarVolumeNode"));
    qDebug() << "Checking volume: " << QString(slicerVolumeNode->GetName());
    bool newNode = true;
    foreach (hostVolumeNode, d->VolumeNodesFromHost)
      {
      if (hostVolumeNode.GetPointer() == slicerVolumeNode)
        {
        newNode = false;
        }
      }
    QString message("");
    if (newNode)
      {
      message += "Found volume to return: ";
      }
    else
      {
      message += "This volume is not new: ";
      }
    message += QString(slicerVolumeNode->GetName());
    QMessageBox::information(this, "Hosting", message);
    }
}
