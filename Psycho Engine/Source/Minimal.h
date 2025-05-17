#pragma once

#ifdef ENGINE_EXPORTS

#define GraphicsAPI __declspec(dllexport)

#else

#define GraphicsAPI __declspec(dllimport)

#endif