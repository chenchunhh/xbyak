#define XBYAK_DONT_READ_LIST
#include <stdio.h>
#include <string.h>
#include "../xbyak/xbyak.h"
#define NUM_OF_ARRAY(x) (sizeof(x) / sizeof(x[0]))

using namespace Xbyak;
#ifdef _MSC_VER
	#pragma warning(disable : 4996) // scanf
	#define snprintf _snprintf_s
#endif

enum {
	PP_NONE = 1 << 0,
	PP_66 = 1 << 1,
	PP_F3 = 1 << 2,
	PP_F2 = 1 << 3,
	MM_0F = 1 << 5,
	MM_0F38 = 1 << 6,
	MM_0F3A = 1 << 7
};

const int NONE = 256; // same as Xbyak::CodeGenerator::NONE

std::string type2String(int type)
{
	std::string str;
	if (type & MM_0F) {
		str = "MM_0F";
	} else if (type & MM_0F38) {
		str = "MM_0F38";
	} else if (type & MM_0F3A) {
		str = "MM_0F3A";
	}
	if (type & PP_66) {
		str += " | PP_66";
	} else if (type & PP_F3) {
		str += " | PP_F3";
	} else if (type & PP_F2) {
		str += " | PP_F2";
	}
	return str;
}

void putOpmask()
{
	{
		const struct Tbl {
			const char *name;
			uint8 code;
		} tbl[] = {
			{ "kadd", 0x4A },
			{ "kand", 0x41 },
			{ "kandn", 0x42 },
			{ "kor", 0x45 },
			{ "kxnor", 0x46 },
			{ "kxor", 0x47 },
		};
		for (size_t i = 0; i < NUM_OF_ARRAY(tbl); i++) {
			const Tbl& p = tbl[i];
			printf("void %sw(const Opmask& r1, const Opmask& r2, const Opmask& r3) { opK(r1, &r2, r3, true, MM_0F, 0x%02X, 0); }\n", p.name, p.code);
			printf("void %sb(const Opmask& r1, const Opmask& r2, const Opmask& r3) { opK(r1, &r2, r3, true, MM_0F | PP_66, 0x%02X, 0); }\n", p.name, p.code);
			printf("void %sq(const Opmask& r1, const Opmask& r2, const Opmask& r3) { opK(r1, &r2, r3, true, MM_0F, 0x%02X, 1); }\n", p.name, p.code);
			printf("void %sd(const Opmask& r1, const Opmask& r2, const Opmask& r3) { opK(r1, &r2, r3, true, MM_0F | PP_66, 0x%02X, 1); }\n", p.name, p.code);
		}
		printf("void kunpckbw(const Opmask& r1, const Opmask& r2, const Opmask& r3) { opK(r1, &r2, r3, true, MM_0F | PP_66, 0x4B, 0); }\n");
		printf("void kunpckwd(const Opmask& r1, const Opmask& r2, const Opmask& r3) { opK(r1, &r2, r3, true, MM_0F, 0x4B, 0); }\n");
		printf("void kunpckdq(const Opmask& r1, const Opmask& r2, const Opmask& r3) { opK(r1, &r2, r3, true, MM_0F, 0x4B, 1); }\n");
	}
	{
		const struct Tbl {
			const char *name;
			uint8 code;
		} tbl[] = {
			{ "knot", 0x44 },
			{ "kortest", 0x98 },
			{ "ktest", 0x99 },
		};
		for (size_t i = 0; i < NUM_OF_ARRAY(tbl); i++) {
			const Tbl& p = tbl[i];
			printf("void %sw(const Opmask& r1, const Opmask& r2) { opK(r1, 0, r2, false, MM_0F, 0x%02X, 0); }\n", p.name, p.code);
			printf("void %sb(const Opmask& r1, const Opmask& r2) { opK(r1, 0, r2, false, MM_0F | PP_66, 0x%02X, 0); }\n", p.name, p.code);
			printf("void %sq(const Opmask& r1, const Opmask& r2) { opK(r1, 0, r2, false, MM_0F, 0x%02X, 1); }\n", p.name, p.code);
			printf("void %sd(const Opmask& r1, const Opmask& r2) { opK(r1, 0, r2, false, MM_0F | PP_66, 0x%02X, 1); }\n", p.name, p.code);
		}
	}
	{
		const struct Tbl {
			const char *name;
			uint8 code;
		} tbl[] = {
			{ "kshiftl", 0x32 },
			{ "kshiftr", 0x30 },
		};
		for (size_t i = 0; i < NUM_OF_ARRAY(tbl); i++) {
			const Tbl& p = tbl[i];
			printf("void %sw(const Opmask& r1, const Opmask& r2, uint8 imm) { opK(r1, 0, r2, false, PP_66 | MM_0F3A, 0x%02X, 1, imm); }\n", p.name, p.code);
			printf("void %sb(const Opmask& r1, const Opmask& r2, uint8 imm) { opK(r1, 0, r2, false, PP_66 | MM_0F3A, 0x%02X, 0, imm); }\n", p.name, p.code);
			printf("void %sq(const Opmask& r1, const Opmask& r2, uint8 imm) { opK(r1, 0, r2, false, PP_66 | MM_0F3A, 0x%02X, 1, imm); }\n", p.name, p.code + 1);
			printf("void %sd(const Opmask& r1, const Opmask& r2, uint8 imm) { opK(r1, 0, r2, false, PP_66 | MM_0F3A, 0x%02X, 0, imm); }\n", p.name, p.code + 1);
		}
	}
}

int main()
{
	puts("#ifdef XBYAK_AVX512");
	putOpmask();
	puts("#endif");
}
