#ifndef _SoundAnalysisWorkspace_h_
#define _SoundAnalysisWorkspace_h_
/* SoundAnalysisWorkspace.h
 *
 * Copyright (C) 2024 David Weenink
 *
 * This code is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This code is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this work. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Sampled.h"
#include "Sound.h"
#include "SampledAnalysisWorkspace.h"

#include "SoundAnalysisWorkspace_def.h"

autoWorkvectorPool WorkvectorPool_create (INTVEC const& vectorSizes, bool reusable);

void SoundAnalysisWorkspace_init (SoundAnalysisWorkspace me, Sound thee, Sampled him, double effectiveAnalysisWidth, kSound_windowShape windowShape);

autoSoundAnalysisWorkspace SoundAnalysisWorkspace_create (Sound thee, Sampled him, double effectiveAnalysisWidth, kSound_windowShape windowShape);

//void SoundAnalysisWorkspace_initExtraAnalysisData (SoundAnalysisWorkspace me, ExtraAnalysisData thee);

void SoundAnalysisWorkspace_initWorkvectorPool (SoundAnalysisWorkspace me, INTVEC const& vectorSizes);

void SoundAnalysisWorkspace_getThreadingInfo (SoundAnalysisWorkspace me, integer maximumNumberOfThreads, integer numberOfFramesPerThread, integer *out_numberOfThreads);

void SoundAnalysisWorkspace_replaceSound (SoundAnalysisWorkspace me, Sound thee);
/*
	Preconditions: 
		my sound->xmin/xmax = thy xmin/xmax
		my sound->nx = thy nx
		my sound->dx = thy dx
		my sound->x1 = thy x1
*/

void SoundAnalysisWorkspace_analyseThreaded (SoundAnalysisWorkspace me, Sound thee, double preEmphasisFrequency);

double getPhysicalAnalysisWidth (double effectiveAnalysisWidth, kSound_windowShape windowShape);

void SoundAnalysisWorkspace_analyseThreaded (SoundAnalysisWorkspace me, Sound thee, double preEmphasisFrequency);

#endif /* _SoundAnalysisWorkspace_h_ */
 
