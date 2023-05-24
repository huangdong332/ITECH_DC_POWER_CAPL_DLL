/*-----------------------------------------------------------------------------
Module: VIA (observed)
Interfaces: This is a public interface header.
-------------------------------------------------------------------------------
External interface for CANoe\CANalyzer CAPL DLLs (CDLL), that makes some of the
VIA APIs available also for CAPL DLLs.
-------------------------------------------------------------------------------
Copyright (c) Vector Informatik GmbH. All rights reserved.
-----------------------------------------------------------------------------*/

#ifndef VIA_CDLL_H
#define VIA_CDLL_H

/*-----------------------------------------------------------------------------
Version of the interfaces, which are declared in this header

2003-08-27   1.0   As     Creation

sometime     2.0          By definition version 2.0 will be the first release 
in the                    of VIA_CDLL interface, that is not longer compatible  
future                    with version 1.x 
-----------------------------------------------------------------------------*/

const int VIACDLLMajorVersion = 1;
const int VIACDLLMinorVersion = 0;

// ============================================================================
// Dependencies
// ============================================================================

#ifndef VIA_H
  #include "VIA.h"
#endif

// ============================================================================
// An object of class VIACapl represents a CAPL programs. By use of this 
// object, you can access the functions, that are implemented in a CAPL program.
//
// While the measurement is running, every loaded CAPL program is identified by
// a unique handle (VIACaplHandle). The CAPL programmer gets this handle by a
// call of the capl function 'registerCAPLDll', whereas the DLL programmer gets
// from the VIACapl when the C-Function 'VIARegisterCDLL' is called.
// ============================================================================

// Unique handle of a CAPL program
typedef uint32 VIACaplHandle;

class VIACapl
{
public:
   // Gets the version of this interface object (VIACDLLMajorVersion and 
   // VIACDLLMinorVersion).
   VIASTDDECL GetVersion (int32* major, int32* minor) = 0;

   // Gets the CAPL handle
   VIASTDDECL GetCaplHandle (uint32* handle) = 0;

   // Gets a CAPL function handle. The handle stays valid until end of
   // measurement or a call of ReleaseCaplFunction.
   // Parameter caplfct:      The created handle
   // Parameter functionName: Name of the function
   VIASTDDECL GetCaplFunction(VIACaplFunction** caplfct,
                              const char* functionName) = 0;

   // Releases the CAPL function handle (see above)
   // Call this method at or before the end of the measurement for every CAPL 
   // function handle, that you have asked for.
   VIASTDDECL ReleaseCaplFunction(VIACaplFunction* caplfct) = 0;
};


// ==========================================================================
// VIARegisterCDLL, a setup function that must be implemented by every CAPL 
// DLL, that whats to use the services of 'VIA_CDLL.h'
//
// The following C-function is for initializing of the DLL. It is called 
// when a CAPL Program executes the function 'registerCAPLDll'.
// ==========================================================================
VIACLIENT(void) VIARegisterCDLL (VIACapl* service);
#endif

