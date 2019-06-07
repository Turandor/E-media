#include <iostream>
#include <fstream>
#include <cstring>
#include <cstddef>
#include <stdlib.h>
#include "structures.h"
using namespace std;

const char* INPUT = "test1.png";
const char* OUTPUT = "out.png";
const char* XOR = "Xor.png";
const char* KEY = "Klucz.png";
const char* KEYE = "Kluczencrypted.png";
const char* KEYD = "Kluczdecrypted.png";

/* Funkcja do wyœwietlania danych nag³ówka */
void printInfo(PNG_header PNGfile)
{
	cout << "--------------------------------INFO--------------------------------" << endl;
	cout << " Informacje z naglowka pliku PNG:" << endl;
	cout << " Szerokosc obrazu: " << PNGfile.Width << endl;
	cout << " Wysokosc obrazu: " << PNGfile.Height << endl;
	cout << " Glebia bitowa: " << PNGfile.BitDepth << endl;
	cout << " Typ koloru: " << PNGfile.ColorType << endl;
	cout << " Metoda kompresji: " << PNGfile.CompressionMethod << endl;
	cout << " Metoda filtrowania: " << PNGfile.FilterMethod << endl;
	cout << " Metoda konwolucji: " << PNGfile.InterlaceMethod << endl;
	cout << "--------------------------------------------------------------------" << endl;
};

int64_t getD(int64_t __enumber, int64_t __fnumber)
{
	__enumber = __enumber % __fnumber;
	for (int ijk = 1; ijk < __fnumber; ijk++)
		if ((__enumber*ijk) % __fnumber == 1)
			return ijk;
}

/* Funkcja zamiany liczb dziesiêtnych do szesnastkowych */
string decToHex(int n)
{
	char hexNum[2];
	int i = 1;
	string Num = "";
	while (i != -1)
	{
		int temp = 0;
		temp = n % 16;

		if (temp < 10)
		{
			hexNum[i] = temp + 48;
			i--;
		}
		else
		{
			hexNum[i] = temp + 55;
			i--;
		}

		n = n / 16;
	}
	Num = hexNum;

	return Num;
};

/* Funkcja zamiany liczb szesnastkowych na dziesiêtne */
int hexToDec(unsigned char Hex)
{
	int r = 0, hex = 0;

	if (Hex >= '0' && Hex <= '9')
		r = Hex - 48;
	else if (Hex >= 'a' && Hex <= 'f')
		r = Hex - 87;
	else if (Hex >= 'A' && Hex <= 'F')
		r = Hex - 55;
	hex += r * pow(16, 0);

	return hex;
};

void randomFile(size_t size)
{
	fstream key;
	key.open(KEY, ios::out | ios::binary);
	for (int i = 0; i < size; i++)
	{
		char cch = 255 + rand() % 255;
		//char cch = '1';
		key << cch;
	}
	key.close();
};

/* Czytanie plików */
char * openread(char* buf, const char* name, size_t size)
{
	ifstream stream;
	stream.open(name, ios::in | ios::binary);
	stream.seekg(0);
	stream.read(buf, size);
	stream.close();
	return buf;
}

void xorplik(size_t size, char* buf, int idata, int iend)
{
	fstream key, xorfile;
	char* bufkey = new char[size];
	char* bufxor = new char[size];
	openread(bufkey, KEY, size);
	xorfile.open(XOR, ios::out | ios::binary);

	for (int i = 0; i < idata + 6; i++)
		xorfile << buf[i];

	for (int i = idata + 6; i < iend - 4; i++)
		xorfile << (char)(buf[i] ^ bufkey[i]);

	for (int i = iend - 4; i < size; i++)
		xorfile << buf[i];

	key.close();
	xorfile.close();
}

void xordefile(size_t size, char* buf, int idata, int iend) {
	fstream key, xorfile;
	char* bufkey = new char[size];
	char* bufxor = new char[size];
	openread(bufkey, KEYD, size);
	xorfile.open(XOR, ios::in | ios::binary);
	xorfile.seekg(0);
	xorfile.read(bufxor, size);
	xorfile.close();
	xorfile.open(XOR, ios::out | ios::binary);

	for (int i = 0; i < idata + 6; i++)
		xorfile << bufxor[i];
	for (int i = idata + 6; i < iend - 4; i++)
		xorfile << (char)(bufxor[i] ^ bufkey[i]);

	for (int i = iend - 4; i < size; i++)
		xorfile << bufxor[i];

	key.close();
	xorfile.close();
}

