/*******************************************************************************
The content of this file includes portions of the AUDIOKINETIC Wwise Technology
released in source code form as part of the SDK installer package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use this file in accordance with the end user license agreement provided 
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

  Version: v2016.2.2  Build: 6022
  Copyright (c) 2006-2017 Audiokinetic Inc.
*******************************************************************************/

// CarSim.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "CarSim.h"
#include "CarSimDlg.h"
#include "CarSimEngine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCarSimApp

BEGIN_MESSAGE_MAP(CCarSimApp, CWinApp)
END_MESSAGE_MAP()


// CCarSimApp construction

CCarSimApp::CCarSimApp()
{
}

CCarSimApp theApp;


// CCarSimApp initialization

BOOL CCarSimApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// Process the command line params
    CCommandLineInfo cmnds;
    ParseCommandLine( cmnds );

	// Init the online help
	EnableHtmlHelp() ;

	TCHAR szPath[ MAX_PATH ];
	::GetModuleFileName( NULL, szPath, MAX_PATH ) ;

	PathRemoveFileSpec( szPath );
	PathAppend( szPath, _T("WwiseCarSimHelp.chm") );

	m_pszHelpFilePath = _tcsdup( szPath ) ;

	// Init the Car Sim itself
	CarSimEngine m_carSimEngine;
	CCarSimDlg dlg( &m_carSimEngine );
	m_pMainWnd = &dlg;

	m_carSimEngine.Init( &dlg );

	if( cmnds.m_nShellCommand == CCommandLineInfo::FileOpen )
	{
		// If the user specify a simulation file we load it now.
		if( ! m_carSimEngine.GetSimulationSerializer().Load( cmnds.m_strFileName ) )
		{
			CString csError;
			csError.Format( IDS_OPEN_ERROR, cmnds.m_strFileName );
			MessageBox( NULL, csError, NULL, MB_ICONERROR | MB_OK );
		}
	}

	INT_PTR nResponse = dlg.DoModal();

	m_carSimEngine.Term();

	return FALSE;
}
