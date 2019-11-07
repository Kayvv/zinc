/*******************************************************************************
FILE : computed_field_trigonometry.c

LAST MODIFIED : 16 May 2008

DESCRIPTION :
Implements a number of basic component wise operations on computed fields.
==============================================================================*/
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#include <math.h>
#include "computed_field/computed_field.h"
#include "computed_field/computed_field_private.hpp"
#include "computed_field/computed_field_set.h"
#include "general/debug.h"
#include "general/mystring.h"
#include "general/message.h"
#include "computed_field/computed_field_trigonometry.h"

class Computed_field_trigonometry_package : public Computed_field_type_package
{
};

namespace {

const char computed_field_sin_type_string[] = "sin";

class Computed_field_sin : public Computed_field_core
{
public:
	Computed_field_sin() : Computed_field_core()
	{
	};

private:
	Computed_field_core *copy()
	{
		return new Computed_field_sin();
	}

	const char *get_type_string()
	{
		return(computed_field_sin_type_string);
	}

	int compare(Computed_field_core* other_field)
	{
		if (dynamic_cast<Computed_field_sin*>(other_field))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	int evaluate(cmzn_fieldcache& cache, FieldValueCache& inValueCache);

	int list();

	char* get_command_string();
};

int Computed_field_sin::evaluate(cmzn_fieldcache& cache, FieldValueCache& inValueCache)
{
	RealFieldValueCache &valueCache = RealFieldValueCache::cast(inValueCache);
	RealFieldValueCache *sourceCache = RealFieldValueCache::cast(getSourceField(0)->evaluate(cache));
	if (sourceCache)
	{
		for (int i = 0 ; i < field->number_of_components ; i++)
		{
			valueCache.values[i] = (FE_value)sin((double)(sourceCache->values[i]));
		}
		int number_of_xi = cache.getRequestedDerivatives();
		if (number_of_xi && sourceCache->derivatives_valid)
		{
			FE_value *derivative = valueCache.derivatives;
			for (int i = 0 ; i < field->number_of_components ; i++)
			{
				for (int j = 0 ; j < number_of_xi ; j++)
				{
					/* d(sin u)/dx = cos u * du/dx */
					*derivative =
						(FE_value)(cos((double)(sourceCache->values[i])) *
						sourceCache->derivatives[i * number_of_xi + j]);
					derivative++;
				}
			}
			valueCache.derivatives_valid = 1;
		}
		else
		{
			valueCache.derivatives_valid = 0;
		}
		return 1;
	}
	return 0;
}

int Computed_field_sin::list()
/*******************************************************************************
LAST MODIFIED : 24 August 2006

DESCRIPTION :
==============================================================================*/
{
	int return_code;

	ENTER(List_Computed_field_sin);
	if (field)
	{
		display_message(INFORMATION_MESSAGE,
			"    source field : %s\n",field->source_fields[0]->name);
		return_code = 1;
	}
	else
	{
		display_message(ERROR_MESSAGE,
			"list_Computed_field_sin.  Invalid field");
		return_code = 0;
	}
	LEAVE;

	return (return_code);
} /* list_Computed_field_sin */

char *Computed_field_sin::get_command_string()
/*******************************************************************************
LAST MODIFIED : 24 August 2006

DESCRIPTION :
Returns allocated command string for reproducing field. Includes type.
==============================================================================*/
{
	char *command_string, *field_name;
	int error;

	ENTER(Computed_field_sin::get_command_string);
	command_string = (char *)NULL;
	if (field)
	{
		error = 0;
		append_string(&command_string,
			computed_field_sin_type_string, &error);
		append_string(&command_string, " field ", &error);
		if (GET_NAME(Computed_field)(field->source_fields[0], &field_name))
		{
			make_valid_token(&field_name);
			append_string(&command_string, field_name, &error);
			DEALLOCATE(field_name);
		}
	}
	else
	{
		display_message(ERROR_MESSAGE,
			"Computed_field_sin::get_command_string.  Invalid field");
	}
	LEAVE;

	return (command_string);
} /* Computed_field_sin::get_command_string */

} //namespace

cmzn_field *cmzn_fieldmodule_create_field_sin(
	struct cmzn_fieldmodule *field_module,
	cmzn_field *source_field)
{
	cmzn_field_id field = 0;
	if (source_field && source_field->isNumerical())
	{
		field = Computed_field_create_generic(field_module,
			/*check_source_field_regions*/true,
			source_field->number_of_components,
			/*number_of_source_fields*/1, &source_field,
			/*number_of_source_values*/0, NULL,
			new Computed_field_sin());
	}
	return (field);
}

int Computed_field_get_type_sin(cmzn_field *field,
	cmzn_field **source_field)
/*******************************************************************************
LAST MODIFIED : 24 August 2006

DESCRIPTION :
If the field is of type COMPUTED_FIELD_SIN, the
<source_field> used by it are returned.
==============================================================================*/
{
	int return_code;

	ENTER(Computed_field_get_type_sin);
	if (field&&(dynamic_cast<Computed_field_sin*>(field->core)))
	{
		*source_field = field->source_fields[0];
		return_code=1;
	}
	else
	{
		display_message(ERROR_MESSAGE,
			"Computed_field_get_type_sin.  Invalid argument(s)");
		return_code = 0;
	}
	LEAVE;

	return (return_code);
} /* Computed_field_get_type_sin */

namespace {

const char computed_field_cos_type_string[] = "cos";

class Computed_field_cos : public Computed_field_core
{
public:
	Computed_field_cos() : Computed_field_core()
	{
	};

private:
	Computed_field_core *copy()
	{
		return new Computed_field_cos();
	}

