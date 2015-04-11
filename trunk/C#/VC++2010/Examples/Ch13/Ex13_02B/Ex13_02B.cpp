// Ex13_02B.cpp : Defines the entry point for the application.
// Computing the Mandelbrot set with parallel_invoke

#include "stdafx.h"
#include "Ex13_02B.h"
#include "ppl.h"
#include <functional>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								        // current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];		// the main window class name
const size_t MaxIterations(8000);       // Maximum iterations before infinity

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
size_t IteratePoint(double zReal, double zImaginary, double cReal, double cImaginary);
COLORREF Color(int n);                  // Selects a pixel color based on n
void DrawSet(HWND hWnd);                // Draws the Mandelbrot set
void DrawSetParallel(HWND hWnd);        // Draws the Mandelbrot set with parallel ops

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_EX13_02B, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EX13_02B));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
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
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EX13_02B));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_EX13_02B);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
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
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      100, 100, 800, 600, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
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
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
      DrawSetParallel(hWnd);
		EndPaint(hWnd, &ps);
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

// Iterates current point to determine membership of Mandelbrot set
size_t IteratePoint(double zReal, double zImaginary, double cReal, double cImaginary)
{
  double zReal2(0.0), zImaginary2(0.0); // z components squared
  size_t n(0);
  for( ; n < MaxIterations ; ++n)       // Iterate equation Zn = Zn-1**2 + K
  {                                     // for Mandelbrot K = c and Z0 = c 
    zReal2 = zReal*zReal;
    zImaginary2 = zImaginary*zImaginary;
    if(zReal2 + zImaginary2 > 4)        // If distance from origin > 2, point will
      break;                            // go to infinity so we are done
    // Calculate next Z value
    zImaginary = 2*zReal*zImaginary + cImaginary;
    zReal = zReal2 - zImaginary2 + cReal;
  }
  return n;
}

// Select a color based on the value of n
COLORREF Color(int n)
{
  if(n == MaxIterations) return RGB(0, 0, 0);

  const int nColors = 16;
  switch(n%nColors)
  {
    case 0: return RGB(100,100,100);   case 1: return RGB(100,0,0);
    case 2: return RGB(200,0,0);       case 3: return RGB(100,100,0);
    case 4: return RGB(200,100,0);     case 5: return RGB(200,200,0);
    case 6: return RGB(0,200,0);       case 7: return RGB(0,100,100);
    case 8: return RGB(0,200,100);     case 9: return RGB(0,100,200);
    case 10: return RGB(0,200,200);    case 11: return RGB(0,0,200);
    case 12: return RGB(100,0,100);    case 13: return RGB(200,0,100);
    case 14: return RGB(100,0,200);    case 15: return RGB(200,0,200);
    default: return RGB(200,200,200);
  };
}

// Draws the Mandelbrot set
void DrawSet(HWND hWnd)
{
  // Get client area dimensions, which will be the image size
  RECT rect;
  GetClientRect(hWnd, &rect);
  int imageHeight(rect.bottom);
  int imageWidth(rect.right);

  // Create bitmap for one row of pixels in image
  HDC hdc(GetDC(hWnd));                  // Get device context
  HDC memDC = CreateCompatibleDC(hdc);   // Get device context to draw pixels
  HBITMAP bmp = CreateCompatibleBitmap(hdc, imageWidth, 1);
  HGDIOBJ oldBmp = SelectObject(memDC, bmp);   // Selct bitmap into DC

  // Client area axes
  const double realMin(-2.1);       // Minimum real value
  double imaginaryMin(-1.3);        // Minimum imaginary value
  double imaginaryMax(+1.3);        // Maximum imaginary value

  // Set maximum imaginary so axes are the same scale
  double realMax(realMin+(imaginaryMax-imaginaryMin)*imageWidth/imageHeight);

  // Get scale factors to convert pixel coordinates
  double realScale((realMax-realMin)/(imageWidth-1)); 
  double imaginaryScale((imaginaryMax-imaginaryMin)/(imageHeight-1));

  double cReal(0.0), cImaginary(0.0);   // Stores c components
  double zReal(0.0), zImaginary(0.0);   // Stores z components

  for(int y = 0 ; y < imageHeight ; ++y)             // Iterate over image rows
  {
    zImaginary = cImaginary = imaginaryMax - y*imaginaryScale;
    for(int x = 0 ; x < imageWidth ; ++x)            // Iterate over pixels in a row
    {
      zReal = cReal = realMin + x*realScale;
      // Set current pixel color based on n
      SetPixel(memDC, x, 0, Color(IteratePoint(zReal, zImaginary, cReal, cImaginary)));
    }
    // Transfer pixel row to client area device context
    BitBlt(hdc, 0, y, imageWidth, 1, memDC, 0, 0, SRCCOPY);
  }
  SelectObject(memDC, oldBmp);
  DeleteObject(bmp);                       // Delete bitmap
  DeleteDC(memDC);                         // and our working DC
  ReleaseDC(hWnd, hdc);                    // Release client area DC
}

