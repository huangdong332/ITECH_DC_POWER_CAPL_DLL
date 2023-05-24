/*--------------------------------------------------------------------
       CDLL.H     "CAPL DLL DEFINITION"
                   HEADER for compiling CAPL DLLs

                   THIS HEADER MUST NOT BE MODIFIED BY THE USER
  --------------------------------------------------------------------
       Copyright (C) 1994-2019 Vector Informatik GmbH, Stuttgart

       Function:      Header for Dynamic CAPL Functions
  --------------------------------------------------------------------*/
#pragma once


#if defined(_WIN32)
  #if !defined(STRICT)
    #define STRICT
  #endif
  #include <windows.h>
  #if defined(_MSC_VER) && !defined(_Windows)
    #define _Windows
  #endif
  #if defined(_MSC_VER) && !defined(__WIN32__)
    #define __WIN32__
  #endif
#endif


// change structure alignment to 1 byte
#if defined(_MSC_VER) || defined(__clang__) || defined(__MINGW32__) || defined(__GNUC__)
  #pragma pack(push,1)
#endif


#define CDLL_VERSION 2
#define CDLL_VERSION_NAME "CDLL_VERSION"
#define CDLL_EXPORT "CDLL_EXP"
#define CDLL_IMPORT "CDLL_IMP"


#if (defined(_MSC_VER) || defined(__MINGW32__)) && defined(_WIN64)
  #define CAPLPASCAL  __stdcall
  #define CAPLCDECL
  #define CAPLEXPORT  __declspec(dllexport)
#elif (defined(_MSC_VER) || defined(__MINGW32__)) && (defined(_WIN32) && !defined(_WIN64))
  #define CAPLPASCAL  __stdcall
  #define CAPLCDECL  __cdecl
  #define CAPLEXPORT  __declspec(dllexport)
#elif (defined(__clang__) || defined(__GNUC__)) && defined(__linux__)
  #define CAPLPASCAL
  #define CAPLCDECL
  #define CAPLEXPORT  __attribute__ ((visibility("default")))
#else
  #define CAPLPASCAL
  #define CAPLCDECL
  #define CAPLEXPORT
#endif


// Macro for static checks
#if defined(__cplusplus)
  #if __cplusplus >= 201103L
    #define VSTATIC_ERROR(check, cond) static_assert(!(cond), #check);
  #else 
    #define VSTATIC_ERROR(check, cond)\
    template <bool> struct condition_##check; \
    template <> struct condition_##check<false> {}; \
    struct check : public condition_##check<cond>{}
  #endif
#else
  #define VSTATIC_ERROR(check, cond)
#endif



/*---------------------------------------------------
 CAPL_FARCALL : type of a function that is exported
   to CAPL. The return type an parameters are
   ignored. Important are the qualifiers
      CAPLEXPORT far CAPLPASCAL
 ---------------------------------------------------*/

typedef void (CAPLPASCAL *CAPL_FARCALL)();


/*---------------------------------------------------
 CAPL_DLL_CALLCONV : Enum to define calling 
   convention used in the DLL
 ---------------------------------------------------*/
enum CAPL_DLL_CALLCONV {
  CAPL_DLL_PASCAL = 0x0000, // Bit 0 == 0
  CAPL_DLL_CDECL  = 0x0001, // Bit 0 == 1
  //
  CAPL_DLL_CAN_MSG_VER_0 = 0x0000, // Binary 0000 0000 0000 0000, used for old CAN messages, CANwin <= 3.2
  CAPL_DLL_CAN_MSG_VER_1 = 0x0002, // Binary 0000 0000 0000 0010, used for CAN messages with additional 64 bit timestamps, CANwin >= 4.0
  CAPL_DLL_CAN_MSG_VER_2 = 0x0004, // Binary 0000 0000 0000 0100, CANwin >= 6.0
  CAPL_DLL_CAN_MSG_VER_3 = 0x0006, // Binary 0000 0000 0000 0110, CANwin >= 7.1
  CAPL_DLL_CAN_MSG_VER_4 = 0x0008, // Binary 0000 0000 0000 1000, used for CAN message version 4: new members frameLen, startOfFrame, CANwin >= 7.2 SP3
  CAPL_DLL_CAN_MSG_VER_5 = 0x000A, // Binary 0000 0000 0000 1010, for later use (CAN message version 5)
  CAPL_DLL_CAN_MSG_VER_6 = 0x000C, // Binary 0000 0000 0000 1100, for later use (CAN message version 6)
  CAPL_DLL_CAN_MSG_VER_7 = 0x000E, // Binary 0000 0000 0000 1110, for later use (CAN message version 7)
};

