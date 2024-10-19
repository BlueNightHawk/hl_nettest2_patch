// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

struct IUnknown;

#include "framework.h"
#include <string>
#include <stdio.h>

#include <ddraw.h>
#include "proxy_dxgi.h"
#include <dxgi.h>
#include "MinHook.h"

#include <vector>
#include <cstdlib>
#include <string>

#include "ini.h"
#include "ofs.h"

int InitHook();
int FreeHook();

#endif //PCH_H
