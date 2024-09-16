/*************************************************************************************************************************************************/
/* FILE:          tokenizertypes.h                                                                                                               */
/*                                                                                                                                               */
/* VERSION:       1.30                                                                                                                           */
/*                                                                                                                                               */
/* PURPOSE:       This is the header file for tokenizer.cpp that contains all the definitions of types, constants, etc., required for the        */
/*                tokenizer.                                                                                                                     */
/*                                                                                                                                               */
/* COPYRIGHT:     (c) 2001 - 2013 Parallax Inc.                                                                                                  */
/*                                                                                                                                               */
/* TERMS OF USE:  MIT License                                                                                                                    */
/*                Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation     */
/*                files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,     */
/*                modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software */
/*                is furnished to do so, subject to the following conditions:                                                                    */
/*                                                                                                                                               */
/*                The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. */
/*                                                                                                                                               */
/*                THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE           */
/*                WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR          */
/*                COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,    */
/*                ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                          */
/*************************************************************************************************************************************************/

#ifndef __TOKENIZER_TYPES_H_
#define __TOKENIZER_TYPES_H_

#include <limits.h>
#include <cstdint>


/* Simple Defines */
#define True  1
#define False 0

#undef byte                             /*Set 'byte' to 8-bits*/
typedef unsigned char byte;

#undef bool                             /* Set 'bool' to 8-bits */
#define bool byte

#undef word                             /*Set 'word' to 16-bits*/
typedef unsigned short int word;

#define STDAPI    uint8_t             /* define STDAPI */

#include "tokenizer/tokenizer_export.hpp"

#define SymbolSize		      32		            // Maximum size of symbol (in characters)
#define SymbolTableSize     1024                    // Maximum symbols allowed in symbol table (MUST BE POWER OF 2 FOR CalcSymbolHash TO WORK)
#define MaxSourceSize       0x10000                 // Maximum source file size
#define EEPROMSize		      0x800	                // 224lc16b eeprom - 2k bytes / 16k bits
#define SrcTokRefSize       ((EEPROMSize*8-14) / 7) // Max size of Source-Token Crossreference list int((# EEPROM Bits - Overhead) / CommandSize)
#define ElementListSize     10240                   // Size of element list
#define PatchListSize       0x400*2                 // Size of address patch list
#define ForNextStackSize    16                      // Max number of nested FOR..NEXT loops (Limited by firmware)
#define IfThenStackSize     16                      // Max number of nested IF..THENs
#define DoLoopStackSize     16                      // Max number of nested DO..LOOPs
#define SelectStackSize     16                      // Max number of nested SELECT CASEs
#define NestingStackSize    ForNextStackSize+IfThenStackSize+DoLoopStackSize+SelectStackSize   // Max number of nested code blocks
#define MaxExits            16                      // Maximum number of Exits within a given loop
#define ExpressionSize      0x200                   // Size of Expression Buffer (in bits)
#define PathNameSize        255                     // Maximum size of path and filename
#define ETX                 3                       // End Of Text Character

/* Macro Defines */
#define IsSymbolChar(C)                ( ((C) == '_') ||                   /* _    */ \
                                       ( ((C) >= '0') && ((C) <= '9') ) || /* 0..9 */ \
                                       ( ((C) >= 'A') && ((C) <= 'Z') ) || /* A..Z */ \
                                       ( ((C) >= 'a') && ((C) <= 'z') ) )  /* a..z */

#define IsDigitChar(C)                 ( ((C) >= '0') && ((C) <= '9') )    /* 0..9 */

#define IllegalCCDirectiveOperator(Op) ( ((Op) == ocHyp) ||  \
                                         ((Op) == ocAtn) ||  \
                                         ((Op) == ocMin) ||  \
                                         ((Op) == ocMax) ||  \
                                         ((Op) == ocMum) ||  \
                                         ((Op) == ocMuh) ||  \
                                         ((Op) == ocMod) ||  \
                                         ((Op) == ocDig) ||  \
                                         ((Op) == ocRev) )

/*File path characters and quoted file path characters are used to parse file names from the $STAMP directive*/
#define IsFilePathChar(C,Quoted)  ( ((C) == '!') ||                      /* !               */ \
                                   (((C) >= '#') && ((C) <= ')')) ||     /* #$%&'()         */ \
                                    ((C) == '+') ||                      /* +               */ \
                                   (((C) >= '-') && ((C) <= ';')) ||     /* -./0123456789:; */ \
                                    ((C) == '=') ||                      /* =               */ \
                                   (((C) >= '@') && ((C) <= 'z')) ||     /* @A..Z[\]^_`a..z */ \
                                    ((C) == '~') ||                      /* ~               */ \
                                    ((Quoted) &&                         /* If Quoted...    */ \
                                      (((C) == ' ') ||                   /* space           */ \
                                       ((C) == ',') ||                   /* ,               */ \
                                       ((C) == '{') ||                   /* {               */ \
                                       ((C) == '}'))) )                  /* }               */