int64_t modExp(int64_t basenumber, int64_t expnumber, int64_t mnumber)
{
	int64_t __answervariable = 1;
	for (int iii = 0; iii < expnumber; iii++)
	{
		__answervariable *= basenumber;
		__answervariable = __answervariable % mnumber;
	}
	return __answervariable;
}

/* Szyfrowanie RSA */
void encryptkey(RSA pq, bool type)
{
	char* buf = new char[pq.size];
	const char* name1;
	const char* name2;
	int charcount = 0;

	int64_t he1 = 0;
	ofstream keyen;
	if (type == false)
	{
		he1 = pq.e;
		name1 = KEY;
		name2 = KEYE;
	}
	else
	{
		he1 = pq.d;
		name1 = KEYE;
		name2 = KEYD;
	}
	buf = openread(buf, name1, pq.size);
	keyen.open(name2, ios::out | ios::binary);



	for (int i = 0; i < pq.size; i++)
	{
		int result = 1;
		for (; he1; he1 >>= 1)
		{
			if (he1 & 1) result = (1LL * result * buf[i]) % pq.n;
			buf[i] = (1LL * buf[i] * buf[i]) % pq.n;
		}
		keyen << buf[i];
	}

}

/* Wyœwietlanie kluczy algorytmu RSA*/
void printRSA(RSA pq) {
	cout << "-------------------------------Klucze-------------------------------" << endl;
	cout << " Para klucza publicznego (n,e): " << pq.n << " " << pq.e << endl;
	cout << " Para klucza prywatnego (n,d): " << pq.n << " " << pq.d << endl;
	cout << "--------------------------------------------------------------------" << endl << endl;
}

/* Generowanie kluczy */
RSA PQ(RSA pq, size_t size) {
	pq.p = 101467;
	pq.q = 103951;
	pq.n = (pq.p * 1)*(pq.q * 1);
	pq.f = (pq.p - 1) * 1 * (pq.q - 1) * 1;
	pq.e = 14293;
	pq.size = size;
	pq.d = getD(pq.e, pq.f);
	printRSA(pq);
	return pq;
}



