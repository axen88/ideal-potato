
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <process.h>
#include <math.h>



// Secound Thread function
void ThreadProc(void *param);
void Thread(PVOID pvoid) ;

int count;
int count2;
int n;

// First thread 
int main()
{
 int i;
 int val = 0;
 HANDLE handle;

 count = 0;
 count2 = 0;

 printf("\t Thread Demo\n");

 handle = (HANDLE) _beginthread( Thread,0,&val); 

 n=4;

 for(i=0;i<n;i++)
 {
  val = i+1;
  handle = (HANDLE) _beginthread( ThreadProc,0,&val); // create thread 
 }

 while(1){}

 return 0;
}


void ThreadProc(void *param)
{
 
 int h=count++;//*((int*)param);

 double d;
 long i,j;

 d = 0;
 printf("%d Thread is Running!\n",h);

 for(i=0; i< 1000*1000; i++)
 {
  for(j=0; j<36; j++)
  {
   d += sin(i*3.14159/180.0+j);
   d += cos(i*3.14159/180.0+j);
  }
 }

 printf("value %d = %.6f\n", h, d);

 count2++;

 _endthread();
}


void Thread(PVOID pvoid) 
{ 
 MSG msg; 
 int timerid = SetTimer(NULL,111,1000,NULL);
 int bRet; 
 int count =0; 

 PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE); 

 while( (bRet = GetMessage( &msg, NULL, 0, 0 )) != 0) 
 {   
  if (bRet == -1) 
  { 
   // handle the error and possibly exit 
  } 
  else if(msg.message==WM_TIMER) 
  { 

   printf("WM_TIMER in work thread count=%d\n",count2); 
   if(count2 == n) 
   break; 
  } 
  else 
  { 
   TranslateMessage(&msg);   
   DispatchMessage(&msg);   
  } 
 } 
 KillTimer(NULL,timerid); 
 printf("thread end here\n"); 

 exit(0);
}
