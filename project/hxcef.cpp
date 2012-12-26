#ifndef STATIC_LINK
	#define IMPLEMENT_API
#endif

#if defined(HX_WINDOWS) || defined(HX_MACOS) || defined(HX_LINUX)
#define NEKO_COMPATIBLE
#endif
#include <hx/CFFI.h>

#include <windows.h>
#include <commdlg.h>
#include <shellapi.h>
#include <direct.h>
#include <sstream>
#include <string>
#include "include/cef_base.h"
#include "include/cef_app.h"
#include "include/cef_client.h"
#include "include/cef_browser.h"
#include "include/cef_frame.h"
#include "include/cef_runnable.h"

#include "include/internal/cef_win.h"

class myClientHandler : public CefClient
{
  public:
    myClientHandler()  {}
    ~myClientHandler() {}

  protected:
    IMPLEMENT_REFCOUNTING(ClientHandler);
};

// Global var
CefRefPtr<myClientHandler> g_handler;

value cef_init()
{
	CefSettings settings;
	CefMainArgs args;
	settings.multi_threaded_message_loop = true;
	CefInitialize(args, settings, NULL);

	HWND hWnd;
	RECT rect;

	// Init First Chrome
	CefBrowserSettings browserSettings;
	CefWindowInfo info;

	g_handler = new myClientHandler();
	DWORD dwProcID = GetCurrentProcessId();
	hWnd = GetTopWindow(GetDesktopWindow());
	while(hWnd)
    {
		DWORD dwWndProcID = 0;
		GetWindowThreadProcessId(hWnd, &dwWndProcID);
		if(dwWndProcID == dwProcID)
			break; 
		hWnd = GetNextWindow(hWnd, GW_HWNDNEXT);
    }

	GetClientRect(hWnd, &rect);
	info.SetAsChild(hWnd,rect);

	CefBrowserHost::CreateBrowser(info,static_cast< CefRefPtr<CefClient> >(g_handler),"http://www.google.com", browserSettings);
	
	return alloc_null();
}

DEFINE_PRIM(cef_init, 0);

// Reference this to bring in all the symbols for the static library
int hxcef_register_prims() { return 0; }