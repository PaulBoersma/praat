#ifndef _Script_h_
#define _Script_h_
/* Script.h
 *
 * Copyright (C) 1997-2005,2007,2011,2015,2016,2020,2024 Paul Boersma
 *
 * This code is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This code is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this work. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Collection.h"

Thing_define (Script, SimpleString) {
	bool trusted;
};

autoScript Script_createFromFile (MelderFile file);

Collection_define (ScriptSet, SortedSetOfStringOf, Script) {
};

void Script_rememberDuringThisAppSession_move (autoScript me);

Script Script_find (conststring32 filePath);   // should exist, cannot be null

/* End of file Script.h */
#endif
