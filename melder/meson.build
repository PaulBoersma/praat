# meson.build of the library "melder"
# David Weenink & Paul Boersma, 31 July 2025

sources = '''
	melder.cpp complex.cpp melder_alloc.cpp melder_str32.cpp melder_colour.cpp
	melder_ftoa.cpp melder_console.cpp melder_atof.cpp melder_files.cpp
	melder_sort.cpp MelderString.cpp melder_textencoding.cpp melder_debug.cpp MelderFile.cpp MelderCat.cpp
	melder_search.cpp MelderThread.cpp
	melder_info.cpp melder_error.cpp melder_warning.cpp melder_progress.cpp
	melder_play.cpp melder_help.cpp melder_time.cpp
	melder_audio.cpp melder_audiofiles.cpp melder_quantity.cpp MelderReadText.cpp melder_tensorio.cpp
	melder_trust.cpp
	abcio.cpp melder_sysenv.cpp melder_app.cpp regularExp.cpp
	NUMmath.cpp
	NUMspecfunc.cpp NUMear.cpp NUMinterpol.cpp NUMmetrics.cpp NUMrandom.cpp NUMfilter.cpp NUMlinprog.cpp
	NUM.cpp STR.cpp VEC.cpp MAT.cpp STRVEC.cpp'''.split()

libmelder = static_library (
	'melder',
	sources : sources,
	dependencies : gtk_dep,
	include_directories : [sys_inc]
)

libmelder_dep = declare_dependency (
	include_directories : include_directories('.'), link_with: libmelder
)
