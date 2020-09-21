# Simple recursive flood fill algorithm implementation

## What the algorithm looks like in action
<p align="center">
<img width=250 src="optimized_example.gif">
</p>

## Introduction
This was made based on the wikipedia page on the flood fill algorithm: https://en.wikipedia.org/wiki/Flood_fill

## Compilation
If you have gcc and libpng-dev installed, you can compile the code by doing:
```
./compile.sh 
```
If you are on Debian, you can install libpng by doing:
```
sudo apt install libpng-dev
```

# Usage
This is an implementation of the recursive version of the flood fill algorithm, so
it is only feasible for usage with smaller images. After compilation, using an imput png you can do:
```
flood_fill_png <png_in> <x_root> <y_root> <R> <G> <B> <A> <R_target> <G_target> <B_target> <A_target> <png_out>
```
Where:
 - *png_in* : Input png image
 - *x_root* : X coordinate to start filling from
 - *y_root* : Y coordinate to start filling from
 - *R* : Red value of target color to replace
 - *G* : Green value of target color to replace
 - *B* : Blue value of target color to replace
 - *A* : Transparency value of target color to replace
 - *R_target* : Red value to replace target color with
 - *G_target* : Green value to replace target color with
 - *B_target* : Blue value to replace target color with
 - *A_target* : Transparency value to replace target color with
 - *png_out* : Output file name

