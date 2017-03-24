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

#include "afxwin.h"

class CarSimNumberEdit : public CEdit
{
public:
	CarSimNumberEdit();

	void Init( bool in_bAllowNegativeValue, UINT in_iNumberofDecimal, const double& in_rMinValue, const double& in_rMaxValue );

	double GetValue();
	void SetValue( const double& in_rValue );

protected:

	DECLARE_MESSAGE_MAP()

	afx_msg BOOL OnEnKillfocus();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

private:
	void UpdateText();
	bool ValidateAndUpdate();

	bool IsValidChar( UINT in_nChar );

	void Clamp();

	bool m_bAllowNegativeValue;
	UINT m_uiNumberOfDecimal;
	double m_dblMinValue;
	double m_dblMaxValue;

	double m_dblValue;
};