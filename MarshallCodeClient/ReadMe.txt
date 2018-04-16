================================================================================
    MICROSOFT FOUNDATION CLASS LIBRARY : MarshallCodeClient Project Overview
===============================================================================

The application wizard has created this MarshallCodeClient application for
you.  This application not only demonstrates the basics of using the Microsoft
Foundation Classes but is also a starting point for writing your application.

This file contains a summary of what you will find in each of the files that
make up your MarshallCodeClient application.

MarshallCodeClient.vcxproj
    This is the main project file for VC++ projects generated using an application wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    application wizard.

MarshallCodeClient.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the assciation between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

MarshallCodeClient.h
    This is the main header file for the application.  It includes other
    project specific headers (including Resource.h) and declares the
    CMarshallCodeClientApp application class.

MarshallCodeClient.cpp
    This is the main application source file that contains the application
    class CMarshallCodeClientApp.

MarshallCodeClient.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited in Microsoft
    Visual C++. Your project resources are in 1033.

res\MarshallCodeClient.ico
    This is an icon file, which is used as the application's icon.  This
    icon is included by the main resource file MarshallCodeClient.rc.

res\MarshallCodeClient.rc2
    This file contains resources that are not edited by Microsoft
    Visual C++. You should place all resources not editable by
    the resource editor in this file.

MarshallCodeClient.reg
    This is an example .reg file that shows you the kind of registration
    settings the framework will set for you.  You can use this as a .reg
    file to go along with your application or just delete it and rely
    on the default RegisterShellFileTypes registration.


/////////////////////////////////////////////////////////////////////////////

For the main frame window:
    The project includes a standard MFC interface.

MainFrm.h, MainFrm.cpp
    These files contain the frame class CMainFrame, which is derived from
    CFrameWnd and controls all SDI frame features.

res\Toolbar.bmp
    This bitmap file is used to create tiled images for the toolbar.
    The initial toolbar and status bar are constructed in the CMainFrame
    class. Edit this toolbar bitmap using the resource editor, and
    update the IDR_MAINFRAME TOOLBAR array in MarshallCodeClient.rc to add
    toolbar buttons.
/////////////////////////////////////////////////////////////////////////////

The application wizard creates one document type and one view:

MarshallCodeClientDoc.h, MarshallCodeClientDoc.cpp - the document
    These files contain your CMarshallCodeClientDoc class.  Edit these files to
    add your special document data and to implement file saving and loading
    (via CMarshallCodeClientDoc::Serialize).
    The Document will have the following strings:
        File extension:      cfx
        File type ID:        MarshallCodeClient.Document
        Main frame caption:  Marshall Code Client
        Doc type name:       MarshallCodeCli
        Filter name:         Marshall Code Client FX Files (*.cfx)
        File new short name: MarshallCodeCli
        File type long name: MarshallCodeClient.Document

MarshallCodeClientView.h, MarshallCodeClientView.cpp - the view of the document
    These files contain your CMarshallCodeClientView class.
    CMarshallCodeClientView objects are used to view CMarshallCodeClientDoc objects.




/////////////////////////////////////////////////////////////////////////////

Help Support:

hlp\MarshallCodeClient.hhp
    This file is a help project file. It contains the data needed to
    compile the help files into a .chm file.

hlp\MarshallCodeClient.hhc
    This file lists the contents of the help project.

hlp\MarshallCodeClient.hhk
    This file contains an index of the help topics.

hlp\afxcore.htm
    This file contains the standard help topics for standard MFC
    commands and screen objects. Add your own help topics to this file.

makehtmlhelp.bat
    This file is used by the build system to compile the help files.

hlp\Images\*.gif
    These are bitmap files required by the standard help file topics for
    Microsoft Foundation Class Library standard commands.


/////////////////////////////////////////////////////////////////////////////

Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named MarshallCodeClient.pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Visual C++ reads and updates this file.

/////////////////////////////////////////////////////////////////////////////

Other notes:

The application wizard uses "TODO:" to indicate parts of the source code you
should add to or customize.

If your application uses MFC in a shared DLL, you will need
to redistribute the MFC DLLs. If your application is in a language
other than the operating system's locale, you will also have to
redistribute the corresponding localized resources MFC100XXX.DLL.
For more information on both of these topics, please see the section on
redistributing Visual C++ applications in MSDN documentation.

/////////////////////////////////////////////////////////////////////////////
