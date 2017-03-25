/*******************************************************************************
The content of this file includes portions of the AUDIOKINETIC Wwise Technology
released in source code form as part of the SDK installer package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use this file in accordance with the end user license agreement provided
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

Apache License Usage

Alternatively, this file may be used under the Apache License, Version 2.0 (the
"Apache License"); you may not use this file except in compliance with the
Apache License. You may obtain a copy of the Apache License at
http://www.apache.org/licenses/LICENSE-2.0.

Unless required by applicable law or agreed to in writing, software distributed
under the Apache License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES
OR CONDITIONS OF ANY KIND, either express or implied. See the Apache License for
the specific language governing permissions and limitations under the License.

Version: v2016.2.2  Build: 6021
Copyright (c) 2006-2017 Audiokinetic Inc.
*******************************************************************************/

/*
* Emulates a subset of the Win32 threading API as a layer on top of WinRT threadpools.
*
* Supported features:
*
*    - CreateThread (returns a standard Win32 handle which can be waited on, then closed)
*    - CREATE_SUSPENDED and ResumeThread
*    - Partial support for SetThreadPriority (see below)
*    - Sleep
*    - Thread local storage (TlsAlloc, TlsFree, TlsGetValue, TlsSetValue)
*
* Differences from Win32:
*
*    - No ExitThread or TerminateThread (just return from the thread function to exit)
*    - No SuspendThread, so ResumeThread is only useful in combination with CREATE_SUSPENDED
*    - SetThreadPriority is only available while a thread is in CREATE_SUSPENDED state
*    - SetThreadPriority only supports three priority levels (negative, zero, or positive)
*    - TLS requires use of this CreateThread (leaks memory if called from other threadpool tasks)
*    - No thread identifier APIs (GetThreadId, GetCurrentThreadId, OpenThread)
*    - No affinity APIs
*    - No GetExitCodeThread
*    - Failure cases return error codes but do not always call SetLastError
*/
#pragma once
#ifdef WWISE_UWP
#include <windows.h>


namespace AK
{
	namespace ThreadEmulation
	{
		HANDLE WINAPI CreateThread(__in LPTHREAD_START_ROUTINE lpStartAddress, __in_opt LPVOID lpParameter, __in DWORD dwCreationFlags);
		DWORD WINAPI ResumeThread(__in HANDLE hThread);
		BOOL WINAPI SetThreadPriority(__in HANDLE hThread, __in int nPriority);

		VOID WINAPI Sleep(__in DWORD dwMilliseconds);
		VOID WINAPI SleepEx(__in DWORD dwMilliseconds, BOOL in_bAlertable);
	}
}
#endif