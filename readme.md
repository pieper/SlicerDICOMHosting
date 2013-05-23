This is a 3D Slicer Extension for DICOM Application Hosting that uses the Common Toolkit DICOM Application Hosting software.

It is still very much a work in progress, developed as part of the CTK Hackfest Canada in May of 2013.

Here is a useful line for starting slicer with an experimental build of this extension:

<pre>
  MODULEBUILDPATH=/path/to/build/lib/Slicer-4.2/qt-loadable-modules
  ./Slicer --python-code "slicer.util.mainWindow().moduleSelector().selectModule('HostedApplication')" --additional-module-paths ${MODULEBUILDPATH}
</pre>

For windows you may need to use the following syntax for the argument to --python-code:
<pre>
 "slicer.util.mainWindow().moduleSelector().selectModule(\"HostedApplication\")"
</pre>

Creating a working version of this requires that slicer be built against a version of CTK that includes DICOM Application Hosting support. 

See this revision for some details: https://github.com/commontk/CTK/commit/c9cf75e1ce14bd0e1c53641de1b142d84fdc2d76
