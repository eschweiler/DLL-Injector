/* ============================================================================
* Copyright (c) 2012, Sebastian Eschweiler <advanced(dot)malware<dot>analyst[at]gmail.com>
* All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the <organization> nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* =============================================================================
*/

#include <gtest/gtest.h>
#include <Injector.h>
#include <tchar.h>
#include <algorithm>
#include <boost/bind.hpp>

#ifdef EXE 
#undef EXE
#define EXE _T("testapp.exe")
#endif

#ifdef DLL
#undef DLL
#define DLL _T("dll.dll")
#endif


class InjectorTest : public ::testing::Test
{

public:
	InjectorTest()
	{
	}

	virtual ~InjectorTest()
	{}

	virtual void SetUp()
	{}

	virtual void TearDown()
	{}

};


TEST_F(InjectorTest, startNew)
{
	Injector injector(EXE, DLL, false);

	Sleep(100);
	HANDLE mutex = OpenMutex(SYNCHRONIZE, FALSE, _T("Injection_Mutex"));
	ASSERT_NE(mutex, (HANDLE) NULL);
	
	vector<MODULEENTRY32> modules = injector.process_.getModules();

	vector<MODULEENTRY32>::iterator dllFoundInLoadedModules = find_if(modules.begin(), modules.end(), boost::bind(&MODULEENTRY32::szModule, _1) == string(DLL));

	ASSERT_NE(dllFoundInLoadedModules, modules.end());
	
	HANDLE finishTestApp = CreateMutex(NULL, TRUE, _T("TestappCanFinish"));
	Sleep(100);

	CloseHandle(finishTestApp);
}


TEST_F(InjectorTest, injectToRunning)
{
	STARTUPINFO startupInfo = {0};
	startupInfo.cb = sizeof(STARTUPINFO);
	PROCESS_INFORMATION processInfo;

	ASSERT_EQ(TRUE, CreateProcess(EXE, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInfo));
	Process process(processInfo.dwProcessId);

	Sleep(100);

	vector<MODULEENTRY32> modules = process.getModules();

	vector<MODULEENTRY32>::iterator dllFoundInLoadedModules = find_if(modules.begin(), modules.end(), boost::bind(&MODULEENTRY32::szModule, _1) == string(DLL));
	ASSERT_EQ(dllFoundInLoadedModules, modules.end());

	Injector injector(processInfo.dwProcessId, DLL, false);

	Sleep(100);
	HANDLE mutex = OpenMutex(SYNCHRONIZE, FALSE, _T("Injection_Mutex"));
	ASSERT_NE(mutex, (HANDLE) NULL);

	modules = process.getModules();

	dllFoundInLoadedModules = find_if(modules.begin(), modules.end(), boost::bind(&MODULEENTRY32::szModule, _1) == string(DLL));
	ASSERT_NE(dllFoundInLoadedModules, modules.end());

	CreateMutex(NULL, TRUE, _T("TestappCanFinish"));

}
