// This file was automatically generated from header file

#ifdef C2NIM
#  dynlib dll
#  cdecl
#  if defined(windows)
#    define dll "bin/autd3capi-link-remote-soem.dll"
#  elif defined(macosx)
#    define dll "bin/libautd3capi-link-remote-soem.dylib"
#  else
#    define dll "bin/libautd3capi-link-remote-soem.so"
#  endif
#endif

void AUTDLinkRemoteSOEM(void** out, char* ip, uint16 port);