	const char *get_type_string()
	{
		return(computed_field_cos_type_string);
	}

	int compare(Computed_field_core* other_field)
	{
		if (dynamic_cast<Computed_field_cos*>(other_field))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	int evaluate(cmzn_fieldcache& cache, FieldValueCache& inValueCache);

	int list();

	char* get_command_string();
};

int Computed_field_cos::evaluate(cmzn_fieldcache& cache, FieldValueCache& inValueCache)
{
	RealFieldValueCache &valueCache = RealFieldValueCache::cast(inValueCache);
	RealFieldValueCache *sourceCache = RealFieldValueCache::cast(getSourceField(0)->evaluate(cache));
	if (sourceCache)
	{
		for (int i = 0 ; i < field->number_of_components ; i++)
		{
			valueCache.values[i] =
				(FE_value)cos((double)(sourceCache->values[i]));
		}
		int number_of_xi = cache.getRequestedDerivatives();
		if (number_of_xi && sourceCache->derivatives_valid)
		{
			FE_value *derivative = valueCache.derivatives;
			for (int i = 0 ; i < field->number_of_components ; i++)
			{
				for (int j = 0 ; j < number_of_xi ; j++)
				{
					/* d(cos u)/dx = -sin u * du/dx */
					*derivative =
						(FE_value)(-sin((double)(sourceCache->values[i])) *
						sourceCache->derivatives[i * number_of_xi + j]);
					derivative++;
				}
			}
			valueCache.derivatives_valid = 1;
		}
		else
		{
			valueCache.derivatives_valid = 0;
		}
		return 1;
	}
	return 0;
}

int Computed_field_cos::list()
/*******************************************************************************
LAST MODIFIED : 24 August 2006

DESCRIPTION :
==============================================================================*/
{
	int return_code;

	ENTER(List_Computed_field_cos);
	if (field)
	{
		display_message(INFORMATION_MESSAGE,
			"    source field : %s\n",field->source_fields[0]->name);
		return_code = 1;
	}
	else
	{
		display_message(ERROR_MESSAGE,
			"list_Computed_field_cos.  Invalid field");
		return_code = 0;
	}
	LEAVE;

	return (return_code);
} /* list_Computed_field_cos */

char *Computed_field_cos::get_command_string()
/*******************************************************************************
LAST MODIFIED : 24 August 2006

DESCRIPTION :
Returns allocated command string for reproducing field. Includes type.
==============================================================================*/
{
	char *command_string, *field_name;
	int error;

	ENTER(Computed_field_cos::get_command_string);
	command_string = (char *)NULL;
	if (field)
	{
		error = 0;
		append_string(&command_string,
			computed_field_cos_type_string, &error);
		append_string(&command_string, " field ", &error);
		if (GET_NAME(Computed_field)(field->source_fields[0], &field_name))
		{
			make_valid_token(&field_name);
			append_string(&command_string, field_name, &error);
			DEALLOCATE(field_name);
		}
	}
	else
	{
		display_message(ERROR_MESSAGE,
			"Computed_field_cos::get_command_string.  Invalid field");
	}
	LEAVE;

	return (command_string);
} /* Computed_field_cos::get_command_string */

} //namespace

cmzn_field *cmzn_fieldmodule_create_field_cos(
	struct cmzn_fieldmodule *field_module,
	cmzn_field *source_field)
{
	cmzn_field_id field = 0;
	if (source_field && source_field->isNumerical())
	{
		field = Computed_field_create_generic(field_module,
			/*check_source_field_regions*/true,
			source_field->number_of_components,
			/*number_of_source_fields*/1, &source_field,
			/*number_of_source_values*/0, NULL,
			new Computed_field_cos());
	}
	return (field);
}

int Computed_field_get_type_cos(cmzn_field *field,
	cmzn_field **source_field)
/*******************************************************************************
LAST MODIFIED : 24 August 2006

DESCRIPTION :
If the field is of type COMPUTED_FIELD_COS, the
<source_field> used by it are returned.
==============================================================================*/
{
	int return_code;

	ENTER(Computed_field_get_type_cos);
	if (field&&(dynamic_cast<Computed_field_cos*>(field->core)))
	{
		*source_field = field->source_fields[0];
		return_code=1;
	}
	else
	{
		display_message(ERROR_MESSAGE,
			"Computed_field_get_type_cos.  Invalid argument(s)");
		return_code = 0;
	}
	LEAVE;

	return (return_code);
} /* Computed_field_get_type_cos */

namespace {

const char computed_field_tan_type_string[] = "tan";

class Computed_field_tan : public Computed_field_core
{
public:
	Computed_field_tan() : Computed_field_core()
	{
	};

private:
	Computed_field_core *copy()
	{
		return new Computed_field_tan();
	}

