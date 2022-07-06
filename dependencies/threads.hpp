#pragma once

#include <string>
#include <processthreadsapi.h>

typedef struct{
      void* data;
      void(*procfunc)(void*);

      procThreadStruct(void* _data, void (*_procfunc)(void*)) {
            data = _data;
            procfunc = _procfunc;
      }

      void func() { this->procfunc(data); }
} procThreadStruct;

DWORD WINAPI procThread(LPVOID lpParam){
      procThreadStruct* procObj = (procThreadStruct*)lpParam;
      procObj->func();
      return 0;
}

template<typename T>
class Thread {
      private:
            void* thread;
      public:
            Thread() {};

            Thread(void (*_procfunc)(void*), void* data) {
                  procThreadStruct procSt = { (void*)data, _procfunc };
                  procThreadStruct *pointerProcSt = new procThreadStruct(procSt);

                  thread = CreateThread(NULL, 0, procThread, (void*)pointerProcSt, 0, 0);
            }

            ~Thread() {
                  deleteThread();
            }

            void createThread(void (*_procfunc)(void*), void* data) {
                  procThreadStruct procSt = { (void*)data, _procfunc };
                  procThreadStruct *pointerProcSt = new procThreadStruct(procSt);

                  thread = CreateThread(NULL, 0, procThread, (void*)pointerProcSt, 0, 0);
            }

            void deleteThread() {
                  TerminateThread(thread, 0);
            }
};