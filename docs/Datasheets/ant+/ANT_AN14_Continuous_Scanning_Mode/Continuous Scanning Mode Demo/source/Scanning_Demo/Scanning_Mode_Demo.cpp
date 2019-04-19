// Scanning_Mode_Demo.cpp : main project file.
// Compile with: /clr

#include "stdafx.h"
#include "Form1.h"
#include <stdio.h>
#include <vcclr.h>
#include <windows.h>

using namespace Scanning_Mode_Demo;

class Thunk
{
// Event Forwarder
public:
	Thunk(Form1^ A_Form1Ptr)
	{
		Form1Ptr = A_Form1Ptr;
	}
	
	static void __stdcall CallbackForwarder(void* param, UCHAR ucEvent_, char* pcBuffer_)
	{
		static_cast<Thunk*>(param)->Form1Ptr->ANTCallback(ucEvent_, pcBuffer_);
	}

private:
	gcroot<Form1^> Form1Ptr; 
};

Thunk *pThunk;

#pragma unmanaged
static void UnmanagedSender(UCHAR ucEvent_, char* pcBuffer_)
{
// Event sender
	pThunk->CallbackForwarder(pThunk, ucEvent_, pcBuffer_);
}


#pragma managed

[STAThreadAttribute]
int main()
{
	System::Threading::Thread::CurrentThread->ApartmentState = System::Threading::ApartmentState::STA;
	Form1 ^pForm1;

	pForm1 = gcnew Form1();
	pThunk = new Thunk(pForm1);

	// Create the main window and run it
	Application::Run(pForm1);
	
	delete pThunk;
	return 0;
}
