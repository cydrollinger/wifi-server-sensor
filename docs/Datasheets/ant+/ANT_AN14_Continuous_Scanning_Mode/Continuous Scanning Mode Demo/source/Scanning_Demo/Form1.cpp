/*
 * Dynastream Innovations Inc.
 * Cochrane, AB, CANADA
 *
 * Copyright © 1998-2007 Dynastream Innovations Inc.
 * All rights reserved. This software may not be reproduced by
 * any means without express written approval of Dynastream
 * Innovations Inc.
 */ 

#include "stdafx.h"
#include <vcclr.h>
#include <windows.h>
#include "Form1.h"

using namespace Scanning_Channel_Demo;


class Thunk
// event forwarder
{
public:   
   Thunk(Form1^ A_Form1Ptr) 
   {
      Form1Ptr = A_Form1Ptr;   
   }

   static void __stdcall CallbackForwarder(void* param, AntResponseEnum eEvent_, char* pcBuffer_)
   {
      static_cast<Thunk*>(param)->Form1Ptr->ANTCallback(eEvent_, pcBuffer_ );
   }   
      
      

private:
   gcroot<Form1^> Form1Ptr;
};

Thunk *pThunk;

#pragma unmanaged
static void UnmanagedSender(AntResponseEnum eEvent_, char* pcBuffer_)
// event sender
{   
   pThunk->CallbackForwarder(pThunk, eEvent_, pcBuffer_);  
}


////////////////////////////////////////////


#pragma managed

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{   
   System::Threading::Thread::CurrentThread->ApartmentState = System::Threading::ApartmentState::STA;
   Form1 ^pForm1;   
   pForm1 = gcnew Form1();   
   pThunk = new Thunk(pForm1);

   Application::Run(pForm1);

   delete pThunk;
   return 0;
}
