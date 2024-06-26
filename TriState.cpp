/*
  File:   TriState.cpp

  Contains: Tool to convert groups of three toolbar buttons into single tri-state buttons.

  Written by: Mark Gavin
              Appligent, Inc.
              22 East Baltimore Avenue
              Lansdowne, PA 19050
              ( 610 ) 284-4006

  Copyright:  �1997-2024 by Appligent, Inc.

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

*/

#include "CorCalls.h"
#include "AVCalls.h"
#include "PDCalls.h"
#include "CosCalls.h"
#include "ASCalls.h"

#if MAC_PLATFORM

#include "SafeResources.h"

#include "PIQuickdraw.h"
#include "MacCalls.h"
#include "stdio.h"
#include "string.h"
#include "Types.h"

#endif // MAC_PLATFORM

#if WIN_ENV
#include "resource.h"
extern HINSTANCE gHINSTANCE ;
#endif

#include "TASUtils.h"
#include "TAVUtils.h"

// --------------------------

#define kUseNoneIcon      13101
#define kUseBookmarksIcon 13102
#define kUseThumbsIcon    13103
#define kZoom100Icon      13141
#define kFitPageIcon      13142
#define kFitVisibleIcon   13143

// --------------------------

ASAtom  gProductASAtom ;
ASAtom  gUseNoneASAtom ;
ASAtom  gUseBookmarksASAtom ;
ASAtom  gUseThumbsASAtom ;
ASAtom  gEndPageModeGroupASAtom ;
ASAtom  gZoom100ASAtom ;
ASAtom  gFitPageASAtom ;
ASAtom  gFitVisibleASAtom ;
ASAtom  gEndZoomGroupASAtom ;

AVToolButton  gUseNoneToolButton            = NULL ;
AVToolButton  gUseBookmarksToolButton       = NULL ;
AVToolButton  gUseThumbsToolButton          = NULL ;
AVToolButton  gEndPageModeGroupToolButton   = NULL ;

AVToolButton  gDAUseNoneToolButton ;
AVToolButton  gDAUseBookmarksToolButton ;
AVToolButton  gDAUseThumbsToolButton ;

AVToolButton  gCurrentPageModeToolButton ;

AVToolButton  gZoom100ToolButton            = NULL ;
AVToolButton  gFitPageToolButton            = NULL ;
AVToolButton  gFitVisibleToolButton         = NULL ;
AVToolButton  gEndZoomGroupToolButton       = NULL ;

AVToolButton  gDAZoom100ToolButton ;
AVToolButton  gDAFitPageToolButton ;
AVToolButton  gDAFitVisibleToolButton ;

AVToolButton  gCurrentViewToolButton ;

AVMenuItem    gUseBookmarksMenuItem ;
AVMenuItem    gUseThumbsMenuItem ;

static AVToolRec  gPageModeTSAVToolRec ;

// --------------------------
// Display the About box for the Print Page plug-in

static ACCB1 void ACCB2 DoAboutTriState( void * data )
  {

    TAVUtils::SimpleAlert( 27235 ) ;
    
    return ;

  } // end DoAboutTriState

// --------------------------

static ACCB1 void ACCB2 DoPageModeTriState( ASInt32 inData )
  {
    ASInt32 theButtonType ;
    
    theButtonType = inData ;
    
    DURING
      
      AVToolBar theAVToolBar = AVAppGetToolBar() ;

      switch ( theButtonType )
        {
          case kUseNoneIcon :   // switch to Bookmark view
            AVToolButtonExecute ( gUseBookmarksToolButton ) ;
            AVToolButtonRemove ( gCurrentPageModeToolButton ) ;
            AVToolBarAddButton ( theAVToolBar, gDAUseBookmarksToolButton, true, gEndPageModeGroupToolButton ) ;
            gCurrentPageModeToolButton = gDAUseBookmarksToolButton ;
            break ;
            
          case kUseBookmarksIcon :  // switch to Thumbnail view
            AVToolButtonExecute( gUseThumbsToolButton ) ;
            AVToolButtonRemove ( gCurrentPageModeToolButton ) ;
            AVToolBarAddButton ( theAVToolBar, gDAUseThumbsToolButton, true, gEndPageModeGroupToolButton ) ;
            gCurrentPageModeToolButton = gDAUseThumbsToolButton ;
            AVToolBarUpdateButtonStates ( theAVToolBar ) ;
            break ;
            
          case kUseThumbsIcon :   // switch to None view
            AVToolButtonExecute( gUseNoneToolButton ) ;
            AVToolButtonRemove ( gCurrentPageModeToolButton ) ;
            AVToolBarAddButton ( theAVToolBar, gDAUseNoneToolButton, true, gEndPageModeGroupToolButton ) ;
            gCurrentPageModeToolButton = gDAUseNoneToolButton ;
            break ;
            
        } // end switch
        
    HANDLER
      TASUtils::DisplayErrorAlert( ERRORCODE ) ;
    END_HANDLER

    return ;

  } // end DoPageModeTriState

