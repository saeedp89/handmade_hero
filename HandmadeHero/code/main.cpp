#include <windows.h>
// #include <wingdi.h>

#define internal static
#define local_persist static
#define global_variable static

global_variable bool Running = false;
global_variable BITMAPINFO BitmapInfo;
global_variable void *BitmapMemory;

internal void Win32ResizeDIBSection(int Width, int Height) {

  BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
  BitmapInfo.bmiHeader.biWidth = Width;
  BitmapInfo.bmiHeader.biHeight = Height;
  BitmapInfo.bmiHeader.biPlanes = 1;
  BitmapInfo.bmiHeader.biBitCount = 32;
  BitmapInfo.bmiHeader.biCompression = BI_RGB;

  int BytesPerPixel = 4;

  int BitmapMemorySize = (Width * Height) * BytesPerPixel;
  BitmapMemory = VirtualAlloc(BitmapMemorySize);
}
internal void Win32UpdateWindow(HDC DeviceContext, int X, int Y, int Width,
                                int Height) {
  StretchDIBits(DeviceContext, X, Y, Width, Height, X, Y, Width, Height,
                BitmapMemory, &BitmapInfo, DIB_RGB_COLORS, SRCCOPY);
}

LRESULT CALLBACK Win32MainWindowCallback(HWND Window, UINT Message,
                                         WPARAM WParam, LPARAM LParam) {

  LRESULT Result = 0;

  switch (Message) {

  case WM_SIZE: {

    RECT ClientRect;
    GetClientRect(Window, &ClientRect); // the actual part you can draw into

    int Width = ClientRect.right - ClientRect.left;
    int Height = ClientRect.bottom - ClientRect.top;

    Win32ResizeDIBSection(Width, Height);
  } break;

  case WM_CLOSE: {
    Running = false;
  } break;

  case WM_ACTIVATEAPP: {

    OutputDebugStringA("WM_ACTIVATEAPP\n");

  } break;

  case WM_DESTROY: {

    Running = false;

  } break;

  case WM_PAINT: {

    PAINTSTRUCT Paint;

    HDC DeviceContext = BeginPaint(Window, &Paint);

    int X = Paint.rcPaint.left;
    int Y = Paint.rcPaint.top;
    int Width = Paint.rcPaint.right - Paint.rcPaint.left;
    int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
    Win32UpdateWindow(DeviceContext, X, Y, Width, Height);
    BOOL endPaint = EndPaint(Window, &Paint);

  } break;

  default: {

    Result = DefWindowProc(Window, Message, WParam, LParam);

  } break;
  }
  return Result;
}

int CALLBACK WinMain(HINSTANCE instance, HINSTANCE prevInstance,
                     LPSTR commandLine, int showCode) {

  WNDCLASS WindowsClass = {};
  // WindowsClass.style = CS_OWNDC|CS_HREDRAW|CS_VREDRAW|CS_PARENTDC;
  WindowsClass.lpfnWndProc = Win32MainWindowCallback;
  WindowsClass.hInstance = instance;
  WindowsClass.lpszClassName = "HandmadeHero";

  if (RegisterClass(&WindowsClass)) {
    HWND WindowHandle = CreateWindowEx(
        0, WindowsClass.lpszClassName, "handmadeHero",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, instance, 0);
    int errorCode = GetLastError();
    if (WindowHandle) {
      Running = true;
      while (Running) {
        MSG message;
        BOOL messageResult = GetMessageA(&message, 0, 0, 0);

        if (messageResult > 0) {
          TranslateMessage(&message);
          DispatchMessage(&message);
        } else {
          break;
        }
      }

    } else {
    }

  } else {
  }
  return (0);
}
