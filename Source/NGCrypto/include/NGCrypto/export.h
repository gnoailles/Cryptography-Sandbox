#pragma once

#ifdef NG_CRYPTO_EXPORT
#define NG_CRYPTO_API __declspec(dllexport)
#else
#define NG_CRYPTO_API __declspec(dllimport)
#endif