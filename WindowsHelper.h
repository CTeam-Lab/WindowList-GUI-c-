#ifndef _WINDOWSHELPER_
#define _WINDOWSHELPER_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/cursorfont.h>
#include <X11/Xmu/WinUtil.h>
#include <cstdlib>
#include <cstring>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <glib.h>
#include <stdlib.h>

#define _NET_WM_STATE_REMOVE        0
#define _NET_WM_STATE_ADD           1
#define _NET_WM_STATE_TOGGLE        2
#define MAX_PROPERTY_VALUE_LEN      4096
#define	EXIT_FAILURE	1	/* Failing exit status.  */
#define	EXIT_SUCCESS	0	/* Successful exit status.  */

using namespace std;

struct Desktop {
	int id;
	string name;
} typedef Desktop;

class WindowsHelper {

public:

	void icon(Display *disp, Window win);

	int client_msg(Display *disp, Window win, char const *msg,
	    unsigned long data0, unsigned long data1,
	    unsigned long data2, unsigned long data3,
	    unsigned long data4);

	void activate_window(Display *disp, Window win);

	/**
	 * GetWindowDesktop - Get desktop id from window
	 */
	bool GetWindowDesktop(Display *disp, Window win, signed long& desktop_id);

	/**
	 * GetClientList - Get the list of clients attached to display.
	 */
	bool GetClientList(Display *disp,vector<Window>& v_wClientList);

	/**
	 * GetWindowTitle - Get Window Title given Window ID
	 */
	bool GetWindowTitle(Display *disp, Window win, string& s_WindowTitle);

	/**
	 * FindWindowMatching - Return first Window ID matching a title fragment.
	 */
	bool FindWindowMatching(Display *disp, string windowName, Window& win);

	/**
	 * SendKeyToWindow - Send a Keypress to the specified window.
	 */
	void SendKeyToWindow(Display *disp, unsigned long win, int iXKeySym, int serial_num);

private:

	/**
	 * GetProperty - Get an X Property.
	 */
	char *GetProperty(Display *disp, Window win, Atom xa_prop_type, string s_PropertyName, unsigned long& size);

};
#endif