	const char *get_type_string()
	{
		return(computed_field_tan_type_string);
	}

	int compare(Computed_field_core* other_field)
	{
		if (dynamic_cast<Computed_field_tan*>(other_field))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	int evaluate(cmzn_fieldcache& cache, FieldValueCache& inValueCache);

	int list();

	char* get_command_string();
};

int Computed_field_tan::evaluate(cmzn_fieldcache& cache, FieldValueCache& inValueCache)
{
	RealFieldValueCache &valueCache = RealFieldValueCache::cast(inValueCache);
	RealFieldValueCache *sourceCache = RealFieldValueCache::cast(getSourceField(0)->evaluate(cache));
	if (sourceCache)
	{
		/* 2. Calculate the field */
		for (int i = 0 ; i < field->number_of_components ; i++)
		{
			valueCache.values[i] =
				(FE_value)tan((double)(sourceCache->values[i]));
		}
		int number_of_xi = cache.getRequestedDerivatives();
		if (number_of_xi && sourceCache->derivatives_valid)
		{
			FE_value *derivative = valueCache.derivatives;
			for (int i = 0 ; i < field->number_of_components ; i++)
			{
				for (int j = 0 ; j < number_of_xi ; j++)
				{
					/* d(tan u)/dx = sec^2 u * du/dx */
					*derivative = (FE_value)(sourceCache->derivatives[i * number_of_xi + j] /
						(cos((double)(sourceCache->values[i])) *
						cos((double)(sourceCache->values[i]))));
					derivative++;
				}
			}
			valueCache.derivatives_valid = 1;
		}
		else
		{
			valueCache.derivatives_valid = 0;
		}
		return 1;
	}
	return 0;
}

int Computed_field_tan::list()
/*******************************************************************************
LAST MODIFIED : 24 August 2006

DESCRIPTION :
==============================================================================*/
{
	int return_code;

	ENTER(List_Computed_field_tan);
	if (field)
	{
		display_message(INFORMATION_MESSAGE,
			"    source field : %s\n",field->source_fields[0]->name);
		return_code = 1;
	}
	else
	{
		display_message(ERROR_MESSAGE,
			"list_Computed_field_tan.  Invalid field");
		return_code = 0;
	}
	LEAVE;

	return (return_code);
} /* list_Computed_field_tan */

char *Computed_field_tan::get_command_string()
/*******************************************************************************
LAST MODIFIED : 24 August 2006

DESCRIPTION :
Returns allocated command string for reproducing field. Includes type.
==============================================================================*/
{
	char *command_string, *field_name;
	int error;

	ENTER(Computed_field_tan::get_command_string);
	command_string = (char *)NULL;
	if (field)
	{
		error = 0;
		append_string(&command_string,
			computed_field_tan_type_string, &error);
		append_string(&command_string, " field ", &error);
		if (GET_NAME(Computed_field)(field->source_fields[0], &field_name))
		{
			make_valid_token(&field_name);
			append_string(&command_string, field_name, &error);
			DEALLOCATE(field_name);
		}
	}
	else
	{
		display_message(ERROR_MESSAGE,
			"Computed_field_tan::get_command_string.  Invalid field");
	}
	LEAVE;

	return (command_string);
} /* Computed_field_tan::get_command_string */

} //namespace

cmzn_field *cmzn_fieldmodule_create_field_tan(
	struct cmzn_fieldmodule *field_module,
	cmzn_field *source_field)
{
	cmzn_field_id field = 0;
	if (source_field && source_field->isNumerical())
	{
		field = Computed_field_create_generic(field_module,
			/*check_source_field_regions*/true,
			source_field->number_of_components,
			/*number_of_source_fields*/1, &source_field,
			/*number_of_source_values*/0, NULL,
			new Computed_field_tan());
	}
	return (field);
}

int Computed_field_get_type_tan(cmzn_field *field,
	cmzn_field **source_field)
/*******************************************************************************
LAST MODIFIED : 24 August 2006

DESCRIPTION :
If the field is of type COMPUTED_FIELD_TAN, the
<source_field> used by it are returned.
==============================================================================*/
{
	int return_code;

	ENTER(Computed_field_get_type_tan);
	if (field&&(dynamic_cast<Computed_field_tan*>(field->core)))
	{
		*source_field = field->source_fields[0];
		return_code=1;
	}
	else
	{
		display_message(ERROR_MESSAGE,
			"Computed_field_get_type_tan.  Invalid argument(s)");
		return_code = 0;
	}
	LEAVE;

	return (return_code);
} /* Computed_field_get_type_tan */

namespace {

const char computed_field_asin_type_string[] = "asin";

class Computed_field_asin : public Computed_field_core
{
public:
	Computed_field_asin() : Computed_field_core()
	{
	};

private:
	Computed_field_core *copy()
	{
		return new Computed_field_asin();
	}

