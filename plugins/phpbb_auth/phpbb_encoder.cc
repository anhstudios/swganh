
#include "phpbb_encoder.h"

#include <cmath>

#include "md5.h"

using namespace plugins::phpbb_auth;
using namespace std;

PhpbbEncoder::PhpbbEncoder()
{
	itoa64_ = "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
}

string PhpbbEncoder::EncodePassword(
    string raw, 
    string test)
{
	string output = "*";

	// Check for correct hash
	if (test.substr(0, 3).compare("$H$")) return output;

	int count_log2 = itoa64_.find_first_of(test[3]);
	if (count_log2 < 7 || count_log2 > 30) return output;

	int count = 1 << count_log2;
	string salt = test.substr(4, 8);
	if (salt.length() != 8) return output;

	string hash = md5_(salt + raw);
	do {
	  hash = md5_(hash + raw);
	} while (--count);

	output = test.substr(0, 12);
	output += encode_(hash, 16);

	return output;
}

bool PhpbbEncoder::IsPasswordValid(
    std::string encoded, 
    std::string raw, 
    std::string salt)
{
    string test_encoded = EncodePassword(raw, encoded);
    return test_encoded.compare(encoded) == 0 ? true : false;
}

string PhpbbEncoder::md5_(string data) {
	unsigned char hash[16];
	md5_state_t state;
	md5_init(&state);
	md5_append(&state, (const unsigned char *)data.c_str(), data.length());
	md5_finish(&state, hash);
	string ret;
	for (int i=0; i<16; i++) ret += hash[i];
	return ret;
}

string PhpbbEncoder::encode_(string input, int count) 
{
	string output = "";

	int i = 0;

	do 
    {
		int value = (unsigned char)input[i++];
		output += itoa64_[value & 0x3f];
		if (i < count) value |= (((unsigned char)input[i]) << 8);
		output += itoa64_[(value >> 6) & 0x3f];
		if (i++ >= count) break;
		if (i < count) value |= (((unsigned char)input[i]) << 16);
		output += itoa64_[(value >> 12) & 0x3f];
		if (i++ >= count) break;
		output += itoa64_[(value >> 18) & 0x3f];
	} while (i < count);

	return output;

}