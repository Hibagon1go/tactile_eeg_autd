// This file was automatically generated from header file

#ifdef C2NIM
#  dynlib dll
#  cdecl
#  if defined(windows)
#    define dll "bin/autd3capi-link-soem.dll"
#  elif defined(macosx)
#    define dll "bin/libautd3capi-link-soem.dylib"
#  else
#    define dll "bin/libautd3capi-link-soem.so"
#  endif
#endif

int32 AUTDGetAdapterPointer(void** out);
void AUTDGetAdapter(void* p_adapter, int32 index, char* desc, char* name);
void AUTDFreeAdapterPointer(void* p_adapter);
void AUTDLinkSOEM(void** out, char* ifname, uint16 sync0_cycle, uint16 send_cycle, bool freerun, void* on_lost, bool high_precision, uint64 state_check_interval);
void AUTDLinkSOEMSetLogLevel(int32 level);
void AUTDLinkSOEMSetDefaultLogger(void* out, void* flush);