int main()
{
	ifstream inFile;
	ofstream outFile;
	RSA pq;
	PNG_header PNG_file;
	size_t size = 0;
	string signature[8];

	int value = 0, nextChunkLenght, actualBit;
	int ihdr, idat, iend;
	int * data1 = 0;
	char * data = 0;
	bool IsFileOk[3] = { false, false, false };
	int pause;

	/* Pliki robocze */
	inFile.open(INPUT, ios::in | ios::binary);
	outFile.open(OUTPUT, ios::out | ios::binary);

	inFile.seekg(0, ios::end);
	size = inFile.tellg();
	inFile.seekg(0);
	data = new char[size];
	data1 = new int[size];
	inFile.read(data, size);
	bool idatawas = 0;

	cout << "Rozpoczynam sprawdzanie poprawnoœci pliku..." << endl;
	/* Bloki w PNG */
	for (int i = 0; i < size; i++)
	{
		if (data[i] == 'I' && data[i + 1] == 'H' && data[i + 2] == 'D' && data[i + 3] == 'R')
		{
			cout << "Znaleziono Naglowek pliku PNG..." << endl;
			ihdr = i;
			IsFileOk[0] = true;
		}
		if (data[i] == 'I' && data[i + 1] == 'D' && data[i + 2] == 'A' && data[i + 3] == 'T' && IsFileOk[1] == false)
		{
			cout << "Znaleziono dane z pikselami..." << endl;
			idat = i;
			IsFileOk[1] = true;

		}
		if (data[i] == 'I' && data[i + 1] == 'E' && data[i + 2] == 'N' && data[i + 3] == 'D')
		{
			cout << "Znaleziono koniec pliku..." << endl;
			iend = i;
			IsFileOk[2] = true;
		}
	}

	if (IsFileOk[0] = false) cout << "B³¹d! Nie znaleziono naglowka pliku PNG!" << endl;
	if (IsFileOk[1] = false) cout << "B³¹d! Nie znaleziono danych z pikselami!" << endl;
	if (IsFileOk[2] = false) cout << "B³¹d!: nie znaleziono zakoñczenia pliku!" << endl;

	for (int i = 0; i < 8; i++)
	{
		signature[i] = decToHex((int)data[i]);
		for (int j = 2; j < 10; j++)
			signature[i][j] = NULL;
	}
	//if ( help[0][0] == '8' && help[0][1] == '9') { P++; }

	/* Wyszukiwanie sygnatury pliku PNG */
	if ((signature[1][0] == '5' && signature[1][1] == '0') &&
		(signature[2][0] == '4' && signature[2][1] == 'E') &&
		(signature[3][0] == '4' && signature[3][1] == '7') &&
		(signature[4][0] == '0' && signature[4][1] == 'D') &&
		(signature[5][0] == '0' && signature[5][1] == 'A') &&
		(signature[6][0] == '1' && signature[6][1] == 'A') &&
		(signature[7][0] == '0' && signature[7][1] == 'A'))
		cout << "Sygnatura pliku PNG zostala znaleziona! Rozpoczynam przetwarzanie..." << endl << endl;

	else
	{
		cout << "Sygnatura pliku PNG nie zosta³a znaleziona! Sprawdz plik." << endl << endl;
		return 0;
	}

	int j = 3;
	actualBit = 8;
	for (int i = actualBit; i < actualBit + 4; i++)
	{
		value += pow(16, j)* data[i];
		j--;
	}
	actualBit += 4;
	nextChunkLenght = value;

	/* Odczytanie wysokoœci obrazka */
	j = 7;
	actualBit += 4;
	value = 0;
	for (int i = actualBit; i < actualBit + 4; i++)
	{
		value += pow(16, j) * hexToDec(decToHex(data[i])[0]);
		j--;
		value += pow(16, j)* hexToDec(decToHex(data[i])[1]);
		j--;
	}
	PNG_file.Height = value;
	actualBit += 4;
	value = 0;


	/* Odczytanie szerokoœci obrazka */
	j = 7;
	for (int i = actualBit; i < actualBit + 4; i++)
	{
		value += pow(16, j)* hexToDec(decToHex(data[i])[0]);
		j--;
		value += pow(16, j)* hexToDec(decToHex(data[i])[1]);
		j--;
	}
	actualBit += 4;
	PNG_file.Width = value;


	/* Odczytanie g³êbi bitowej */
	PNG_file.BitDepth = pow(16, 1)* hexToDec(decToHex(data[actualBit])[0]);
	PNG_file.BitDepth += pow(16, 0)* hexToDec(decToHex(data[actualBit++])[1]);
	/* Odczytanie typu kolorów */
	PNG_file.ColorType = pow(16, 1)* hexToDec(decToHex(data[actualBit])[0]);
	PNG_file.ColorType += pow(16, 0)* hexToDec(decToHex(data[actualBit++])[1]);
	/* Odczytanie metody kompresji */
	PNG_file.CompressionMethod = pow(16, 1)* hexToDec(decToHex(data[actualBit])[0]);
	PNG_file.CompressionMethod += pow(16, 0)* hexToDec(decToHex(data[actualBit++])[1]);
	/* Odczytanie metody filtracji */
	PNG_file.FilterMethod = pow(16, 1)* hexToDec(decToHex(data[actualBit])[0]);
	PNG_file.FilterMethod += pow(16, 0)* hexToDec(decToHex(data[actualBit++])[1]);
	/* Odczytanie metody przeplotu */
	PNG_file.InterlaceMethod = pow(16, 1)* hexToDec(decToHex(data[actualBit])[0]);
	PNG_file.InterlaceMethod += pow(16, 0)* hexToDec(decToHex(data[actualBit++])[1]);
	//cout << data << " ";


	randomFile(size);
	xorplik(size, data, idat, iend);
	pq = PQ(pq, size);
	encryptkey(pq, false);
	encryptkey(pq, true);
	xordefile(size, data, idat, iend);
	outFile.write(data, size);
	outFile.close();
	inFile.close();

	printInfo(PNG_file);
	cin >> pause;
	return 0;
}