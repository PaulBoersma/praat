# Makefile of the library "LPC"
# David Weenink and Paul Boersma, 29 July 2025

include ../makefile.defs

CPPFLAGS = -I . -I ../kar -I ../melder -I ../fon -I ../foned -I ../dwtools -I ../sys -I ../dwsys -I ../stat \
	-I ..external/num

OBJECTS = Cepstrum.o Cepstrumc.o Cepstrum_and_Spectrum.o \
	Cepstrogram.o \
	Formant_extensions.o \
	FormantModeler.o FormantModelerList.o \
	FormantPath.o FormantPath_to_IntervalTier.o FormantPathArea.o FormantPathEditor.o \
	LineSpectralFrequencies.o \
	LPCFrameIntoSampledFrame.o LPCFrameIntoFormantFrame.o LPCFrameAndLineSpectralFrequenciesFrame.cpp \
	LPC.o \
	LPC_and_Formant.o LPC_and_Cepstrumc.o \
	LPC_and_LFCC.o \
	LPC_and_LineSpectralFrequencies.o LPC_and_Polynomial.o \
	LPC_to_Spectrum.o  LPC_to_Spectrogram.o \
	LPC_and_Tube.o \
	PowerCepstrum.o PowerCepstrumWorkspace.o PowerCepstrogram.o \
	PowerCepstrogramFrameIntoMatrixFrame.o \
	Roots_and_Formant.o \
	SoundFrameIntoLPCFrame.o SoundFrameIntoPowerCepstrogramFrame.o SoundFrameIntoFormantFrame.o \
	Sound_and_LPC.o Sound_to_Formant_mt.o \
	Sound_and_Cepstrum.o Tube.o \
	VocalTractTier.o \
	praat_LPC_init.o manual_LPC.o

.PHONY: all clean

all: libLPC.a

clean:
	$(RM) $(OBJECTS)
	$(RM) libLPC.a

libLPC.a: $(OBJECTS)
	touch libLPC.a
	rm libLPC.a
	$(AR) cq libLPC.a $(OBJECTS)
	$(RANLIB) libLPC.a

$(OBJECTS): *.h ../kar/*.h ../melder/*.h ../dwtools/*.h ../fon/*.h ../foned/*.h ../sys/*.h ../dwsys/*.h ../stat/*.h


