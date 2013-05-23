This is a 3D Slicer Extension for DICOM Application Hosting that uses the Common Toolkit DICOM Application Hosting software.

It is still very much a work in progress, developed as part of the CTK Hackfest Canada in May of 2013.

Here is a useful line for starting slicer with an experimental build of this extension:

<pre>
  MODULEBUILDPATH=/path/to/build/lib/Slicer-4.2/qt-loadable-modules
  ./Slicer --python-code "slicer.util.mainWindow().moduleSelector().selectModule('HostedApplication')" --additional-module-paths ${MODULEBUILDPATH}
</pre>

<b>The current implementation may still aspect hard-coded URLs for communication:</b> 
For windows you may need to use the following syntax for the argument to --python-code:
<pre>
--hostURL http://localhost:8080/HostInterface --applicationURL http://localhost:8081/ApplicationInterface
 "slicer.util.mainWindow().moduleSelector().selectModule(\"HostedApplication\")"
</pre>


Creating a working version of this requires that slicer be built against a version of CTK that includes DICOM Application Hosting support:
<ul>
  <li>Change the GIT TAG in ./Slicer/SuperBuild/External_CTK.cmake to 06c8e3172522446230fc95d3a0a4e7b6ceb1ffd2 (or newer)</li>
  <li>Switch on CTK_ENABLE_DICOMApplicationHosting in &lt;Slicerbuild-dir&gt;/CTK-build</li>
  <li>If you want to use the ctkDICOMHost as hosting system, switch on CTK_APP_CtkDICOMHost in &lt;Slicerbuild-dir&gt;/CTK-build</li>
  <li>Rebuild the CTK-superbuild (which will download QtSoap)</li>
  <li>Create a script/batch file for starting Slicer as a hosted application
<pre>
MODULEBUILDPATH=/path/to/build/lib/Slicer-4.2/qt-loadable-modules
QT_PLUGIN_PATH=&lt;Slicerbuild-dir&gt;/CTK-build/CTK-build/bin
./Slicer --python-code "slicer.util.mainWindow().moduleSelector().selectModule('HostedApplication')" --additional-module-paths ${MODULEBUILDPATH}
</pre>
  </li>
  <li>Start the hosting system (if compiled, the ctkDICOMHost can be found in &lt;Slicerbuild-dir&gt;/CTK-build/CTK-build/bin)</li>
  <li>Set the script/batch file (see above) as the hosted application</li>
</ul>

See this revision for some details: https://github.com/commontk/CTK/commit/06c8e3172522446230fc95d3a0a4e7b6ceb1ffd2
