/* IOManager.h
 *
 * Non-blocking / asynchronous I/O for Win32.
 *
 * (c) sof, 2002-2003
 */
#ifndef __IOMANAGER_H__
#define __IOMANAGER_H__
/* On the yucky side..suppress -Wmissing-declarations warnings when
 * including <windows.h>
 */
extern void* GetCurrentFiber ( void );
extern void* GetFiberData ( void );
#include <windows.h>

/*
 The IOManager subsystem provides a non-blocking view
 of I/O operations. It lets one (or more) OS thread(s)
 issue multiple I/O requests, which the IOManager then 
 handles independently of/concurrent to the thread(s)
 that issued the request. Upon completion, the issuing
 thread can inspect the result of the I/O operation &
 take appropriate action.

 The IOManager is intended used with the GHC RTS to
 implement non-blocking I/O in Concurrent Haskell.
 */

/*
 * Our WorkQueue holds WorkItems, encoding IO and
 * delay requests.
 *
 */
typedef void (*CompletionProc)(unsigned int requestID,
			       void* param,
			       int   fd,
			       int   len,
			       char* buf,
			       int   errCode);

typedef struct WorkItem {
  unsigned int   workKind;
  int            fd;
  int            len;
  char*          buf;
  void*          param;
  unsigned int   requestID;
  CompletionProc onCompletion;
} WorkItem;

extern CompletionProc onComplete;

/* the kind of operations supported; you could easily imagine
 * that instead of passing a tag describing the work to be performed,
 * a function pointer is passed instead. Maybe later.
 */
#define WORKER_READ       1
#define WORKER_WRITE      2
#define WORKER_DELAY      4
#define WORKER_FOR_SOCKET 8

/*
 * Starting up and shutting down. 
 */ 
extern BOOL StartIOManager     ( void );
extern void ShutdownIOManager  ( void );

/*
 * Adding I/O and delay requests. With each request a
 * completion routine is supplied, which the worker thread
 * will invoke upon completion.
 */
extern int AddDelayRequest ( unsigned int   msecs,
			     void*          data,
			     CompletionProc onCompletion);

extern int AddIORequest ( int            fd,
			  BOOL           forWriting,
			  BOOL           isSocket,
			  int            len,
			  char*          buffer,
			  void*          data,
			  CompletionProc onCompletion);

#endif /* __IOMANAGER_H__ */
