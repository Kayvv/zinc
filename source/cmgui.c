/*******************************************************************************
FILE : cmgui.c

LAST MODIFIED : 7 January 2003

DESCRIPTION :
==============================================================================*/
/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is cmgui.
 *
 * The Initial Developer of the Original Code is
 * Auckland Uniservices Ltd, Auckland, New Zealand.
 * Portions created by the Initial Developer are Copyright (C) 2005
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */
/*SAB I have concatenated the correct version file for each version
  externally in the shell with cat #include "version.h"*/
#if defined (BUILD_WITH_CMAKE)
#include "configure/version.h"
#endif /* defined (BUILD_WITH_CMAKE) */

#include "command/cmiss.h"
#include "context/context.h"
#include "context/user_interface_module.h"
#include "general/debug.h"
#include "user_interface/message.h"
#if defined (UNEMAP)
#include "unemap_application/unemap_command.h"
#include "command/cmiss_unemap_link.h"
#endif /* defined (UNEMAP) */

#if defined (WX_USER_INTERFACE) && (DARWIN)
#include <ApplicationServices/ApplicationServices.h>
#endif
#if defined (USE_XML2)
#include <libxml/catalog.h>
#include <libxml/xmlschemastypes.h>
#endif

/*
Global functions
----------------
*/

#if !defined (WIN32_USER_INTERFACE) && !defined (_MSC_VER)
int main(int argc,const char *argv[])
#else /* !defined (WIN32_USER_INTERFACE)  && !defined (_MSC_VER)*/
int WINAPI WinMain(HINSTANCE current_instance,HINSTANCE previous_instance,
	LPSTR command_line,int initial_main_window_state)
	/* using WinMain as the entry point tells Windows that it is a gui and to use
		the graphics device interface functions for I/O */
	/*???DB.  WINDOWS a zero return code if WinMain does get into the message
		loop.  Other application interfaces may expect something else.  Should this
		failure code be #define'd ? */
	/*???DB. Win32 SDK says that don't have to call it WinMain */
#endif /* !defined (WIN32_USER_INTERFACE)  && !defined (_MSC_VER)*/
/*******************************************************************************
LAST MODIFIED : 7 January 2003

DESCRIPTION :
Main program for the CMISS Graphical User Interface
==============================================================================*/
{
	int return_code = 0;
#if defined (WIN32_USER_INTERFACE) || defined (_MSC_VER)
	int argc = 1, i;
	char **argv, *p, *q;
#endif /* defined (WIN32_USER_INTERFACE) */
	struct Context *context = NULL;
	struct User_interface_module *UI_module = NULL;
	struct Cmiss_command_data *command_data;

#if !defined (WIN32_USER_INTERFACE) && !defined (_MSC_VER)
	ENTER(main);
#else /* !defined (WIN32_USER_INTERFACE)  && !defined (_MSC_VER)*/
	ENTER(WinMain);

	//_CrtSetBreakAlloc(28336);
	for (p = command_line; p != NULL && *p != 0;)
	{
		p = strchr(p, ' ');
		if (p != NULL)
			p++;
		argc++;
	}

	argv = malloc(sizeof(*argv) * argc);

	argv[0] = "cmgui";

	for (i = 1, p = command_line; p != NULL && *p != 0;)
	{
		q = strchr(p, ' ');
		if (q != NULL)
			*q++ = 0;
		if (p != NULL)
			argv[i++] = p;
		p = q;
	}
#endif /* !defined (WIN32_USER_INTERFACE)  && !defined (_MSC_VER)*/

	/* display the version */
	display_message(INFORMATION_MESSAGE, "%s version %s  %s\n%s\n"
		"Build information: %s %s\n", CMISS_NAME_STRING, CMISS_VERSION_STRING, 
		CMISS_DATE_STRING, CMISS_COPYRIGHT_STRING, CMISS_BUILD_STRING,
		CMISS_SVN_REVISION_STRING);

#if defined (CARBON_USER_INTERFACE) || (defined (WX_USER_INTERFACE) && defined (DARWIN))
	ProcessSerialNumber PSN;
	GetCurrentProcess(&PSN);
	TransformProcessType(&PSN,kProcessTransformToForegroundApplication);
#endif
	context = Cmiss_context_create("default");
	if (context)
	{
#if !defined (WIN32_USER_INTERFACE) && !defined (_MSC_VER)
		UI_module = Cmiss_context_create_user_interface(context, argc, argv, NULL);
#else /* !defined (WIN32_USER_INTERFACE)  && !defined (_MSC_VER)*/
		UI_module = Cmiss_context_create_user_interface(context, argc, argv, current_instance,
			previous_instance, command_line, initial_main_window_state, NULL);
#endif /* !defined (WIN32_USER_INTERFACE)  && !defined (_MSC_VER)*/
		if (UI_module)
		{
			if (NULL != (command_data = Cmiss_context_get_default_command_interpreter(context)))
			{
				Cmiss_command_data_set_cmgui_string(command_data, CMISS_NAME_STRING, 
					CMISS_VERSION_STRING, CMISS_DATE_STRING, CMISS_COPYRIGHT_STRING, CMISS_BUILD_STRING,
					CMISS_SVN_REVISION_STRING);
				Cmiss_command_data_main_loop(command_data);
				Cmiss_command_data_destroy(&command_data);
				return_code = 0;
			}
			else
			{
				return_code = 1;
			}
			User_interface_module_destroy(&UI_module);
		}
		else
		{
			return_code = 1;
		}
		Cmiss_context_destroy(&context);
		/* FieldML does not cleanup the global varaibles xmlSchematypes and
		 * xmlCatalog at this moment, so we clean it up here instead*/
#if defined (USE_XML2)
		xmlCatalogCleanup();
		xmlSchemaCleanupTypes();
#endif
	}
	else
	{
		return_code = 1;
	}
#if defined (WIN32_USER_INTERFACE) || defined (_MSC_VER)
	free(argv)
#endif
	LEAVE;
	
	return (return_code);
} /* main */
