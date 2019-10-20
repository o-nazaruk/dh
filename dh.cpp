// Implementing diffie-hellman algorithm in cpp.
#include <iostream>
#include <random>
#include <fstream>
#include <limits>
using namespace std;
typedef unsigned long long ull;

bool isPrime(int number);

//modular exponentiation function for large numbers.
// TODO: make it cross-platform
ull modpow(__uint128_t base, ull exp, ull modulus) {
  base %= modulus;
  ull result = 1;
  while (exp > 0) {
    if (exp & 1) result = (result * base) % modulus;
    base = (base * base) % modulus;
    exp >>= 1;
  }
  return result;
}
int main(){
	ull g;
	ull n;

	cout << "Generate new public keys? ";
	char ch;
	cin >> ch;
	if(ch == 'y'){
		ofstream file;
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<ull> dis;
		uniform_int_distribution<> prime(7, 100);
		n = dis(gen);
  	file.open ("dh_keys/n.txt", ios::binary);
		file.write(reinterpret_cast<const char *>(&n), sizeof(n));
		file.close();

		for (int currentNum = prime(gen); currentNum!=2; currentNum--) {
        if (isPrime(currentNum)) {
            g = currentNum;
						break;
        }
    }
  	file.open ("dh_keys/g.txt", ios::out | ios::binary);
		file.write(reinterpret_cast<const char *>(&g), sizeof(g));
		file.close();
		cout << "Public keys generated successfully!" << endl;

	} else {
		ifstream file;
		file.open ("dh_keys/n.txt", ios::binary);
		file.read(reinterpret_cast<char *>(&n), sizeof(n));
		file.close();

		file.open("dh_keys/g.txt", ios::binary);
		file.read(reinterpret_cast<char *>(&g), sizeof(g));
		file.close();
	}

	cout << "Public keys:\ng = " << g << "\nn = " << n << endl;

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<ull> dis;
	ull privateKey = dis(gen);
	ull sharedKey = modpow(g, privateKey, n);
	cout << "Shared key: " << sharedKey <<  endl;

	ull inShKey; // Shared key of other participant
	/*
	Here both parties need to share their key between each other.
	While in this process those keys can be exposed to the attacker but they should be protected from potential modifications
	*/
	cout << "Enter another party shared key: ";
	cin >> inShKey;

	ull chKey = modpow(inShKey, privateKey, n);
	cout << "\nFinal key: " << chKey;
	return 0;
}

bool isPrime(int number){
	for (int i = 2; i < number - 1; i++)
	{
		if(number%i==0){
			return false;
		}
	}
	return true;
}