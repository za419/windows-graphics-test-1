// Project1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Project1.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECT1));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            //TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

float scale(1);

class GameObject {
public:
	size_t size;
	COLORREF color;
	int xcenter;
	int ycenter;

	int8_t xvelocity = 0;
	int8_t yvelocity = 0;

	float rotation;

	GameObject() : size(0),
		color(RGB(0, 0, 0)),
		xcenter(size / 2),
		ycenter(size / 2),
		rotation(0) {}

	GameObject(size_t s) : size(s),
		color(RGB(0, 0, 0)),
		xcenter(size / 2),
		ycenter(size / 2) {}

	GameObject(size_t s, COLORREF c) : size(s),
		color(c),
		xcenter(size / 2),
		ycenter(size / 2),
		rotation(0) {}

	GameObject(size_t s, COLORREF c, int x, int y) :size(s),
		color(c),
		xcenter(x),
		ycenter(y),
		rotation(0) {}

	GameObject(size_t s, COLORREF c, int x, int y, float r) :size(s),
		color(c),
		xcenter(x),
		ycenter(y),
		rotation(r) {}

	virtual ~GameObject() {}

	void beginDrawTo(HDC hdc) {
		doTrig();

		setScale(hdc);
		setRotation(hdc);
	}

	virtual void drawTo(HDC hdc) = 0;

	void endDrawTo(HDC hdc) {
		clearRotation(hdc);
		clearScale(hdc);
	}

	virtual void tick(HWND hWnd) {
		RECT window;
		GetWindowRect(hWnd, &window);
		auto t = window.top;
		auto l = window.left;
		window.left -= l;
		window.right -= l;
		window.top -= t;
		window.bottom -= t;
		InvalidateRect(hWnd, &window, true);

		auto width = window.right - window.left;
		auto height = window.bottom - window.top;

		xcenter += xvelocity*cosr;
		xcenter -= yvelocity*sinr;

		ycenter += xvelocity*sinr;
		ycenter += yvelocity*cosr;

		int halfsize = size / 2;

		if (xcenter < -halfsize) {
			xcenter = width + halfsize;
		}
		else if (xcenter > width + halfsize) {
			xcenter = -halfsize;
		}

		if (ycenter < -halfsize) {
			ycenter = height + halfsize;
		}
		else if (ycenter > width + halfsize) {
			ycenter = -halfsize;
		}
	}

	const float cos() {
		return cosr;
	}

	const float sin() {
		return sinr;
	}
protected:
	float cosr;
	float sinr;

	void doTrig() {
		cosr = ::cos(rotation);
		sinr = ::sin(rotation);
	}

	void setScale(HDC hdc) {
		XFORM transform{
			scale,
			0,
			0,
			scale,
			-(xcenter*(scale - 1)),
			-(ycenter*(scale - 1))
		};

		ModifyWorldTransform(hdc, &transform, MWT_RIGHTMULTIPLY);
	}

	void clearScale(HDC hdc) {
		float sc(scale);
		scale = 1 / scale;
		setScale(hdc);
		scale = sc;
	}

	void setRotation(HDC hdc) {
		XFORM transform{
			cosr,
			sinr,
			-sinr,
			cosr,
			xcenter - (xcenter*cosr - ycenter*sinr),
			ycenter - (xcenter*sinr + ycenter*cosr)
		};

		ModifyWorldTransform(hdc, &transform, MWT_RIGHTMULTIPLY);
	}

	void clearRotation(HDC hdc) {
		rotation = -rotation;
		doTrig();
		setRotation(hdc);
		rotation = -rotation;
	}
};

class Square : public GameObject {
public:
	Square() {}

	Square(size_t s) : GameObject(s) {}

	Square(size_t s, COLORREF c) : GameObject(s, c) {}

	Square(size_t s, COLORREF c, int x, int y) : GameObject(s, c, x, y) {}

	Square(size_t s, COLORREF c, int x, int y, float r) : GameObject(s, c, x, y, r) {}

	virtual void drawTo(HDC hdc) override {
		LONG halfsize = size / 2;
		RECT rect{ xcenter - halfsize, ycenter - halfsize, xcenter + halfsize, ycenter + halfsize };
		auto brush(CreateSolidBrush(color));
		FillRect(hdc, &rect, brush);
		DeleteObject(brush);
	}

	const float cos() {
		return cosr;
	}

	const float sin() {
		return sinr;
	}
protected:
	float cosr;
	float sinr;

	void doTrig() {
		cosr = ::cos(rotation);
		sinr = ::sin(rotation);
	}

	void setScale(HDC hdc) {
		XFORM transform{
			scale,
			0,
			0,
			scale,
			-(xcenter*(scale-1)),
			-(ycenter*(scale-1))
		};

		ModifyWorldTransform(hdc, &transform, MWT_RIGHTMULTIPLY);
	}

	void clearScale(HDC hdc) {
		float sc(scale);
		scale = 1 / scale;
		setScale(hdc);
		scale = sc;
	}

	void setRotation(HDC hdc) {
		XFORM transform{
			cosr,
			sinr,
			-sinr,
			cosr,
			xcenter-(xcenter*cosr-ycenter*sinr),
			ycenter-(xcenter*sinr+ycenter*cosr)
		};

		ModifyWorldTransform(hdc, &transform, MWT_RIGHTMULTIPLY);
	}

	void clearRotation(HDC hdc) {
		rotation = -rotation;
		doTrig();
		setRotation(hdc);
		rotation = -rotation;
	}
};

Square player(100, RGB(255, 0, 0), 850, 250);
Square test(50, RGB(0, 0, 255), 0, 0);

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
			auto next = std::chrono::steady_clock::now();
			std::chrono::milliseconds d(16);
			next += d;

            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			SetGraphicsMode(hdc, GM_ADVANCED);
			player.drawTo(hdc);
			test.drawTo(hdc);
            EndPaint(hWnd, &ps);

			player.tick(hWnd);

			auto diff = std::chrono::duration_cast<std::chrono::milliseconds> (next - std::chrono::steady_clock::now());
			Sleep((DWORD)diff.count());
        }
        break;
	case WM_KEYDOWN:
		{
			const static size_t acceleration(1);
			const static float rotational(3.14f/32);
			const static size_t scaleDiff(1);
			const static float scaleFactor(0.03125); // 1/32

			auto cosr = player.cos();
			auto sinr = player.sin();

			switch (LOWORD(wParam))
			{
			case 0x57: // W
				player.yvelocity -= acceleration;
				break;
			case 0x41: // A
				player.xvelocity -= acceleration;
				break;
			case 0x53: // S
				player.yvelocity += acceleration;
				break;
			case 0x44: // D
				player.xvelocity += acceleration;
				break;
			case 0x51: // Q
				player.size += scaleDiff;
				break;
			case 0x45: // E
				player.size -= scaleDiff;
				break;
			case VK_UP:
				scale += scaleFactor;
				break;
			case VK_DOWN:
				scale -= scaleFactor;
				break;
			case VK_LEFT:
				player.rotation -= rotational;
				break;
			case VK_RIGHT:
				player.rotation += rotational;
				break;
			case VK_TAB:
				player.size = 50;
				break;
			case VK_OEM_5: // For US standard, \ and |
				scale=1;
				break;
			case VK_RETURN:
				player.rotation = 0;
				break;
			case VK_SPACE:
				player.xvelocity = player.yvelocity = 0;
				break;
			}
		}
		break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
