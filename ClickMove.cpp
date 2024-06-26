/*
  File:   ClickMove.cpp

  Contains: If the document is in full screen mode then a single click will advance
            the page and a double click will step back through pages.

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
// Display the About box for the ClickMove plug-in

static ACCB1 void ACCB2 DoAboutClickMove( void * data )
  {

    TAVUtils::SimpleAlert( 27336 ) ;
    
    return ;

  } // end DoAboutClickMove

// --------------------------

static ACCB1 ASBool ACCB2 DoAVPageViewClickProc ( AVPageView inAVPageView, ASInt16 x, ASInt16 y, 
                                                  ASInt16 inFlags, ASInt16 inClickNumber, void * data )
  {
    AVDoc         theAVDoc ;
    PDDoc         thePDDoc ;
    ASInt32       thePageNumber ;
    ASInt32       theTotalPages ;
    ASBool        bResult ;
    
    if ( inAVPageView == NULL )
      return false ;
    
    DURING
    
      bResult = AVAppDoingFullScreen() ;
      if ( bResult == false )
        E_RETURN( false ) ;
        
      thePageNumber = AVPageViewGetPageNum ( inAVPageView ) ;
      
      theAVDoc = AVPageViewGetAVDoc ( inAVPageView ) ;
      thePDDoc = AVDocGetPDDoc( theAVDoc ) ;
      
      theTotalPages = PDDocGetNumPages ( thePDDoc ) ;
      
      switch ( inClickNumber )
        {
          case 1 :
            if ( inFlags & AV_SHIFT )
              AVPageViewGoForward ( inAVPageView ) ;
            else
              {
                if ( thePageNumber + 1 == theTotalPages )
                  thePageNumber = 0 ;
                else
                  thePageNumber += 1 ;
                AVPageViewGoTo ( inAVPageView, thePageNumber ) ;
              }
            E_RETURN( true ) ;
            break ;
      
          case 2 :
            if ( inFlags & AV_SHIFT )
              AVPageViewGoBack ( inAVPageView ) ;
            else
              {
                if ( thePageNumber == 0 )
                  thePageNumber = theTotalPages - 1 ;
                else
                  thePageNumber -= 1 ;
                AVPageViewGoTo ( inAVPageView, thePageNumber ) ;
              }
            E_RETURN( true ) ;
            break ;
            
          case 3 :
            if ( inFlags & AV_SHIFT )
              AVPageViewGoTo ( inAVPageView, 0 ) ;
            else
              AVPageViewGoTo ( inAVPageView, theTotalPages - 1 ) ;
            E_RETURN( true ) ;
            break ;

        } // end switch

    HANDLER
      TASUtils::DisplayErrorAlert( ERRORCODE ) ;
    END_HANDLER
  
    return false ;
    
  } // end DoAVPageViewClickProc

// -------------------------
#pragma mark -- init
// -------------------------

static ACCB1 boolean ACCB2 InitPlugInMenus( void )
  {
    AVMenubar     theAVMenubar ;
    
    DURING
    
      theAVMenubar  = AVAppGetMenubar() ;   // get the main menu
      if ( ! theAVMenubar ) 
        E_RETURN( false ) ;
        
      TAVUtils::AppendToAboutMenu( "ClickMove...", "DGAP:DoAboutClickMove", &DoAboutClickMove ) ;
      
    HANDLER
      TASUtils::DisplayErrorAlert( ERRORCODE ) ;
    END_HANDLER
    
    return true ;
    
  } // end InitPlugInMenus

// -------------------------

static ACCB1 boolean ACCB2 InitPlugIn( void )
  {
    boolean   theResult ;
    
    theResult = InitPlugInMenus() ;
    if ( theResult == false )
      return theResult ;
      
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

    AVAppRegisterForPageViewClicks ( ASCallbackCreateProto( AVPageViewClickProc, ( void * )DoAVPageViewClickProc ), NULL ) ;

    return true ;
    
  } // end PreInitPlugIn

// -------------------------

ACCB1 boolean ACCB2 PIHandshake( Uns32 handshakeVersion, void *handshakeData )
  {
    if ( handshakeVersion == HANDSHAKE_V0200 )
      {
        PIHandshakeData_V0200 *hsData = ( PIHandshakeData_V0200 * )handshakeData ;
    
        hsData->extensionName = ASAtomFromString( "DGAP:ClickMove" ) ;  
    
        hsData->exportHFTsCallback = NULL ;
        
        hsData->importReplaceAndRegisterCallback = ASCallbackCreateProto( PIImportReplaceAndRegisterProcType, ( void * )PreInitPlugIn ) ;
        
        hsData->initCallback = ASCallbackCreateProto( PIInitProcType, ( void * )InitPlugIn ) ;
      
        hsData->unloadCallback = ASCallbackCreateProto( PIUnloadProcType, ( void * )UnloadPlugIn ) ;
    
        return true ;
      }
    
    return false ;
    
  } // end PIHandshake

// -------------------------

