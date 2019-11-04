project('libpredict', 'c')

libpredict_deps = []

libpredict_src = files([
    'src/julian_date.c',
    'src/moon.c',
    'src/observer.c',
    'src/orbit.c',
    'src/refraction.c',
    'src/sdp4.c',
    'src/sgp4.c',
    'src/sun.c',
    'src/unsorted.c',
    'src/version.c',
])

libpredict_inc = include_directories('include')

libpredict_lib = static_library('libpredict',
	sources: libpredict_src,
	include_directories : libpredict_inc,
	dependencies : libpredict_deps,
)

libpredict_dep = declare_dependency(
	include_directories : libpredict_inc,
	link_whole : libpredict_lib,
)
