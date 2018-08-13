/*
 * Themerrr - Theme Rereader Utility
 * Copyright (C) 2012 Dmirty Lavnikevich
 * Contact: haff@midgard.by
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/*
 * This software uses parts of Qt and GTK+ source code which both are
 * availabe under LGPL v.2 Their source code can be found at
 * http://qt-project.org/ and http://www.gtk.org/ correspondingly.
 */


#include <stdio.h>
#include <string.h>

#include <X11/Xlib.h>

#include "themerrr.h"


/* Send an X11 event to a specified window on the specified display
 */
int send_xevent(Display *d, Window w, XEvent *event)
{
    Status result;

    if (d == 0)
        {
            return False;
        }

    event->xclient.window = w;
    printf("win=0x%08lx\n", w);
    result = XSendEvent(d, w, False, NoEventMask, event);
    XSync(d, False);

    return result;
}

/* Send an X event to all toplevel client windows
 */
char send_xevent_to_all_recurse(Display *d, XEvent *xev, Window w, unsigned int level)
{
    Atom type = None;
    int format;
    unsigned long nitems, after;
    unsigned char *data;
    Window *ret_children, ret_root, ret_parent;
    unsigned int ret_nchildren;
    char send = False;
    char found = False;
    char result = False;

    int atom_num;
    atom_num = XInternAtom(d, "WM_STATE", False);
    printf("atom: %d\n", atom_num);

    if (XGetWindowProperty(
            d,                 // Display *display
            w,                 // Window w
            atom_num,          // Atom property
            0,                 // long long_offset
            0,                 // long long_length
            False,             // Bool delete
            AnyPropertyType,   // Atom req_type
            &type,             // Atom *actual_type_return
            &format,           // int *actual_format_return
            &nitems,           // unsigned long *nitems_return
            &after,            // unsigned long *bytes_after_return
            &data)             // unsigned char **prop_return
        != Success) {
        fprintf(stderr, "Error: gtk+: No such property\n");
        return result;
    }

    if (type) {
        send = True;
        XFree(data);
    } else {
        // Ok, we're all set, now let's find some windows to send this to
        if (!XQueryTree(d,
                w,
                &ret_root,
                &ret_parent,
                &ret_children,
                &ret_nchildren)) {
            return result;
        }

        for (int i = 0; i < ret_nchildren; i++) {
            if (send_xevent_to_all_recurse(d, xev, ret_children[i], level + 1))
                found = True;
        }

        XFree(ret_children);
    }

    if (send || (!found && (level == 1))) {
        xev->xclient.window = w;
        send_xevent(d, w, xev);
    }

    result = send || found;

    return result;
}

/* Custom X11 ClientMessage event
 * Used for reloading urxvt config
 */
int reread_config_generic(char *atom_name)
{
    // Get default display and root window
    Display *d = XOpenDisplay(0);
    if (!d) {
        fprintf(stderr, "Error: gtk+: cannot open X11-display\n");
        return 1;
    }
    Window w = DefaultRootWindow(d);

    // Get message atom
    int atom_num;
    atom_num = XInternAtom(d, atom_name, False);
    if (atom_num == 0) {
        fprintf(stderr, "Error: gtk+: cannot find atom \"%s\"\n", atom_name);
        return 1;
    }

    // Set up our event to send
    XEvent sev;
    sev.xclient.type = ClientMessage;
    sev.xclient.display = d;
    sev.xclient.format = 32;
    memset(&sev.xclient.data, 0, sizeof(sev.xclient.data));
    sev.xclient.message_type = atom_num;

    // Send 'reread config' event to all
    send_xevent_to_all_recurse(d, &sev, w, 0);

    return 0;
}

/* Reread Gtk+ theme from a config file
 * Returns 0 if success, error code - otherwise
 */
int reread_config_gtk()
{
    return reread_config_generic("_GTK_READ_RCFILES");
}
