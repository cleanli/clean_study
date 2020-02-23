#include <Windows.h>
#include <iostream>

using namespace std;
 
DWORD WINAPI ThreadFuncFirst(LPVOID param)
{
 int iCount = 50;
 while(iCount--){
  cout<<"\nThreadFuncFirst:"<<iCount<<std::flush;
  Sleep(1000);
 }
 return 0;
}

DWORD WINAPI ThreadFuncSecond(LPVOID param)
{
 int iCount = 50;
 while(iCount--){
  cout<<"\nThreadFuncSecond:"<<iCount<<std::flush;
  Sleep(2000);
 }
 return 0;
}
int main()
{
 DWORD dwThreadID = 0;
 HANDLE handleFirst = CreateThread(NULL, 0, ThreadFuncFirst, 0, 0, &dwThreadID);
 if (!handleFirst)
 {
  cout<<"create thread 1 error:"<<endl;
 }
 HANDLE handleSecond = CreateThread(NULL, 0, ThreadFuncSecond, 0, 0, &dwThreadID);
 if (!handleSecond)
 {
   cout<<"create thread 2 error:"<<endl;
 }

//HANDLE arrayHandle[] = {handleFirst, handleSecond};
 //WaitForMultipleObjects(2, arrayHandle, TRUE, INFINITE);
 WaitForSingleObject(handleFirst, INFINITE);//等待线程返回，用sleep()就太山寨了
 WaitForSingleObject(handleSecond, INFINITE);
 CloseHandle(handleFirst);//句柄默认值2 这里减1，线程函数执行完后释放资源。
 CloseHandle(handleSecond);
 return 0;
}
