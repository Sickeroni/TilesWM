#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/cursorfont.h>
#include <X11/Xmu/WinUtil.h>

#include <Imlib2.h>

/* Copied from xprop (dsimple.c) and slightly modified. */

Window select_window(Display *disp, int screen)
{
  int status;
  Cursor cursor;
  XEvent event;
  Window target_win = None;
  Window root = RootWindow(disp, screen);
  int buttons = 0;

  /* Make the target cursor */
  cursor = XCreateFontCursor(disp, XC_crosshair);

  /* Grab the pointer using target cursor, letting it room all over */
  status = XGrabPointer(disp, root, False,
			ButtonPressMask|ButtonReleaseMask, GrabModeSync,
			GrabModeAsync, root, cursor, CurrentTime);

  if (status != GrabSuccess) {
    fprintf(stderr, "Can't grab the mouse.");
    exit(1);
  }

  /* Let the user select a window... */
  while ((target_win == None) || (buttons != 0)) {
    /* allow one more event */
    XAllowEvents(disp, SyncPointer, CurrentTime);
    XWindowEvent(disp, root, ButtonPressMask|ButtonReleaseMask, &event);
    switch (event.type) {
    case ButtonPress:
      if (target_win == None) {
	target_win = event.xbutton.subwindow; /* window selected */
	if (target_win == None) target_win = root;
      }
      buttons++;
      break;
    case ButtonRelease:
      if (buttons > 0) /* there may have been some down before we started */
	buttons--;
       break;
    }
  } 

  XUngrabPointer(disp, CurrentTime);      /* Done with pointer */

  return(target_win);
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <icon file>\n", argv[0]);
    exit(1);
  }

  Display *disp = XOpenDisplay(0);
  int screen = XDefaultScreen(disp);

  /* Let the user select the window. */

  Window win = select_window(disp, screen);
  win = XmuClientWindow(disp, win);

  /* ... */

  unsigned long nitems, bytesafter;
  unsigned char *ret;
  int format;
  Atom type;

  Atom net_wm_icon = XInternAtom(disp, "_NET_WM_ICON", 1);

  /* Get the width of the icon. */

  XGetWindowProperty(disp, 
		     win, 
		     net_wm_icon, 
		     0, 1, 0, 
		     XA_CARDINAL,
		     &type, 
		     &format, 
		     &nitems, 
		     &bytesafter, 
		     &ret);

  if (!ret) {
    fprintf(stderr, "No icon found.\n");
    exit(1);
  }

  int width = *(int*)ret;
  XFree(ret);
  
  /* Get the height of the icon. */

  XGetWindowProperty(disp, 
		     win, 
		     net_wm_icon, 
		     1, 1, 0, 
		     XA_CARDINAL,
		     &type, 
		     &format, 
		     &nitems, 
		     &bytesafter, 
		     &ret);

  if (!ret) {
    fprintf(stderr, "Failed to read the height of the icon.\n");
    exit(1);
  }

  int height = *(int*)ret;
  XFree(ret);

  /* Get the icon data. */

  int size = width * height;

  XGetWindowProperty(disp, 
		     win, 
		     net_wm_icon, 
		     2, size, 0, 
		     XA_CARDINAL,
		     &type, 
		     &format, 
		     &nitems, 
		     &bytesafter, 
		     &ret);  

  if (!ret) {
    fprintf(stderr, "Failed to read the icon data.\n");
    exit(1);
  }
  
  /* Setup Imlib. */

  Imlib_Image image = imlib_create_image(width, height);

  imlib_context_set_image(image);
  imlib_image_set_has_alpha(1);
  
  /* Create an Imlib image with the icon data. */

  DATA32 *imagedata = imlib_image_get_data();

  memcpy(imagedata, ret, width * height * 4);
  XFree(ret);

  /* Save the image into the specified file. */

  imlib_image_put_back_data(imagedata);
  imlib_image_set_format(strrchr(argv[1], '.') + 1);
  imlib_save_image(argv[1]);

  return 0;
}
