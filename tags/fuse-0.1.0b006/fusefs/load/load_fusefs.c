/*
 * Copyright (c) 2000-2004 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <unistd.h>

/*****************************************************************************/

/* Local Definitions */

#define LOAD_COMMAND "/sbin/kextload"
#define FUSE_MODULE_PATH "/System/Library/Extensions/fusefs.kext"

/*****************************************************************************/

int main(int argc, const char *argv[])
{
	#pragma unused(argc, argv)
	int pid;
	int result = -1;
	union wait status;
	
	pid = fork();
	if (pid == 0)
	{
		result = execl(LOAD_COMMAND, LOAD_COMMAND, FUSE_MODULE_PATH, NULL);
		/* We can only get here if the exec failed */
		goto Return;
	}
	
	if (pid == -1)
	{
		result = errno;
		goto Return;
	}
	
	/* Success! */
	if ((wait4(pid, (int *) & status, 0, NULL) == pid) && (WIFEXITED(status)))
	{
		result = status.w_retcode;
	}
	else
	{
		result = -1;
	}
	
Return:

	_exit(result);
}

/*****************************************************************************/