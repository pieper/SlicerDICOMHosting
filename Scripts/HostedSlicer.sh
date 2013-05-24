#!/usr/bin/env bash

MODULEBUILDPATH=/Users/pieper/slicer4/latest/DICOMHosting-build/lib/Slicer-4.2/qt-loadable-modules
SLICER_SUPERBUILD=/Users/pieper/slicer4/latest/Slicer-superbuild
QT_PLUGIN_PATH=${SLICER_SUPERBUILD}/CTK-build/CTK-build/bin

${SLICER_SUPERBUILD}/Slicer-build/Slicer --python-code "slicer.util.mainWindow().moduleSelector().selectModule('HostedApplication')" --additional-module-paths ${MODULEBUILDPATH}
