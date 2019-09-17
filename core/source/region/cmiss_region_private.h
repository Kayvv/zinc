/*******************************************************************************
FILE : cmiss_region_private.h

LAST MODIFIED : 1 October 2002

DESCRIPTION :
Private interface for attaching any object type to cmzn_region objects.
==============================================================================*/
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#if !defined (CMZN_REGION_PRIVATE_H)
#define CMZN_REGION_PRIVATE_H

#include "opencmiss/zinc/types/contextid.h"
#include "general/any_object.h"
#include "region/cmiss_region.h"

/*
Global functions
----------------
*/

/***************************************************************************//**
 * Private function for adding field to region. Ensures the new field has a
 * unique cache_index.
 */
int cmzn_region_add_field_private(cmzn_region_id region, cmzn_field_id field);

/***************************************************************************//**
 * Private function for clearing field value caches for field in all caches
 * listed in region.
 */
void cmzn_region_clear_field_value_caches(cmzn_region_id region, cmzn_field_id field);

/***************************************************************************//**
 * Deaccesses fields from region and all child regions recursively.
 * Temporary until circular references sorted out - certain fields access
 * regions. Call ONLY before deaccessing root_region in cmzn_context.
 */
void cmzn_region_detach_fields_hierarchical(struct cmzn_region *region);

int cmzn_region_private_attach_any_object(struct cmzn_region *region,
	struct Any_object *any_object);
/*******************************************************************************
LAST MODIFIED : 1 October 2002

DESCRIPTION :
Adds <any_object> to the list of objects attached to <region>.
This function is only externally visible to context objects.
==============================================================================*/

int cmzn_region_private_detach_any_object(struct cmzn_region *region,
	struct Any_object *any_object);
/*******************************************************************************
LAST MODIFIED : 1 October 2002

DESCRIPTION :
Removes <any_object> from the list of objects attached to <region>.
Note that only in the case that <any_object> is the exact Any_object stored in
<region> may it be cleaned up. In any other case the <any_object> passed in
must be cleaned up by the calling function.
This function is only externally visible to context objects.
==============================================================================*/

struct LIST(Any_object) *
cmzn_region_private_get_any_object_list(struct cmzn_region *region);
/*******************************************************************************
LAST MODIFIED : 1 October 2002

DESCRIPTION :
Returns the list of objects, abstractly stored as struct Any_object from
<region>. It is important that this list not be modified directly.
This function is only externally visible to context objects.
==============================================================================*/

/** private for use by cmzn_fieldmodulenotifier */
void cmzn_region_add_fieldmodulenotifier(cmzn_region *region,
	cmzn_fieldmodulenotifier *notifier);

/** private for use by cmzn_fieldmodulenotifier */
void cmzn_region_remove_fieldmodulenotifier(cmzn_region *region,
	cmzn_fieldmodulenotifier *notifier);

/**
 * Returns pointer to context this region was created for. Can be NULL if
 * context is destroyed already during clean-up.
 *
 * @param region  The region to query.
 * @return  Non-accessed pointer to cmzn_context or NULL if none/cleared.
 */
cmzn_context *cmzn_region_get_context_private(cmzn_region *region);

/**
 * Record the context this region was created for.
 * Should only be set by context / region creation functions.
 */
void cmzn_region_set_context_private(cmzn_region *region, cmzn_context *context);

/**
 * Callback for changes to FE_region attached to region.
 * Updates definitions of Computed_field wrappers for changed FE_fields in the
 * region.
 * Ensures region has cmiss_number and xi fields, at the appropriate time.
 * Triggers computed field changes if not already changed.
 * @private  Should only be called from finite_element_region.cpp
 */
void cmzn_region_FE_region_change(cmzn_region *region);

#endif /* !defined (CMZN_REGION_PRIVATE_H) */
