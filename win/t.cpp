#include <Windows.h>
#include <WindowsX.h>
#include <stdio.h>

#define IDR_CONTEXT  200
#define IDM_OPT1     301
#define IDM_OPT2     302


#define IDB_ONE     3301
#define IDB_TWO     3302
#define IDB_THREE   3303
#define ID_DATA     3304

#define IDC_RADBTN1		50001
#define IDC_RADBTN2		50002
#define IDC_RADBTN3		50003
#define IDC_RADBTNBLUE		51001
#define IDC_RADBTNRED		51002
#define IDC_RADBTNGREEN		51003

HMENU hRoot;
void CreateMyMenu();//create menu
int timer_count=0;
HANDLE hTimer = NULL;
HANDLE hTimerQueue = NULL;
char strbuf[128];
HINSTANCE hg_app;
HWND editHd;
HWND rb1Hd;

LRESULT CALLBACK WindowProc(
        HWND hwnd,
        UINT uMsg,
        WPARAM wParam,
        LPARAM lParam
        );

VOID CALLBACK TimerRoutine(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
    HWND hwnd;
    timer_count++;
    if (lpParam != NULL)
    {
        HWND hwnd=(HWND)lpParam;
        InvalidateRect(hwnd,NULL,TRUE);
    }
}

//entry of program
int CALLBACK WinMain(
        HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nCmdShow
        )
{
    CreateMyMenu();
    // class name
    const char* cls_Name = "My Class";
    // design windows class
    WNDCLASS wc = { };
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpfnWndProc = WindowProc;
    wc.lpszClassName = cls_Name;
    wc.hInstance = hInstance;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    // register windows class
    RegisterClass(&wc);

    hg_app = hInstance;
    // create windows
    HWND hwnd = CreateWindow(
            cls_Name,           //class name, same with registered
            "My window",  //title of windows
            WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX& ~WS_THICKFRAME,
            38,                 //x cordinate in father windows
            20,                 //y cordinate in father windows
            680,                // width of windos
            650,                // height of windows
            NULL,               //no father windows, is NULL
            hRoot,               //menu
            hInstance,          //handle of current windows
            NULL);              //no attached data, NULL
    if(hwnd == NULL) // check if successfully created windows
        return 0;

    ShowWindow(hwnd, SW_SHOW);

    UpdateWindow(hwnd);

    //message loop
    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
LRESULT CALLBACK WindowProc(
        HWND hwnd,
        UINT uMsg,
        WPARAM wParam,
        LPARAM lParam
        )
{
    HBITMAP hBitmap;
    static HDC s_hdcMem;

    switch(uMsg)
    {
        case WM_CREATE:
            {
                //create three button
                CreateWindow("Button", "B1", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                        35, 10, 160, 60, hwnd, (HMENU)IDB_ONE, hg_app, NULL);
                CreateWindow("Button", "B2", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                        35, 80, 160, 60, hwnd, (HMENU)IDB_TWO, hg_app, NULL);
                CreateWindow("Button", "B3", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                        35, 150, 160, 60, hwnd, (HMENU)IDB_THREE, hg_app, NULL);
                //CreateWindow(TEXT("edit"),TEXT("myedit"),WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_BORDER|ES_LEFT|ES_MULTILINE|ES_AUTOVSCROLL,
                editHd = CreateWindow(TEXT("edit"),TEXT("myedit"),WS_CHILD|WS_VISIBLE|WS_BORDER|ES_LEFT,
                        35, 250, 380, 60, hwnd,(HMENU)ID_DATA, hg_app,NULL);
                // y cordinate, base
                int yLoc = 0;
                // text
                yLoc += 10;
                CreateWindow("Static","Your sex is:",
                        SS_SIMPLE | WS_CHILD | WS_VISIBLE,
                        280,yLoc,160,18,
                        hwnd, NULL,
                        hg_app,
                        NULL);
                // group 1
                yLoc += 20;
                rb1Hd = CreateWindow("Button","male",
                        WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,
                        282, yLoc, 60, 16,
                        hwnd,
                        (HMENU)IDC_RADBTN1,
                        hg_app,NULL);
                yLoc += 20;
                CreateWindow("Button","female",
                        WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
                        282,yLoc, 60, 16,
                        hwnd,(HMENU)IDC_RADBTN2,hg_app,NULL);
                yLoc += 20;
                CreateWindow("Button","renyao",WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
                        282,yLoc,60,16,hwnd,(HMENU)IDC_RADBTN3,hg_app,NULL);
                // text
                yLoc += 38;
                CreateWindow("Static","What color do you like",
                        WS_CHILD | WS_VISIBLE | SS_SIMPLE,
                        280,yLoc,150,18,hwnd,NULL,hg_app,NULL);
                // group 2
                yLoc += 22;
                CreateWindow("Button","blue",WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,
                        282,yLoc,60,16,hwnd,(HMENU)IDC_RADBTNBLUE,hg_app,NULL);
                yLoc += 20;
                CreateWindow("Button","red",WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
                        282,yLoc,60,16,hwnd,(HMENU)IDC_RADBTNRED,hg_app,NULL);
                yLoc += 20;
                CreateWindow("Button","green",WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
                        282,yLoc,60,16,hwnd,(HMENU)IDC_RADBTNGREEN,hg_app,NULL);
            }

            memset(strbuf, 0, 128);
            SetWindowText(hwnd, "changed");
            // set dlg size changable
            //SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) | WS_SIZEBOX);
            // load image
            hBitmap = (HBITMAP)LoadImage(NULL, "background.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
            if (hBitmap == NULL)
            {
                MessageBox(hwnd, "LoadImage failed", "Error", MB_ICONERROR);
            }
            else
            {
                // put image to HDC - s_hdcMem
                HDC hdc;
                hdc = GetDC(hwnd);
                s_hdcMem = CreateCompatibleDC(hdc);
                SelectObject(s_hdcMem, hBitmap);
                ReleaseDC(hwnd, hdc);
            }
            //timer init
            // Create the timer queue.
            hTimerQueue = CreateTimerQueue();
            if (NULL == hTimerQueue)
            {
                MessageBox(hwnd, "CreateTimerQueue failed", "Error", MB_ICONERROR);
                break;
            }
            // Set a timer to call the timer routine in 10 seconds.
            if (!CreateTimerQueueTimer( &hTimer, hTimerQueue,
                        (WAITORTIMERCALLBACK)TimerRoutine, hwnd , 2000, 1000, 0))
            {
                MessageBox(hwnd, "CreateTimerQueueTimer failed", "Error", MB_ICONERROR);
                break;
            }
            break;
        case WM_CONTEXTMENU:
            {
                //load menu rc
                //HMENU hroot = LoadMenu((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), MAKEINTRESOURCE(IDR_CONTEXT));
                HMENU hroot = hRoot;
                if(hroot)
                {
                    // get first pop menu
                    HMENU hpop = GetSubMenu(hroot,0);
                    // get cordinate of mouse
                    int px = GET_X_LPARAM(lParam);
                    int py = GET_Y_LPARAM(lParam);
                    // display pop menu
                    TrackPopupMenu(hpop,
                            TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RIGHTBUTTON,
                            px,
                            py,
                            0,
                            (HWND)wParam,
                            NULL);
                    // destroy after use
                    //DestroyMenu(hroot);
                }
            }
            break;
        case WM_COMMAND:
            {
                // get Id of menu and check which menu user selected
                switch(LOWORD(wParam))
                {
                    case IDM_OPT1:
                        MessageBox(hwnd,"plane coming","Notice",MB_OK);
                        break;
                    case IDM_OPT2:
                        MessageBox(hwnd,"mt gun coming","notice",MB_OK);
                        break;
                    case IDB_ONE:
                        //MessageBox(hwnd, "you clicked first", "Notice", MB_OK | MB_ICONINFORMATION);
                        char szBuf[1000];
                        GetWindowText(editHd, szBuf, 1000);
                        if(Button_GetCheck(rb1Hd)){
                            sprintf(szBuf, "Male choosed");
                        }
                        MessageBox(hwnd, szBuf, "Notice", MB_OK | MB_ICONINFORMATION);
                        //SendMessage((HWND)lParam, WM_SETTEXT, (WPARAM)NULL, (LPARAM)"first clicked");
                        break;
                    case IDB_TWO:
                        //MessageBox(hwnd, "your clicked two", "Notice", MB_OK | MB_ICONINFORMATION);
                        SendMessage((HWND)lParam, WM_SETTEXT, (WPARAM)NULL, (LPARAM)"second clicked");
                        break;
                    case IDB_THREE:
                        //MessageBox(hwnd, "you clicked tree", "notice", MB_OK | MB_ICONINFORMATION);
                        SendMessage((HWND)lParam, WM_SETTEXT, (WPARAM)NULL, (LPARAM)"third clicked");
                        break;
                    default:
                        break;
                }
            }
            break;
        case WM_PAINT:
            {
                HDC hdc;
                RECT rt;
                PAINTSTRUCT ps;
                hdc = BeginPaint(hwnd, &ps);

                //image of background
                GetClientRect(hwnd, &rt);
                BitBlt(hdc, 0, 0, rt.right, rt.bottom, s_hdcMem, 0, 0, SRCCOPY);

                memset(strbuf, 0, 128);
                sprintf(strbuf, "%02d", timer_count);
                SetTextColor(ps.hdc, RGB(10, 0, 255));
                DrawText(ps.hdc, "hello friends",strlen("hello friends"), &(ps.rcPaint), DT_CENTER);
                int arr1[2]= {45,0};
                int arr2[3] = { 35, 40, 0 };
                int arr3[3] = { 9, 10, 0 };
                POLYTEXT polys[] =  { {2,25,3,"AL",ETO_CLIPPED,ps.rcPaint,&arr1[0]},
                    {2,65,3,"hap",ETO_CLIPPED,ps.rcPaint,&arr2[0]},
                    {2,90,3,strbuf,ETO_CLIPPED,ps.rcPaint,&arr3[0]}
                };
                PolyTextOut(ps.hdc, &polys[0],3);
                HBRUSH hb = CreateSolidBrush(RGB(0,255,0));
                HBRUSH orgBrs = (HBRUSH)SelectObject(ps.hdc, hb);
                Ellipse(ps.hdc,135,35,202,170);
                SelectObject(ps.hdc, orgBrs);
                DeleteObject(hb);
                EndPaint(hwnd, &ps);
            }
            break;
        case WM_DESTROY:
            // Delete all timers in the timer queue.
            if (!DeleteTimerQueue(hTimerQueue))
                printf("DeleteTimerQueue failed (%d)\n", GetLastError());
            {
                PostQuitMessage(0);
                return 0;
            }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void CreateMyMenu()
{
    hRoot = CreateMenu();
    if(!hRoot)
        return;
    HMENU pop1 = CreatePopupMenu();
    AppendMenu(hRoot,
            MF_POPUP,
            (UINT_PTR)pop1,
            "Opera");
    //one way is using AppendMenu
    AppendMenu(pop1,
            MF_STRING,
            IDM_OPT1,
            "Plane");

    //another way is using InsertMenuItem
    MENUITEMINFO mif;
    mif.cbSize = sizeof(MENUITEMINFO);
    mif.cch = 100;
    mif.dwItemData  = NULL;
    mif.dwTypeData = "MT_GUN";
    mif.fMask = MIIM_ID | MIIM_STRING | MIIM_STATE;
    mif.fState = MFS_ENABLED;
    mif.fType = MIIM_STRING;
    mif.wID = IDM_OPT2;

    InsertMenuItem(pop1,IDM_OPT2,FALSE,&mif);
}
