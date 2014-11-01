#include "WindowsHelper.h"

using namespace std;

void WindowsHelper::icon(Display *disp, Window win)
{
	unsigned long int size;
	Atom net_wm_icon = XInternAtom(disp, "_NET_WM_ICON", False);

	//desktop = (image *) GetProperty(disp, win, XA_CARDINAL, "_NET_WM_ICON", size);

}

int WindowsHelper::client_msg(Display *disp, Window win, char const *msg,
    unsigned long data0, unsigned long data1,
    unsigned long data2, unsigned long data3,
    unsigned long data4)
{
  XEvent event;
  long mask = SubstructureRedirectMask | SubstructureNotifyMask;

  event.xclient.type = ClientMessage;
  event.xclient.serial = 0;
  event.xclient.send_event = True;
  event.xclient.message_type = XInternAtom(disp, msg, False);
  event.xclient.window = win;
  event.xclient.format = 32;
  event.xclient.data.l[0] = data0;
  event.xclient.data.l[1] = data1;
  event.xclient.data.l[2] = data2;
  event.xclient.data.l[3] = data3;
  event.xclient.data.l[4] = data4;

  if (XSendEvent(disp, DefaultRootWindow(disp), False, mask, &event)) {
    return 1;
  }
  else {
    fprintf(stderr, "Cannot send %s event.\n", msg);
    return 0;
  }
}

void WindowsHelper::activate_window (Display *disp, Window win)
{
	unsigned long int size;
	Window *desktop;

	long signed desktop_id;
	GetWindowDesktop(disp, win, desktop_id);

	//desktop = (Window *) GetProperty(disp, win, XA_CARDINAL, "_NET_WM_DESKTOP", size);
	/* desktop ID
    if ((desktop = (Window *) GetProperty(disp, win, XA_CARDINAL, "_NET_WM_DESKTOP", size)) == NULL) {

        if ((desktop = (Window *) GetProperty(disp, win, XA_CARDINAL, "_WIN_WORKSPACE", size)) == NULL) {
            return;
        }
        if (client_msg(disp, DefaultRootWindow(disp),"_NET_CURRENT_DESKTOP", *desktop, 0, 0, 0, 0) != 0) {
        	g_print("Cannot switch desktop.\n");
	    }
	    g_free(desktop);
    }

    client_msg(disp, win, "_NET_ACTIVE_WINDOW",0, 0, 0, 0, 0);

    XMapRaised(disp, win);*/
}

/**
 * GetProperty - Get an X Property.
 */
char* WindowsHelper::GetProperty(Display *disp, Window win, Atom xa_prop_type, string s_PropertyName, unsigned long& size)
{
	Atom xa_prop_name;
	Atom xa_ret_type;
	int ret_format;
	unsigned long ret_nitems;
	unsigned long ret_bytes_after;
	unsigned long tmp_size;
	unsigned char *ret_prop;
	char *ret;

	xa_prop_name = XInternAtom(disp, s_PropertyName.c_str(), False);

	if (XGetWindowProperty(disp, win, xa_prop_name, 0, MAX_PROPERTY_VALUE_LEN / 4, False,
		xa_prop_type, &xa_ret_type, &ret_format,
			&ret_nitems, &ret_bytes_after, &ret_prop) != Success)
	{
		return NULL;
	}

	if (xa_ret_type != xa_prop_type)
	{
		XFree(ret_prop);
		return NULL;
	}

	 /* null terminate the result to make string handling easier */
	tmp_size = (ret_format / (32 / sizeof(long))) * ret_nitems;
	ret = (char *)malloc(tmp_size+1);

	memcpy(ret, ret_prop, tmp_size);
	ret[tmp_size] = '\0';
	if (size) {
		size = tmp_size;
	}

	XFree(ret_prop);
	return ret;

}


/**
 * GetClientList - Get the list of clients attached to display.
 */
bool WindowsHelper::GetClientList(Display *disp,vector<Window>& v_wClientList)
{
	Window *client_list;
	unsigned int i;
	unsigned long client_list_size;

	if ((client_list = (Window *)GetProperty(disp,DefaultRootWindow(disp),XA_WINDOW,"_NET_CLIENT_LIST",client_list_size)) == NULL)
	{
		if ((client_list = (Window *)GetProperty(disp,DefaultRootWindow(disp), XA_CARDINAL, "_WIN_CLIENT_LIST",client_list_size)) == NULL)
		{
			return false;
		}
	}

	// Now transform it into a vector of Windows
	for (i=0;i<client_list_size/sizeof(Window);i++)
	{
		v_wClientList.push_back(client_list[i]);
	}

	return true;

}

/**
 * GetWindowDesktop - Get Window Desktop Id given Window ID
 */
bool WindowsHelper::GetWindowDesktop(Display *disp, Window win, signed long& desktop_id)
{
	unsigned long int wm_desktop_size;

	unsigned long *desktop;

	/* desktop ID */
	if ((desktop = (unsigned long *) GetProperty(disp, win, XA_CARDINAL, "_NET_WM_DESKTOP", wm_desktop_size)) == NULL) {
		desktop = (unsigned long *) GetProperty(disp, win, XA_CARDINAL, "_WIN_WORKSPACE", wm_desktop_size);
	}

	desktop_id = (signed long)*desktop;


	return true;
}

/**
 * GetWindowTitle - Get Window Title given Window ID
 */
bool WindowsHelper::GetWindowTitle(Display *disp, Window win, string& s_Title)
{
	char *wm_name;
	unsigned long int wm_name_size;
	char *net_wm_name;
	unsigned long int net_wm_name_size;

	if ((net_wm_name = GetProperty(disp, win, XInternAtom(disp, "UTF8_STRING", False), "_NET_WM_NAME", net_wm_name_size)) == NULL)
	{
		// no _NET_WM property for window title, get it the old way.
		if ((wm_name = GetProperty(disp, win, XA_STRING, "WM_NAME", wm_name_size)) == NULL)
		{
			return false;
		} else {
			s_Title = string(wm_name,(size_t)wm_name_size);
			return true;
		}
	}
	else
	{
		s_Title = string(net_wm_name,net_wm_name_size);
		return true;
	}

}

/**
 * FindWindowMatching - Return first Window ID matching a title fragment.
 */
bool WindowsHelper::FindWindowMatching(Display *disp, string windowName, Window& win)
{
	vector<Window> v_wClientList;
	string title;
	unsigned int i;

	if (!GetClientList(disp,v_wClientList))
	{
		return false;
	}

	for (i=0;i<v_wClientList.size();i++)
	{
		if (!GetWindowTitle(disp,v_wClientList[i],title))
		{
			continue;
		}
		else
		{
			if (title.find(windowName,0) == string::npos)
			{
				continue;
			}
			else
			{
				win = (Window)v_wClientList[i];
				return true;
			}
		}
	}
	return false;

}

