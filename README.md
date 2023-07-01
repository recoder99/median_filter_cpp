# Median Filter CLI

A median filter implementation using C++. Currently supports grayscale images only.

Algorithms used:
 - Merge Sort
 - Median Filter

# Dependencies
 - OpenCV C++ Library (Required)- must set `bin` folder to environment variables or `PATH`
 - Visual Studio (Optional) - For building the source code. Not required if using prebuilt binary files.
 - VS Development Kit for C++ (Optional)

# Usage

`Median_Filter_CLI.exe` `<option> <argument>`

Options:
 - `--input <filepath>` or `-i <filepath>`  specify the file path image
   source.
 - `--output <filepath>` or `-o <filepath>`  specifies the save path of
   a filtered image
 - `--window <window size>` or `-w <window size>` specify the window
   size (It must be greater than or equal 3 and an odd number). Will set to 3 as
   a default if specified incorrectly
 - `--log <save_path\filename>` enables detailed logging of merge sort and pixel manipulation and specifies the save path of the log file
 - `--verbose` outputs each pixel being currently manipulated into the console.
 - `--enable-preview` shows the filtered image

## Examples 

**Using Powershell:**
The following command reads the `C:\Users\billg\Pictures\noisy_image.png` as an image source. The program will set the window size to `3` after the filter process, the program will preview the filtered image and waits for the user to press any key. Finally, the save the image in `C:\Users\billg\Pictures\` with file name 
`denoise_image.png`

> `.\Median_Filter_CLI.exe --input
> C:\Users\billg\Pictures\noisy_image.png --output
> C:\Users\billg\Pictures\denoise_image.png --window 3 --enable-preview`

This can also be written as:

> `.\Median_Filter_CLI.exe -i C:\Users\billg\Pictures\noisy_image.png -o
> C:\Users\billg\Pictures\denoise_image.png -w 3 --enable-preview`


## How to build
(This section is still being updated)
If you want to build the code from scratch here are the steps:

 1. Install OpenCV and set the `bin` folder to environment variables or `PATH`
 2. Run the solution file on visual studio
 3. Set the include directories, libraries, and linkers in the project properties.  Instructions for setting up OpenCV Libraries can be found here: https://subwaymatch.medium.com/opencv-410-with-vs-2019-3d0bc0c81d96
 4. Build the code and run