#define IsConstantOperatorCode(C) ( ((C) == ocShl) || ((C) == ocShr) ||  /* Allowed operators for constant declarations */ \
                                    ((C) == ocAnd) || ((C) == ocOr)  || \
                                    ((C) == ocXor) || ((C) == ocAdd) || \
                                    ((C) == ocSub) || ((C) == ocMul) || \
                                    ((C) == ocDiv) )


#define IsMultiFileCapable(Module) ( (Module == tmBS2e) || (Module == tmBS2sx) || (Module == tmBS2p) || (Module == tmBS2pe) )


/* Types */
/*Define Bases*/
typedef enum TBase {bBinary, bDecimal, bHexadecimal, bNumElements} TBase;

/*Define Target Modules*/
typedef enum TTargetModule {tmNone, tmBS1, tmBS2, tmBS2e, tmBS2sx, tmBS2p, tmBS2pe, tmNumElements} TTargetModule;

/*Define Source Code Element Types.  These are ordered and grouped by function, but also by the order in which they
  should appear in the GetReservedWords function.  See comments beginning with "GRW" to find out how the GetReservedWord
  function should show their type.  NOTE: If TElementType is changed, change ElementTypeNames and ResWordTypeID (inside
  of GetReservedWords) appropriately.*/
typedef enum TElementType
             {etDirective,            /* $STAMP, $PORT, etc. */
              etTargetModule,         /* BS1, BS2, etc. */
              etCCDirective,          /* #DEFINE, #IF, #SELECT, etc. */
              etInstruction,          /* OUTPUT, HIGH, LOW, etc. */
              etCon,	                /* CON                           {GRW: "Declaration"} */
              etVariable,	            /* INS, OUTS, DIRS, etc. */
              etVariableAuto,         /* WORD, BYTE, NIB, BIT          {GRW: "VariableType"} */
              etVariableMod,          /* HIGHBYTE, LOWNIB, BIT15, etc. */
              etAnyNumberIO,          /* (S)NUM                        {GRW: "IOFormatter"} */
              etCond1Op,              /* <, <=, =>, >, =, <>           {GRW: "ConditionalOp"} */
              etBinaryOp,	            /* HYP, ATN, &, etc. */
              etUnaryOp,              /* SQR, ABS, ~, etc. */
              etConstant,	            /* 99, $FF, %11 */
              etPeriod,               /* . */
              etComma,                /* , */
              etQuestion,             /* ?                             {GRW: "QuestionMark"} */
              etBackslash,            /* \ */
              etAt,                   /* @                             {GRW: "AtSign"} */
              etLeft,                 /* (                             {GRW: "Parentheses"} */
              etLeftBracket,          /* [                             {GRW: "Brackets"} */
              etRightCurlyBrace,      /* } */
              etCCThen,               /* #THEN                         {GRW: etCCDirective} */
              etData,	                /* DATA                          {GRW: etInstruction} */
              etStep,                 /* STEP                          {GRW: etInstruction} */
              etTo,                   /* TO                            {GRW: etInstruction} */
              etThen,                 /* THEN                          {GRW: etInstruction} */
              etWhile,                /* WHILE                         {GRW: etInstruction} */
              etUntil,                /* UNTIL                         {GRW: etInstruction} */
              etPin,                  /* PIN                           {GRW: etCon} */
              etVar,	                /* VAR                           {GRW: etCon} */
              etASCIIIO,              /* ASC (must be followed by '?') {GRW: etAnyNumberIO} */
              etNumberIO,             /* (I)(S)DEC/HEX/BIN(1-16)       {GRW: etAnyNumberIO} */
              etRepeatIO,             /* REP                           {GRW: etAnyNumberIO} */
              etSkipIO,               /* SKIP                          {GRW: etAnyNumberIO} */
              etSpStringIO,           /* SPSTR - for BS2p and BS2pe    {GRW: etAnyNumberIO} */
              etStringIO,             /* STR                           {GRW: etAnyNumberIO} */
              etWaitIO,               /* WAIT                          {GRW: etAnyNumberIO} */
              etWaitStringIO,         /* WAITSTR                       {GRW: etAnyNumberIO} */
              etCond2Op,              /* AND, OR, XOR                  {GRW: etCond1Op} */
              etCond3Op,              /* NOT                           {GRW: etCond1Op} */
              etRight,                /* )                             {GRW: "etLeft"} */
              etRightBracket,         /* ]                             {GRW: "etLeftBracket"} */
              etPinNumber,            /* 0, 1,..15 */
              etAddress,	            /* (address symbol) */
              etCCConstant,	          /* 99, $FF, %11 (compile-time constant only) */
              etFileName,             /* Project member file: Slot2.bsx, etc */
              etUndef,	              /* (undefined symbol) */
              etEnd,                  /* end-of-line */
              etCancel} TElementType; /* canceled element record */