	const char *get_type_string()
	{
		return(computed_field_asin_type_string);
	}

	int compare(Computed_field_core* other_field)
	{
		if (dynamic_cast<Computed_field_asin*>(other_field))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	int evaluate(cmzn_fieldcache& cache, FieldValueCache& inValueCache);

	int list();

	char* get_command_string();
};

int Computed_field_asin::evaluate(cmzn_fieldcache& cache, FieldValueCache& inValueCache)
{
	RealFieldValueCache &valueCache = RealFieldValueCache::cast(inValueCache);
	RealFieldValueCache *sourceCache = RealFieldValueCache::cast(getSourceField(0)->evaluate(cache));
	if (sourceCache)
	{
		for (int i = 0 ; i < field->number_of_components ; i++)
		{
			valueCache.values[i] = (FE_value)asin((double)(sourceCache->values[i]));
		}
		int number_of_xi = cache.getRequestedDerivatives();
		if (number_of_xi && sourceCache->derivatives_valid)
		{
			FE_value *derivative = valueCache.derivatives;
			for (int i = 0 ; i < field->number_of_components ; i++)
			{
				for (int j = 0 ; j < number_of_xi ; j++)
				{
					/* d(asin u)/dx = 1.0/sqrt(1.0 - u^2) * du/dx */
					if (sourceCache->values[i] != 1.0)
					{
						*derivative =
							(FE_value)(sourceCache->derivatives[i * number_of_xi + j] /
							sqrt(1.0 - ((double)sourceCache->values[i] *
							(double)sourceCache->values[i])));
					}
					else
					{
						*derivative = 0.0;
					}
					derivative++;
				}
			}
			valueCache.derivatives_valid = 1;
		}
		else
		{
			valueCache.derivatives_valid = 0;
		}
		return 1;
	}
	return 0;
}

int Computed_field_asin::list()
/*******************************************************************************
LAST MODIFIED : 24 August 2006

DESCRIPTION :
==============================================================================*/
{
	int return_code;

	ENTER(List_Computed_field_asin);
	if (field)
	{
		display_message(INFORMATION_MESSAGE,
			"    source field : %s\n",field->source_fields[0]->name);
		return_code = 1;
	}
	else
	{
		display_message(ERROR_MESSAGE,
			"list_Computed_field_asin.  Invalid field");
		return_code = 0;
	}
	LEAVE;

	return (return_code);
} /* list_Computed_field_asin */

char *Computed_field_asin::get_command_string()
/*******************************************************************************
LAST MODIFIED : 24 August 2006

DESCRIPTION :
Returns allocated command string for reproducing field. Includes type.
==============================================================================*/
{
	char *command_string, *field_name;
	int error;

	ENTER(Computed_field_asin::get_command_string);
	command_string = (char *)NULL;
	if (field)
	{
		error = 0;
		append_string(&command_string,
			computed_field_asin_type_string, &error);
		append_string(&command_string, " field ", &error);
		if (GET_NAME(Computed_field)(field->source_fields[0], &field_name))
		{
			make_valid_token(&field_name);
			append_string(&command_string, field_name, &error);
			DEALLOCATE(field_name);
		}
	}
	else
	{
		display_message(ERROR_MESSAGE,
			"Computed_field_asin::get_command_string.  Invalid field");
	}
	LEAVE;

	return (command_string);
} /* Computed_field_asin::get_command_string */

} //namespace

cmzn_field *cmzn_fieldmodule_create_field_asin(
	struct cmzn_fieldmodule *field_module,
	cmzn_field *source_field)
{
	cmzn_field_id field = 0;
	if (source_field && source_field->isNumerical())
	{
		field = Computed_field_create_generic(field_module,
			/*check_source_field_regions*/true,
			source_field->number_of_components,
			/*number_of_source_fields*/1, &source_field,
			/*number_of_source_values*/0, NULL,
			new Computed_field_asin());
	}
	return (field);
}

int Computed_field_get_type_asin(cmzn_field *field,
	cmzn_field **source_field)
/*******************************************************************************
LAST MODIFIED : 24 August 2006

DESCRIPTION :
If the field is of type COMPUTED_FIELD_ASIN, the
<source_field> used by it are returned.
==============================================================================*/
{
	int return_code;

	ENTER(Computed_field_get_type_asin);
	if (field&&(dynamic_cast<Computed_field_asin*>(field->core)))
	{
		*source_field = field->source_fields[0];
		return_code=1;
	}
	else
	{
		display_message(ERROR_MESSAGE,
			"Computed_field_get_type_asin.  Invalid argument(s)");
		return_code = 0;
	}
	LEAVE;

	return (return_code);
} /* Computed_field_get_type_asin */

namespace {

const char computed_field_acos_type_string[] = "acos";

class Computed_field_acos : public Computed_field_core
{
public:
	Computed_field_acos() : Computed_field_core()
	{
	};

private:
	Computed_field_core *copy()
	{
		return new Computed_field_acos();
	}

