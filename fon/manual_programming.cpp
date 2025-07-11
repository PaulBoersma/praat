/* manual_programming.cpp
 *
 * Copyright (C) 1992-2010,2011,2013,2015-2025 Paul Boersma
 *
 * This code is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
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

#include "ManPagesM.h"

void manual_programming_init (ManPages me);
void manual_programming_init (ManPages me) {

MAN_PAGES_BEGIN
R"~~~(
################################################################################
"Interoperability"
© Paul Boersma 2022-01-15

You can use Praat in workflows that involve other programs.

1. General ways to access Praat from other programs
======================================

• To send messages from another program to a running Praat,
use @@sendpraat@ or @@Scripting 6.9. Calling from the command line|praat --send@.
This method is used by the programs CHAT (Childes) and Phon to view a sound in a Sound window.

• To execute a Praat script as a subprocess of another program, see @@Scripting 6.9. Calling from the command line@.

2. General ways to access other programs from Praat
======================================

• To execute another program as a subprocess of Praat, see @@Scripting 6.5. Calling system commands@.

3. Files
========

• Many programs read and/or write Praat TextGrid files. If you want create a TextGrid reading or writing procedure,
consult @@TextGrid file formats@.

################################################################################
"TextGrid file formats"
© Paul Boersma 2018-08-21

This page describes the syntax and semantics of TextGrid files that Praat can read and/or write.

1. The full text format of a minimal TextGrid
======================================

If you record a Sound with a duration of 2.3 seconds, and then do ##To TextGrid...#,
you are asked to provide tier names and to say which of these tiers are point tiers.
If you click OK without changing the settings from their standard values,
you obtain a TextGrid with two interval tiers, called %Mary and %John, and one point tier called %bell.
When you save this TextGrid to disk by choosing @@Save as text file...@ from the #New menu,
the resulting text file, when opened in a text editor, will look as follows:
`
	File type = "ooTextFile"
	Object class = "TextGrid"

	xmin = 0
	xmax = 2.3
	tiers? <exists>
	size = 3
	item []:
		item [1]:
			class = "IntervalTier"
			name = "Mary"
			xmin = 0
			xmax = 2.3
			intervals: size = 1
			intervals [1]:
				xmin = 0
				xmax = 2.3
				text = ""
		item [2]:
			class = "IntervalTier"
			name = "John"
			xmin = 0
			xmax = 2.3
			intervals: size = 1
			intervals [1]:
				xmin = 0
				xmax = 2.3
				text = ""
		item [3]:
			class = "TextTier"
			name = "bell"
			xmin = 0
			xmax = 2.3
			points: size = 0
`
This text file contains the following pieces of data, which Praat will use when reading this file from disk
and turning it into a TextGrid object again:

• The text "ooTextFile", which appears in the first line of any text file that you write with @@Save as text file...@,
i.e. not only in text files created from a TextGrid object, but also in text files created from e.g. a Pitch
or ExperimentMFC object or from any other of the hundreds of types of objects that Praat can have in its Objects list.

• The text "TextGrid", which designates the type of the object that has been saved to this file.

• The real number 0, which is the starting time (in seconds) of this TextGrid.

• The real number 2.3, which is the end time (in seconds) of the TextGrid.

• The flag <exists>, which tells us that this TextGrid contains tiers (this value would be <absent>
if the TextGrid contained no tiers, in which case the file would end here; however, you cannot really create TextGrid objects
without tiers in Praat, so this issue can be ignored).

• The integer number 3, which is the number of tiers that you created.

• The text "IntervalTier", which designates the type of the first tier (an interval tier, therefore).

• The text "Mary", which is the name you gave to the first tier.

• The real number 0, which is the starting time (in seconds) of the first tier.

• The real number 2.3, which is the end time (in seconds) of the first tier.
When you create a TextGrid with an interval tier, the time domain of the interval tier is automatically set equal
to the time domain of the whole TextGrid.

• The integer number 1, which is the number of intervals in the first tier.
Although you did not add any intervals and did not add any text to the first tier,
all interval tiers always contain at least one interval, which is created when you create the TextGrid.

• The real number 0, which is the starting time (in seconds) of the first (only) interval of the first tier.

• The real number 2.3, which is the end time (in seconds) of the first interval of the first tier.
The interval that is automatically created for you when you create a TextGrid with an interval tier,
spans the whole tier.

• The text "", which is the contents of the interval on the first tier. This text is empty.

• The text "IntervalTier", which gives the type of the second tier (again an interval tier).

• The text "John", which is the name you gave to the second tier.

• The real number 0, which is the starting time (in seconds) of the second tier.

• The real number 2.3, which is the end time (in seconds) of the second tier.

• The integer number 1, which is the number of intervals in the second tier.

• The real number 0, which is the starting time (in seconds) of the first (only) interval of the second tier.

• The real number 2.3, which is the end time (in seconds) of the first interval of the second tier.

• The text "", which is the contents of the interval on the second tier. This text is empty.

• The text "TextTier", which designates the type of the third tier (a point tier this time).

• The text "bell", which is the name you gave to the third tier.

• The real number 0, which is the starting time (in seconds) of the third tier.

• The real number 2.3, which is the end time (in seconds) of the third tier.

• The integer number 0, which is the number of points in the third tier.
A newly created point tier contains no points yet.

You will have noticed that the file contains a lot of stuff that was not mentioned in this list.
All of that stuff are %comments that are present only to help the human reader understand the contents
of the file: labels for all tiers (such as `item [2]`), labels for the starting times of the TextGrid or
a tier or an interval (`xmin`), labels for end times (`xmax`); labels for the number of tiers or
intervals or points (`size`), and little numbers enclosed in square brackets to tell the reader where they are (`[2]`).

When reading a text file containing a TextGrid (or any other object), Praat totally ignores these comments,
so if you e.g. replace `[2]` with `[4]` somewhere, Praat will not notice. Praat will consider as data only the following
types of information in the file:
• free-standing numbers, such as `0` and `2.3` above, but not `[1]` or `[3]`;
• free-standing text enclosed within double quotes, such as `"TextGrid"` and `""` above;
• free-standing flags, such as `<exists>` above (this is the only flag that appears in TextGrid files;
see @ExperimentMFC for a much broader use of flags).

In this list, "free-standing" means that the number, text or flag is preceded by the beginning of the file,
the beginning of a line, or a space, and that it is followed by the end of the file, the end of a line, or a space.

2. The shortest text format of a minimal TextGrid
======================================

From the description above of what in the file is considered data (namely free-standing numbers, texts and flags)
and what is not (namely everything else), you can conclude that Praat will be able to read a much shorter version of
the file above. And indeed, when you choose @@Save as short text file...@ from the #New menu, your file will consist of
the following text,
with every piece of data alone on a separate line:
`
	File type = "ooTextFile"
	Object class = "TextGrid"

	0
	2.3
	<exists>
	3
	"IntervalTier"
	"Mary"
	0
	2.3
	1
	0
	2.3
	""
	"IntervalTier"
	"John"
	0
	2.3
	1
	0
	2.3
	""
	"TextTier"
	"bell"
	0
	2.3
	0
`
That’s much less human-readable than before, but equally computer-readable, at least by Praat.
When you write your own TextGrid file recognizer, you should be able to interpret both versions shown above,
and also any version that is intermediate between the two, as long as it has the data written as freestanding numbers,
texts, and flags. For instance, a human-readable TextGrid file that can be interpreted by Praat could look
as follows:
`
	"ooTextFile"
	"TextGrid"
	0  2.3  ! time domain of TextGrid
	<exists>
	3 tiers
	"IntervalTier"  "Mary"  ! type and name of tier 1
	0  2.3  ! time domain of tier 1
	1 interval coming
	0  2.3  ""  ! interval 1 on tier 1
	"IntervalTier"  "John"  ! type and name of tier 2
	0  2.3  ! time domain of tier 2
	1 interval coming
	0  2.3  ""  ! interval 1 on tier 2
	"TextTier"  "bell"  ! type and name of tier 3
	0  2.3  ! time domain of tier 3
	0 points coming
`
Here we see that multiple pieces of data can be together on a line, as long as each of them stands free;
the only layout requirement is that the text `"ooTextFile"` has to be alone on the first line.

Another thing we see is that there is an additional type of comment: everything that follows an
exclamation mark on the same line is considered a comment. Thus, although some lines seem to end in a free-standing
number (1, 2, or 3), those numbers do not count as data, because they are part of a comment that follows
an exclamation mark.

3. Reading a TextGrid file
==========================

Reading the contents of a TextGrid file into your own data structure is fairly easy.
For instance, to figure out how many intervals there are in a tier, you do not have to read multiple lines and then backtrack
to see where the list of intervals ends. Instead, the number of intervals is given before the intervals are enumerated.

4. Writing a TextGrid file
==========================

Writing the contents of a TextGrid file involves deciding on a level of human readability.

5. A TextGrid file with more than the minimal content
======================================

The above example was about a rather uninteresting TextGrid object, with no text in it. Suppose instead that
the sound was a recording of speaker saying a sentence, and your TextGrid annotates the sentence orthographically
as well as phonetically, and also annotates two chimes of the bell:
{- 6.0x3.0
	textgrid = Create TextGrid: 0, 2.3, "sentence phonemes bell", "bell"
	Set interval text: 1, 1, "říkej ""ahoj"" dvakrát"
	Insert boundary: 2, 0.7
	Insert boundary: 2, 1.6
	Set interval text: 2, 1, "r̝iːkɛj"
	Set interval text: 2, 2, "ʔaɦɔj"
	Set interval text: 2, 3, "dʋakraːt"
	Insert point: 3, 0.9, "ding"
	Insert point: 3, 1.3, "dong"
	Draw: 0.0, 0.0, 1, 1, 1
}
When you save this as a full text file, it will look as follows:
`
	File type = "ooTextFile"
	Object class = "TextGrid"

	xmin = 0
	xmax = 2.3
	tiers? <exists>
	size = 3
	item []:
		item [1]:
			class = "IntervalTier"
			name = "sentence"
			xmin = 0
			xmax = 2.3
			intervals: size = 1
			intervals [1]:
				xmin = 0
				xmax = 2.3
				text = "říkej ""ahoj"" dvakrát"
		item [2]:
			class = "IntervalTier"
			name = "phonemes"
			xmin = 0
			xmax = 2.3
			intervals: size = 3
			intervals [1]:
				xmin = 0
				xmax = 0.7
				text = "r̝iːkɛj"
			intervals [2]:
				xmin = 0.7
				xmax = 1.6
				text = "ʔaɦɔj"
			intervals [3]:
				xmin = 1.6
				xmax = 2.3
				text = "dʋakraːt"
		item [3]:
			class = "TextTier"
			name = "bell"
			xmin = 0
			xmax = 2.3
			points: size = 2
			points [1]:
				number = 0.9
				mark = "ding"
			points [2]:
				number = 1.3
				mark = "dong"
`
We now see how points on a point tier are written: with their time (`number`) and their text (`mark`).
As usual, most of the contents of the file is comments; for instance, older TextGrid files may call
the time of the point $time instead of $number, but as these are just comments, Praat will ignore these words
when reading the file, and so should anybody who programs a TextGrid file parser.

A thing to note is the doubling of double quotes. The sentence in tier 1 contains double quotes around %ahoj,
as you can see in the picture above, and you typed only these two double quotes in the TextGrid window to start with.
However, the file format uses double quotes to mark texts, so if a text itself contains a double quote,
something special has to be done about it lest Praat think that you’re ending the text. The solution that Praat chooses
is that a double quote that appears in a text is written as a %doubled double quote in the text file, as above on tier 1.

6. Restrictions in a TextGrid object
====================================

TextGrid objects maintain several invariants, some stronger and some weaker.

The two strongest invariants within an interval tier are %%positive duration% and %adjacency.
That is, the end time of each interval has to be greater than the starting time of that same interval,
and the starting time of each interval (except the first) has to be equal to the end time of the previous interval.
As a result, the union of the time domains of the set of intervals in a tier is a contiguous stretch of time,
and no intervals overlap. If a TextGrid file violates these invariants, Praat may refuse to read the file and give
an error message instead (or Praat may try to repair the TextGrid, but that is not guaranteed).

A weaker invariant is that the starting time of the first interval on a tier equals the starting time of that tier,
and the end time of the last interval equals the end time of the tier. When you create a TextGrid with Praat,
this invariant is automatically maintained, and most types of modifications also maintain it, except sometimes the commands
that combine multiple TextGrid objects with different durations into a new TextGrid.
Praat will happily read TextGrid files that do not honour this weak invariant, and will also display such a TextGrid
reasonably well in a TextGrid window. It is nevertheless advisable for other programs that create TextGrids
to honour this weak invariant.

For a point tier, a strong invariant is that the time of each point (except the first) has to be greater than the time
of the previous point. Praat maintains this invariant for instance by refusing to insert a point at the time of an existing
point. TextGrid files that violate this invariant may or may not be read by Praat, and may cause strange behaviour in Praat
if they are read.

A further weak invariant is that the starting and end times of each tier equal the starting and end times of the whole
TextGrid. This can be violated by combining multiple TextGrids into one, but other programs are advised to create TextGrids
that honour this invariant, because TextGrids that violate it may look strange to the user.

7. Text encoding
================

Existing TextGrid text files come in various encodings. When creating a parser for TextGrid text files,
you should be prepared for reading it in UTF-8 encoding (without Byte Order Mark), or in UTF-16 encoding
(either Big-Endian or Little-Endian, with Byte Order Mark). Pre-Unicode TextGrid text files may have a Latin-1 encoding
if they were created on Windows or Linux, or a MacRoman encoding if they were created on a Mac,
so it would be good to prepare for such old files as well, although it may be difficult to figure out which is which
(line separators, as described below, may help).

When writing a TextGrid text file, you can use UTF-8 encoding (without Byte Order Mark), or UTF-16 encoding
(either Big-Endian or Little-Endian, with Byte Order Mark).
Please never write a limited encoding such as Latin-1 or MacRoman, which do not support international characters
or phonetic characters as in “`ʔaɦɔj`”.

The lines in the file are typically separated by a newline symbol (Linux or modern Mac),
or by a Return symbol (old Mac), or by a Return symbol followed by a newline symbol (Windows).
When reading a TextGrid text file, you should be prepared for each of these line separators.
When writing a TextGrid text file, you can use any of these line separators, because most text editors
on all platforms can meanwhile open and view all these versions correctly.

8. Interpreting trigraphs
=========================

The example above contains several phonetic symbols, and it is not always easy to type those into a text field.
For this reason, Praat provides %trigraphs for most phonetic characters, as well as for many non-ASCII characters
used in the languages of the world. For instance, the vowel “ɔ” (a “turned c”) can be typed as `\\ct` into
the TextGrid window as well as anywhere else in Praat where you want to draw graphical text
(see @@Special symbols@ for all trigraphs). Thus, the file above could have looked as follows:
`
	File type = "ooTextFile"
	Object class = "TextGrid"

	xmin = 0
	xmax = 2.3
	tiers? <exists>
	size = 3
	item []:
		item [1]:
			class = "IntervalTier"
			name = "sentence"
			xmin = 0
			xmax = 2.3
			intervals: size = 1
			intervals [1]:
				xmin = 0
				xmax = 2.3
				text = "\r<\i'kej ""ahoj"" dvakr\a't"
		item [2]:
			class = "IntervalTier"
			name = "phonemes"
			xmin = 0
			xmax = 2.3
			intervals: size = 3
			intervals [1]:
				xmin = 0
				xmax = 0.7
				text = "r\T^i\:fk\efj"
			intervals [2]:
				xmin = 0.7
				xmax = 1.6
				text = "\?ga\h^\ctj"
			intervals [3]:
				xmin = 1.6
				xmax = 2.3
				text = "d\vsakra\:ft"
		item [3]:
			class = "TextTier"
			name = "bell"
			xmin = 0
			xmax = 2.3
			points: size = 2
			points [1]:
				number = 0.9
				mark = "ding"
			points [2]:
				number = 1.3
				mark = "dong"
`
On the screen, this TextGrid looks the same as before:
{- 6.0x3.0
	textgrid = Create TextGrid: 0, 2.3, "sentence phonemes bell", "bell"
	Set interval text: 1, 1, "\r<\i'kej ""ahoj"" dvakr\a't"
	Insert boundary: 2, 0.7
	Insert boundary: 2, 1.6
	Set interval text: 2, 1, "r\T^i\:fk\efj"
	Set interval text: 2, 2, "\?ga\h^\ctj"
	Set interval text: 2, 3, "d\vsakra\:ft"
	Insert point: 3, 0.9, "ding"
	Insert point: 3, 1.3, "dong"
	Draw: 0.0, 0.0, 1, 1, 1
}
There are several hundreds of such trigraphs, and a few more might be added in the future.
When you create a TextGrid file parser and want to interpret the trigraphs in the correct way,
it is advisable to have Praat do the conversion for you: read the TextGrid file into Praat,
call the menu command ##Convert to Unicode#, and save the TextGrid as a new text file.

9. The binary TextGrid file format
==================================

Besides the TextGrid text file format described above, TextGrid objects can also be saved as binary files
with @@Save as binary file...@. If you need a description of this format, we can add it here on request.
When you want to create only a TextGrid %text file parser, and still want to support binary TextGrid files,
you can have Praat do the conversion for you: read the binary TextGrid file into Praat with @@Read from file...@
and save the resulting TextGrid object as a text file with @@Save as text file...@.

################################################################################
"Programming with Praat"
© Paul Boersma 2023-04-09, 2024

You can extend the functionality of the Praat program
by adding modules written in C or C++ to it. All of Praat's source code
is available under the General Public Licence.

1. Warning
==========

Before trying the task of learning how to write Praat extensions in C or C++,
you should be well aware of the possibilities of @scripting.
Many built-in commands in Praat have started their
lives as Praat scripts, and scripts are easier to write than extensions in C or C++.
If you have a set of scripts, you can distribute them as a @@plug-ins|plug-in@.

2. Getting the existing source code
===================================

You obtain the Praat source code from GitHub (https://github.com/praat), in a file with a name like
`praat6499_sources.zip` or `praat6499_sources.tar.gz` (depending on the Praat version), and unpack this by double-clicking.
The result will be a set of directories
called `kar`, `melder`, `external` (with `clapack`, `gsl`, `glpk`, `flac`, `mp3`, `portaudio`, `espeak`, `vorbis` and `opusfile` in it),
`sys`, `dwsys`, `stat`, `fon`, `sensors`, `dwtools`, `LPC`, `FFNet`, `gram`, `artsynth`, `EEG`, `main`, `makefiles`, `test`, `dwtest`, and `generate`,
plus a makefile and Xcode project for macOS and a README.md file.

3. Building Praat
=================

Consult the README file on GitHub for directions to compile and link Praat for your platform.

4. Extending Praat
==================

To start extending Praat’s functionality, you can edit `main/main_Praat.cpp`.
This example shows you how to create a very simple program with all the functionality
of the Praat program, and a single bit more (namely an additional command in the New menu):
`
	#include "praatM.h"

	DIRECT (HelloFromJane) {
		Melder_information (U"Hello, I am Jane.");
	}

	int main (int argc, char **argv) {
		praat_init (U"Praat_Jane", argc, argv);
		INCLUDE_LIBRARY (praat_uvafon_init)
		praat_addMenuCommand (U"Objects", U"New", U"Hello from Jane...", nullptr, 0, DO_HelloFromJane);
		praat_run ();
		return 0;
	}
`
5. Learning how to program
==========================

To see how objects are defined, take a look at `sys/Thing.h`, `sys/Daata.h`,
`sys/oo.h`, the `XXX_def.h` files in the `fon` folder, and the corresponding
`XXX.cpp` files in the `fon` folder. To see how commands show up on the buttons
in the fixed and dynamic menus, take a look at the large interface description file
`fon/praat_Fon.cpp`.

6. Using the Praat shell only
=============================

For building the Praat shell (the Objects and Picture windows) only, you need only the code in the nine directories
`kar`, `melder`, `external/{clapack,gsl,flac,mp3,portaudio,lame,opusfile,vorbis}`, `sys`, and `dwsys`. You delete the inclusion of `praat_uvafon_init` from `main`.
You will be able to build a Praat shell, i.e. an Objects and a Picture window,
which has no knowledge of the world, i.e., which does not know any objects
that can be included in the list of objects. You could use this Praat shell
for modelling your own world and defining your own classes of objects. For advanced programmers only.

)~~~"
MAN_PAGES_END

}

/* End of file manual_programming.cpp */
