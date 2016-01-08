Symmetricality
==============

More symmetry for everyone :)

Basic Usage
-----------

As of this point Symmetricality is primarily a Keyboard based program, with no
mouse interactivity. This will change at some point and then mouse usage will
once again become the preferred method for quick and dirty application of
designations.

| Key              | Effect                                           |
|------------------|--------------------------------------------------|
| F5               | Save File (.ser)                                 |
| Shift-F5         | Export to Csv                                    |
| F6               | Load File (.ser)                                 |
| Arrow Keys       | Move around                                      |
| Shift Arrow keys | Move around in increments of 10                  |
| Return           | Start Designating.                               |
| Shift-Return     | Start Designating circle.                        |
| Control-Return   | Start Designating a line.                        |
| r                | Add Radial Symmetry at cursor                    |
| 9                | Add Rotational(at 90 degree increments) symmetry |
| x                | Add X axis Symmetry                              |
| y                | Add Y Axis Symmetry                              |
| =                | Change Designation type                          |
| \-               | Change Designation type                          |
| Period(.)        | Move down a Z-level                              |
| Comma(,)         | Move up a Z-level                                |
| F2               | Take a screenshot.                               |
| F1               | Toggle the text overlay.                         |
|b				   | Toggle build mode								  |

The primary cause for the lack of Csv import support comes from the difficulty
of adapting a 3d representation of a blueprint into a 2 dimensional file
format(Yes, quickfort does have support for 3d plans), however supporting that
as an import format is more difficult than exporting because it has to accept
most mistakes that a human designer would make in their creation of a Csv file.

As such, the Csv files that this program generates are not quite up to the
highest orders of the standard, but given the simplicity of the format, most
spreadsheet programs should handle the output without issue.

How does the Symmetry work?
---------------------------

Symmetries in this are not merely toggled on and off, with this, multiple
symmetries across each axis may be used at once. The Symmetries are applied one
at a time, with the points produced cascading across each symmetry in the order
applied.

As a result, the number of inserts called is at least 2n, so use cautiously.

Rotational Symmetry is the most complicated. It rotates the point around it's
center 3 times.

![Rotational Symmetry](<http://i.imgur.com/guuqSBR.png>)

Radial Symmetry is the next most complicated(but not by much).

![Radial Symmetry](<http://i.imgur.com/ZwfO5zN.png>)

![Radial Symmetry 2](<http://i.imgur.com/6k1PaCd.png>)

Next up is X Symmetry.

![x Symmetry](<http://i.imgur.com/7pN3ii2.png>)

And last is Y Symmetry.

![Y symmetry](<http://i.imgur.com/u8SsO6G.png>)

Symmetry is applied in the order added. So adding 90 degree symmetry before an X
symmetry will mirror the complete symmetry of the rotation before mirroring
across the point on the x axis.

This can produce effects like this

![Combinations](<http://i.imgur.com/FdDpbov.png>)

![Combinations 2](<http://i.imgur.com/1CN9EY9.png>)

If Swastikas offend you, then you may find yourself offended quite a lot while
using the 90 degree symmetry, but as that is a feature of the geometry itself, I
see no means of correcting it other than fixing its users.

Configuration
-------------

Configuration support is provided by [GetPot](http://getpot.sourceforge.net/),
and therefore the issues that come up in configuration may prove to be best
answered in its own docs. Other than that, it’s not terribly complicated.

 

The configuration file is `Symmetricality.pot`. It *must* be in the working directory of Symmetricality(As will the assets such as the linked libraries(for windows only), the font, the texture files, etc).

The only top level directive is `default_path` which defines where the save/export/open dialog opens to. If it is not a valid path, it will open the empty path, which is generally not terribly convenient.

The file is divided into sections which work a bit like directories(again, go to the GetPot documentation).

For example, this defines the color of the dig designation:

``` 
[colors/dig]
R = 200 G = 200 B = 255
```

The color components are fairly straightforwards here, the subsection names for the designations are `downward_stairs`, `up_down_stairs`,`upwards_stairs`, `ramp`, and `channel`. 
The section names in full should be `[colors/<designation_type>]` or if following another designation `[../<designation_type]` is acceptable.

The `[designation]` section contains information specific to the configuration of a texture for rendering designations in addition to colorizing them. 
The item `use_textures` toggles this functionality.
The `width` and `height` directives are the width and height of each designation "glyph".

If you are creating a different texture for the designations, they must appear on a horizontal arrangement across a single image in the order 

1) dig
2) down stairs
3) up stairs
4) up/down stairs
5) ramp
6) channel

These glyphs are colorizes by replacing the color white in the texture during runtime, so multiple colors are allowed.

Buildings are significantly more complex

### Buildings

Not all buildings that exist in Dwarf Fortress are also in Symmetricality. The editor is designed so that it is possible to 
create building entries without editing the code and recompiling. They exist in the same configuration file that is used to 
set the designation colors and glyphs(right below them actually).

Buildings are more complicated because they are populated by number instead of by name, as `building/building_count` determines the id ranges that it checks.

Also of note in the toplevel building section is `building_sheet` which is the path(or name of) the image file that contains the sprites.

Each building is defined by six variables at the top level.

1) `Buildingname` specifies what the editor calls it on the gui
2) `key_sequence` defines the string which builds it in Dwarf Fortress
3) `size_x` specifies the width of the building(in grid units)
4) `size_y` specifies the height of the building(in grid units)
5) `center_x` specifies the center of the building(not used right now)
6) `center_y` specifies the y center of the building(not used right now)

After that, there is another section called `texturecoords` that is inside the `[building/<id>/]` section.
This defines the area of the spritesheet that are shown when the building is drawn. `X1` is the starting point on the left hand side of the sprite(starting at 0)
`X2` is the right side of the sprite. 
`Y1` is the top of the sprite. `Y2` is the bottom of the sprite.

Strange things happen if any of these are not defined, so check these values if the building is not displayed properly.