| Supported Targets |   ESP32-S3 |
| ----------------- |  -------- |

# _RTOS Watch_

This is a smart watch that runs on the ESP-IDF RTOS, a specialized version of FreeRTOS. I used Squareline Studio to generate a UI and incorporated it in my code using LVGL libraries.

To run, click build project in ESP-IDF and then flash the program using the command `idf.py -p COMx flash monitor`

## Example folder contents

The project **sample_project** contains one source file in C language [main.c](main/main.c). The file is located in folder [main](main).

ESP-IDF projects are built using CMake. The project build configuration is contained in `CMakeLists.txt`
files that provide set of directives and instructions describing the project's source files and targets
(executable, library, or both). 

Below is short explanation of remaining files in the project folder.

```
├── CMakeLists.txt
├── main
│   ├── CMakeLists.txt
│   └── main.c
└── README.md                  This is the file you are currently reading
```
Additionally, the sample project contains Makefile and component.mk files, used for the legacy Make based build system. 
They are not used or needed when building with CMake and idf.py.
