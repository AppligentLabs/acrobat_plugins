# acrobat_plugins
A collection of source files provided as examples of how to write plug-ins for Adobe Acrobat.

Most of this source code is very old and we have not tried to compile these plug-ins under more recent versions of Acrobat, however the basic scructure is sound and has not changed significantly since the release of the original Acrobat SDK.  In addition, in some cases not all of the utility source is provided.

The source files are best viewed when setting tab spacing equivalent to two space characters.

// --------------------

ClickMove
Designed as a utility for presentations using PDF files in full screen mode.  If the document is in full screen mode then a single click will advance the page and a double click will step back through pages.

This code demonstrates the setup and usage of an AVPageViewClickProc.

// --------------------

ListMenuNames
Utility to list all of the menu names used by Acrobat and its plug-ins.

This code demonstrates enumerating all of the Acrobat menu items.

// --------------------

ReversePages
Older Postscript documents available on the Internet will sometimes Distill into PDF in the reverse page order.  This is typically because they were originally printed using one of the original Apple LaserWriter print drivers. The original LaserWriters did not flip the paper before placing it in the output tray; so, the print driver would send the document to the printer in reverse order to collate properly.

// --------------------

TriState
Acrobat 3 contained two sets of three toolbar buttons; Page Mode and Page View.

Page Mode contained the following three buttons:
None, Show Bookmarks and Show Thumbnails
Page View contained the following three buttons:
Fit Page, Fit Visible & Zoom 100%

To save space on the toolbar; TriState was built to convert groups of three toolbar buttons into single tri-state buttons.

This source demonstrates the use of the following notifications:

AVDocDidOpen
AVAppFrontDocDidChange
AVPageViewDidChange

In addition, it also demonstrates adding and removing toolbar buttons.

// --------------------
