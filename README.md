# libColorTool

libColorTool is a modern C++17 library for type-safe image processing. It provides types for the major relative color spaces used in image processing, as well as ways to convert between them. It is lightweight with the STL being the only dependecy.

```cpp
// float based
sRGB
HSV
LinearRGB
LinearHSV
HCY
// uint8_t based
sRGB_uint8
// Versions with alpha values
sRGB_Alpha
sRGB_uint8_Alpha
LinearRGB_Alpha
```

All of these are vector types and are designed to be used in conjunction with SIMD instructions and automatic vectorization.

```cpp
sRGB(0.5, 0.5, 0.5) + sRGB(0.2, 0.2, 0.2);
// Result is sRGB(0.7, 0.7, 0.7)
sRGB(0.5, 0.5, 0.5) * 2.0;
// Result is sRGB(1.0, 1.0, 1.0)
```

Simple conversions are implicit. (LinearRGB to sRGB, sRGB to LinearRGB, sRGB to sRGB_uint8, sRGB_uint8 to sRGB, HSV to sRGB, LinearHSV to LinearRGB, HCY to LinearRGB)

```cpp
LinearRGB val = sRGB(0.5, 0.5, 0.5);
```

Every colorspace can be converted to every other colorspace using colorspace_cast.

```cpp
HSV result = colorspace_cast<HSV>(LinearRGB(1.0, 0.0, 0.0));
```

libColorTool also features functions for processing images and color palettes, see the Examples section below for details.

## Including libColorTool in your project

The easiest way to use libColorTool is using git submodules and CMake.

Create a submodule...
```bash
git submodule add https://github.com/API-Beast/libColorTool.git
```

... and add it to your CMakeLists.txt.
```cmake
add_subdirectory(libColorTool)
target_link_library(YourLib libColorTool)
```

CMake will take care of the linker options and include directories, so that now you can include libColorTool.h to get access to all of it's functionality.
```cpp
#include <libColorTool.h>
```

<!-- everything below this will be replaced by the generate_readme.py script -->
<!-- auto-generated -->

## Color palettes

```cpp
#include "Palette/Palette.h"
```

Palettes are simply std::vectors of one of the color space classes.


### Stat functions

Stats functions are functions that analyze a color and return a array of all factors to consider.

|                           |       [0]       |       [1]       |       [2]       |    [3]    |    [4]   |
|---------------------------|:---------------:|:---------------:|:---------------:|:---------:|:--------:|
| Stats::perceptive_factors | luminance709    | vibrance        | redness         | greenness | blueness |
| Stats::linrgb_factors     | LinearRGB red   | LinearRGB green | LinearRGB blue  |           |          |
| Stats::srgb_factors       | sRGB red        | sRGB green      | sRGB blue       |           |          |

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