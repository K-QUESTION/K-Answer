#include "aes.h"

int main()
{
	// AES TEST Para//
	uint8_t pt[64] = { (uint8_t)0x6b, (uint8_t)0xc1, (uint8_t)0xbe, (uint8_t)0xe2, (uint8_t)0x2e, (uint8_t)0x40, (uint8_t)0x9f, (uint8_t)0x96, (uint8_t)0xe9, (uint8_t)0x3d, (uint8_t)0x7e, (uint8_t)0x11, (uint8_t)0x73, (uint8_t)0x93, (uint8_t)0x17, (uint8_t)0x2a,
					   (uint8_t)0xae, (uint8_t)0x2d, (uint8_t)0x8a, (uint8_t)0x57, (uint8_t)0x1e, (uint8_t)0x03, (uint8_t)0xac, (uint8_t)0x9c, (uint8_t)0x9e, (uint8_t)0xb7, (uint8_t)0x6f, (uint8_t)0xac, (uint8_t)0x45, (uint8_t)0xaf, (uint8_t)0x8e, (uint8_t)0x51,
					   (uint8_t)0x30, (uint8_t)0xc8, (uint8_t)0x1c, (uint8_t)0x46, (uint8_t)0xa3, (uint8_t)0x5c, (uint8_t)0xe4, (uint8_t)0x11, (uint8_t)0xe5, (uint8_t)0xfb, (uint8_t)0xc1, (uint8_t)0x19, (uint8_t)0x1a, (uint8_t)0x0a, (uint8_t)0x52, (uint8_t)0xef,
					   (uint8_t)0xf6, (uint8_t)0x9f, (uint8_t)0x24, (uint8_t)0x45, (uint8_t)0xdf, (uint8_t)0x4f, (uint8_t)0x9b, (uint8_t)0x17, (uint8_t)0xad, (uint8_t)0x2b, (uint8_t)0x41, (uint8_t)0x7b, (uint8_t)0xe6, (uint8_t)0x6c, (uint8_t)0x37, (uint8_t)0x10 };
	uint8_t key[16] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };

	// 구조체와 평문을 AES 함수에 적용
	test_aes(&key, &pt);

	return 0;
}
