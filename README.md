# libColorTool

Work in progress. The library itself is fully functional, but the documentation still needs to be written.

For now here is a rough outline of what the final documentation will contain.

## Color space classes
```cpp
#include "Colors/ColorSpaces.h"
```

```cpp
// float based
sRGB
sHSV
LinRGB
LinHSV
HCY
// uint8_t based
sRGBu8
```

All color space classes are 3 dimensional vectors and have operator overloads. (Similar to how colors are handled in GLSL.)

```cpp
sRGB(0.5, 0.5, 0.5) + sRGB(0.2, 0.2, 0.2);
// Result is sRGB(0.7, 0.7, 0.7)
sRGB(0.5, 0.5, 0.5) * 2.0;
// Result is sRGB(1.0, 1.0, 1.0)
```

Simple conversions are implicit. (LinRGB to sRGB, sRGB to LinRGB, sRGB to sRGBu8, sRGBu8 to sRGB, sHSV to sRGB, LinHSV to LinRGB, HCY to LinRGB)

```cpp
LinRGB(0.5, 0.5, 0.5) + sRGB(0.2, 0.2, 0.2);
// Result is LinRGB(0.53333)
sRGB(0.5, 0.5, 0.5) + sHSV(0.0, 1.0, 0.5);
// Result is sRGB(1.0, 0.5, 0.5)
```

All colorspace types can be converted using colorspace_cast.
```cpp
sHSV result = colorspace_cast<sHSV>(LinRGB(1.0, 0.0, 0.0));
```

### Color functions

```cpp
#include "Colors/Colors.h
```

```cpp
namespace Colors
{

constexpr float redness(LinRGB color); // Red - (Blue + Green) / 2.0
constexpr float greenness(LinRGB color); // Green - (Blue + Red) / 2.0
constexpr float blueness(LinRGB color); // Blue - (Green + Red) / 2.0
constexpr float luminance709(LinRGB color); // Luminance after Rec. 709 standard
constexpr float chroma(LinRGB rgbf); // Value of saturated component of color
constexpr float chroma(sRGB rgbf); // Also available for sRGB colors
Vec3f saturate_hue(float hue); // Creates a fully saturated generic vector from a hue.
float hue(Vec3f color); // Gets the hue of a generic vector.
float vibrance(LinRGB color); // Distance between the color and a gray of equal luminance, subjectively perceived as vibrance.

}
```

## Color palettes

```cpp
#include "Palette/Palette.h"
```

Palettes are simply std::vectors of one of the color space classes.


### Stat functions

Stats functions are functions that analyze a color and return a array of all factors to consider.

|                           |      [0]     |      [1]     |     [2]     |    [3]    |    [4]   |
|---------------------------|:------------:|:------------:|:-----------:|:---------:|:--------:|
| Stats::perceptive_factors | luminance709 |   vibrance   |   redness   | greenness | blueness |
| Stats::linrgb_factors     |  LinRGB red  | LinRGB green | LinRGB blue |           |          |
| Stats::srgb_factors       |   sRGB red   |  sRGB green  |  sRGB blue  |           |          |

Perceptive factors are the factors used by ColorTool. sRGB factors are the ones used in most graphics applications such as GIMP or Photoshop.

These inform the palette functions to what factors to use as axis for multidimensional comparisons.

### Color reduction
```cpp
ColorPalette<sRGB> palette(512);
// Fill the palette with data
float r = 0.0;
std::generate(palette.begin(), palette.end(), [&r](){ r += 1.0 / 512; return sRGB(r, r, r); });
// Reduce the palette to 32 colors
ColorPalette<sRGB> reduced = Palette::reduce_using_median_split(palette, 32, Stats::srgb_factors);
```

### Color sorting
Sorts the colors in a palette so that similar colors are next to each other.

```cpp
ColorPalette<sRGB> reduced = Palette::sort(pal, Stats::srgb_factors);
```