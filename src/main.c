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
#include <getopt.h>

#include "themerrr.h"


#define OPT_OFF 0
#define OPT_ON  1


void print_help();
void print_version();
const char VERSION[] = "20120911";

// Command line option flags
int opt_help    = OPT_OFF;
int opt_version = OPT_OFF;
int opt_no_gtk  = OPT_OFF;
int opt_no_qt   = OPT_OFF;

struct option longopts[] = {{ "help",    no_argument, &opt_help,    OPT_ON },
                            { "version", no_argument, &opt_version, OPT_ON },
                            { "no-gtk",  no_argument, &opt_no_gtk,  OPT_ON },
                            { "no-qt",   no_argument, &opt_no_qt,   OPT_ON }};


int main(int argc, char *argv[])
{
    int ret_code = 0;

    // Parse command line arguments
    while (getopt_long(argc, argv, "h", longopts, NULL) != -1);

    if (opt_help == OPT_ON) {
        print_help();
        return 0;
    }
    if (opt_version == OPT_ON) {
        print_version();
        return 0;
    }

    // Do reread confings
    if (opt_no_gtk == OPT_OFF)
        ret_code = reread_config_gtk();
    if (opt_no_qt == OPT_OFF)
        ret_code = reread_config_qt();

    return ret_code;
}


// Print brief help
void print_help()
{
    puts("Usage: themerrr [OPTION]...");
    puts("Theme rereader. Light and small tool for making heavy and large");
    puts("frameworks reread their config files and apply changes on the fly.");
    puts("");
    puts("Options");
    puts("  -h, --help  Print brief help");
    puts("  --no-gtk    Do not reread gtk config file");
    puts("  --no-qt     Do not reread qt config file");
}

// Print program version
void print_version()
{
    printf("%s\n", VERSION);
}
