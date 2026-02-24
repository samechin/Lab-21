#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define ID_BTN_PLUS  1001
#define ID_BTN_MINUS 1002
#define ID_BTN_MUL   1003
#define ID_BTN_DIV   1004
#define ID_EDIT_1    2001
#define ID_EDIT_2    2002

HWND hEdit1, hEdit2;

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		case WM_CREATE: {
			CreateWindow("STATIC", " Please input two numbers", 
				WS_VISIBLE | WS_CHILD | SS_LEFT, 
				20, 20, 200, 25, hwnd, NULL, NULL, NULL);

			hEdit1 = CreateWindow("EDIT", "", 
				WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 
				40, 55, 160, 25, hwnd, (HMENU)ID_EDIT_1, NULL, NULL);

			hEdit2 = CreateWindow("EDIT", "", 
				WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 
				40, 90, 160, 25, hwnd, (HMENU)ID_EDIT_2, NULL, NULL);

			CreateWindow("BUTTON", "+", WS_VISIBLE | WS_CHILD, 65, 130, 25, 25, hwnd, (HMENU)ID_BTN_PLUS, NULL, NULL);
			CreateWindow("BUTTON", "-", WS_VISIBLE | WS_CHILD, 95, 130, 25, 25, hwnd, (HMENU)ID_BTN_MINUS, NULL, NULL);
			CreateWindow("BUTTON", "*", WS_VISIBLE | WS_CHILD, 125, 130, 25, 25, hwnd, (HMENU)ID_BTN_MUL, NULL, NULL);
			CreateWindow("BUTTON", "/", WS_VISIBLE | WS_CHILD, 155, 130, 25, 25, hwnd, (HMENU)ID_BTN_DIV, NULL, NULL);
			break;
		}

		case WM_COMMAND: {
			if (LOWORD(wParam) >= 1001 && LOWORD(wParam) <= 1004) {
				char buf1[256], buf2[256], resStr[256];
				GetWindowText(hEdit1, buf1, 256);
				GetWindowText(hEdit2, buf2, 256);
				
				double n1 = atof(buf1);
				double n2 = atof(buf2);
				double res = 0;
				bool valid = true;

				switch(LOWORD(wParam)) {
					case ID_BTN_PLUS:  res = n1 + n2; break;
					case ID_BTN_MINUS: res = n1 - n2; break;
					case ID_BTN_MUL:   res = n1 * n2; break;
					case ID_BTN_DIV:   
						if(n2 != 0) res = n1 / n2; 
						else { MessageBox(hwnd, "Divide by Zero!", "Error", MB_OK); valid = false; }
						break;
				}

				if(valid) {
					sprintf(resStr, "%f", res);
					MessageBox(hwnd, resStr, "Result", MB_OK);
				}
			}
			break;
		}

		case WM_CTLCOLORSTATIC: {
			HDC hdcStatic = (HDC)wParam;
			SetBkColor(hdcStatic, RGB(200, 20, 100));
			return (INT_PTR)CreateSolidBrush(RGB(200, 20, 100));
		}

		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc;
	HWND hwnd;
	MSG msg;

	memset(&wc,0,sizeof(wc));
	wc.cbSize	 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc;
	wc.hInstance	 = hInstance;
	wc.hCursor	 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = CreateSolidBrush(RGB(200, 20, 100)); 
	wc.lpszClassName = "WindowClass";
	wc.hIcon	 = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm	 = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(0, "WindowClass", "My Calculator", 
		WS_VISIBLE | WS_SYSMENU | WS_CAPTION,
		CW_USEDEFAULT, CW_USEDEFAULT, 250, 200, 
		NULL, NULL, hInstance, NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	while(GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}