/*---------------------------------------------------
 CAPL_DLL_INFO (CAPL_DLL_INFO2, CAPL_DLL_INFO3, CAPL_DLL_INFO4, CAPL_DLL_INFO5): Structure for description of one
   to CAPL exported function
 ---------------------------------------------------*/
#define MAX_CDL_NAME 20
// Maximum number of parameters of a CAPL function, valid for CANoe versions <= 8.0 MR
#define MAXCAPLFUNCPARS 10

/*-----------------------------------------------------------------------------
Alignment check for structures (force byte alignment for tables)
-------------------------------------------------------------------------------*/

struct VCdllAlignmentTest1 {
  char f[1];
  unsigned int i;
};

struct VCdllAlignmentTest2 {
  char f[1];
  unsigned int i;
};

VSTATIC_ERROR(allignmentCheck1, sizeof(VCdllAlignmentTest1) != 5);
VSTATIC_ERROR(allignmentCheck2, sizeof(VCdllAlignmentTest2) != 5);


/*-----------------------------------------------------------------------------
CAPL tables in different versions
-------------------------------------------------------------------------------*/

typedef struct CAPL_DLL_INFO{
  char              cdlName[MAX_CDL_NAME];     // name of the function
  CAPL_FARCALL      adr;                       // address of the function
  char              resultType;                // type of func result
  int               parCount;                  // no. of parameters
  char              parTypes[MAXCAPLFUNCPARS]; // types of parameters
  unsigned char     array[MAXCAPLFUNCPARS];    // depth of array. Without: \0
} CAPL_DLL_INFO;

// New struct supporting function names with up to 50 characters
#define MAX_CDL_NAME2 50
typedef struct CAPL_DLL_INFO2{
  char              cdlName[MAX_CDL_NAME2];    // name of the function
  CAPL_FARCALL      adr;                       // address of the function
  char              resultType;                // type of func result
  int               parCount;                  // no. of parameters
  char              parTypes[MAXCAPLFUNCPARS]; // types of parameters
  unsigned char     array[MAXCAPLFUNCPARS];    // depth of array. Without: \0
} CAPL_DLL_INFO2;

// New struct supporting parameter names
typedef struct CAPL_DLL_INFO3{
  char              cdlName[MAX_CDL_NAME2];    // name of the function
  CAPL_FARCALL      adr;                       // address of the function
  const char*       categoryName;              // name of function category
  const char*       hintText;                  // hint text for function selection
  char              resultType;                // type of func result
  int               parCount;                  // no. of parameters
  char              parTypes[MAXCAPLFUNCPARS]; // types of parameters
  unsigned char     array[MAXCAPLFUNCPARS];    // depth of array. Without: \0
  const char*       parNames[MAXCAPLFUNCPARS]; // names of parameters
} CAPL_DLL_INFO3;

// Maximum number of parameters of a CAPL function, valid for CANoe versions >= 8.1 MR
#define MAXCAPLFUNCPARS_8_1 64
// New struct supporting up to 64 parameters
typedef struct CAPL_DLL_INFO4{
  char              cdlName[MAX_CDL_NAME2];        // name of the function
  CAPL_FARCALL      adr;                           // address of the function
  const char*       categoryName;                  // name of function category
  const char*       hintText;                      // hint text for function selection
  char              resultType;                    // type of func result
  int               parCount;                      // no. of parameters
  char              parTypes[MAXCAPLFUNCPARS_8_1]; // types of parameters
  unsigned char     array[MAXCAPLFUNCPARS_8_1];    // depth of array. Without: \0
  const char*       parNames[MAXCAPLFUNCPARS_8_1]; // names of parameters
} CAPL_DLL_INFO4;

// New struct supporting context and function flags, valid for CANoe versions >= 8.2 MR
// Context
#define CAPL_CONTEXT_ALL            0           // no restriction
#define CAPL_CONTEXT_RT             0x00000001  // needs RT (CANoe or CANalyzer)
#define CAPL_CONTEXT_ANALYSIS       0x00000002  // needs CANoe simulation/test or CANalyzer send
#define CAPL_CONTEXT_TEST           0x00000004  // needs CANoe test node
#define CAPL_CONTEXT_CANOE          0x00000008  // needs CANoe simulation/test/anlyz
#define CAPL_CONTEXT_TESTUNIT       0x00000010  // needs CANoe test unit
#define CAPL_CONTEXT_TESTUNITGEN    0x00000020  // needs CANoe test unit generated CAPL library
#define CAPL_CONTEXT_SIMNODE        0x00000040  // needs CANoe simulation node (not available in test nodes / units)
// Function flags
#define CAPL_FUNCTION_FLAG_DEFAULT    0
#define CAPL_FUNCTION_FLAG_HIDDEN     1
#define CAPL_FUNCTION_FLAG_DEPRECATED 2
#define CAPL_FUNCTION_FLAG_THREADSAFE 4
typedef struct CAPL_DLL_INFO5{
  char              cdlName[MAX_CDL_NAME2];         // name of the function
  CAPL_FARCALL      adr;                            // address of the function
  unsigned int      usageMask;                      // context
  const char*       categoryName;                   // name of function category
  const char*       hintText;                       // hint text for function selection
  char              resultType;                     // type of func result
  int               parCount;                       // no. of parameters
  char              parTypes[MAXCAPLFUNCPARS_8_1];  // types of parameters
  unsigned char     array[MAXCAPLFUNCPARS_8_1];     // depth of array. Without: \0
  const char*       parNames[MAXCAPLFUNCPARS_8_1];  // names of parameters
  unsigned int      flags;                          // flags
} CAPL_DLL_INFO5;