/*Define PBASIC Instruction Types.  The order they appear here DOES NOT affect the tokenizer operation.  They are
  alphabetically arranged for clarity only*/
typedef enum TInstructionType {itAuxio,itBranch,itButton,itCase,itCount,itDebug,itDebugIn,itDefine,itDo,itDtmfout,itElse,
                       itElseIf,itEnd,itEndIf,itEndSelect,itError,itExit,itFor,itFreqout,itGet,itGosub,itGoto,itHigh,
                       itI2cin,itI2cout,itIf,itInput,itIoterm,itLcdcmd,itLcdin,itLcdout,itLookdown,itLookup,itLoop,itLow,
                       itMainio,itNap,itNext,itOn,itOutput,itOwin,itOwout,itPause,itPollin,itPollmode,itPollout,
                       itPollrun,itPollwait,itPulsin,itPulsout,itPut,itPwm,itSleep,itRandom,itRctime,itRead,itReturn,
                       itReverse,itRun,itSelect,itSerin,itSerout,itShiftin,itShiftout,itStop,itStore,itToggle,itWrite,
                       itXout} TInstructionType;

/*Define PBASIC Instruction Code names - Used to index into the InstCode array.  The order they appear here IS CRITICAL
  to the operation of the tokenizer.  If the order is changed here, it ABSOLUTELY must be changed in the InstCode array,
  defined below, as well!*/
typedef enum TInstructionCode {icEnd,icSleep,icNap,icStop,icOutput,icHigh,icToggle,icLow,icReverse,icGoto,icGosub,
                               icReturn,icInput,icIf,icNext,icBranch,icLookup,icLookdown,icRandom,icRead,icWrite,icPause,
                               icFreqout1,icFreqout2,icDtmfout,icXout,icDone,icGet,icPut,icRun,
                               icMainio,icAuxio,icSeroutNoFlow,icSeroutFlow,icSerinNoFlow,icSerinFlow,icPulsout,
                               icPulsin,icCount,icShiftin,icShiftout,icRctime,icButton,icPwm,icLcdin,icLcdout,
                               icLcdcmd,icI2cin_ex,icI2cin_noex,icI2cout_ex,icI2cout_noex,icPollrun,icPollmode,
                               icPollin,icPollout,icPollwait,icOwout,icOwin,icIoterm,icStore,
                               icReserved60,icReserved61,icReserved62,icReserved63,icNumElements} TInstructionCode;

/*Define Directive Types*/
typedef enum TDirectiveType {dtStamp,dtPort,dtPBasic} TDirectiveType;

/*Define Nesting types/modes for IF..THEN..ELSE..ENDIF, FOR..NEXT, DO..LOOP and SELECT CASE.  For IF..THEN, indicates
  whether the IF is a single or multi-line and what portion is currently being parsed (Main/Else).  NE, "no end", means
  don't look for END element.*/
typedef enum TNestingType {ntIFSingleElse,ntIFSingleElseNE,ntIFSingleMain,ntIFSingleMainNE,ntIFMultiElse,ntIFMultiMain,
                           ntFOR,ntDO,ntSELECT} TNestingType;

/*Define PBASIC Operator Codes*/
/*Unary Operators = SQR - SIN, Binary Operators = Hyp - Rev, Conditional Operators = AE - B
  NOTE: EnterExpressionOperator routine relies on there being no more than $1F operators*/
typedef enum TOperatorCode {ocSqr,ocAbs,ocNot,ocNeg,ocDcd,ocNcd,ocCos,ocSin,
                            ocHyp,ocAtn,ocAnd,ocOr,ocXor,ocMin,ocMax,ocAdd,
                            ocSub,ocMum,ocMul,ocMuh,ocMod,ocDiv,ocDig,ocShl,
                            ocShr,ocRev,ocAE,ocBE,ocE,ocNE,ocA,ocB} TOperatorCode;

