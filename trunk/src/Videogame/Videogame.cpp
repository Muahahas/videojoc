#include <Windows.h>

#define APPLICATION_NAME	"VIDEOGAME"

//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	//Christian estuvo aqu�
	// canvi irrellevant
  switch( msg )
  {
  case WM_DESTROY:
    {
      PostQuitMessage( 0 );
      return 0;
    }
    break;
  case WM_KEYDOWN:
    {
      switch( wParam )
      {
      case VK_ESCAPE:
        //Cleanup();
        PostQuitMessage( 0 );
        return 0;
        break;
      }
    }
    break;
  }//end switch( msg )

  return DefWindowProc( hWnd, msg, wParam, lParam );
}

//-----------------------------------------------------------------------
// WinMain
//-----------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, int _nCmdShow)
{
  // Register the window class
  WNDCLASSEX wc = {	sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, APPLICATION_NAME, NULL };
  
  RegisterClassEx( &wc ); 

  RECT rc = { 0, 0, 800, 600 };
  AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

  // Create the application's window
  HWND hWnd = CreateWindow(APPLICATION_NAME, APPLICATION_NAME, WS_OVERLAPPEDWINDOW, 100, 100, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, wc.hInstance, NULL);

  // A�adir aqu� el Init de la applicacio�n

  ShowWindow( hWnd, SW_SHOWDEFAULT );
  UpdateWindow( hWnd );
  MSG msg;
  ZeroMemory( &msg, sizeof(msg) );

  // A�adir en el while la condici�n de salida del programa de la aplicaci�n
  float angle = 0.2f;
  float height = 2;

  while( msg.message != WM_QUIT )
  {
    if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
    {
      TranslateMessage( &msg );
      DispatchMessage( &msg );
    }
    else
    {
       
    }
  }
  
  UnregisterClass( APPLICATION_NAME, wc.hInstance );

  // A�adir una llamada a la alicaci�n para finalizar/liberar memoria de todos sus datos

  return 0;
}
