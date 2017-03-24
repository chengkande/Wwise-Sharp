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

#include <AK/IBytes.h>

#include <math.h>
#include <float.h>

#define ReleaseClear(ptr) (((ptr) ? (ptr)->Release() : NULL), (ptr) = NULL)
#define SafeAddRef(ptr) ((ptr) ? (ptr)->AddRef() : NULL)

inline bool DoubleEqual( const double& in_rdblFirst, const double& in_rdblSecond, const double& in_rdblEpsilon = DBL_EPSILON )
{
	return fabs( in_rdblFirst - in_rdblSecond ) < in_rdblEpsilon;
}

namespace CarSimHelper
{
	CString GetFileNameOnly( const CString& in_rFileFullPath );
	void EnsureFileExtension( CString& io_rFile, const CString& in_rFileExtension );
}

// -----------------------------------------------------------------------------

class ReadBytesInFile
	: public AK::IReadBytes
{
public:
	ReadBytesInFile();
	virtual ~ReadBytesInFile();

	bool Open( const CString& in_rFileName );
	void Close();

	// AK::IReadBytes
	virtual bool ReadBytes( void * in_pData, long in_cBytes, long & out_cRead );

private:
	FILE * m_pFile;
};

// -----------------------------------------------------------------------------

class WriteBytesInFile
	: public AK::IWriteBytes
{
public:
	WriteBytesInFile();
	virtual ~WriteBytesInFile();

	bool Open( const CString& in_rFileName );
	void Close();

	// AK::IWriteBytes
	virtual bool WriteBytes( const void * in_pData, long in_cBytes, long & out_cWritten );

private:
	FILE * m_pFile;
};