// --------------------------

static ACCB1 void ACCB2 UpdatePageModeTriState( AVDoc inAVDoc, void *data )
  {
    AVToolBar     theAVToolBar ;
    PDPageMode    thePDPageMode ;
    
    DURING
      
      theAVToolBar = AVAppGetToolBar() ;

      thePDPageMode = AVDocGetViewMode ( inAVDoc ) ;
      
      switch ( thePDPageMode )
        {
          case PDUseNone :    // switch to None view
            AVToolButtonRemove ( gCurrentPageModeToolButton ) ;
            AVToolBarAddButton ( theAVToolBar, gDAUseNoneToolButton, true, gEndPageModeGroupToolButton ) ;
            gCurrentPageModeToolButton = gDAUseNoneToolButton ;
            break ;
            
          case PDUseBookmarks :   // switch to Bookmark view
            AVToolButtonRemove ( gCurrentPageModeToolButton ) ;
            AVToolBarAddButton ( theAVToolBar, gDAUseBookmarksToolButton, true, gEndPageModeGroupToolButton ) ;
            gCurrentPageModeToolButton = gDAUseBookmarksToolButton ;
            break ;
            
          case PDUseThumbs :  // switch to Thumbnail view
            AVToolButtonRemove ( gCurrentPageModeToolButton ) ;
            AVToolBarAddButton ( theAVToolBar, gDAUseThumbsToolButton, true, gEndPageModeGroupToolButton ) ;
            gCurrentPageModeToolButton = gDAUseThumbsToolButton ;
            break ;
            
        } // end switch

    HANDLER
      TASUtils::DisplayErrorAlert( ERRORCODE ) ;
    END_HANDLER

    return ;
    
  } // end UpdatePageModeTriState

// --------------------------

static ACCB1 void ACCB2 DoViewTriState( ASInt32 inData )
  {
    AVToolBar     theAVToolBar ;
    AVRect        theAVRect ;
    ASInt32       theButtonType ;
    
    theButtonType = inData ;
    
    DURING
      
      theAVToolBar = AVAppGetToolBar() ;

      switch ( theButtonType )
        {
          case kZoom100Icon :   // switch to FitPage view
            AVToolButtonExecute ( gFitPageToolButton ) ;
            AVToolButtonRemove ( gCurrentViewToolButton ) ;
            AVToolBarAddButton ( theAVToolBar, gDAFitPageToolButton, true, gEndZoomGroupToolButton ) ;
            gCurrentViewToolButton = gDAFitPageToolButton ;
            break ;
            
          case kFitPageIcon : // switch to FitVisible view
            AVToolButtonExecute( gFitVisibleToolButton ) ;
            AVToolButtonRemove ( gCurrentViewToolButton ) ;
            AVToolBarAddButton ( theAVToolBar, gDAFitVisibleToolButton, true, gEndZoomGroupToolButton ) ;
            gCurrentViewToolButton = gDAFitVisibleToolButton ;
            break ;
            
          case kFitVisibleIcon :    // switch to 100% Zoom view
            AVToolButtonExecute( gZoom100ToolButton ) ;
            AVToolButtonRemove ( gCurrentViewToolButton ) ;
            AVToolBarAddButton ( theAVToolBar, gDAZoom100ToolButton, true, gEndZoomGroupToolButton ) ;
            gCurrentViewToolButton = gDAZoom100ToolButton ;
            break ;
            
        } // end switch
        
      AVDoc theAVDoc = AVAppGetActiveDoc() ;
      ASInt16 theSplitterPosition = AVDocGetSplitterPosition ( theAVDoc ) ;
      if ( theSplitterPosition != 0 )
        {
          AVWindow theAVWindow = AVDocGetAVWindow ( theAVDoc ) ;
          AVWindowGetInterior ( theAVWindow, &theAVRect ) ;
          AVWindowInvalidateRect ( theAVWindow, &theAVRect ) ;
        }
        
    HANDLER
      TASUtils::DisplayErrorAlert( ERRORCODE ) ;
    END_HANDLER

    return ;

  } // end DoViewTriState

