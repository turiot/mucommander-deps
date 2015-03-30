/*******************************************************************************
 * Copyright (c) 2002, 2010 QNX Software Systems and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *     QNX Software Systems - initial API and implementation
 *     Wind River Systems, Inc.
 *     Mikhail Zabaluev (Nokia) - bug 82744
 *     Corey Ashford (IBM) - bug 272370, bug 272372
 *******************************************************************************/

/* _XOPEN_SOURCE is needed to bring in the header for ptsname */
#define _XOPEN_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <grp.h>

#include <stdlib.h>

/**
 * This is taken from R. W. Stevens book.
 * Alain Magloire.
 */

void
set_noecho(int fd)
{
	struct termios stermios;
	if (tcgetattr(fd, &stermios) < 0) {
		return ;
	}

	/* turn off echo */
	stermios.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL);
	/* Turn off the NL to CR/NL mapping ou output.  */
	/*stermios.c_oflag &= ~(ONLCR);*/

	stermios.c_iflag |= (IGNCR);

	tcsetattr(fd, TCSANOW, &stermios);
}

int
ptys_open(int fdm, const char * pts_name)
{
	int fds;
	/* following should allocate controlling terminal */
	fds = open(pts_name, O_RDWR);
	if (fds < 0) {
		close(fdm);
		return -5;
	}

#if	defined(TIOCSCTTY)
	/*  TIOCSCTTY is the BSD way to acquire a controlling terminal. */
	if (ioctl(fds, TIOCSCTTY, (char *)0) < 0) {
		// ignore error: this is expected in console-mode
	}
#endif
	return fds;
}
