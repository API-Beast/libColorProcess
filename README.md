# libColorTool

libColorTool is a modern C++17 library for type-safe image processing. It provides types for the major relative color spaces used in image processing, as well as ways to convert between them. It is lightweight with the STL being the only dependecy.

In addition to the different color vectors it also features an ImageData class, iterators for image data as well as functions to deal with color palettes.

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

## Documentation

- [Color spaces](#color-spaces)
- [Image data](#image-data)
  * [Simple image processing](#simple-image-processing)
  * [Basic color remapping](#basic-color-remapping)
  * [Advanced color remapping](#advanced-color-remapping)
- [Color palettes](#color-palettes)

### Color spaces

The following color spaces are supported by libColorTool.

|      Colorspace | sRGB             | HSV                    | LinearRGB        | LinearHSV              | HCY                    |
|----------------:|------------------|------------------------|------------------|------------------------|------------------------|
|      Components | red, green, blue | hue, saturation, value | red, green, blue | hue, saturation, value | hue, chroma, luminance |
| Underlying Type | float            | float                  | float            | float                  | float                  |
|       Alignment | 16 bytes         | 16 bytes               | 16 bytes         | 16 bytes               | 16 bytes               |

|      Colorspace | sRGB_uint8       | sRGB_uint8_Alpha        | sRGB_Alpha              | LinearRGB_Alpha         |
|----------------:|------------------|-------------------------|-------------------------|-------------------------|
|      Components | red, green, blue | red, green, blue, alpha | red, green, blue, alpha | red, green, blue, alpha |
| Underlying Type | uint8_t          | uint8_t                 | float                   | float                   |
|       Alignment | 4 bytes          | 4 bytes                 | 16 bytes                | 16 bytes                |

All of these share the same API, having a complete set of operator overloads, scalar constructors and vector specific member functions.

sRGB_uint8 and sRGB_uint8_Alpha are both intended as a intermediate when importing or exporting external data. For any kind of mathematical operations you should use one of the float-based color spaces to avoid overflows. LinearRGB is preferable to sRGB as blending colors in sRGB space is low quality and creates unnatural transitions. HSV should be used together with sRGB, LinearHSV together with LinearRGB.

You might notice that all of these are aligned as if they had 4 components, even though most only have 3 and could fit into 12 bytes. This is because they are optimized for performance, not storage space. This allows them to be used efficiently with SIMD instructions and it allows the compiler to automatically vectorize loops involving these structures. Automatic vectorization is enabled by default with -O2 on clang and with -O3 on GCC.

```cpp
LinearRGB color(0.5, 0.5, 0.5);
color += 0.2;             // Equivalent to color += LinearRGB(0.2, 0.2, 0.2);
color += sRGB(0.2);       // Equivalent to += colorspace_cast<LinearRGB>(sRGB(0.2, 0.2, 0.2));
color *= {1.0, 0.0, 0.0}; // Green and blue are now 0.0
color.green += 0.5;       // Directly accessing color components

float len = color.length(); // There is also square_length for performance sensitive code
color = color.normalized(); // Equivalent to color /= len;
float largest_value = color.max_value(); // Returns the highest component of the three, there is also color.min_value()

// Component-wise execution of functions.
color = color.visit(&std::abs); // Non-vector functions can be called using "visit", it will be applied to each element individually
color = color.visit(&std::max, {0.4, 0.5, 0.6}); // Equivalent to {std::max(color.red, 0.4), std::max(color.green, 0.5), std::max(color.blue, 0.6)}
// Of course you can also use lambdas with .visit

LinearRGB_Alpha with_alpha = color; // Equivalent to LinearRGB_Alpha with_alpha(color, 1.0);

HSV hsv_result = colorspace_cast<HSV>(color); // Every colorspace can be converted to every other colorspace via colorspace_cast
```

There are implicit conversions for a few specific color spaces. This is intended so you can call for example `luminance709(LinearRGB)` with a sRGB color.

| Implicit Conversions | sRGB | sRGB_uint8 | HSV | LinearRGB | LinearHSV | HCY |
|:--------------------:|:----:|:----------:|:---:|:---------:|:---------:|:---:|
|         sRGB         |   -  |      ✔     |  ✔  |     ✔     |     ❌     |  ❌  |
|      sRGB_uint8      |   ✔  |      -     |  ❌  |     ❌     |     ❌     |  ❌  |
|          HSV         |   ✔  |      ❌     |  -  |     ❌     |     ❌     |  ❌  |
|       LinearRGB      |   ✔  |      ❌     |  ❌  |     -     |     ✔     |  ✔  |
|       LinearHSV      |   ❌  |      ❌     |  ❌  |     ✔     |     -     |  ❌  |
|          HCY         |   ❌  |      ❌     |  ❌  |     ✔     |     ❌     |  -  |

### Image data

#### Simple image processing

![Result of running invert_colors example](docs/invert_colors.jpg)
```cpp
```

#### Basic color remapping

```cpp
```

#### Advanced color remapping

### Color palettes