/*Define Compiler Error Codes} {If these are changed, modify Errors array to match*/
typedef enum TErrorCode {ecS,ecECS,ecETQ,ecUC,ecEHD,ecEBD,ecSETC,ecTME,ecCESD,ecCESB,ecUS,ecUL,ecEAC,ecCDBZ,ecLIOOR,
                         ecLACD,ecEQ,ecLIAD,ecEB,ecEL,ecER,ecELB,ecERB,ecERCB,ecERCBCSMTSAPF,ecSIAD,ecDOSLAP,ecASCBZ,
                         ecOOVS,ecEF,ecSTF,ecECOEOL,ecECEOLOC,ecESEOLOC,ecNMBPBF,ecEC,ecECORB,ecEAV,ecEABV,ecEAVM,
                         ecVIABS,ecEASSVM,ecVMIOOR,ecEACVUOOL,ecEABOOR,ecEACOOLB,ecEITC,ecLOTFFGE,ecLOSNFNLE,ecLOSVE,
                         ecEAL,ecEALVOI,ecEE,ecET,ecETO,ecETM,ecEFN,ecED,ecDD,ecECP,ecUTMSDNF,ecNTT,ecLOSNITSE,ecEMBPBIOC,
                         ecEIMBPBI,ecEALVIOE,ecEALVIOEOL,ecLOSNDLSE,ecLMBPBD,ecWOUCCAABDAL,ecEWUEOLOC,ecEOAWFNADLS,
                         ecIWEI,ecFWN,ecDWL,ecLOSESWLSE,ecEVOW,ecEAWV,ecLIMC,ecPNMBZTF,ecEALVION,ecEALVIOL,ecLOSNSSE,
                         ecECE,ecCMBPBS,ecLOSCSWSSE,ecEALVIOES,ecESMBPBS,ecSWE,ecEGOG,ecCCBLTO,ecIPVNMBTZTF,ecENEDDSON,
                         ecIOICCD,ecECCT,ecCCIWCCEI,ecCCSWCCE,ecCCEMBPBCCI,ecCCEIMBPBCCI,ecISICCD,ecEAUDS,ecLOSNCCICCTSE,
                         ecEACOC,ecUDE,ecECCEI,ecLOSNCCSSE,ecECCCE,ecCCCMBPBCCS,ecCCESMBPBCCS,ecEADRTSOCCE,ecEADRTSOCCES,
                         ecECCE,ecENEDODS,ecESTFPC,ecEACVOW,ecELIMBPBI,ecLOSELISWISE,ecELINAAE,ecNumElements} TErrorCode;

/*Define symbol table structure*/
struct TOKENIZER_EXPORT TSymbolTable
{
/*32 bytes*/   char         Name[SymbolSize+1];
/*4 bytes*/	   TElementType ElementType;
/*2 bytes*/	   word         Value;
/*4 bytes*/	   int          NextRecord; /* Next record ID if Symbol hash used more than once.
                                           Also used to indicate undefined non-DEFINE symbol (0)
                                           or undefined DEFINE symbol (1) in Symbol variable*/
};

/*Define custom symbol structure*/
struct TOKENIZER_EXPORT TCustomSymbolTable
{
    int          Targets;  /*Or'd pattern represents stamps supporting this symbol. Bit1 = BS1, Bit2 = BS2, etc*/
    TSymbolTable Symbol;
};

/*Define undefined symbol table structure*/
struct TOKENIZER_EXPORT TUndefSymbolTable
{
/*32 bytes*/    char         Name[SymbolSize+1];
/*4 bytes*/     int          NextRecord;                       /*Next record ID if Symbol hash used more than once*/
};

/*Define element list structure*/
struct TOKENIZER_EXPORT TElementList
{
/*4 bytes*/     TElementType ElementType;
/*2 bytes*/ 	word         Value;
/*2 bytes*/     word         Start;
/*1 byte */	    byte         Length;
};

/*Define source token crossreference structure*/
struct TOKENIZER_EXPORT TSrcTokReference
{
/*2 bytes*/     word         SrcStart;
/*2 bytes*/     word         TokStart;
};


/*Define Nesting Stack structure for FOR..NEXT, IF..THEN..ELSE..ENDIF, DO..LOOP and SELECT CASE*/
struct TOKENIZER_EXPORT TNestingStack
{
    TNestingType  NestType;                 /*The type (and mode for IF..THENs) of the current nested code block*/
    word          ElementIdx;               /*The element where the code block started (FOR, IF, DO, SELECT)*/
    word          ExpIdx;                   /*The element where a SELECT statment's expression started*/
    TOperatorCode AutoCondOp;               /*The automatic conditional opererator to insert in condition during a SELECT*/
    word          JumpLabel;                /*The address to jump back to from NEXT or LOOP*/
    word          SkipLabel;                /*The EEPROM address of a "SkipLabel" address to patch*/
    word          Exits[MaxExits];          /*A list of EXIT command's "SkipLabel" addresses, or SELECT's GOTO "ExitLabel" address, to patch.
                                              EXIT commands are allowed in FOR and DO loops only*/
};