// --------------------------

static ACCB1 void ACCB2 UpdateViewTriState( AVDoc inAVDoc, void *data )
  {
    AVToolBar     theAVToolBar ;
    AVPageView    theAVPageView ;
    AVZoomType    theAVZoomType ;
    
    DURING
      
      theAVToolBar = AVAppGetToolBar() ;

      theAVPageView = AVDocGetPageView ( inAVDoc ) ;
      theAVZoomType = AVPageViewGetZoomType ( theAVPageView ) ;
      
      switch ( theAVZoomType )
        {
          case AVZoomNoVary :   // switch to FitPage view
            AVToolButtonRemove ( gCurrentViewToolButton ) ;
            AVToolBarAddButton ( theAVToolBar, gDAZoom100ToolButton, true, gEndZoomGroupToolButton ) ;
            gCurrentViewToolButton = gDAZoom100ToolButton ;
            break ;
            
          case AVZoomFitPage :    // switch to FitVisible view
            AVToolButtonRemove ( gCurrentViewToolButton ) ;
            AVToolBarAddButton ( theAVToolBar, gDAFitPageToolButton, true, gEndZoomGroupToolButton ) ;
            gCurrentViewToolButton = gDAFitPageToolButton ;
            break ;
            
          case AVZoomFitVisibleWidth :  // switch to 100% Zoom view
            AVToolButtonRemove ( gCurrentViewToolButton ) ;
            AVToolBarAddButton ( theAVToolBar, gDAFitVisibleToolButton, true, gEndZoomGroupToolButton ) ;
            gCurrentViewToolButton = gDAFitVisibleToolButton ;
            break ;
            
        } // end switch

    HANDLER
      TASUtils::DisplayErrorAlert( ERRORCODE ) ;
    END_HANDLER

    return ;
    
  } // end UpdateViewTriState

// --------------------------
// This is used in addition to AVAppFrontDocDidChange because the notification is not sent in the 
// case of the document first being opened.

static ACCB1 void ACCB2 DoAVDocDidOpen( AVDoc inAVDoc, ASInt32 outError, void *data )
  {
    if ( inAVDoc == NULL )    // if there is no frontmost document (e.g.,the last document was just closed)
      return ;
    
    UpdatePageModeTriState( inAVDoc, data ) ;

    UpdateViewTriState( inAVDoc, data ) ;

    return ;
    
  } // end DoAVDocDidOpen

// --------------------------

static ACCB1 void ACCB2 DoAVAppFrontDocDidChange( AVDoc inAVDoc, void *data )
  {
    if ( inAVDoc == NULL )    // if there is no frontmost document (e.g.,the last document was just closed)
      return ;
    
    UpdatePageModeTriState( inAVDoc, data ) ;

    UpdateViewTriState( inAVDoc, data ) ;

    return ;
    
  } // end DoAVAppFrontDocDidChange

// --------------------------

static ACCB1 void ACCB2 DoAVPageViewDidChange( AVPageView inAVPageView, ASInt16 inHowChanged, void *data )
  {
    if ( inAVPageView == NULL )   // if there is no frontmost document (e.g.,the last document was just closed)
      return ;
      
    //if ( ( inHowChanged == PAGEVIEW_UPDATE_PAGESIZE ) || ( inHowChanged == PAGEVIEW_UPDATE_ZOOM ) )
      {
        AVDoc theAVDoc = AVPageViewGetAVDoc ( inAVPageView ) ;
        
        UpdatePageModeTriState( theAVDoc, data ) ;

        UpdateViewTriState( theAVDoc, data ) ;
      }

    return ;
    
  } // end DoAVPageViewDidChange

// -------------------------
#pragma mark -- init
// -------------------------
// Initialize all of the ASAtoms once

