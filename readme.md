
* This is a 3D Slicer Extension for DICOM Application Hosting that uses the Common Toolkit DICOM Application Hosting software.

It is still very much a work in progress, developed as part of the CTK Hackfest Canada in May of 2013.

Here is a useful line for starting slicer with an experimental build of this extension:

  MODULEBUILDPATH=/path/to/build/lib/Slicer-4.2/qt-loadable-modules
  ./Slicer --python-code "slicer.util.mainWindow().moduleSelector().selectModule('HostedApplication')" --additional-module-paths ${MODULEBUILDPATH}


