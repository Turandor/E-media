#pragma once

using namespace std;

/* Struktura do danych algorytmu RSA */
struct RSA
{
	int64_t n = 0;
	int64_t p = 0;
	int64_t q = 0;
	int64_t e = 0;
	int64_t f = 0;
	int64_t d = 0;
	size_t size = 0;
};

/* Struktura przechowuj¹ca informacje nag³ówka */
struct PNG_header
{
	int Width = 0;
	int Height = 0;
	int BitDepth = 0;
	int ColorType = 0;
	int CompressionMethod = 0;
	int FilterMethod = 0;
	int InterlaceMethod = 0;
};