/*Define module compile object code type*/
typedef byte   TPacketType[int(EEPROMSize/16*18)];
struct TOKENIZER_EXPORT TModuleRec
{
/*1 byte*/		               bool         Succeeded;                  /*Pass or failed on compile*/
/*4 bytes*/			           char         *Error;                     /*Error message if failed*/
/*1 byte*/			           bool         DebugFlag;                  /*Indicates there's debug data*/

/*1 byte*/				       byte         TargetModule;               /*BASIC Stamp Module to compile for. 0=None, 1=BS1, 2=BS2, 3=BS2e, 4=BS2sx, 5=BS2p, 6=BS2pe*/
/*4 bytes*/			           int          TargetStart;                /*Beginning of $STAMP directive target in source*/
/*4*7 bytes*/		           char         *ProjectFiles[7];           /*Paths and names of related project files, if any*/
/*4*7 bytes*/		           int          ProjectFilesStart[7];       /*Beginning of project file name in source*/

/*4 bytes*/			           char         *Port;                      /*COM port to download to*/
/*4 bytes*/			           int          PortStart;                  /*Beginning of port name in source*/

/*4 bytes*/			           int          LanguageVersion;            /*Version of target PBASIC language. 200 = 2.00, 250 = 2.50, etc*/
/*4 bytes*/		               int          LanguageStart;              /*Beginning of language version in source*/

/*4 bytes*/		     	       int          SourceSize;                 /*Enter source code length here*/
/*4 bytes*/			           int          ErrorStart;                 /*Beginning of error in source*/
/*4 bytes*/			           int          ErrorLength;                /*Number of bytes in error selection*/

/*1*EEPROMSize bytes*/	       byte         EEPROM[EEPROMSize];         /*Tokenized data if compile worked*/
/*1*EEPROMSize bytes*/	       byte         EEPROMFlags[EEPROMSize];    /*EEPROM flags, bit 7: 0=unused, 1=used
                                                                          bits 0..6: 0=empty, 1=undef data, 2=def data, 3=program*/

/*1*4 bytes*/			       byte         VarCounts[4];               /*# of.. [0]=bits, [1]=nibbles, [2]=bytes, [3]=words*/
/*1 byte*/				       byte         PacketCount;                /*# of packets*/
/*1*(EEPROMSize/16*18) bytes*/ TPacketType  PacketBuffer;       /*packet data*/
};

/*Define global variables*/
extern TModuleRec        *tzModuleRec;                         /*tzModuleRec is a pointer to an externally accessible structure*/
extern char              *tzSource;                            /*tzSource is a pointer to an externally accessible byte array*/
extern TSrcTokReference  *tzSrcTokReference;                   /*tzSrcTokReference is a pointer to an externally accessible Source vs. Token Reference array*/
extern TSymbolTable      Symbol;
extern TSymbolTable      Symbol2;
extern TSymbolTable      SymbolTable[SymbolTableSize];
extern int               SymbolVectors[SymbolTableSize];       /*Vectors used for hashing into Symbol Table*/
extern int               SymbolTablePointer;
extern TUndefSymbolTable UndefSymbolTable[SymbolTableSize];
extern int               UndefSymbolVectors[SymbolTableSize];  /*Vectors used for hashing into Undefined Symbol Table.  Used to distinguish between undefined DEFINE'd symbols and undefined DATA, VAR, CON or PIN symbols*/
extern int               UndefSymbolTablePointer;
extern TElementList      ElementList[ElementListSize];
extern word              ElementListIdx;
extern word              ElementListEnd;
extern word              EEPROMPointers[EEPROMSize*2];
extern word              EEPROMIdx;
extern word              GosubCount;
extern word              PatchList[PatchListSize];
extern int               PatchListIdx;
extern TNestingStack     NestingStack[NestingStackSize];
extern byte              NestingStackIdx;                      /*Index of next available nesting stack element*/
extern byte              ForNextCount;                         /*Current count of Nested FOR..NEXT loops*/
extern byte              IfThenCount;                          /*Current count of Nested IF THENs*/
extern byte              DoLoopCount;                          /*Current count of Nested DO..LOOPs*/
extern byte              SelectCount;                          /*Current count of Nested SELECT CASEs*/
extern word              Expression[4][int(ExpressionSize / 16)]; /*4 arrays of (1 word size, N words data)*/
extern byte              ExpressionStack[256];
extern byte              ExpStackTop;
extern byte              ExpStackBottom;
extern byte              StackIdx;                             /*Run-time stack pointer*/
extern byte              ParenCount;
extern int               SrcIdx;                               /*Used by Element Engine*/
extern word              StartOfSymbol;                        /*Used by Element Engine*/
extern byte              CurChar;                              /*Used by Element Engine*/
extern TElementType      ElementType;                /*Used by Element Engine*/
extern bool		      	   EndEntered;								           /*Used by Element Engine.  Indicates if End was just entered.*/
extern bool              AllowStampDirective;                  /*Set by Compile routine*/
extern byte              VarBitCount;                          /*# of var bits; used by variable parsing routines*/
extern byte              VarBases[4];                          /*start of.. [0]=bits, [1]=nibbles, [2]=bytes, [3]=words*; used by variable parsing routines*/
extern bool              Lang250;                              /*False = PBASIC 2.00, True = PBASIC 2.50*/
extern int               SrcTokReferenceIdx;


/*Define global constants*/
const int SymbolTableLimit   = SymbolTableSize-SymbolSize-5;

/*Define array for binary, decimal and hexidecimal range value*/
const int  BaseValue[bNumElements] = {2,10,16};

