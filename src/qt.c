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
#include <time.h>

#include <X11/Xlib.h>

#include "themerrr.h"


#define TIMESTAMP_SIZE 9
unsigned char timestamp_buf[TIMESTAMP_SIZE];
unsigned char *get_stamp();
uint get_jd(int year, int month, int day);
uint get_hms(int hour, int min, int sec);


/* Reread Qt theme from config file
 * Return 0 - if success, otherwise - error code
 */
int reread_config_qt()
{
    // Open display and root window
    Display *d = XOpenDisplay(0);
    if (!d) {
        fprintf(stderr, "Error: qt: cannot open X11-display\n");
        return 1;
    }
    Window w = DefaultRootWindow(d);

    // Get property atom
    int atom_num;
    char atom_name[512];
    sprintf(atom_name, "_QT_SETTINGS_TIMESTAMP_%s", XDisplayName(NULL));
    atom_num = XInternAtom(d, atom_name, False);
    if (atom_num == 0) {
        fprintf(stderr, "Error: qt: cannot find atom \"%s\"\n", atom_name);
        return 1;
    }

    // Calculate timestamp
    unsigned char *data = get_stamp();

    // Call config rereading
    XChangeProperty(d, w,
                    atom_num,
                    atom_num,
                    8,
                    PropModeReplace,
                    data,
                    TIMESTAMP_SIZE);
    XSync(d, False);

    return 0;
}

/* Get full time stamp
 * Return piunter to char array of size TIMESTAMP_SIZE
 */
unsigned char *get_stamp()
{
    time_t t = time(NULL);
    struct tm time = *localtime(&t);
    int dy = time.tm_year + 1900;
    int dm = time.tm_mon;
    int dd = time.tm_mday;
    int th = time.tm_hour;
    int tm = time.tm_min;
    int ts = time.tm_sec;

    unsigned int jd = get_jd(dy, dm, dd);
    unsigned int hms = get_hms(th, tm, ts);
    unsigned char spec = 0; // for Qt ver. >= 4.0

    timestamp_buf[0] = (jd & 0xff000000) >> 24;
    timestamp_buf[1] = (jd & 0x00ff0000) >> 16;
    timestamp_buf[2] = (jd & 0x0000ff00) >> 8;
    timestamp_buf[3] = (jd & 0x000000ff);
    timestamp_buf[4] = (hms & 0xff000000) >> 24;
    timestamp_buf[5] = (hms & 0x00ff0000) >> 16;
    timestamp_buf[6] = (hms & 0x0000ff00) >> 8;
    timestamp_buf[7] = (hms & 0x000000ff);
    timestamp_buf[8] = spec;

    return timestamp_buf;
}

/* Get julian day from gregorian date
 * Return integer representation of specified julian date
 */
uint get_jd(int year, int month, int day)
{
    return (1461 * (year + 4800 + (month - 14) / 12)) / 4
           + (367 * (month - 2 - 12 * ((month - 14) / 12))) / 12
           - (3 * ((year + 4900 + (month - 14) / 12) / 100)) / 4
           + day - 32075;
}

/* Get time
 * Return integer representation of specifiet time (in msecs)
 */
uint get_hms(int hour, int min, int sec)
{
    return (hour * 3600 + min * 60 + sec) * 1000;
}
