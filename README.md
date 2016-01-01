# Symmetricality
More symmetry for everyone :)

## Basic Usage

As of this point Symmetricality
is primarily a Keyboard based program, with no mouse interactivity.
This will change at some point and then mouse usage will
once again become the preferred method for quick and dirty application of
designations.


|Key|Effect|
|---|------|
|F5|Save File (.ser)|
|Shift-F5|Export to Csv|
|F6|Load File (.ser)|
|Arrow Keys|Move around|
|Shift Arrow keys|Move around in increments of 10|
|Return|Start Designating.|
|Shift-Return| Start Designating circle.|
|Control-Return|Start Designating a line.|
|r|Add Radial Symmetry at cursor|
|9|Add Rotational(at 90 degree increments) symmetry|
|x|Add X axis Symmetry|
|y|Add Y Axis Symmetry|
|=|Change Designation type|
|-|Change Designation type|
|Period(.)|Move down a Z-level|
|Comma(,)|Move up a Z-level|
|F2|Take a screenshot.|
|F1|Toggle the text overlay.|

The primary cause for the lack of Csv import support comes from
the difficulty of adapting a 3d representation of a blueprint into a
2 dimensional file format(Yes, quickfort does have support for 3d plans),
however supporting that as an import format is more difficult than exporting
because it has to accept most mistakes that a human designer would make in their
creation of a Csv file.

As such, the Csv files that this program generates are not quite up to
 the highest orders of the standard, but given the simplicity of the format,
  most spreadsheet programs should handle the output without issue.

## How does the Symmetry work?

Symmetries in this are not merely toggled on and off,
with this, multiple symmetries across each axis may be
used at once. The Symmetries are applied one at a time,
with the points produced cascading across each symmetry
in the order applied.

As a result, the number of inserts called is at least 2<sup>n</sup>, so use cautiously.

Rotational Symmetry is the most complicated. It rotates the point around it's center 3 times.

![Rotational Symmetry](http://i.imgur.com/guuqSBR.png)

Radial Symmetry is the next most complicated(but not by much).

![Radial Symmetry](http://i.imgur.com/ZwfO5zN.png)
![Radial Symmetry 2](http://i.imgur.com/6k1PaCd.png)

Next up is X Symmetry.

![x Symmetry](http://i.imgur.com/7pN3ii2.png)

And last is Y Symmetry.

![Y symmetry](http://i.imgur.com/u8SsO6G.png)

Symmetry is applied in the order added.
So adding 90 degree symmetry before an
X symmetry will mirror the complete symmetry
of the rotation before mirroring across the
point on the x axis.

This can produce effects like this

![Combinations](http://i.imgur.com/FdDpbov.png)
![Combinations 2](http://i.imgur.com/1CN9EY9.png)

If Swastikas offend you, then you may find yourself
offended quite a lot while using the 90 degree symmetry,
but as that is a feature of the geometry itself,
I see no means of correcting it other than fixing its users.

