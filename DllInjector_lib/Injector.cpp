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
#include <iostream>
#include "Injector.h"

// NInjectLib from Jan Newger <http://newgre.net/>
#include <InjectLib.h>
#include <IATModifier.h>

using namespace std;

Injector::Injector()
{}

Injector::Injector(const string& executable, const string& injectedDll, bool pause)
{
	STARTUPINFO startupInfo = {0};
	startupInfo.cb = sizeof(STARTUPINFO);
	PROCESS_INFORMATION processInfo;

	cout << "creating process " << executable << endl;
	if (!CreateProcess(executable.c_str(), NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &startupInfo, &processInfo))
		throw exception("could not create process");

	process_ = Process(processInfo.dwProcessId);

	cout << "injecting DLL " << injectedDll << endl;

	Process process(processInfo.dwProcessId);
	IATModifier iatModifier(process);
	iatModifier.setImageBase(process.getImageBase(processInfo.hThread));
	iatModifier.writeIAT(injectedDll);

	if (!pause)
		ResumeThread(processInfo.hThread);

}


Injector::Injector(const DWORD pid, const string& injectedDll, bool pause)
{
	process_ = Process(pid);
	InjectLibrary injectLib(injectedDll, process_);
	injectLib.injectLib();
	
	// TODO: suspend all threads
	if (pause)
		cerr << "not implemented yet" << endl;

}

