/*
Copyright (c) 2005-2007, Thomas BERNARD
All rights reserved.

Copyright (c) 2007, Dirk Meyer

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * The name of the author may not be used to endorse or promote products
          derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

$Id$

*/

#ifdef USE_UPNP

/* include the headers */
#include "iroffer_config.h"
#include "iroffer_defines.h"
#include "iroffer_headers.h"

#include "upnp.h"

#include <miniupnpc/miniwget.h>
#include <miniupnpc/miniupnpc.h>
#include <miniupnpc/upnpcommands.h>

static struct UPNPUrls urls;
static struct IGDdatas data;

void init_upnp (void)
{
	struct UPNPDev * devlist;
	struct UPNPDev * dev;
	char * descXML;
	int descXMLsize = 0;

	ioutput(CALLTYPE_NORMAL, OUT_S|OUT_L|OUT_D, COLOR_YELLOW, "UPnP discover" );
	tostdout_write();
	memset(&urls, 0, sizeof(struct UPNPUrls));
	memset(&data, 0, sizeof(struct IGDdatas));
	devlist = upnpDiscover(2000, NULL);
	if (devlist)
	{
		dev = devlist;
		while (dev)
		{
			if (strstr (dev->st, "InternetGatewayDevice"))
				break;
			dev = dev->pNext;
		}
		if (!dev)
			dev = devlist; /* defaulting to first device */

		ioutput(CALLTYPE_NORMAL, OUT_S|OUT_L|OUT_D, COLOR_YELLOW,
			"UPnP device : desc: %s\n st: %s",
			dev->descURL, dev->st);

		descXML = miniwget(dev->descURL, &descXMLsize);
		if (descXML)
		{
			parserootdesc (descXML, descXMLsize, &data);
			free (descXML); descXML = 0;
			GetUPNPUrls (&urls, &data, dev->descURL);
		}
		freeUPNPDevlist(devlist);
	}
}

void upnp_add_redir (const char * addr, const char * port)
{
	int r;

	ioutput(CALLTYPE_NORMAL, OUT_S|OUT_L|OUT_D, COLOR_YELLOW, "UPnP add redirect (%s, %s)", addr, port);
	if(urls.controlURL == NULL)
	{
		outerror(OUTERROR_TYPE_WARN_LOUD, "UPnP not found");
		return;
	}
	if(urls.controlURL[0] == '\0')
	{
		outerror(OUTERROR_TYPE_WARN_LOUD, "UPnP not found");
		return;
	}
	r = UPNP_AddPortMapping(urls.controlURL, data.servicetype,
	                        port, port, addr, 0, "TCP");
	if(r==0)
		ioutput(CALLTYPE_NORMAL, OUT_S|OUT_L|OUT_D, COLOR_YELLOW,
			"AddPortMapping(%s, %s, %s) failed" , port, port, addr);
}

void upnp_rem_redir (int port)
{
	char port_str[16];

	if (port == 0)
		return;

	sprintf(port_str, "%d", port);
	ioutput(CALLTYPE_NORMAL, OUT_S|OUT_L|OUT_D, COLOR_YELLOW, "UPnP remove redirect (%s)", port_str);
	if(urls.controlURL == NULL)
	{
		outerror(OUTERROR_TYPE_WARN_LOUD, "UPnP not found");
		return;
	}
	if(urls.controlURL[0] == '\0')
	{
		outerror(OUTERROR_TYPE_WARN_LOUD, "UPnP not found");
		return;
	}
	UPNP_DeletePortMapping(urls.controlURL, data.servicetype, port_str, "TCP");
}

#endif /* USE_UPNP */

/* End of File */