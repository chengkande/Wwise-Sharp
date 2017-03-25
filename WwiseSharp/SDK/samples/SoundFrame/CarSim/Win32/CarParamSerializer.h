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

class CarSimEngine;

class CarParamSerializer
{
public:
	CarParamSerializer();
	~CarParamSerializer();

	void Init( CarSimEngine* in_pEngine );

	void ClearFileName();

	bool Save( const CString& in_rFileName );
	bool Load( const CString& in_rFileName );

	const CString& GetCurrentFilePath() const;

	// Return the file name with no extension
	CString GetCurrentFileNameOnly() const;

	const CString& GetFileExtension() const;

private:

	void EnsureFileExtension( CString& io_rFileName );

	CString m_csLastFile;

	CarSimEngine* m_pCarSimEngine;
};