// Draws the Mandelbrot set
void DrawSetParallel(HWND hWnd)
{
  HDC hdc(GetDC(hWnd));                  // Get device context

  // Get client area dimensions, which will be the image size
  RECT rect;
  GetClientRect(hWnd, &rect);
  int imageHeight(rect.bottom);
  int imageWidth(rect.right);

  // Create bitmap for one row of pixels in image
  HDC memDC1 = CreateCompatibleDC(hdc);   // Get device context to draw pixels
  HDC memDC2 = CreateCompatibleDC(hdc);   // Get device context to draw pixels
  HBITMAP bmp1 = CreateCompatibleBitmap(hdc, imageWidth, 1);
  HBITMAP bmp2 = CreateCompatibleBitmap(hdc, imageWidth, 1);
  HGDIOBJ oldBmp1 = SelectObject(memDC1, bmp1);   // Select bitmap into DC1
  HGDIOBJ oldBmp2 = SelectObject(memDC2, bmp2);   // Select bitmap into DC2

  // Client area axes
  const double realMin(-2.1);       // Minimum real value
  double imaginaryMin(-1.3);        // Minimum imaginary value
  double imaginaryMax(+1.3);        // Maximum imaginary value

  // Set maximum real so axes are the same scale
  double realMax(realMin+(imaginaryMax-imaginaryMin)*imageWidth/imageHeight);

  // Get scale factors to convert pixel coordinates
  double realScale((realMax-realMin)/(imageWidth-1)); 
  double imaginaryScale((imaginaryMax-imaginaryMin)/(imageHeight-1));

    // Lambda expression to create an image row
    std::function<void(HDC&, int)> rowCalc = [&] (HDC& memDC, int yLocal) 
    {
      double zReal(0.0), cReal(0.0);
      double zImaginary(imaginaryMax - yLocal*imaginaryScale);
      double cImaginary(zImaginary);
      for(int x = 0 ; x < imageWidth ; ++x)            // Iterate over pixels in a row
      {
        zReal = cReal = realMin + x*realScale;
        // Set current pixel color based on n
        SetPixel(memDC, x, 0, Color(IteratePoint(zReal, zImaginary, cReal, cImaginary)));
      }
    };

  for(int y = 1 ; y < imageHeight ; y += 2)             // Iterate over image rows
  {
    Concurrency::parallel_invoke([&]{rowCalc(memDC1, y-1);}, [&]{rowCalc(memDC2, y);});
    // Transfer pixel rows to client area device context
    BitBlt(hdc, 0, y-1, imageWidth, 1, memDC1, 0, 0, SRCCOPY);
    BitBlt(hdc, 0, y, imageWidth, 1, memDC2, 0, 0, SRCCOPY);
  }
  // If there's an odd number of rows, take care of the last one
  if(imageHeight%2 == 1)
  {
    rowCalc(memDC1, imageWidth-1);
    BitBlt(hdc, 0, imageHeight-1, imageWidth, 1, memDC1, 0, 0, SRCCOPY);
  }

  SelectObject(memDC1, oldBmp1);
  SelectObject(memDC2, oldBmp2);
  DeleteObject(bmp1);                       // Delete bitmap 1
  DeleteObject(bmp2);                       // Delete bitmap 2
  DeleteDC(memDC1);                         // and our working DC 1
  DeleteDC(memDC2);                         // and our working DC 2
  ReleaseDC(hWnd, hdc);                     // Release client area DC
}

