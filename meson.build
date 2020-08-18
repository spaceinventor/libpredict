project('predict', 'c')

predict_deps = []

predict_src = files([
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

predict_inc = include_directories('include')

predict_lib = static_library('predict',
	sources: predict_src,
	include_directories : predict_inc,
	dependencies : predict_deps,
)

predict_dep = declare_dependency(
	include_directories : predict_inc,
	link_with : predict_lib,
)


predict_dep_whole = declare_dependency(
	include_directories : predict_inc,
	link_whole : predict_lib,
)