/*Define 6-bit Instruction Codes (OpCodes).  Unused instruction codes have no comments.  Each column of this constant
array contains the codes for a particular Stamp module.  This array is indexed into using the TInstructionCode
enumerations and the tzModuleRec->TargetModule.

Note that some Stamps have additional instructions that have been placed in the unused space.  The icDone instruction,
however, has a different instruction code between certain Stamps.  While this constant array is not the most efficient
means of managing this nuance, it is used to provide a clear definition of all instruction codes for all the Stamps
supported by this tokenizer.

Also, tmNone and tmBS1 are not included by the current implementation of this tokenizer.  The code that references
the InstCode array MUST account for this!*/

const byte InstCode[icNumElements][tmNumElements-2 /*tmNone and tmBS1 are missing*/] = \
    /*         tmBS2                    tmBS2e                  tmBS2sx                  tmBS2p                   tmBS2pe         */
    /*-----------------------  -----------------------  -----------------------  -----------------------  ------------------------*/
{ {         /*icEnd*/ 0x00,          /*icEnd*/ 0x00,          /*icEnd*/ 0x00,          /*icEnd*/ 0x00,          /*icEnd*/ 0x00},
    {       /*icSleep*/ 0x01,        /*icSleep*/ 0x01,        /*icSleep*/ 0x01,        /*icSleep*/ 0x01,        /*icSleep*/ 0x01},
    {         /*icNap*/ 0x02,          /*icNap*/ 0x02,          /*icNap*/ 0x02,          /*icNap*/ 0x02,          /*icNap*/ 0x02},
    {        /*icStop*/ 0x03,         /*icStop*/ 0x03,         /*icStop*/ 0x03,         /*icStop*/ 0x03,         /*icStop*/ 0x03},
    {      /*icOutput*/ 0x04,       /*icOutput*/ 0x04,       /*icOutput*/ 0x04,       /*icOutput*/ 0x04,       /*icOutput*/ 0x04},
    {        /*icHigh*/ 0x05,         /*icHigh*/ 0x05,         /*icHigh*/ 0x05,         /*icHigh*/ 0x05,         /*icHigh*/ 0x05},
    {      /*icToggle*/ 0x06,       /*icToggle*/ 0x06,       /*icToggle*/ 0x06,       /*icToggle*/ 0x06,       /*icToggle*/ 0x06},
    {         /*icLow*/ 0x07,          /*icLow*/ 0x07,          /*icLow*/ 0x07,          /*icLow*/ 0x07,          /*icLow*/ 0x07},
    {     /*icReverse*/ 0x08,      /*icReverse*/ 0x08,      /*icReverse*/ 0x08,      /*icReverse*/ 0x08,      /*icReverse*/ 0x08},
    {        /*icGoto*/ 0x09,         /*icGoto*/ 0x09,         /*icGoto*/ 0x09,         /*icGoto*/ 0x09,         /*icGoto*/ 0x09},
    {       /*icGosub*/ 0x0A,        /*icGosub*/ 0x0A,        /*icGosub*/ 0x0A,        /*icGosub*/ 0x0A,        /*icGosub*/ 0x0A},
    {      /*icReturn*/ 0x0B,       /*icReturn*/ 0x0B,       /*icReturn*/ 0x0B,       /*icReturn*/ 0x0B,       /*icReturn*/ 0x0B},
    {       /*icInput*/ 0x0C,        /*icInput*/ 0x0C,        /*icInput*/ 0x0C,        /*icInput*/ 0x0C,        /*icInput*/ 0x0C},
    {          /*icIf*/ 0x0D,           /*icIf*/ 0x0D,           /*icIf*/ 0x0D,           /*icIf*/ 0x0D,           /*icIf*/ 0x0D},
    {        /*icNext*/ 0x0E,         /*icNext*/ 0x0E,         /*icNext*/ 0x0E,         /*icNext*/ 0x0E,         /*icNext*/ 0x0E},
    {      /*icBranch*/ 0x0F,       /*icBranch*/ 0x0F,       /*icBranch*/ 0x0F,       /*icBranch*/ 0x0F,       /*icBranch*/ 0x0F},
    {      /*icLookup*/ 0x10,       /*icLookup*/ 0x10,       /*icLookup*/ 0x10,       /*icLookup*/ 0x10,       /*icLookup*/ 0x10},
    {    /*icLookdown*/ 0x11,     /*icLookdown*/ 0x11,     /*icLookdown*/ 0x11,     /*icLookdown*/ 0x11,     /*icLookdown*/ 0x11},
    {      /*icRandom*/ 0x12,       /*icRandom*/ 0x12,       /*icRandom*/ 0x12,       /*icRandom*/ 0x12,       /*icRandom*/ 0x12},
    {        /*icRead*/ 0x13,         /*icRead*/ 0x13,         /*icRead*/ 0x13,         /*icRead*/ 0x13,         /*icRead*/ 0x13},
    {       /*icWrite*/ 0x14,        /*icWrite*/ 0x14,        /*icWrite*/ 0x14,        /*icWrite*/ 0x14,        /*icWrite*/ 0x14},
    {       /*icPause*/ 0x15,        /*icPause*/ 0x15,        /*icPause*/ 0x15,        /*icPause*/ 0x15,        /*icPause*/ 0x15},
    {    /*icFreqout1*/ 0x16,     /*icFreqout1*/ 0x16,     /*icFreqout1*/ 0x16,     /*icFreqout1*/ 0x16,     /*icFreqout1*/ 0x16},
    {    /*icFreqout2*/ 0x17,     /*icFreqout2*/ 0x17,     /*icFreqout2*/ 0x17,     /*icFreqout2*/ 0x17,     /*icFreqout2*/ 0x17},
    {     /*icDtmfout*/ 0x18,      /*icDtmfout*/ 0x18,      /*icDtmfout*/ 0x18,      /*icDtmfout*/ 0x18,      /*icDtmfout*/ 0x18},
    {        /*icXout*/ 0x19,         /*icXout*/ 0x19,         /*icXout*/ 0x19,         /*icXout*/ 0x19,         /*icXout*/ 0x19},
    {        /*icDone*/ 0x1A,         /*icDone*/ 0x1D,         /*icDone*/ 0x1D,         /*icDone*/ 0x1F,         /*icDone*/ 0x1F},
    {              /**/ 0x1B,          /*icGet*/ 0x1A,          /*icGet*/ 0x1A,          /*icGet*/ 0x1A,          /*icGet*/ 0x1A},
    {              /**/ 0x1C,          /*icPut*/ 0x1B,          /*icPut*/ 0x1B,          /*icPut*/ 0x1B,          /*icPut*/ 0x1B},
    {              /**/ 0x1D,          /*icRun*/ 0x1C,          /*icRun*/ 0x1C,          /*icRun*/ 0x1C,          /*icRun*/ 0x1C},
    {              /**/ 0x1E,               /**/ 0x1E,               /**/ 0x1E,       /*icMainio*/ 0x1D,       /*icMainio*/ 0x1D},
    {              /**/ 0x1F,               /**/ 0x1F,               /**/ 0x1F,        /*icAuxio*/ 0x1E,        /*icAuxio*/ 0x1E},
    {/*icSeroutNoFlow*/ 0x20, /*icSeroutNoFlow*/ 0x20, /*icSeroutNoFlow*/ 0x20, /*icSeroutNoFlow*/ 0x20, /*icSeroutNoFlow*/ 0x20},
    {  /*icSeroutFlow*/ 0x21,   /*icSeroutFlow*/ 0x21,   /*icSeroutFlow*/ 0x21,   /*icSeroutFlow*/ 0x21,   /*icSeroutFlow*/ 0x21},
    { /*icSerinNoFlow*/ 0x22,  /*icSerinNoFlow*/ 0x22,  /*icSerinNoFlow*/ 0x22,  /*icSerinNoFlow*/ 0x22,  /*icSerinNoFlow*/ 0x22},
    {   /*icSerinFlow*/ 0x23,    /*icSerinFlow*/ 0x23,    /*icSerinFlow*/ 0x23,    /*icSerinFlow*/ 0x23,    /*icSerinFlow*/ 0x23},
    {     /*icPulsout*/ 0x24,      /*icPulsout*/ 0x24,      /*icPulsout*/ 0x24,      /*icPulsout*/ 0x24,      /*icPulsout*/ 0x24},
    {      /*icPulsin*/ 0x25,       /*icPulsin*/ 0x25,       /*icPulsin*/ 0x25,       /*icPulsin*/ 0x25,       /*icPulsin*/ 0x25},
    {       /*icCount*/ 0x26,        /*icCount*/ 0x26,        /*icCount*/ 0x26,        /*icCount*/ 0x26,        /*icCount*/ 0x26},
    {     /*icShiftin*/ 0x27,      /*icShiftin*/ 0x27,      /*icShiftin*/ 0x27,      /*icShiftin*/ 0x27,      /*icShiftin*/ 0x27},
    {    /*icShiftout*/ 0x28,     /*icShiftout*/ 0x28,     /*icShiftout*/ 0x28,     /*icShiftout*/ 0x28,     /*icShiftout*/ 0x28},
    {      /*icRctime*/ 0x29,       /*icRctime*/ 0x29,       /*icRctime*/ 0x29,       /*icRctime*/ 0x29,       /*icRctime*/ 0x29},
    {      /*icButton*/ 0x2A,       /*icButton*/ 0x2A,       /*icButton*/ 0x2A,       /*icButton*/ 0x2A,       /*icButton*/ 0x2A},
    {         /*icPwm*/ 0x2B,          /*icPwm*/ 0x2B,          /*icPwm*/ 0x2B,          /*icPwm*/ 0x2B,          /*icPwm*/ 0x2B},
    {              /**/ 0x2C,               /**/ 0x2C,               /**/ 0x2C,        /*icLcdin*/ 0x2C,        /*icLcdin*/ 0x2C},
    {              /**/ 0x2D,               /**/ 0x2D,               /**/ 0x2D,       /*icLcdout*/ 0x2D,       /*icLcdout*/ 0x2D},
    {              /**/ 0x2E,               /**/ 0x2E,               /**/ 0x2E,       /*icLcdcmd*/ 0x2E,       /*icLcdcmd*/ 0x2E},
    {              /**/ 0x2F,               /**/ 0x2F,               /**/ 0x2F,     /*icI2cin_ex*/ 0x2F,     /*icI2cin_ex*/ 0x2F},
    {              /**/ 0x30,               /**/ 0x30,               /**/ 0x30,   /*icI2cin_noex*/ 0x30,   /*icI2cin_noex*/ 0x30},
    {              /**/ 0x31,               /**/ 0x31,               /**/ 0x31,    /*icI2cout_ex*/ 0x31,    /*icI2cout_ex*/ 0x31},
    {              /**/ 0x32,               /**/ 0x32,               /**/ 0x32,  /*icI2cout_noex*/ 0x32,  /*icI2cout_noex*/ 0x32},
    {              /**/ 0x33,               /**/ 0x33,               /**/ 0x33,      /*icPollrun*/ 0x33,      /*icPollrun*/ 0x33},
    {              /**/ 0x34,               /**/ 0x34,               /**/ 0x34,     /*icPollmode*/ 0x34,     /*icPollmode*/ 0x34},
    {              /**/ 0x35,               /**/ 0x35,               /**/ 0x35,       /*icPollin*/ 0x35,       /*icPollin*/ 0x35},
    {              /**/ 0x36,               /**/ 0x36,               /**/ 0x36,      /*icPollout*/ 0x36,      /*icPollout*/ 0x36},
    {              /**/ 0x37,               /**/ 0x37,               /**/ 0x37,     /*icPollwait*/ 0x37,     /*icPollwait*/ 0x37},
    {              /**/ 0x38,               /**/ 0x38,               /**/ 0x38,        /*icOwout*/ 0x38,        /*icOwout*/ 0x38},
    {              /**/ 0x39,               /**/ 0x39,               /**/ 0x39,         /*icOwin*/ 0x39,         /*icOwin*/ 0x39},
    {              /**/ 0x3A,               /**/ 0x3A,               /**/ 0x3A,       /*icIoterm*/ 0x3A,       /*icIoterm*/ 0x3A},
    {              /**/ 0x3B,               /**/ 0x3B,               /**/ 0x3B,        /*icStore*/ 0x3B,        /*icStore*/ 0x3B},
    {              /**/ 0x3C,               /**/ 0x3C,               /**/ 0x3C,               /**/ 0x3C,               /**/ 0x3C},
    {              /**/ 0x3D,               /**/ 0x3D,               /**/ 0x3D,               /**/ 0x3D,               /**/ 0x3D},
    {              /**/ 0x3E,               /**/ 0x3E,               /**/ 0x3E,               /**/ 0x3E,               /**/ 0x3E},
    {              /**/ 0x3F,               /**/ 0x3F,               /**/ 0x3F,               /**/ 0x3F,               /**/ 0x3F}};

