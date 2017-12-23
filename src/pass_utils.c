#include "pass_utils.h"

void observe_orbit_at(const predict_observer_t *observer, const predict_orbital_elements_t *orbital_elements, predict_julian_date_t curr_time, struct predict_observation *observation)
{
	struct predict_position position;
	predict_orbit(orbital_elements, &position, curr_time);
	predict_observe_orbit(observer, &position, observation);
}

predict_julian_date_t step_pass(const predict_observer_t *observer, const predict_orbital_elements_t *orbital_elements, predict_julian_date_t curr_time, enum step_pass_direction direction)
{
	struct predict_observation observation;
	observe_orbit_at(observer, orbital_elements, curr_time, &observation);

	if (observation.elevation < 0) {
		return curr_time;
	}

	return 0;
}

predict_julian_date_t step_until_pass(const predict_observer_t *observer, const predict_orbital_elements_t *orbital_elements, predict_julian_date_t curr_time, enum step_pass_direction direction)
{
	struct predict_observation observation;
	observe_orbit_at(observer, orbital_elements, curr_time, &observation);

	if (observation.elevation > 0) {
		return curr_time;
	}

	double mean_period = 1.0/orbital_elements->mean_motion;

	const int NUM_POINTS = 4;
	double step = mean_period/NUM_POINTS;
	struct predict_observation observations[NUM_POINTS];
	bool found_pass = false;
	while (!found_pass) {
		for (int i=0; i < NUM_POINTS; i++) {
			curr_time += step;
			observe_orbit_at(observer, orbital_elements, curr_time, &observations[i]);
		}
		found_pass = true;

	}

	return 0;
}

bool aoslos_is_feasible(const predict_observer_t *observer, const predict_orbital_elements_t *orbital_elements, predict_julian_date_t start_utc)
{
	struct predict_position orbit;
	predict_orbit(orbital_elements, &orbit, start_utc);
	return !predict_is_geosynchronous(orbital_elements) && predict_aos_happens(orbital_elements, observer->latitude) && !orbit.decayed;
}

struct predict_observation next_aos_los(const predict_observer_t *observer, const predict_orbital_elements_t *orbital_elements, predict_julian_date_t start_utc, enum aos_los_search search_type)
{
	struct predict_observation observation = {0};
	if (!aoslos_is_feasible(observer, orbital_elements, start_utc)) {
		return observation;
	}

	struct predict_position orbit = {0};
	predict_orbit(orbital_elements, &orbit, start_utc);
	predict_observe_orbit(observer, &orbit, &observation);

	//find lower and upper brackets for solution
	predict_julian_date_t lower_bracket;
	predict_julian_date_t upper_bracket;

	if (search_type == AOS_SEARCH) {
		if (observation.elevation > 0) {
			lower_bracket = step_pass(observer, orbital_elements, start_utc, POSITIVE_DIRECTION);
		} else {
			lower_bracket = start_utc;
		}
		upper_bracket = step_until_pass(observer, orbital_elements, start_utc, POSITIVE_DIRECTION);
	} else {
		if (observation.elevation > 0) {
			lower_bracket = start_utc;
		} else {
			lower_bracket = step_until_pass(observer, orbital_elements, start_utc, POSITIVE_DIRECTION);
		}
		upper_bracket = step_pass(observer, orbital_elements, start_utc, POSITIVE_DIRECTION);
	}

	predict_orbit(orbital_elements, &orbit, lower_bracket + (upper_bracket - lower_bracket)/2.0);
	predict_observe_orbit(observer, &orbit, &observation);

	return observation;
}
