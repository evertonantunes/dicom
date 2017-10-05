#! /bin/bash

wget -c -r -l 1 http://dicom.nema.org/medical/dicom/current/output/pdf
cp -r dicom.nema.org/medical/dicom/current/output/pdf/* .
rm -r dicom.nema.org/
