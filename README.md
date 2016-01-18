Symmetricality
==============

Symmetricality is a fortress designer for [Dwarf Fortress](http://bay12games.com/dwarves) which focuses on providing a simple means of building designation commands for Quickfort, with an added focus on providing functions to make building a symmetric design simple.

Basic Usage
-----------

Symmetricality supports some very basic mouse input at the moment. Basically you can mark areas for designation quickly using a mouse, at the moment, it does not support building, switching designations, or anything of that sort. This too will change with future updates.

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
|Control-=         | Zoom in(sort of)                                 |
|Control-\-        | Zoom out(sort of)                                |
| Period(.)        | Move down a Z-level                              |
| Comma(,)         | Move up a Z-level                                |
| F2               | Take a screenshot.                               |
| F1               | Toggle the text overlay.                         |
|b				         | Toggle build mode								                |

The following mouse based actions are also included.

| Mouse action     | Result |
|------------------|--------|
|Right click drag  | Pan around the blueprint|
|Left click drag   | Designate from one point to another|
|Right double click|Move back to the center of the blueprint|
|Right click| Open menu |
|mousewheel | Zoom in or out |



The primary cause for the lack of Csv import support comes from the difficulty
of adapting a 3d representation of a blueprint into a 2 dimensional file
format(Yes, quickfort does have support for 3d plans), however supporting that
as an import format is more difficult than exporting because it has to accept
most mistakes that a human designer would make in their creation of a Csv file.


How does the Symmetry work?
---------------------------

Symmetries in this are not merely toggled on and off, with this, multiple
symmetries across each axis may be used at once. The Symmetries are applied one
at a time, with the points produced cascading across each symmetry in the order
applied.

As a result, the number of inserts called is at least 2n, so use cautiously.

Rotational Symmetry is the most complicated. It rotates the point around it's
center 3 times.

![Rotational Symmetry](http://i.imgur.com/guuqSBR.png)

Radial Symmetry is the next most complicated(but not by much).

![Radial Symmetry](http://i.imgur.com/ZwfO5zN.png)

![Radial Symmetry 2](<http://i.imgur.com/6k1PaCd.png>)

Next up is X Symmetry.

![x Symmetry](<http://i.imgur.com/7pN3ii2.png>)

And last is Y Symmetry.

![Y symmetry](<http://i.imgur.com/u8SsO6G.png>)

Symmetry is applied in the order added. So adding 90 degree symmetry before an X
symmetry will mirror the complete symmetry of the rotation before mirroring
across the point on the x axis.

This can produce effects like this

![Combinations](http://i.imgur.com/FdDpbov.png)

![Combinations 2](http://i.imgur.com/1CN9EY9.png)

### Telling Symmetries apart

Symmetries are now denoted by different symbols.

Here's a table explaining what each color means:

|Symbol|Type|
|-----|----|
|![Rotational](http://i.imgur.com/KmCXy9t.png)| 90 degree rotational (counter clockwise) increments|
|![Radial](http://i.imgur.com/oWRxJaA.png)| Radial|
|![X axis](http://i.imgur.com/B6gxOqw.png)|X axis|
|![Y axis](http://i.imgur.com/hHd014N.png)|Y axis|
|![Start point](http://i.imgur.com/xqWElbw.png)|The starting point for the quickfort file generated|


### Turning off Symmetries

Radial symmetries must have the cursor over them to toggle.

X axis symmetry may have the cursor anywhere so long as the x-coordinate is the same.

Y axis symmetry is the same as the x axis symmetry excepting that it cares about the Y coordinate.

Rotational symmetries must also have the cursor over the same spot as the symmetry.


Configuration
-------------

Configuration as of now is provided through [jsoncpp](<https://github.com/open-source-parsers/jsoncpp>). This means that all of the previous documentation about configuration no longer applies. Luckily, the configuration has been made more concise as well as reliable.

## Top Level Variables

Json is a hierarchic data store format. Luckily that means that you can get away with ignoring other things when editing one item. The top level directives are the most general in affecting your configuration.

1) `"default_file_path"` is the path which is opened by default when saving a blueprint.
2) `"building_sheet"` is the path to the texture file which buildings are stored in.
3) `"buildings"` is the variable that contains all of the buildings that are configured currently.
4) `"designation"` contains the variables relevant to the types of designation.

Any additional variables will be ignored, so don't bother.

### Building Definition

So let's just run through the process. There are 3 basic steps to take when adding a building to Symmetricality.

1) Create sprite art
2) Enter details of the building into the json file
3) Test

#### Create Sprite Art

Okay, so let's say that we're making the statue that is already in the editor by default.

Normally the dimensions of each sprite is `(10*width) x (10*height)`, but this isn't set in stone, unlike this statue, given that normally the user will see the sprites at a 1 to 1 ratio, 10x10 is the same size as the grid resolution by default(you can't set this now). 

![Building texture example](http://i.imgur.com/Baujezg.png)

Great, now add it to a free spot in the `buildings.png` file in the directory of Symmetricality. 

This sprite occupies the ranges from pixel (60,0) to (69,9).

```language=json
"buildings":[
{
  "name":"statue",
  "key_sequence":"s",
  "size_x":1,"size_y":1,
  "render_start_x":0,
  "render_start_y":0,
  "render_end_x":1,
  "render_end_y":1,
  "texture_coordinates":[
    60,69,0,9]
},
<other buildings>
]
```

The `"texture_coordinates"` variable is a list of the positions of the pixels in the order \[x_0,x_1,y_0,y_1\].

The `"size_x"` and `"size_y"` properties mean that the statue takes up a 1x1 space, which is true in Dwarf Fortress.

The `"render_start_x"` and `"render_start_y"` control where the start of the displayed building is, and the `"render_end_x"` and `"render_end_y"` control where the opposing vertex of the square ends up.

This will work for the statue, but they may need to be adjusted for larger buildings to display properly(an example of this is in the forge and smelter buildings).

There are also another two variables that aren't used called `"center_x"` and `"center_y"` which when implemented properly will determine which piece of the building will end up underneath the cursor.