enum VXMLTableEntryType
{
  TET_Pattern = 1,
  TET_Check   = 2,
  TET_PrivCheckControl = 3,
  TET_PrivCheckQuery = 4
};

typedef struct CAPL_DLL_XML_INFO
{
  VXMLTableEntryType  entryType;
  const char*         patternName;
  CAPL_FARCALL        invocationFunction;         // Invocation-function
  CAPL_FARCALL        compilationFunction;        // Compilation-function
  char                resultType;                 //
  int                 parCount;                   // no. of parameters
  char                parTypes[MAXCAPLFUNCPARS];  // types of parameters
  unsigned char       array[MAXCAPLFUNCPARS];     // depth of array. Without: \0
} CAPL_DLL_XML_INFO;

typedef struct CAPL_DLL_XML_INFO2
{
  VXMLTableEntryType  entryType;
  const char*         patternName;
  CAPL_FARCALL        invocationFunction;             // Invocation-function
  CAPL_FARCALL        compilationFunction;            // Compilation-function
  char                resultType;                     //
  int                 parCount;                       // no. of parameters
  char                parTypes[MAXCAPLFUNCPARS_8_1];  // types of parameters
  unsigned char       array[MAXCAPLFUNCPARS_8_1];     // depth of array. Without: \0
} CAPL_DLL_XML_INFO2;

typedef struct CAPL_DLL_ELFMAPPING{
  CAPL_FARCALL      x86Function;                // address of the function (x86 implementation)
  const char*       mappedElfFunction;          // symbol of function implementation in ELF file
}CAPL_DLL_ELFMAPPING;

/*------------------------------------------------------------------------

Since CANoe / CANalyzer 6.0 SP2, you may export the DLL function tables
as variables instead of using caplDllGetTable5 / caplDllGetCallbackTable5.

You should do that if the compiler shall not attempt to load additional 
modules referenced by your DLL or to execute code in your DLL. Examples are
DLLs for Windows CE or DLLs which need special driver DLLs. Technically, the
compiler will load the DLL using the Windows function LoadLibraryEx and the
parameter DONT_RESOLVE_REFERENCES.

Export your function table with the name caplDllTable5/4/3/2 and your callback table
with the name caplDllCallbackTable5/4/3/2. 

Example (in the implementation file):

CAPL_DLL_INFO5 table[] = {

{CDLL_VERSION_NAME, (CAPL_FARCALL)CDLL_VERSION, "", "", CAPL_DLL_CDECL, 0xabcd, CDLL_EXPORT },

{"HelloWorld", (CAPL_FARCALL) HelloWorld, CAPL_CONTEXT_ALL, "Hello World", "Copies 'Hello, World!' to a buffer", 
'L', 2, "CD", "\001\000", {"buffer", "bufferSize"} },

{0, 0}
};
CAPLEXPORT CAPL_DLL_INFO5 far * caplDllTable5 = table;

Be careful to define all elements of the table statically, i.e., none of them
may be calculated at runtime. In particular, do not use other runtime-initialized
variables (e.g. variables of type char*) in the table definition.

If the variable caplDllTable5 is not found, the compiler will reload the DLL
using LoadLibrary and call the exported functions if the library is for Win32. 
If the library is for Windows CE, the compiler will call the exported functions
without reloading the DLL.

----------------------------------------------------------------------------*/

#ifdef  __cplusplus
extern "C" {
#endif

  extern CAPLEXPORT CAPL_DLL_INFO5* caplDllTable5;
  extern CAPLEXPORT CAPL_DLL_INFO5* caplDllCallbackTable5;

  extern CAPLEXPORT CAPL_DLL_INFO4* caplDllTable4;
  extern CAPLEXPORT CAPL_DLL_INFO4* caplDllCallbackTable4;

  extern CAPLEXPORT CAPL_DLL_INFO3* caplDllTable3;
  extern CAPLEXPORT CAPL_DLL_INFO3* caplDllCallbackTable3;

  extern CAPLEXPORT CAPL_DLL_INFO2* caplDllTable2;
  extern CAPLEXPORT CAPL_DLL_INFO2* caplDllCallbackTable2;

  extern CAPLEXPORT CAPL_DLL_ELFMAPPING* caplDllElfMappingCOB;

#ifdef  __cplusplus
}
#endif

