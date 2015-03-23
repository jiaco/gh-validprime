ValidPrime is a method to correct for gDNA contamination in qPCR. GhValidPrime is an open source implementation of this method and contains features to enable both CqRNA calculation and to validate assay (screen for assays that do not amplify gDNA).

The software is in beta right now and at version 0.9.4.

There is a windows NSIS installer available to download from this site. If you have a previous install, be sure to use the uninstaller of the previous version first. This should be accessible from the Start menu (GeneHuggers->uninstall).

If you have used an old version (pre-alpha version numbers > 1.0) then you should also run the command regedit and delete the entire folder in HKEY\_LOCAL\_SETTINGS/Software called GeneHuggers. regedit should be used with caution. Locate this folder:

HKEY\_LOCAL\_SETTINGS/Software/GeneHuggers

right click it and select Delete from the popup menu.

[Source code install](SourceCodeInstall.md)