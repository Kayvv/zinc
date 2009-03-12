/*******************************************************************************
FILE : computed_field_time.h

LAST MODIFIED : 19 September 2003

DESCRIPTION :
Implements computed fields that control the time behaviour.
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
#if !defined (COMPUTED_FIELD_TIME_H)
#define COMPUTED_FIELD_TIME_H

#include "time/time_keeper.h"

int Computed_field_register_types_time(
	struct Computed_field_package *computed_field_package,
	struct Time_keeper *time_keeper);
/*******************************************************************************
LAST MODIFIED : 19 September 2003

DESCRIPTION :
==============================================================================*/

int Computed_field_set_type_time_lookup(struct Computed_field *field,
	struct Computed_field *source_field, struct Computed_field *time_field);
/*******************************************************************************
LAST MODIFIED : 16 December 2002

DESCRIPTION :
Converts <field> to type COMPUTED_FIELD_TIME_LOOKUP with the supplied
fields, <source_field> is the field the values are returned from but rather
than using the current time, the scalar <time_field> is evaluated and its value
is used for the time.
==============================================================================*/

#endif /* !defined (COMPUTED_FIELD_TIME_H) */