static ACCB1 boolean ACCB2 InitASAtoms( void )
  {
    
  DURING
  
    gProductASAtom          = ASAtomFromString( "Product" ) ;           // used to test Exchange, LE or Reader
    gUseNoneASAtom          = ASAtomFromString( "UseNone" ) ;           // Displays only the document, but neither bookmarks nor thumbnail images.
    gUseBookmarksASAtom     = ASAtomFromString( "UseBookmarks" ) ;      // Displays the document and bookmarks.
    gUseThumbsASAtom        = ASAtomFromString( "UseThumbs" ) ;         // Displays the document and thumbnail images.
    gEndPageModeGroupASAtom = ASAtomFromString( "endPageModeGroup" ) ;  // Separator
    gZoom100ASAtom          = ASAtomFromString( "Zoom100" ) ;           // Sets the zoom factor to 100% (i.e., actual size).
    gFitPageASAtom          = ASAtomFromString( "FitPage" ) ;           // Sets the zoom factor to fit the entire page into the window.
    gFitVisibleASAtom       = ASAtomFromString( "FitVisible" ) ;        // Sets the zoom factor to fit the portion of the page on which drawing appears into the window.
    gEndZoomGroupASAtom     = ASAtomFromString( "endZoomGroup" ) ;      // Separator

  HANDLER
    TASUtils::DisplayErrorAlert( ERRORCODE ) ;
    return false ;
  END_HANDLER
  
    return true ;
    
  } // end InitASAtoms

// --------------------------

