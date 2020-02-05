#include <Windows.h>
#include "resource.h"
#include <WindowsX.h>

#define IDR_CONTEXT  200
#define IDM_OPT1     301
#define IDM_OPT2     302

HMENU hRoot;
void CreateMyMenu();//create menu

LRESULT CALLBACK WindowProc(
      HWND hwnd,
      UINT uMsg,
      WPARAM wParam,
      LPARAM lParam
);

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

    // create windows
    HWND hwnd = CreateWindow(
        cls_Name,           //class name, same with registered
        "My window",  //title of windows
        WS_OVERLAPPEDWINDOW, //style of windows
        38,                 //x cordinate in father windows
        20,                 //y cordinate in father windows
        480,                // width of windos
        250,                // height of windows
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
    switch(uMsg)
    {
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
			default:
				break;
			}
		}
        break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint(hwnd, &ps);
            SetTextColor(ps.hdc, RGB(10, 0, 255));
			DrawText(ps.hdc, "hello friends",strlen("hello friends"), &(ps.rcPaint), DT_CENTER);
            int arr1[2]= {45,0};
			int arr2[3] = { 35, 40, 0 };
			int arr3[2] = { 32, 0 };
			POLYTEXT polys[] =  { {2,25,3,"AL",ETO_CLIPPED,ps.rcPaint,&arr1[0]},
				{2,65,3,"hap",ETO_CLIPPED,ps.rcPaint,&arr2[0]},
				{20,90,3,"hap",ETO_CLIPPED,ps.rcPaint,&arr3[0]}
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