	const char *get_type_string()
	{
		return(computed_field_acos_type_string);
	}

	int compare(Computed_field_core* other_field)
	{
		if (dynamic_cast<Computed_field_acos*>(other_field))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	int evaluate(cmzn_fieldcache& cache, FieldValueCache& inValueCache);

	int list();

	char* get_command_string();
};

int Computed_field_acos::evaluate(cmzn_fieldcache& cache, FieldValueCache& inValueCache)
{
	RealFieldValueCache &valueCache = RealFieldValueCache::cast(inValueCache);
	RealFieldValueCache *sourceCache = RealFieldValueCache::cast(getSourceField(0)->evaluate(cache));
	if (sourceCache)
	{
		for (int i = 0 ; i < field->number_of_components ; i++)
		{
			valueCache.values[i] = (FE_value)acos((double)(sourceCache->values[i]));
		}
		int number_of_xi = cache.getRequestedDerivatives();
		if (number_of_xi && sourceCache->derivatives_valid)
		{
			FE_value *derivative = valueCache.derivatives;
			for (int i = 0 ; i < field->number_of_components ; i++)
			{
				for (int j = 0 ; j < number_of_xi ; j++)
				{
					/* d(asin u)/dx = -1.0/sqrt(1.0 - u^2) * du/dx */
					if (sourceCache->values[i] != 1.0)
					{
						*derivative =
							(FE_value)(-sourceCache->derivatives[i * number_of_xi + j] /
							sqrt(1.0 - ((double)sourceCache->values[i] *
							(double)sourceCache->values[i])));
					}
					else
					{
						*derivative = 0.0;
					}
					derivative++;
				}
			}
			valueCache.derivatives_valid = 1;
		}
		else
		{
			valueCache.derivatives_valid = 0;
		}
		return 1;
	}
	return 0;
}

int Computed_field_acos::list()
/*******************************************************************************
LAST MODIFIED : 24 August 2006

DESCRIPTION :
==============================================================================*/
{
	int return_code;

	ENTER(List_Computed_field_acos);
	if (field)
	{
		display_message(INFORMATION_MESSAGE,
			"    source field : %s\n",field->source_fields[0]->name);
		return_code = 1;
	}
	else
	{
		display_message(ERROR_MESSAGE,
			"list_Computed_field_acos.  Invalid field");
		return_code = 0;
	}
	LEAVE;

	return (return_code);
} /* list_Computed_field_acos */

char *Computed_field_acos::get_command_string()
/*******************************************************************************
LAST MODIFIED : 24 August 2006

DESCRIPTION :
Returns allocated command string for reproducing field. Includes type.
==============================================================================*/
{
	char *command_string, *field_name;
	int error;

	ENTER(Computed_field_acos::get_command_string);
	command_string = (char *)NULL;
	if (field)
	{
		error = 0;
		append_string(&command_string,
			computed_field_acos_type_string, &error);
		append_string(&command_string, " field ", &error);
		if (GET_NAME(Computed_field)(field->source_fields[0], &field_name))
		{
			make_valid_token(&field_name);
			append_string(&command_string, field_name, &error);
			DEALLOCATE(field_name);
		}
	}
	else
	{
		display_message(ERROR_MESSAGE,
			"Computed_field_acos::get_command_string.  Invalid field");
	}
	LEAVE;

	return (command_string);
} /* Computed_field_acos::get_command_string */

} //namespace

cmzn_field *cmzn_fieldmodule_create_field_acos(
		struct cmzn_fieldmodule *field_module,
	cmzn_field *source_field)
{
	cmzn_field_id field = 0;
	if (source_field && source_field->isNumerical())
	{
		field = Computed_field_create_generic(field_module,
			/*check_source_field_regions*/true,
			source_field->number_of_components,
			/*number_of_source_fields*/1, &source_field,
			/*number_of_source_values*/0, NULL,
			new Computed_field_acos());
	}
	return (field);
}

int Computed_field_get_type_acos(cmzn_field *field,
	cmzn_field **source_field)
/*******************************************************************************
LAST MODIFIED : 24 August 2006

DESCRIPTION :
If the field is of type COMPUTED_FIELD_ACOS, the
<source_field> used by it are returned.
==============================================================================*/
{
	int return_code;

	ENTER(Computed_field_get_type_acos);
	if (field&&(dynamic_cast<Computed_field_acos*>(field->core)))
	{
		*source_field = field->source_fields[0];
		return_code=1;
	}
	else
	{
		display_message(ERROR_MESSAGE,
			"Computed_field_get_type_acos.  Invalid argument(s)");
		return_code = 0;
	}
	LEAVE;

	return (return_code);
} /* Computed_field_get_type_acos */

namespace {

const char computed_field_atan_type_string[] = "atan";

class Computed_field_atan : public Computed_field_core
{
public:
	Computed_field_atan() : Computed_field_core()
	{
	};

private:
	Computed_field_core *copy()
	{
		return new Computed_field_atan();
	}