static boolean InitPlugInToolBar( void )
  {
    ASInt32       theButtonType ;
    ASBool        theProblem      = false ;

    DURING
      
      AVToolBar theAVToolBar = AVAppGetToolBar() ;
      
      gUseNoneToolButton          = AVToolBarGetButtonByName ( theAVToolBar, gUseNoneASAtom ) ;
      if ( ! gUseNoneToolButton )
        theProblem = true ;
      gUseBookmarksToolButton     = AVToolBarGetButtonByName ( theAVToolBar, gUseBookmarksASAtom ) ;
      if ( ! gUseBookmarksToolButton )
        theProblem = true ;
      gUseThumbsToolButton        = AVToolBarGetButtonByName ( theAVToolBar, gUseThumbsASAtom ) ;
      if ( ! gUseThumbsToolButton )
        theProblem = true ;
      gEndPageModeGroupToolButton = AVToolBarGetButtonByName ( theAVToolBar, gEndPageModeGroupASAtom ) ;
      if ( ! gEndPageModeGroupToolButton )
        theProblem = true ;
      
      gZoom100ToolButton          = AVToolBarGetButtonByName ( theAVToolBar, gZoom100ASAtom ) ;
      if ( ! gZoom100ToolButton )
        theProblem = true ;
      gFitPageToolButton          = AVToolBarGetButtonByName ( theAVToolBar, gFitPageASAtom ) ;
      if ( ! gFitPageToolButton )
        theProblem = true ;
      gFitVisibleToolButton       = AVToolBarGetButtonByName ( theAVToolBar, gFitVisibleASAtom ) ;
      if ( ! gFitVisibleToolButton )
        theProblem = true ;
      gEndZoomGroupToolButton     = AVToolBarGetButtonByName ( theAVToolBar, gEndZoomGroupASAtom ) ;
      if ( ! gEndZoomGroupToolButton )
        theProblem = true ;
      
      if ( theProblem == true )
        {
          TASUtils::DisplayErrorAlert( "TriState failed to load because it could not find a required toolbar button." ) ;
          return false ;
        }
        
      gDAUseNoneToolButton        = AVToolButtonNew( ASAtomFromString( "DGAP:UseNone" ),      TAVUtils::GetButtonIcon( kUseNoneIcon ),      false, false ) ;
      gDAUseBookmarksToolButton   = AVToolButtonNew( ASAtomFromString( "DGAP:UseBookmarks" ), TAVUtils::GetButtonIcon( kUseBookmarksIcon ), false, false ) ;
      gDAUseThumbsToolButton      = AVToolButtonNew( ASAtomFromString( "DGAP:UseThumbs" ),    TAVUtils::GetButtonIcon( kUseThumbsIcon ),    false, false ) ;
                                                  
      gDAZoom100ToolButton        = AVToolButtonNew( ASAtomFromString( "DGAP:Zoom100" ),      TAVUtils::GetButtonIcon( kZoom100Icon ),      false, false ) ;
      gDAFitPageToolButton        = AVToolButtonNew( ASAtomFromString( "DGAP:FitPage" ),      TAVUtils::GetButtonIcon( kFitPageIcon ),      false, false ) ;
      gDAFitVisibleToolButton     = AVToolButtonNew( ASAtomFromString( "DGAP:FitVisible" ),   TAVUtils::GetButtonIcon( kFitVisibleIcon ),   false, false ) ;
                                                  
      // add the new useNone button after the initial 3 state buttons
      AVToolBarAddButton( theAVToolBar, gDAUseNoneToolButton, true, gEndPageModeGroupToolButton ) ;
      gCurrentPageModeToolButton = gDAUseNoneToolButton ;
      
      AVToolBarAddButton( theAVToolBar, gDAZoom100ToolButton, true, gEndZoomGroupToolButton ) ;
      gCurrentViewToolButton = gDAZoom100ToolButton ;
      
      // now remove the initial 3 state buttons
      AVToolButtonRemove ( gUseNoneToolButton ) ;
      AVToolButtonRemove ( gUseBookmarksToolButton ) ;
      AVToolButtonRemove ( gUseThumbsToolButton ) ;
      
      AVToolButtonRemove ( gZoom100ToolButton ) ;
      AVToolButtonRemove ( gFitPageToolButton ) ;
      AVToolButtonRemove ( gFitVisibleToolButton ) ;
      
      theButtonType = kUseNoneIcon ;
      AVToolButtonSetExecuteProc( gDAUseNoneToolButton, ASCallbackCreateProto( AVExecuteProc, &DoPageModeTriState ), ( void * )theButtonType ) ;
      AVToolButtonSetComputeEnabledProc( gDAUseNoneToolButton, ASCallbackCreateProto( AVComputeEnabledProc, TAVUtils::ComputeEnabled ), NULL ) ;
    
      theButtonType = kUseBookmarksIcon ;
      AVToolButtonSetExecuteProc( gDAUseBookmarksToolButton, ASCallbackCreateProto( AVExecuteProc, &DoPageModeTriState ), ( void * )theButtonType ) ;
      AVToolButtonSetComputeEnabledProc( gDAUseBookmarksToolButton, ASCallbackCreateProto( AVComputeEnabledProc, TAVUtils::ComputeEnabled ), NULL ) ;
    
      theButtonType = kUseThumbsIcon ;
      AVToolButtonSetExecuteProc( gDAUseThumbsToolButton, ASCallbackCreateProto( AVExecuteProc, &DoPageModeTriState ), ( void * )theButtonType ) ;
      AVToolButtonSetComputeEnabledProc( gDAUseThumbsToolButton, ASCallbackCreateProto( AVComputeEnabledProc, TAVUtils::ComputeEnabled ), NULL ) ;
    
      theButtonType = kZoom100Icon ;
      AVToolButtonSetExecuteProc( gDAZoom100ToolButton, ASCallbackCreateProto( AVExecuteProc, &DoViewTriState ), ( void * )theButtonType ) ;
      AVToolButtonSetComputeEnabledProc( gDAZoom100ToolButton, ASCallbackCreateProto( AVComputeEnabledProc, TAVUtils::ComputeEnabled ), NULL ) ;
    
      theButtonType = kFitPageIcon ;
      AVToolButtonSetExecuteProc( gDAFitPageToolButton, ASCallbackCreateProto( AVExecuteProc, &DoViewTriState ), ( void * )theButtonType ) ;
      AVToolButtonSetComputeEnabledProc( gDAFitPageToolButton, ASCallbackCreateProto( AVComputeEnabledProc, TAVUtils::ComputeEnabled ), NULL ) ;
    
      theButtonType = kFitVisibleIcon ;
      AVToolButtonSetExecuteProc( gDAFitVisibleToolButton, ASCallbackCreateProto( AVExecuteProc, &DoViewTriState ), ( void * )theButtonType ) ;
      AVToolButtonSetComputeEnabledProc( gDAFitVisibleToolButton, ASCallbackCreateProto( AVComputeEnabledProc, TAVUtils::ComputeEnabled ), NULL ) ;
    
    HANDLER
      TASUtils::DisplayErrorAlert( ERRORCODE ) ;
    END_HANDLER

    return true ;
        
  } // end InitPlugInToolBar