// parameter and function result types:
//      V       void
//      L       long
//      D       unsigned long
//      I       int             only in connection with array size != 0
//      W       unsigned int    only in connection with array size != 0
//      B       unsigned char   only in connection with array size != 0
//      C       char            only in connection with array size != 0
//      F       float (means double in 8 Byte 80387 format)
//      0xabcd  sign for valid table

/*---------------------------------------------------
 CAPL_DLL_INFO_LIST : the list of all to CAPL exported
   functions. The list ends with the first occurrence
   of name = NULL.
   This array is only visible inside the DLL!
 ---------------------------------------------------*/
extern CAPL_DLL_INFO CAPL_DLL_INFO_LIST[];
extern CAPL_DLL_INFO2 CAPL_DLL_INFO_LIST2[];
extern CAPL_DLL_INFO3 CAPL_DLL_INFO_LIST3[];
extern CAPL_DLL_INFO4 CAPL_DLL_INFO_LIST4[];
extern CAPL_DLL_INFO5 CAPL_DLL_INFO_LIST5[];
extern CAPL_DLL_ELFMAPPING CAPL_DLL_ELFMAPPING_COB_LIST[];

/*---------------------------------------------------
 caplDllGetTable, caplDllGetTable2: Used to export the list of
   dynamic info to the application (loader).
   CAPL_DLL_GETTABLE (CAPL_DLL_GETTABLE2) is the type of that function.
   Of the same type is caplDllGetCallbackTable. But
   this function is optional. Only DLLs which need
   callbacks, have to implement that function. The
   function may return a NULL table.
 ---------------------------------------------------*/
#ifdef  __cplusplus
extern "C"
{
#endif

  CAPLEXPORT CAPL_DLL_INFO* CAPLCDECL caplDllGetTable(void);
  CAPLEXPORT CAPL_DLL_INFO2* CAPLCDECL caplDllGetTable2(void);
  CAPLEXPORT CAPL_DLL_INFO3* CAPLCDECL caplDllGetTable3(void);
  CAPLEXPORT CAPL_DLL_INFO4* CAPLCDECL caplDllGetTable4(void);
  CAPLEXPORT CAPL_DLL_INFO5* CAPLCDECL caplDllGetTable5(void);
  CAPLEXPORT CAPL_DLL_XML_INFO* CAPLCDECL XMLDLLGetTable(void);
  CAPLEXPORT CAPL_DLL_XML_INFO2* CAPLCDECL XMLDLLGetTable2(void);
  CAPLEXPORT CAPL_DLL_ELFMAPPING* CAPLCDECL caplDllGetElfMappingCOB(void);

  CAPLEXPORT CAPL_DLL_INFO* CAPLPASCAL caplDllGetCallbackTable(void);
  CAPLEXPORT CAPL_DLL_INFO2* CAPLPASCAL caplDllGetCallbackTable2(void);
  CAPLEXPORT CAPL_DLL_INFO3* CAPLPASCAL caplDllGetCallbackTable3(void);
  CAPLEXPORT CAPL_DLL_INFO4* CAPLPASCAL caplDllGetCallbackTable4(void);
  CAPLEXPORT CAPL_DLL_INFO5* CAPLPASCAL caplDllGetCallbackTable5(void);

#ifdef  __cplusplus
}
#endif


typedef CAPL_DLL_INFO* (CAPLCDECL *CAPL_DLL_GETTABLE)(void);
typedef CAPL_DLL_INFO2* (CAPLCDECL *CAPL_DLL_GETTABLE2)(void);
typedef CAPL_DLL_INFO3* (CAPLCDECL *CAPL_DLL_GETTABLE3)(void);
typedef CAPL_DLL_INFO4* (CAPLCDECL *CAPL_DLL_GETTABLE4)(void);
typedef CAPL_DLL_INFO5* (CAPLCDECL *CAPL_DLL_GETTABLE5)(void);
typedef CAPL_DLL_XML_INFO* (CAPLCDECL *XML_DLL_GETTABLE)(void);
typedef CAPL_DLL_XML_INFO2* (CAPLCDECL *XML_DLL_GETTABLE2)(void);
typedef CAPL_DLL_ELFMAPPING* (CAPLCDECL *CAPL_DLL_GETELFMAPPING_COB)(void);


// restore structure alignment
#if defined(_MSC_VER) || defined(__clang__) || defined(__MINGW32__) || defined(__GNUC__)
  #pragma pack(pop)
#endif