	const char *get_type_string()
	{
		return(computed_field_atan_type_string);
	}

	int compare(Computed_field_core* other_field)
	{
		if (dynamic_cast<Computed_field_atan*>(other_field))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	int evaluate(cmzn_fieldcache& cache, FieldValueCache& inValueCache);

	int list();

	char* get_command_string();
};

int Computed_field_atan::evaluate(cmzn_fieldcache& cache, FieldValueCache& inValueCache)
{
	RealFieldValueCache &valueCache = RealFieldValueCache::cast(inValueCache);
	RealFieldValueCache *sourceCache = RealFieldValueCache::cast(getSourceField(0)->evaluate(cache));
	if (sourceCache)
	{
		for (int i = 0 ; i < field->number_of_components ; i++)
		{
			valueCache.values[i] = (FE_value)atan((double)(sourceCache->values[i]));
		}
		int number_of_xi = cache.getRequestedDerivatives();
		if (number_of_xi && sourceCache->derivatives_valid)
		{
			FE_value *derivative = valueCache.derivatives;
			for (int i = 0 ; i < field->number_of_components ; i++)
			{
				for (int j = 0 ; j < number_of_xi ; j++)
				{
					/* d(atan u)/dx = 1.0/(1.0 + u^2) * du/dx */
					*derivative =
						(FE_value)(sourceCache->derivatives[i * number_of_xi + j] /
						(1.0 + ((double)sourceCache->values[i] *
						(double)sourceCache->values[i])));
					derivative++;
				}
			}
			valueCache.derivatives_valid = 1;
		}
		else
		{
			valueCache.derivatives_valid = 0;
		}
		return 1;
	}
	return 0;
}

int Computed_field_atan::list()
/*******************************************************************************
LAST MODIFIED : 24 August 2006

DESCRIPTION :
==============================================================================*/
{
	int return_code;

	ENTER(List_Computed_field_atan);
	if (field)
	{
		display_message(INFORMATION_MESSAGE,
			"    source field : %s\n",field->source_fields[0]->name);
		return_code = 1;
	}
	else
	{
		display_message(ERROR_MESSAGE,
			"list_Computed_field_atan.  Invalid field");
		return_code = 0;
	}
	LEAVE;

	return (return_code);
} /* list_Computed_field_atan */

char *Computed_field_atan::get_command_string()
/*******************************************************************************
LAST MODIFIED : 24 August 2006

DESCRIPTION :
Returns allocated command string for reproducing field. Includes type.
==============================================================================*/
{
	char *command_string, *field_name;
	int error;

	ENTER(Computed_field_atan::get_command_string);
	command_string = (char *)NULL;
	if (field)
	{
		error = 0;
		append_string(&command_string,
			computed_field_atan_type_string, &error);
		append_string(&command_string, " field ", &error);
		if (GET_NAME(Computed_field)(field->source_fields[0], &field_name))
		{
			make_valid_token(&field_name);
			append_string(&command_string, field_name, &error);
			DEALLOCATE(field_name);
		}
	}
	else
	{
		display_message(ERROR_MESSAGE,
			"Computed_field_atan::get_command_string.  Invalid field");
	}
	LEAVE;

	return (command_string);
} /* Computed_field_atan::get_command_string */

} //namespace

cmzn_field *cmzn_fieldmodule_create_field_atan(
		struct cmzn_fieldmodule *field_module,
	cmzn_field *source_field)
{
	cmzn_field_id field = 0;
	if (source_field && source_field->isNumerical())
	{
		field = Computed_field_create_generic(field_module,
			/*check_source_field_regions*/true,
			source_field->number_of_components,
			/*number_of_source_fields*/1, &source_field,
			/*number_of_source_values*/0, NULL,
			new Computed_field_atan());
	}
	return (field);
}

int Computed_field_get_type_atan(cmzn_field *field,
	cmzn_field **source_field)
/*******************************************************************************
LAST MODIFIED : 24 August 2006

DESCRIPTION :
If the field is of type COMPUTED_FIELD_ATAN, the
<source_field> used by it are returned.
==============================================================================*/
{
	int return_code;

	ENTER(Computed_field_get_type_atan);
	if (field&&(dynamic_cast<Computed_field_atan*>(field->core)))
	{
		*source_field = field->source_fields[0];
		return_code=1;
	}
	else
	{
		display_message(ERROR_MESSAGE,
			"Computed_field_get_type_atan.  Invalid argument(s)");
		return_code = 0;
	}
	LEAVE;

	return (return_code);
} /* Computed_field_get_type_atan */

namespace {

const char computed_field_atan2_type_string[] = "atan2";

class Computed_field_atan2 : public Computed_field_core
{
public:
	Computed_field_atan2() : Computed_field_core()
	{
	};

private:
	Computed_field_core *copy()
	{
		return new Computed_field_atan2();
	}

