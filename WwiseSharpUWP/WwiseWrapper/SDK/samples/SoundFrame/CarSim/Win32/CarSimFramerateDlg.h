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

#pragma once

#include "resource.h"

class CarSimFramerateDlg : public CDialog
{
public:
	CarSimFramerateDlg( CWnd* pParent = NULL );

	void SetFramerate( const UINT in_uiFramerate );
	UINT GetFramerate() const;

	// Dialog Data
	enum { IDD = IDD_FRAMERATE_DIALOG };

protected:

	virtual BOOL OnInitDialog();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()

private:

	UINT m_uiFramerate;
};