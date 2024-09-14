#pragma once

#include "tokenizer/tokenizer_export.hpp"
#include "tokenizer/tokenizer_types.hpp"  /* Make sure this is the last include! */


#define DevDebug                          /*Uncomment this to enable the GetVariableItem, GetSymbolTableItem, GetElementListItem and
       	                                    GetGosubCount routines.  This is only for development support*/

#define TokenizerVersion  130			  /*Version of this tokenizer; xyy means version x.yy*/

class TOKENIZER_EXPORT tokenizer {

/*Define forward declarations*/

/*---Published Library Functions---*/
#ifdef __APPLE_CC__

  STDAPI TestRecAlignment(TModuleRec *Rec);
  STDAPI Version(void);
  #if defined(DevDebug)
    STDAPI GetVariableItems(byte *VBitCount, byte *VBases);
    STDAPI GetSymbolTableItem(TSymbolTable *Sym, int Idx);
    STDAPI GetUndefSymbolTableItem(TUndefSymbolTable *Sym, int Idx);
    STDAPI GetElementListItem(TElementList *Ele, int Idx);
    STDAPI GetGosubCount(void);
  #endif
  STDAPI Compile(TModuleRec *Rec, char *Src, bool DirectivesOnly, bool ParseStampDirective, TSrcTokReference *Ref);
  STDAPI GetReservedWords(TModuleRec *Rec, char *Src);

#endif

/*---Misc---*/
byte       ResWordTypeID(TElementType ElementType);
void       InitializeRec(void);
void       ClearEEPROM(void);
void       ClearSrcTokReference(void);

/*---Expression Engine---(Compiles algebraic expressions)*/
TErrorCode GetReadWrite(bool Write);
TErrorCode GetValueConditional(bool Conditional, bool PinIsConstant, int SplitExpression);
TErrorCode GetExpression(bool Conditional, bool PinIsConstant, int SplitExpression, bool CCDirective);
TErrorCode EnterExpressionConstant(TElementList Element);
TErrorCode EnterExpressionVariable(TElementList Element, bool Write);
TErrorCode EnterExpressionOperator(byte Data);
TErrorCode EnterExpressionBits(byte Bits, word Data);
TErrorCode PushLeft(void);
TErrorCode Push(byte Data);
TErrorCode PopLeft(void);
TErrorCode PopOperators(TElementList *Element);
bool       Pop1Operator(TElementList *Element);
void       CopyExpression(byte SourceNumber, byte DestinationNumber);
TErrorCode EnterExpression(byte ExpNumber, bool Enter1Before);

/*---Symbol Engine---(Builds and searches the Symbol Table)-*/
TErrorCode InitSymbols(void);
TErrorCode AdjustSymbols(void);
TErrorCode EnterSymbol(TSymbolTable Symbol);
TErrorCode EnterUndefSymbol(char *Name);
bool       FindSymbol(TSymbolTable *Symbol);
bool       ModifySymbolValue(const char *Name, word Value);
int        GetSymbolVector(const char *Name);
int        GetUndefSymbolVector(char *Name);
int        CalcSymbolHash(const char *SymbolName);

/*---Element Engine---(Elementizes the source code into the ElementList)-*/
TErrorCode ElementError(bool IncLength, TErrorCode ErrorID);
void       SkipToEnd(void);
TErrorCode GetString(void);
TErrorCode GetNumber(TBase Base, byte DPDigits, word *Result);
TErrorCode GetSymbol(void);
TErrorCode GetFilename(bool Quoted);
TErrorCode GetDirective(void);
TErrorCode EnterElement(TElementType ElementType, word Value, bool IsEnd);
TErrorCode Elementize(bool LastPass);
bool       GetElement(TElementList *Element);
bool       PreviewElement(TElementList *Preview);
void       CancelElements(word Start, word Finish);
void       VoidElements(word Start, word Finish);
void       GetSymbolName(int Start, int Length);

/*---Directive Compilers---(These compile the compile-time items, editor directives and compiler directives)-*/
TErrorCode CompileEditorDirectives(void);
TErrorCode CompileStampDirective(void);
TErrorCode CompilePortDirective(void);
TErrorCode CompilePBasicDirective(void);
TErrorCode CompileCCDirectives(void);
TErrorCode CompileCCIf(void);
TErrorCode CompileCCElse(void);
TErrorCode CompileCCEndIf(void);
TErrorCode CompileCCDefine(void);
TErrorCode CompileCCError(void);
TErrorCode CompileCCSelect(void);
TErrorCode AppendExpression(byte SourceExpression, TOperatorCode AppendOperator);
TErrorCode CompileCCCase(void);
TErrorCode CompileCCEndSelect(void);
TErrorCode CompilePins(bool LastPass);
TErrorCode CompileConstants(bool LastPass);
TErrorCode AssignSymbol(bool *SymbolFlag, word *EEPROMIdx);
TErrorCode EnterData(TElementList *Element, word *EEPROMValue, word *EEPROMIdx, bool WordFlag, bool DefinedFlag, bool LastPass);
TErrorCode CompileData(bool LastPass);
TErrorCode GetModifiers(TElementList *Element);
TErrorCode CompileVar(bool LastPass);
TErrorCode ResolveConstant(bool LastPass, bool CCDefine, bool *Resolved);
TErrorCode GetCCDirectiveExpression(int SplitExpression);
int        GetExpBits(byte Bits, word *BitIdx);
int        RaiseToPower(int Base, int Exponent);
int        ResolveCCDirectiveExpression(void);

/*---Instruction Compilers---(These are the high-level routines that compile actual BASIC Stamp instructions)-*/
TErrorCode CompileInstructions(void);
TErrorCode CompileAuxio(void);
TErrorCode CompileBranch(void);
TErrorCode CompileButton(void);
TErrorCode CompileCase(void);
TErrorCode CompileCount(void);
TErrorCode CompileDebug(void);
TErrorCode CompileDebugIn(void);
TErrorCode CompileDo(void);
TErrorCode CompileDtmfout(void);
TErrorCode CompileElse(void);
TErrorCode CompileEnd(void);
TErrorCode CompileEndIf(void);
TErrorCode CompileEndSelect(void);
TErrorCode CompileExit(void);
TErrorCode CompileFor(void);
TErrorCode CompileFreqout(void);
TErrorCode CompileGet(void);
TErrorCode CompileGosub(void);
TErrorCode CompileGoto(void);
TErrorCode CompileHigh(void);
TErrorCode CompileI2cin(void);
TErrorCode CompileI2cout(void);
TErrorCode CompileIf(bool ElseIf);
TErrorCode CompileInput(void);
TErrorCode CompileIoterm(void);
TErrorCode CompileLcdcmd(void);
TErrorCode CompileLcdin(void);
TErrorCode CompileLcdout(void);
TErrorCode CompileLet(void);
TErrorCode CompileLookdown(void);
TErrorCode CompileLookup(void);
TErrorCode CompileLoop(void);
TErrorCode CompileLow(void);
TErrorCode CompileMainio(void);
TErrorCode CompileNap(void);
TErrorCode CompileNext(void);
TErrorCode CompileOn(void);
TErrorCode CompileOutput(void);
TErrorCode CompileOwin(void);
TErrorCode CompileOwout(void);
TErrorCode CompilePause(void);
TErrorCode CompilePollin(void);
TErrorCode CompilePollmode(void);
TErrorCode CompilePollout(void);
TErrorCode CompilePollrun(void);
TErrorCode CompilePollwait(void);
TErrorCode CompilePulsin(void);
TErrorCode CompilePulsout(void);
TErrorCode CompilePut(void);
TErrorCode CompilePwm(void);
TErrorCode CompileRandom(void);
TErrorCode CompileRctime(void);
TErrorCode CompileRead(void);
TErrorCode CompileReturn(void);
TErrorCode CompileReverse(void);
TErrorCode CompileRun(void);
TErrorCode CompileSelect(void);
TErrorCode GetTimeout(void);
TErrorCode CompileSerin(void);
TErrorCode CompileSerout(void);
TErrorCode CompileShiftin(void);
TErrorCode CompileShiftout(void);
TErrorCode CompileSleep(void);
TErrorCode CompileStop(void);
TErrorCode CompileStore(void);
TErrorCode CompileToggle(void);
TErrorCode CompileWrite(void);
TErrorCode CompileXout(void);

/*---Compiler Routines---(Supporting routines during the compilation process)-*/
TErrorCode GetValue(byte ExpNumber, bool PinIsConstant);
TErrorCode GetRead(byte ExpNumber);
TErrorCode GetWrite(byte ExpNumber);
TErrorCode GetValueEnterExpression(bool Enter1Before, bool PinIsConstant);
TErrorCode GetWriteEnterExpression(void);
TErrorCode EnterOperator(TOperatorCode Operator);
TErrorCode GetEnd(bool *Result);
TErrorCode GetLeft(void);
TErrorCode GetRight(void);
TErrorCode GetLeftBracket(void);
TErrorCode GetRightBracket(void);
TErrorCode GetEqual(void);
TErrorCode GetTO(void);
TErrorCode GetComma(void);
TErrorCode GetQuestion(void);
TErrorCode GetBackslash(void);
TErrorCode CopySymbol(void);
bool       CheckQuestion(void);
bool       CheckBackslash(void);
TErrorCode GetCommaOrBracket(bool *FoundBracket);
TErrorCode GetCommaOrEnd(bool *EndFound);
TErrorCode GetByteVariable(TElementList *Element);
TErrorCode GetUndefinedSymbol(void);
TErrorCode Enter0Code(TInstructionCode Code);
TErrorCode EnterChr(char Character);
TErrorCode EnterText(bool ASCFlag);
TErrorCode CompileInputSequence(void);
TErrorCode CompileOutputSequence(void);
TErrorCode GetAddressEnter(void);
TErrorCode EnterAddress(word Address);
TErrorCode EnterConstant(word Constant, bool Enter1Before);
TErrorCode CountGosubs(void);
int        Lowest(int Value1, int Value2);
TErrorCode NestingError(void);
TErrorCode CCNestingError(void);
TErrorCode Error(TErrorCode ErrorID);
bool       InBaseRange(char C, TBase Base);
/*---Object Engine---(Generates the EEPROM and PacketBuffer data for successful compilations)-*/
TErrorCode EnterEEPROM(byte Bits, word Data);
void       EnterSrcTokRef(void);
TErrorCode PatchAddress(word SourceAddress);
TErrorCode PatchSkipLabels(bool Exits);
TErrorCode PatchRemainingAddresses(void);
void       PreparePackets(void);


#ifdef WIN32
/*------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------*/
/*                 Start of Windows 32-bit DLL Entry Point Code                 */
/*------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------*/
CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()
/* Implementation of DLL Exports.
   Note: Proxy/Stub Information
         To build a separate proxy/stub DLL,
         run nmake -f tokenizerps.mk in the project directory.*/

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        _Module.Init(ObjectMap, hInstance, &LIBID_TOKENIZERLib);
        DisableThreadLibraryCalls(hInstance);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
        _Module.Term();
    return TRUE;    /* ok */
}

STDAPI DllCanUnloadNow(void)
{ /* Used to determine whether the DLL can be unloaded by OLE*/
  return (_Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{ /* Returns a class factory to create an object of the requested type */
  return _Module.GetClassObject(rclsid, riid, ppv);
}

STDAPI DllRegisterServer(void)
{  /* DllRegisterServer - Adds entries to the system registry */
   /* registers object, typelib and all interfaces in typelib */
   return _Module.RegisterServer(TRUE);
}

STDAPI DllUnregisterServer(void)
{  /* DllUnregisterServer - Removes entries from the system registry */
   return _Module.UnregisterServer(TRUE);
}

#endif /*WIN32*/

/*------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------*/
/*                  End of Windows 32-bit DLL Entry Point Code                  */
/*------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------*/

}; // class Tokenizer
