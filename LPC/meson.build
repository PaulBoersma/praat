# meson.build of the library "LPC"
# David Weenink, 21 July 2025

sources = '''
	Cepstrum.cpp Cepstrumc.cpp Cepstrum_and_Spectrum.cpp
	Cepstrogram.cpp
	Formant_extensions.cpp
	FormantModeler.cpp FormantModelerList.cpp
	FormantPath.cpp FormantPath_to_IntervalTier.cpp FormantPathArea.cpp FormantPathEditor.cpp
	LineSpectralFrequencies.cpp LPCFrameAndLineSpectralFrequenciesFrame.cpp
	LPCFrameIntoSampledFrame.cpp LPCFrameIntoFormantFrame.cpp
	LPC.cpp
	LPC_and_Formant.cpp LPC_and_Cepstrumc.cpp
	LPC_and_LFCC.cpp
	LPC_and_LineSpectralFrequencies.cpp LPC_and_Polynomial.cpp
	LPC_to_Spectrum.cpp  LPC_to_Spectrogram.cpp
	LPC_and_Tube.cpp
	PowerCepstrum.cpp PowerCepstrumWorkspace.cpp PowerCepstrogram.cpp
	PowerCepstrogramFrameIntoMatrixFrame.cpp
	Roots_and_Formant.cpp
	SoundFrameIntoLPCFrame.cpp SoundFrameIntoPowerCepstrogramFrame.cpp SoundFrameIntoFormantFrame.cpp
	Sound_and_LPC.cpp Sound_to_Formant_mt.cpp
	Sound_and_Cepstrum.cpp Tube.cpp
	VocalTractTier.cpp
	praat_LPC_init.cpp manual_LPC.cpp'''.split()

libLPC = static_library (
	'LPC',
	sources : sources,
	dependencies : gtk_dep,
	include_directories : [dwsys_inc, dwtools_inc, fon_inc, foned_inc, kar_inc, melder_inc, stat_inc, sys_inc]
)

libLPC_dep = declare_dependency (
	include_directories : include_directories('.'), link_with: libLPC
)
