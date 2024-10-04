# More Icons
A mod that loads custom icons.

## Adding Icons
To add a custom icon, you need an icon spritesheet (.plist) and an icon atlas (.png).

The spritesheet should be in the format of a typical Geometry Dash icon spritesheet, with the primary sprite, secondary sprite, glow sprite, and an optional detail sprite. UFOs contain a fifth sprite for the dome.

The atlas should be a single image containing all the sprites in the spritesheet. Tools like [Colon's Spritesheet Splitter](https://gdcolon.com/gdsplitter) can be used to split a spritesheet into individual sprites and combine them into an atlas.

The spritesheet should have the icon's name correspond to these sprites:
- Primary: `(name)_001.png`
- Secondary: `(name)_2_001.png`
- Dome (UFO only): `(name)_3_001.png`
- Glow: `(name)_glow_001.png`
- Detail (optional): `(name)_extra_001.png`

The atlas should be named `(name).png`, and the spritesheet should be named `(name).plist`. Optionally, you can add `-hd` and `-uhd` for high-definition spritesheets and atlases, and it is much recommended to do so.

The spritesheets and atlases should be placed in `(Geometry Dash folder)/geode/config/hiimjustin000.more_icons/(gamemode)`, where `(gamemode)` is the gamemode the icon is for. The gamemodes are:
- icon
- ship
- ball
- ufo
- wave
- robot
- spider
- swing
- jetpack

If anything goes wrong, the mod will log warnings and errors to the console, which can be checked in `(Geometry Dash folder)/geode/logs`.

## Using Icons
To use a custom icon, you will need to go into the icon kit. In the icon kit, there is a second row of dots that you can click on to view the extra icons. Click on the dot to view a page of extra icons, and click on one of the icons to select it.

To deselect a custom icon, use the first row of dots to go back to the default icons, and click on one of the default icons to select it.