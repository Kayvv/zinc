/***************************************************************************//**
 * FILE : timekeeper.hpp
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_TIMEKEEPER_HPP__
#define CMZN_TIMEKEEPER_HPP__

#include "zinc/timekeeper.h"
#include "zinc/timenotifier.hpp"

namespace OpenCMISS
{
namespace Zinc
{

class Timekeeper
{
protected:
	cmzn_timekeeper_id id;

public:

	Timekeeper() : id(0)
	{   }

	// takes ownership of C handle, responsibility for destroying it
	explicit Timekeeper(cmzn_timekeeper_id in_timekeeper_id) :
		id(in_timekeeper_id)
	{  }

	Timekeeper(const Timekeeper& timeKeeper) :
		id(cmzn_timekeeper_access(timeKeeper.id))
	{  }

	Timekeeper& operator=(const Timekeeper& timeKeeper)
	{
		cmzn_timekeeper_id temp_id = cmzn_timekeeper_access(timeKeeper.id);
		if (0 != id)
		{
			cmzn_timekeeper_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Timekeeper()
	{
		if (0 != id)
		{
			cmzn_timekeeper_destroy(&id);
		}
	}

	bool isValid()
	{
		return (0 != id);
	}

	cmzn_timekeeper_id getId()
	{
		return id;
	}

	Timenotifier createTimenotifierRegular(double updateFrequency, double timeOffset)
	{
		return Timenotifier(cmzn_timekeeper_create_timenotifier_regular(
			id, updateFrequency, timeOffset));
	}

	int addTimenotifier(Timenotifier timenotifier)
	{
		return cmzn_timekeeper_add_timenotifier(id, timenotifier.getId());
	}

	int removeTimenotifier(Timenotifier timenotifier)
	{
		return cmzn_timekeeper_remove_timenotifier(id, timenotifier.getId());
	}

	double getMaximumTime()
	{
		return cmzn_timekeeper_get_maximum_time(id);
	}

	int setMaximumTime(double maximumTime)
	{
		return cmzn_timekeeper_set_maximum_time(id, maximumTime);
	}

	double getMinimumTime()
	{
		return cmzn_timekeeper_get_minimum_time(id);
	}

	int setMinimumTime(double minimumTime)
	{
		return cmzn_timekeeper_set_minimum_time(id, minimumTime);
	}

	double getTime()
	{
		return cmzn_timekeeper_get_time(id);
	}

	int setTime(double time)
	{
		return cmzn_timekeeper_set_time(id, time);
	}

};

}  // namespace Zinc
}

#endif
