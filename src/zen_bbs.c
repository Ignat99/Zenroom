/*
 * This file is part of zenroom
 * 
 * Copyright (C) 2017-2024 Dyne.org foundation
 * designed, written and maintained by Denis Roio <jaromil@dyne.org>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License v3.0
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * 
 * Along with this program you should have received a copy of the
 * GNU Affero General Public License v3.0
 * If not, see http://www.gnu.org/licenses/agpl.txt
 * 
 * Last modified by Filippo Trotter
 */
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <zenroom.h>
#include <zen_error.h>
#include <zen_memory.h>
#include <lua_functions.h>
#include <zen_octet.h>
#include <zen_error.h>
#include <amcl.h>
#include <zen_big.h>
#include <zen_ecp.h>




BIG_384_29 SSWU_A1_BLS381 = {0xd584c1d, 0x07a14041, 0x183e5fd7, 0x06df1b41, 0x081ac989, 0xc0d77ec, 0x1aa363a2, 0x0a707dcc, 0x02b0ea98, 0x164b6a4c, 0x0f5a4e80, 0x0771d286, 0x0144698a, 0x0};
BIG_384_29 SSWU_B1_BLS381 = {0xe172be0, 0x0e62474c, 0x1b3aa974, 0x0642b462, 0x15ef55a2, 0x0a7e779, 0x01c282e7, 0x1e1e49e8, 0x1b2016c1, 0x03a9f771, 0x0062c4ba, 0x02d10060, 0x0e2908d1, 0x9};
BIG_384_29 SSWU_Z1_BLS381 = {0x000000b, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0};
BIG_384_29 H_EFF_G1 = {0x10001, 0x10080000, 0x34, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

BIG_384_29 ISO11_XNUM_BLS381[12] = {
  {0x134649b7, 0x1560b313, 0x198b5bab, 0x0185abe5, 0x0e2c8561, 0x1dab66da, 0x017fc989, 0x11145ae0, 0x056b303e, 0x0eccc0ac, 0x0e024407, 0x1d066681, 0x1a05f2b1, 0x8},
  {0x13cb83bb, 0x01a7778d, 0x0630d5ba, 0x11e54de6, 0x1e86b483, 0x119e3868, 0x105fd597, 0x0b65ed50, 0x1c7c17e7, 0x110a3d40, 0x01622eac, 0x1287565e, 0x1294ed3e, 0xb},
  {0x0c9edcb0, 0x00bcfced, 0x025ca7f8, 0x187c7a54, 0x0e25c958, 0x1280f634, 0x0f95a1e3, 0x0e652b30, 0x1bce0324, 0x0e8854d0, 0x07441231, 0x12ecf1d8, 0x154005db, 0x6},
  {0x1d9b6861, 0x0d9c4320, 0x041c64f1, 0x0dc4b9c6, 0x13083533, 0x1944f8d9, 0x1c97c6cc, 0x0cad51b7, 0x12d7f5e4, 0x183f2aa0, 0x13818274, 0x1f98db6e, 0x178e7166, 0xb},
  {0x0c8895d9, 0x08aa674d, 0x079df114, 0x1450de60, 0x1ac18985, 0x15b2cc17, 0x0cfc21bb, 0x0b424aff, 0x1499db99, 0x1f208c72, 0x1990ad2c, 0x0333e886, 0x099726a3, 0x7},
  {0x0f652983, 0x089e0e33, 0x19cf4673, 0x0e1a5b95, 0x08f90a08, 0x15c84bf3, 0x066e7b4e, 0x0fbb2a4f, 0x15db3cb1, 0x1fbd3a55, 0x00744806, 0x1ae627fe, 0x030c3250, 0xb},
  {0x0139ed84, 0x0ebf912d, 0x014bb2b7, 0x04a25182, 0x06b2a8da, 0x110c7ce4, 0x13864023, 0x04c9e1f1, 0x1fb11586, 0x1c573295, 0x1a8dc9b0, 0x1fc89a52, 0x16ed6553, 0x6},
  {0x03f0c88e, 0x065ab0c7, 0x1d1d6be7, 0x0f91f191, 0x0753339b, 0x03177879, 0x16c69a0b, 0x1564eb69, 0x13356de5, 0x06888bf2, 0x1a1d0e21, 0x0357b7c5, 0x1b81e770, 0xb},
  {0x0497e317, 0x0b8cc354, 0x0dd3a55b, 0x052be52d, 0x1d1de4fa, 0x0b649462, 0x15d28b16, 0x0d9cf3ea, 0x0dc43b75, 0x0b1df4c8, 0x1ee42ccd, 0x134f1f88, 0x00d3cf1f, 0x4},
  {0x1e390c9e, 0x1920833d, 0x00c9de5f, 0x12165db8, 0x11b7fa31, 0x0a5d7a5d, 0x12659d8c, 0x1007418b, 0x02dd2ecb, 0x0ae89c79, 0x0b830dd4, 0x179f4f88, 0x09b1f8e1, 0xb},
  {0x1605fb7b, 0x133ef9f8, 0x0a177b32, 0x16ee3f18, 0x14866f69, 0x19b001d8, 0x1e5b542b, 0x1bbccf0f, 0x0dfa7dcc, 0x0e92b2d8, 0x1cb63b02, 0x139c0fc4, 0x0321da07, 0x8},
  {0x0ba2d229, 0x0e45d174, 0x134e47ea, 0x1637016c, 0x06b68c24, 0x1f8de126, 0x1ef08f02, 0x0fc45906, 0x1d31d79d, 0x1c0f6f71, 0x0f47a588, 0x1c4c1ce1, 0x0e08c248, 0x3}
};

BIG_384_29 ISO11_XDEN_BLS381[11] = {
  {0x00d21b1c, 0x09e7cfd2, 0x0d0f7e26, 0x11ad037c, 0x0ac62b55, 0x0430bfe4, 0x02ea7256, 0x09746b69, 0x0f01d5ef, 0x1a5e9fd3, 0x062cb98b, 0x19fe335c, 0x0ca8d548, 0x4},
  {0x082b3bff, 0x0e413b76, 0x0c09ba79, 0x155108d9, 0x0bf5713d, 0x012c4624, 0x0030049b, 0x19419e10, 0x167041e8, 0x14c729b1, 0x122d1c44, 0x16ab3886, 0x0561a5de, 0x9},
  {0x1cb83e19, 0x0611cdd2, 0x053fb73f, 0x07a12cf9, 0x0ceacd6a, 0x0700588d, 0x1347f299, 0x0deb4e31, 0x1f6f8941, 0x0dff94c8, 0x004df98a, 0x0f4644bd, 0x12962fe5, 0x5},
  {0x0dc62cd8, 0x186f449c, 0x1b3d7104, 0x0daa487d, 0x16fd0497, 0x1455e146, 0x15455332, 0x07e2d62c, 0x145b0824, 0x1be2075a, 0x120eabfb, 0x0b15c5fd, 0x1425581a, 0x1},
  {0x1532a21e, 0x1ce9cad9, 0x0d5e0754, 0x0537503e, 0x106da9bd, 0x027419d9, 0x0aee35ad, 0x0b34240c, 0x1dffdfc7, 0x1a1f3d03, 0x029bc757, 0x04522950, 0x1a8e1620, 0x9},
  {0x1f6304a5, 0x016fcd14, 0x08a3c470, 0x01a49788, 0x0982f740, 0x1e77925c, 0x1534290e, 0x1d39d395, 0x09395735, 0x18283637, 0x154e43df, 0x09cccf72, 0x07355f8e, 0x7},
  {0x0ee84a3a, 0x012ba24b, 0x03781b3b, 0x0766a71e, 0x0de9cea7, 0x03983157, 0x062538b8, 0x1335ea74, 0x01570f57, 0x1f02cb39, 0x03cf8318, 0x02d26c32, 0x172caacf, 0x3},
  {0x1dcc5a5e, 0x0fbeccdd, 0x0478b4c4, 0x0b72913a, 0x02c580fa, 0x10e6fcc1, 0x02a0665b, 0x1843794d, 0x196e7f63, 0x03a6780c, 0x0c2cfd6c, 0x1ac95164, 0x0a7ac2a9, 0xa},
  {0x19a1d641, 0x1bb761d3, 0x0e90dc11, 0x04cd2557, 0x18835038, 0x06d33f9c, 0x19add040, 0x03ae2c26, 0x0ce07f8d, 0x0d7e3d1e, 0x17a482cf, 0x1b4a9f04, 0x010ecf6a, 0x5},
  {0x08ecdd0a, 0x0b1c268b, 0x1e19400b, 0x0e9c9696, 0x11c15931, 0x099cbc79, 0x00dddb7d, 0x1dd2defa, 0x00f682b4, 0x159d2b34, 0x11db5b8f, 0x13d255a8, 0x15fc13ab, 0x4},
  {0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0}
};

BIG_384_29 ISO11_YNUM_BLS381[16] = {
  {0x1707bb33, 0x14c22b8c, 0x0ee8f0af, 0x18f5dd36, 0x143d3cd0, 0x17b64ab2, 0x0548ad4a, 0x11c9150d, 0x1a11ad13, 0x0a4c06e7, 0x096747c2, 0x17449dc0, 0x10d97c81, 0x4},
  {0x0e41c696, 0x04bf3ad1, 0x0bea2ff8, 0x0ace232c, 0x1ad34d6c, 0x11a1f5b3, 0x00f43e41, 0x0d84a9e7, 0x031223e9, 0x1bb7da34, 0x15440db5, 0x09dcb023, 0x14996a10, 0x9},
  {0x072de1f6, 0x06ff1206, 0x0c0148ee, 0x1aa42c51, 0x00da7d26, 0x1f25c8a0, 0x138b0d12, 0x1acb1463, 0x142552e2, 0x0351da4c, 0x1d28e132, 0x152cdccd, 0x0cc786ba, 0x0},
  {0x10e5f4cb, 0x11aae3bd, 0x11877b29, 0x00b5753d, 0x11cf9de4, 0x11f60192, 0x04702792, 0x1721dd6f, 0x17d42aa7, 0x16c3a33a, 0x1e261d46, 0x11303842, 0x1f86376e, 0x0},
  {0x12e8fedb, 0x0db6d767, 0x04102a10, 0x0ff1b813, 0x11adc2ee, 0x1fe9109a, 0x02e1e60c, 0x1f7c79ca, 0x04195536, 0x1510a94e, 0x172bd3f8, 0x1fc1fe26, 0x0cc03fde, 0x4},
  {0x1633a5f0, 0x0d91d589, 0x16a01ca6, 0x1ec64d92, 0x1544e203, 0x0e1e9d6a, 0x1ef5d941, 0x1a95f5b6, 0x0074a7d0, 0x0dc78535, 0x08847847, 0x00c696d4, 0x0603fca4, 0xb},
  {0x1fe9d6f2, 0x0b0fc42a, 0x03d057b2, 0x10f5848c, 0x14f3747a, 0x009e26b1, 0x132d48c5, 0x19457c30, 0x1ce75bb8, 0x013bcb59, 0x0cb25df4, 0x1f583779, 0x0ab0b9bc, 0x2},
  {0x1870fb29, 0x0af26518, 0x17fa4d68, 0x0c8aa1fd, 0x0842642f, 0x06d36136, 0x007ff40e, 0x17fc77bb, 0x14170a05, 0x09653633, 0x17a649af, 0x067570df, 0x187c8d53, 0x4},
  {0x1bdba587, 0x01b872bb, 0x0181e8d8, 0x0ca4038f, 0x0cabe69d, 0x17350f90, 0x09b07a2d, 0x02ccf3b8, 0x1b8f3abd, 0x10f26d0d, 0x1a232788, 0x1b2cd097, 0x1fc4018b, 0x4},
  {0x0a731c30, 0x1d7d575e, 0x13ae9bca, 0x1ee0abba, 0x0d43b9b3, 0x0f3f68f2, 0x1bf81a61, 0x14f22b5e, 0x03c42a0c, 0x1d6d0a51, 0x088eaf79, 0x030d7b6a, 0x01bba7a1, 0x7},
  {0x1011c132, 0x009b88d6, 0x0feebf3a, 0x1e74b99c, 0x1e61031b, 0x1f20b1c4, 0x04ff4460, 0x196d95e9, 0x13cd2fcb, 0x18ea1fdc, 0x037f42e3, 0x06f9a37c, 0x1713e479, 0xc},
  {0x10074d8e, 0x103e4526, 0x113581b3, 0x139be836, 0x1643249d, 0x1f3fc88f, 0x0918b9af, 0x17155e18, 0x0c523559, 0x1ff6976e, 0x0e463050, 0x1e6dedbd, 0x0b46a908, 0xc},
  {0x0b971ef8, 0x0a602780, 0x04847c83, 0x10a38323, 0x0633f06c, 0x087403da, 0x023b009c, 0x054684d6, 0x047aa7b1, 0x0027a9fa, 0x14554258, 0x00372733, 0x1182cac1, 0x5},
  {0x1b980133, 0x16ce9fae, 0x08ca9910, 0x1f215a38, 0x0659cc6c, 0x11969e20, 0x16004f99, 0x0101a982, 0x1c757b3b, 0x13df18ae, 0x1cbf002b, 0x1a3d9536, 0x045a394a, 0x1},
  {0x1475224b, 0x1358f38a, 0x1e6bede1, 0x020936ca, 0x07ce46ba, 0x07ae9cb5, 0x15a366ac, 0x103afd0c, 0x1c5e673d, 0x1a46251f, 0x00a8567d, 0x1c899e22, 0x1c129645, 0x2},
  {0x09c8b604, 0x05a2b5f3, 0x10071dc1, 0x0a04fdfd, 0x101b2b66, 0x0a7d4ad7, 0x08e55eb7, 0x11f092cb, 0x15cb181d, 0x1a16f975, 0x13a942ce, 0x121e079c, 0x1e6be4e9, 0xa}
};

BIG_384_29 ISO11_YDEN_BLS381[16] = {
  {0x103663c1, 0x0a3c929d, 0x03081b40, 0x06d11dec, 0x12e7a07f, 0x1195adf3, 0x0f9bbb0c, 0x1caf1301, 0x09601a6d, 0x07d68757, 0x14860450, 0x15393164, 0x0112c4c3, 0xb},
  {0x0e49a03d, 0x17b08161, 0x14a78d4c, 0x084c0ec6, 0x1e01f78a, 0x01ab7a29, 0x16729284, 0x1ee6389a, 0x1885c84f, 0x021e1a45, 0x06832f5b, 0x0702403c, 0x162d75c2, 0xc},
  {0x1dbf67f2, 0x1129c5a9, 0x1e5be247, 0x0af9ac6d, 0x0d2eca67, 0x12ee93ce, 0x1cc430d6, 0x0aaa35cf, 0x1778c485, 0x0b74758a, 0x1beaab9f, 0x0c81b44e, 0x18df3306, 0x2},
  {0x045f5416, 0x06936cc2, 0x00a5eb6a, 0x06c9e585, 0x0af41727, 0x1244f393, 0x0c3848f6, 0x1b7bb79a, 0x11d115c5, 0x1c4f6da6, 0x1c8348ef, 0x131ca72b, 0x0b7d2887, 0xb},
  {0x11a5001d, 0x11c8a118, 0x14bb7b76, 0x162bb81f, 0x0c916a20, 0x0d07e4ef, 0x0ec150bb, 0x13e1ed37, 0x1cc6d19c, 0x17c1146e, 0x0c033244, 0x08be87c9, 0x1e0e0795, 0x5},
  {0x0af9b7ac, 0x16323bfd, 0x0a733880, 0x071b73bf, 0x15a6449f, 0x0c3db787, 0x020717b3, 0x18caaa1b, 0x02b70152, 0x1563c18c, 0x07ec99ba, 0x030db65b, 0x0d9e5297, 0x4},
  {0x126a775c, 0x08d09cc8, 0x02c7ee4f, 0x1538034b, 0x00051d5f, 0x12de2005, 0x03bd774d, 0x1f51a19f, 0x0b5eecfd, 0x05674c12, 0x10eea1cd, 0x1533b65f, 0x06007c08, 0xb},
  {0x15812ed9, 0x07720ad0, 0x0077b918, 0x01eb6010, 0x17132b92, 0x07e9031a, 0x1f5ffacd, 0x0bdf43e9, 0x0ee5a437, 0x15dd37fb, 0x00ef377e, 0x1c7d4fd4, 0x0a3ef08b, 0xb},
  {0x15535d4a, 0x1919ecea, 0x049220da, 0x1fc5ef77, 0x19b4852c, 0x1a8625f9, 0x0482af15, 0x1c98d5eb, 0x04f9fb0c, 0x1e8eba66, 0x0686f953, 0x06d8c246, 0x066c8ed3, 0xc},
  {0x18913f55, 0x0377a45d, 0x0a6cd78d, 0x10bd47aa, 0x1d4fbc73, 0x0c973f53, 0x1eed4c21, 0x0c7c27b0, 0x103216f7, 0x1eca5424, 0x1aa08165, 0x0e14dc39, 0x07a55cda, 0xb},
  {0x1a8f6aa8, 0x07c5a4e5, 0x00c18100, 0x0b853e9f, 0x0a5c871a, 0x0d9b731b, 0x18a43964, 0x07376c34, 0x1d9c6dd0, 0x00d69488, 0x123c0428, 0x1d480b7a, 0x0d2f259e, 0x2},
  {0x02561092, 0x1425a94f, 0x1faefaa5, 0x12d130de, 0x1913516f, 0x0d446753, 0x0b4a303e, 0x115df9c8, 0x077f94ff, 0x12462862, 0x1d614b07, 0x103a067f, 0x0ccbb674, 0x5},
  {0x173345cc, 0x14cd89c2, 0x0e42b047, 0x000ec7c7, 0x19b86930, 0x177cd006, 0x0899f573, 0x1b315be0, 0x16543346, 0x05a2f8a4, 0x10d84c51, 0x18ecffc7, 0x0d6b9514, 0x5},
  {0x06ed06f7, 0x0fd6e099, 0x05332034, 0x0a2f7b0e, 0x0480e420, 0x06f93ca1, 0x1f072dd2, 0x129ce524, 0x12bf565b, 0x0a9e6bb7, 0x18a2f743, 0x165c9e76, 0x0660400e, 0x1},
  {0x1d634b8f, 0x00aa39d0, 0x0d25e011, 0x05eae1e2, 0x0aa205ca, 0x1e6b1ab6, 0x014cc93b, 0x0cbc4e77, 0x0171c40f, 0x106bc0ce, 0x1ac90957, 0x0dbb807c, 0x00fa1d81, 0x7},
  {0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0}
};

static inline int sgn0_fp(FP_BLS381 a)
{
    BIG b;
    FP_BLS381_redc(b, &a);
    return BIG_parity(b);
}

// draft-irtf-cfrg-hash-to-curve-16 Appendix F.2
// It returns the (x,y) coordinate of a point over E' (isogenous curve)
int ECP_sswu(FP_BLS381 *x, FP_BLS381 *y, FP_BLS381 u) {
 
    FP_BLS381 a;
    FP_BLS381 b;
    FP_BLS381 z;
    FP_BLS381 gx;
    FP_BLS381 tv1;
    FP_BLS381 tmp;

    FP_BLS381_nres(&a, SSWU_A1_BLS381);
    FP_BLS381_nres(&b, SSWU_B1_BLS381);
    FP_BLS381_nres(&z, SSWU_Z1_BLS381);
    FP_BLS381_one(&gx);

    // tv1 = 1 / (Z^2 * u^4 + Z * u^2)
    FP_BLS381_sqr(&tmp, &u);
    FP_BLS381_mul(&tmp, &tmp, &z);
    FP_BLS381_sqr(&tv1, &tmp);
    FP_BLS381_add(&tv1, &tv1, &tmp);
    FP_BLS381_inv(&tv1, &tv1);

    // x = (-B / A) * (1 + tv1)
    FP_BLS381_add(x, &tv1, &gx);
    FP_BLS381_mul(x, x, &b);
    FP_BLS381_neg(x, x);
    FP_BLS381_inv(&gx, &a);
    FP_BLS381_mul(x, x, &gx);

    // if tv1 == 0, set x = B / (Z * A)
    if (FP_BLS381_iszilch(&tv1)) {
        FP_BLS381_mul(x, &b, &gx);
        FP_BLS381_inv(&gx, &z);
        FP_BLS381_mul(x, x, &gx);
    }

    // gx = x^3 + A * x + B
    FP_BLS381_sqr(&gx, x);
    FP_BLS381_add(&gx, &gx, &a);
    FP_BLS381_mul(&gx, &gx, x);
    FP_BLS381_add(&gx, &gx, &b);
    // y = sqrt(gx)
    FP_BLS381_sqrt(y, &gx);
    // tv1 = y^2
    FP_BLS381_sqr(&tv1, y);

    // If !(is_square(gx)), set x = Z * u^2 * x1 and y = sqrt(x2^3 + A * x2 + B)
    if (!FP_BLS381_equals(&gx, &tv1)) {
        FP_BLS381_mul(x, x, &tmp);
        FP_BLS381_sqr(&gx, x);
        FP_BLS381_add(&gx, &gx, &a);
        FP_BLS381_mul(&gx, &gx, x);
        FP_BLS381_add(&gx, &gx, &b);
        FP_BLS381_sqrt(y, &gx);
    }

    if (sgn0_fp(*y) != sgn0_fp(u))
        FP_BLS381_neg(y, y);

    return SUCCESS;
}

// draft-irtf-cfrg-hash-to-curve-16 Appendix E.2
// It maps a point to BLS12-381 from an isogenous curve.
static int iso11_to_ecp(ECP *P, FP_BLS381 x_prime, FP_BLS381 y_prime) {

    BIG x;
    BIG y;
    FP_BLS381 k;
    FP_BLS381 xnum;
    FP_BLS381 xden;
    FP_BLS381 ynum;
    FP_BLS381 yden;

    FP_BLS381_zero(&xnum);
    FP_BLS381_zero(&ynum);
    FP_BLS381_copy(&xden, &x_prime);
    FP_BLS381_copy(&yden, &x_prime);

    // x_num = k_(1,11) * x'^11 + k_(1,10) * x'^10 + k_(1,9) * x'^9 + ... + k_(1,0)
    for(int i = 11; i > 0; i--) {
        FP_BLS381_nres(&k, ISO11_XNUM_BLS381[i]);
        FP_BLS381_add(&xnum, &xnum, &k);
        FP_BLS381_mul(&xnum, &xnum, &x_prime);
    }
    FP_BLS381_nres(&k, ISO11_XNUM_BLS381[0]);
    FP_BLS381_add(&xnum, &xnum, &k);

    // x_den = x'^10 + k_(2,9) * x'^9 + k_(2,8) * x'^8 + ... + k_(2,0)
    for(int i = 9; i > 0; i--) {
        FP_BLS381_nres(&k, ISO11_XDEN_BLS381[i]);
        FP_BLS381_add(&xden, &xden, &k);
        FP_BLS381_mul(&xden, &xden, &x_prime);
    }
    FP_BLS381_nres(&k, ISO11_XDEN_BLS381[0]);
    FP_BLS381_add(&xden, &xden, &k);

    // y_num = k_(3,15) * x'^15 + k_(3,14) * x'^14 + k_(3,13) * x'^13 + ... + k_(3,0)
    for(int i = 15; i > 0; i--) {
        FP_BLS381_nres(&k, ISO11_YNUM_BLS381[i]);
        FP_BLS381_add(&ynum, &ynum, &k);
        FP_BLS381_mul(&ynum, &ynum, &x_prime);
    }
    FP_BLS381_nres(&k, ISO11_YNUM_BLS381[0]);
    FP_BLS381_add(&ynum, &ynum, &k);

    // y_den = x'^15 + k_(4,14) * x'^14 + k_(4,13) * x'^13 + ... + k_(4,0)
    for(int i = 14; i > 0; i--) {
        FP_BLS381_nres(&k, ISO11_YDEN_BLS381[i]);
        FP_BLS381_add(&yden, &yden, &k);
        FP_BLS381_mul(&yden, &yden, &x_prime);
    }
    FP_BLS381_nres(&k, ISO11_YDEN_BLS381[0]);
    FP_BLS381_add(&yden, &yden, &k);

    // x = x_num / x_den
    FP_BLS381_inv(&xden, &xden);
    FP_BLS381_mul(&xnum, &xnum, &xden);
    // y = y' * y_num / y_den
    FP_BLS381_inv(&yden, &yden);
    FP_BLS381_mul(&ynum, &ynum, &yden);
    FP_BLS381_mul(&ynum, &ynum, &y_prime);
    // P = (x, y)
    FP_BLS381_redc(x, &xnum);
    FP_BLS381_redc(y, &ynum);
    if (ECP_set(P, x, y) == 0)
        return 1;

    return SUCCESS;
}





static int map_to_curve_G1(lua_State *L) {
    BEGIN();
    char *failed_msg = NULL;
    big *el = big_arg(L,1);
    if (el == NULL) {
        failed_msg = "Could not allocate big element";
		goto end;
    }
    FP_BLS381 u;
    FP_BLS381 x;
    FP_BLS381 y;
    FP_BLS381_nres(&u, el->val);
    // (x, y) = map_to_curve_simple_swu(u)
    ECP_sswu(&x, &y, u);
    ecp *P = ecp_new(L);
    // P = (x, y) = iso_map(x, y)
    iso11_to_ecp(&(P->val), x, y);

    end:
    big_free(L, el);
	if(failed_msg) {
		THROW(failed_msg);
	}
    END(1);
}


int luaopen_bbs(lua_State *L) {
	(void)L;
	const struct luaL_Reg bbs_class[] = {
        {"map_to_curve",map_to_curve_G1},

		{NULL,NULL}
	};
	const struct luaL_Reg bbs_methods[] = {
		{NULL,NULL}
	};

	zen_add_class(L, "bbs", bbs_class, bbs_methods);
	return 1;
}