/*Define all error messages*/
/*Note: Error 000 is only used as a "successful" return value for many functions and is not actually returned to the
calling program*/
extern const char *Errors[ecNumElements];

/*Common Symbols are used by all Stamps.  See Custom Symbols for Stamp Module-specific symbols*/
extern TSymbolTable CommonSymbols[363];

  /*Custom Symbols are those that are either not supported by every Stamp or not supported by every version of the PBASIC
  language syntax.  The Targets field is a bit pattern that defines the module which supports the particular symbol and
  the version of PBASIC.  The bit pattern defines module support from LSB (tmBS1) toward MSB (tmBS2pe) and PBASIC
  version 2.0 (bit 6) to PBASIC version 2.5 (bit 7).  For example, a pattern of $BA, which is %1011 1010, indicates the
  symbol is supported by the BS2, BS2sx, BS2p and BS2pe (read from right to left), but not the BS1 (indicated by 0 in
  bit 0) and not the BS2e (indicated by 0 in bit 2) and in not in version 2.0 (indicated by 0 in bit 6) and in version
  2.5 (indicated by 1 in bit 7).  These symbols are entered into the symbol table by the AdjustSymbols procedure after
  the Target Module and Language Version is identified.*/

#define CustomSymbolTableSize 53    /*Size of Custom Symbol Table (used in more than one place)*/
extern TCustomSymbolTable CustomSymbols[CustomSymbolTableSize];


#endif