// The folowing is a version you can use instead of the function above
// If you have a quad core processor
/*
void DrawSetParallel(HWND hWnd)
{
  HDC hdc(GetDC(hWnd));                  // Get device context

  // Get client area dimensions, which will be the image size
  RECT rect;
  GetClientRect(hWnd, &rect);
  int imageHeight(rect.bottom);
  int imageWidth(rect.right);

  // Create bitmap for one row of pixels in image
  HDC memDC1 = CreateCompatibleDC(hdc);   // Get device context to draw pixels
  HDC memDC2 = CreateCompatibleDC(hdc);   // Get device context to draw pixels
  HDC memDC3 = CreateCompatibleDC(hdc);   // Get device context to draw pixels
  HDC memDC4 = CreateCompatibleDC(hdc);   // Get device context to draw pixels
  HBITMAP bmp1 = CreateCompatibleBitmap(hdc, imageWidth, 1);
  HBITMAP bmp2 = CreateCompatibleBitmap(hdc, imageWidth, 1);
  HBITMAP bmp3 = CreateCompatibleBitmap(hdc, imageWidth, 1);
  HBITMAP bmp4 = CreateCompatibleBitmap(hdc, imageWidth, 1);
  HGDIOBJ oldBmp1 = SelectObject(memDC1, bmp1);   // Select bitmap into DC1
  HGDIOBJ oldBmp2 = SelectObject(memDC2, bmp2);   // Select bitmap into DC2
  HGDIOBJ oldBmp3 = SelectObject(memDC3, bmp3);   // Select bitmap into DC3
  HGDIOBJ oldBmp4 = SelectObject(memDC4, bmp4);   // Select bitmap into DC2

  // Client area axes
  const double realMin(-2.1);       // Minimum real value
  double imaginaryMin(-1.3);        // Minimum imaginary value
  double imaginaryMax(+1.3);        // Maximum imaginary value

  // Set maximum real so axes are the same scale
  double realMax(realMin+(imaginaryMax-imaginaryMin)*imageWidth/imageHeight);

  // Get scale factors to convert pixel coordinates
  double realScale((realMax-realMin)/(imageWidth-1)); 
  double imaginaryScale((imaginaryMax-imaginaryMin)/(imageHeight-1));

    // Lambda expression to create an image row
    std::function<void(HDC&, int)> rowCalc = [&] (HDC& memDC, int yLocal) 
    {
      double zReal(0.0), cReal(0.0);
      double zImaginary(imaginaryMax - yLocal*imaginaryScale);
      double cImaginary(zImaginary);
      for(int x = 0 ; x < imageWidth ; ++x)            // Iterate over pixels in a row
      {
        zReal = cReal = realMin + x*realScale;
        // Set current pixel color based on n
        SetPixel(memDC, x, 0, Color(IteratePoint(zReal, zImaginary, cReal, cImaginary)));
      }
    };

  for(int y = 3 ; y < imageHeight ; y += 4)             // Iterate over image rows
  {
    Concurrency::parallel_invoke([&]{rowCalc(memDC1, y-3);}, [&]{rowCalc(memDC2, y-2);},
                                 [&]{rowCalc(memDC3, y-1);}, [&]{rowCalc(memDC4, y);} );
    // Transfer pixel rows to client area device context
    BitBlt(hdc, 0, y-3, imageWidth, 1, memDC1, 0, 0, SRCCOPY);
    BitBlt(hdc, 0, y-2, imageWidth, 1, memDC2, 0, 0, SRCCOPY);
    BitBlt(hdc, 0, y-1, imageWidth, 1, memDC3, 0, 0, SRCCOPY);
    BitBlt(hdc, 0, y  , imageWidth, 1, memDC4, 0, 0, SRCCOPY);
  }
  // If there are not a multiple of 4 rows, take care of the last onse
  if(imageHeight%4 == 1)
  {
    rowCalc(memDC1, imageWidth-1);
    BitBlt(hdc, 0, imageHeight-1, imageWidth, 1, memDC1, 0, 0, SRCCOPY);
  }
  else if(imageHeight%4 == 2)
  {
    Concurrency::parallel_invoke([&]{rowCalc(memDC1, imageWidth-2);}, [&]{rowCalc(memDC2, imageWidth-1);});
    BitBlt(hdc, 0, imageHeight-2, imageWidth, 1, memDC1, 0, 0, SRCCOPY);
    BitBlt(hdc, 0, imageHeight-1, imageWidth, 1, memDC2, 0, 0, SRCCOPY);
  }
  else if(imageHeight%4 == 3)
  {
    Concurrency::parallel_invoke([&]{rowCalc(memDC1, imageWidth-3);}, [&]{rowCalc(memDC2, imageWidth-2);},
    [&]{rowCalc(memDC3, imageWidth-1);});
    BitBlt(hdc, 0, imageHeight-3, imageWidth, 1, memDC1, 0, 0, SRCCOPY);
    BitBlt(hdc, 0, imageHeight-2, imageWidth, 1, memDC2, 0, 0, SRCCOPY);
    BitBlt(hdc, 0, imageHeight-1, imageWidth, 1, memDC3, 0, 0, SRCCOPY);
  }

  SelectObject(memDC1, oldBmp1);
  SelectObject(memDC2, oldBmp2);
  SelectObject(memDC3, oldBmp3);
  SelectObject(memDC4, oldBmp4);
  DeleteObject(bmp1);                       // Delete bitmap 1
  DeleteObject(bmp2);                       // Delete bitmap 2
  DeleteObject(bmp3);                       // Delete bitmap 3
  DeleteObject(bmp4);                       // Delete bitmap 4
  DeleteDC(memDC1);                         // and our working DC 1
  DeleteDC(memDC2);                         // and our working DC 2
  DeleteDC(memDC3);                         // and our working DC 1
  DeleteDC(memDC4);                         // and our working DC 2
  ReleaseDC(hWnd, hdc);                     // Release client area DC
}
*/