	const char *get_type_string()
	{
		return(computed_field_atan2_type_string);
	}

	int compare(Computed_field_core* other_field)
	{
		if (dynamic_cast<Computed_field_atan2*>(other_field))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	int evaluate(cmzn_fieldcache& cache, FieldValueCache& inValueCache);

	int list();

	char* get_command_string();
};

int Computed_field_atan2::evaluate(cmzn_fieldcache& cache, FieldValueCache& inValueCache)
{
	RealFieldValueCache &valueCache = RealFieldValueCache::cast(inValueCache);
	RealFieldValueCache *source1Cache = RealFieldValueCache::cast(getSourceField(0)->evaluate(cache));
	RealFieldValueCache *source2Cache = RealFieldValueCache::cast(getSourceField(1)->evaluate(cache));
	if (source1Cache && source2Cache)
	{
		for (int i = 0 ; i < field->number_of_components ; i++)
		{
			valueCache.values[i] =
				(FE_value)atan2((double)(source1Cache->values[i]),
				(double)(source2Cache->values[i]));
		}
		int number_of_xi = cache.getRequestedDerivatives();
		if (number_of_xi && source1Cache->derivatives_valid && source2Cache->derivatives_valid)
		{
			FE_value *derivative = valueCache.derivatives;
			for (int i = 0 ; i < field->number_of_components ; i++)
			{
				for (int j = 0 ; j < number_of_xi ; j++)
				{
					/* d(atan (u/v))/dx =  ( v * du/dx - u * dv/dx ) / ( v^2 * u^2 ) */
					FE_value u = source1Cache->values[i];
					FE_value v = source2Cache->values[i];
					*derivative =
						(FE_value)(v * source1Cache->derivatives[i * number_of_xi + j] -
						u * source2Cache->derivatives[i * number_of_xi + j]) /
						(u * u + v * v);
					derivative++;
				}
			}
			valueCache.derivatives_valid = 1;
		}
		else
		{
			valueCache.derivatives_valid = 0;
		}
		return 1;
	}
	return 0;
}

int Computed_field_atan2::list()
/*******************************************************************************
LAST MODIFIED : 24 August 2006

DESCRIPTION :
==============================================================================*/
{
	int return_code;

	ENTER(List_Computed_field_atan2);
	if (field)
	{
		display_message(INFORMATION_MESSAGE,
			"    source fields : %s %s\n",field->source_fields[0]->name,
			field->source_fields[1]->name);
		return_code = 1;
	}
	else
	{
		display_message(ERROR_MESSAGE,
			"list_Computed_field_atan2.  Invalid field");
		return_code = 0;
	}
	LEAVE;

	return (return_code);
} /* list_Computed_field_atan2 */

char *Computed_field_atan2::get_command_string()
/*******************************************************************************
LAST MODIFIED : 24 August 2006

DESCRIPTION :
Returns allocated command string for reproducing field. Includes type.
==============================================================================*/
{
	char *command_string, *field_name;
	int error;

	ENTER(Computed_field_atan2::get_command_string);
	command_string = (char *)NULL;
	if (field)
	{
		error = 0;
		append_string(&command_string,
			computed_field_atan2_type_string, &error);
		append_string(&command_string, " fields ", &error);
		if (GET_NAME(Computed_field)(field->source_fields[0], &field_name))
		{
			make_valid_token(&field_name);
			append_string(&command_string, field_name, &error);
			DEALLOCATE(field_name);
		}
		if (GET_NAME(Computed_field)(field->source_fields[1], &field_name))
		{
			make_valid_token(&field_name);
			append_string(&command_string, " ", &error);
			append_string(&command_string, field_name, &error);
			DEALLOCATE(field_name);
		}
	}
	else
	{
		display_message(ERROR_MESSAGE,
			"Computed_field_atan2::get_command_string.  Invalid field");
	}
	LEAVE;

	return (command_string);
} /* Computed_field_atan2::get_command_string */

} //namespace

cmzn_field *cmzn_fieldmodule_create_field_atan2(
	struct cmzn_fieldmodule *field_module,
	cmzn_field *source_field_one,
	cmzn_field *source_field_two)
{
	cmzn_field *field = NULL;
	if (source_field_one && source_field_one->isNumerical() &&
		source_field_two && source_field_two->isNumerical() &&
		(source_field_one->number_of_components ==
			source_field_two->number_of_components))
	{
		cmzn_field *source_fields[2];
		source_fields[0] = source_field_one;
		source_fields[1] = source_field_two;
		field = Computed_field_create_generic(field_module,
			/*check_source_field_regions*/true,
			source_field_one->number_of_components,
			/*number_of_source_fields*/2, source_fields,
			/*number_of_source_values*/0, NULL,
			new Computed_field_atan2());
	}
	else
	{
		display_message(ERROR_MESSAGE,
			"cmzn_fieldmodule_create_field_atan2.  Invalid argument(s)");
	}

	return (field);
}

int Computed_field_get_type_atan2(cmzn_field *field,
	cmzn_field **source_field_one,
	cmzn_field **source_field_two)
/*******************************************************************************
LAST MODIFIED : 24 August 2006

DESCRIPTION :
If the field is of type COMPUTED_FIELD_ATAN2, the
<source_field_one> and <source_field_two> used by it are returned.
==============================================================================*/
{
	int return_code;

	ENTER(Computed_field_get_type_atan2);
	if (field&&(dynamic_cast<Computed_field_atan2*>(field->core)))
	{
		*source_field_one = field->source_fields[0];
		*source_field_two = field->source_fields[1];
		return_code=1;
	}
	else
	{
		display_message(ERROR_MESSAGE,
			"Computed_field_get_type_atan2.  Invalid argument(s)");
		return_code = 0;
	}
	LEAVE;

	return (return_code);
} /* Computed_field_get_type_atan2 */