// -------------------------

static ACCB1 boolean ACCB2 InitPlugInMenus( void )
  {
    AVMenubar     theAVMenubar ;
    AVMenuItem    theAVMenuItem ;
    ASInt32       theButtonType ;
    
    DURING
    
      theAVMenubar  = AVAppGetMenubar() ;   // get the main menu
      if ( ! theAVMenubar ) 
        E_RETURN( false ) ;
        
      TAVUtils::AppendToAboutMenu( "TriState...", "DGAP:DoAboutTriState", &DoAboutTriState ) ;
      
      theAVMenuItem = AVMenubarAcquireMenuItemByName ( theAVMenubar, "ShowBookmarks" ) ;
  
      theButtonType = kUseNoneIcon ;
      AVMenuItemSetExecuteProc( theAVMenuItem, ASCallbackCreateProto( AVExecuteProc, &DoPageModeTriState ), ( void * )theButtonType ) ;
    
      theAVMenuItem = AVMenubarAcquireMenuItemByName ( theAVMenubar, "ShowThumbs" ) ;
  
      theButtonType = kUseBookmarksIcon ;
      AVMenuItemSetExecuteProc( theAVMenuItem, ASCallbackCreateProto( AVExecuteProc, &DoPageModeTriState ), ( void * )theButtonType ) ;
    
    HANDLER
      TASUtils::DisplayErrorAlert( ERRORCODE ) ;
    END_HANDLER
    
    return true ;
    
  } // end InitPlugInMenus

// -------------------------

static ACCB1 boolean ACCB2 InitPlugIn( void )
  {
    boolean   theResult ;
    
    theResult = InitASAtoms() ;
    if ( theResult == false )
      return theResult ;

    theResult = InitPlugInMenus() ;
    if ( theResult == false )
      return theResult ;
      
    theResult = InitPlugInToolBar() ;
    
    return theResult ;
    
  } // end InitPlugIn

// -------------------------

static ACCB1 boolean ACCB2 UnloadPlugIn( void )
  {
    return true ;
    
  } // end UnloadPlugIn

// -------------------------
//  This is the pre-initialization routine for the plug-in.
//  The pre-initialization routine is installed in PIHandshake().  Use the
//  pre-initialization routine to register for any event notifications.
//  This is important in this particular plug-in since the main
//  initialization function itself causes events to occur.  Since this
//  plug-in counts various events, we don't want to miss out on any of the action.

static ACCB1 boolean ACCB2 PreInitPlugIn( void )
  {

    AVAppRegisterNotification( AVDocDidOpenNSEL, gExtensionID, ASCallbackCreateNotification( AVDocDidOpen, ( void * )DoAVDocDidOpen ), NULL ) ;

    AVAppRegisterNotification( AVAppFrontDocDidChangeNSEL, gExtensionID, ASCallbackCreateNotification( AVAppFrontDocDidChange, ( void * )DoAVAppFrontDocDidChange ), NULL ) ;

    AVAppRegisterNotification( AVPageViewDidChangeNSEL, gExtensionID, ASCallbackCreateNotification( AVPageViewDidChange, ( void * )DoAVPageViewDidChange ), NULL ) ;

    return true ;
    
  } // end PreInitPlugIn

// -------------------------

ACCB1 boolean ACCB2 PIHandshake( Uns32 handshakeVersion, void *handshakeData )
  {
    if ( handshakeVersion == HANDSHAKE_V0200 )
      {
        PIHandshakeData_V0200 *hsData = ( PIHandshakeData_V0200 * )handshakeData ;
    
        hsData->extensionName = ASAtomFromString( "DGAP:TriStatePlugIn" ) ; 
    
        hsData->exportHFTsCallback = NULL ;
        
        hsData->importReplaceAndRegisterCallback = ASCallbackCreateProto( PIImportReplaceAndRegisterProcType, ( void * )PreInitPlugIn ) ;
        
        hsData->initCallback = ASCallbackCreateProto( PIInitProcType, ( void * )InitPlugIn ) ;
      
        hsData->unloadCallback = ASCallbackCreateProto( PIUnloadProcType, ( void * )UnloadPlugIn ) ;
    
        return true ;
      }
    
    return false ;
    
  } // end PIHandshake

// -------------------------

