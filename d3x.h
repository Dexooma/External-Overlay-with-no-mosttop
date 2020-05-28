#pragma once

#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <memory>
#include <string_view>
#include "Overlay.h"

// need to include those libs else we will get errors.
#include <Dwmapi.h> 
#include <d3d9.h>
#include <d3dx9.h>
#include <time.h>
#pragma comment(lib, "Dwrite")
#pragma comment(lib, "Dwmapi.lib") 