//********************************************************************************************
// VietnamChar.H
//********************************************************************************************

#ifndef	__VIETNAMCHAR_H__
#define __VIETNAMCHAR_H__

#define PASS1_MASK	0x0001fe00
#define PASS2_MASK	0x000001f0
#define PASS3_MASK	0x0000000f
#define KEY_SHIFT_MASK	0x10000000

#define DB3_Key		0x00000001
#define W3_Key		0x00000002

#define A_Key		0x00000200
#define E_Key		0x00000400
#define I_Key		0x00000800
#define O_Key		0x00001000
#define U_Key		0x00002000
#define Y_Key		0x00004000
#define D_Key		0x00008000
#define W_Key		0x00010000

#define F_Key		0x00000010
#define S_Key		0x00000020
#define R_Key		0x00000040
#define X_Key		0x00000080
#define J_Key		0x00000100

#define UW_Key		0x00002002

#define NUMBER_1    0x00000031
#define NUMBER_2    0x00000032
#define NUMBER_3    0x00000033
#define NUMBER_4    0x00000034
#define NUMBER_5    0x00000035
#define NUMBER_6    0x00000036
#define NUMBER_7    0x00000037
#define NUMBER_8    0x00000038
#define NUMBER_9    0x00000039


#define VIETCHAR_COUNT	150
struct stVietnamChar
{
	UINT32		index;
	char		chWord[2];
};

stVietnamChar g_VietnamChar[VIETCHAR_COUNT];

void SetVietnamChar()
{
#ifdef LANGUAGE_VIETNAM
	// Setting
	// a
	g_VietnamChar[0].index = 0x00000200;
	strcpy(g_VietnamChar[0].chWord, "a");
	g_VietnamChar[1].index = 0x00000201;
	strcpy(g_VietnamChar[1].chWord, "â");
	g_VietnamChar[2].index = 0x00000202;
	strcpy(g_VietnamChar[2].chWord, "ã");
	g_VietnamChar[3].index = 0x00000210;
	strcpy(g_VietnamChar[3].chWord, "aÌ");
	g_VietnamChar[4].index = 0x00000220;
	strcpy(g_VietnamChar[4].chWord, "aì");
	g_VietnamChar[5].index = 0x00000240;
	strcpy(g_VietnamChar[5].chWord, "aÒ");
	g_VietnamChar[6].index = 0x00000280;
	strcpy(g_VietnamChar[6].chWord, "aÞ");
	g_VietnamChar[7].index = 0x00000300;
	strcpy(g_VietnamChar[7].chWord, "aò");
	g_VietnamChar[8].index = 0x00000211;
	strcpy(g_VietnamChar[8].chWord, "âÌ");
	g_VietnamChar[9].index = 0x00000212;
	strcpy(g_VietnamChar[9].chWord, "ãÌ");
	g_VietnamChar[10].index = 0x00000221;
	strcpy(g_VietnamChar[10].chWord, "âì");
	g_VietnamChar[11].index = 0x00000222;
	strcpy(g_VietnamChar[11].chWord, "ãì");
	g_VietnamChar[12].index = 0x00000241;
	strcpy(g_VietnamChar[12].chWord, "âÒ");
	g_VietnamChar[13].index = 0x00000242;
	strcpy(g_VietnamChar[13].chWord, "ãÒ");
	g_VietnamChar[14].index = 0x00000281;
	strcpy(g_VietnamChar[14].chWord, "âÞ");
	g_VietnamChar[15].index = 0x00000282;
	strcpy(g_VietnamChar[15].chWord, "ãÞ");
	g_VietnamChar[16].index = 0x00000301;
	strcpy(g_VietnamChar[16].chWord, "âò");
	g_VietnamChar[17].index = 0x00000302;
	strcpy(g_VietnamChar[17].chWord, "ãò");

	// e
	g_VietnamChar[18].index = 0x00000400;
	strcpy(g_VietnamChar[18].chWord, "e");
	g_VietnamChar[19].index = 0x00000401;
	strcpy(g_VietnamChar[19].chWord, "ê");
	g_VietnamChar[20].index = 0x00000410;
	strcpy(g_VietnamChar[20].chWord, "eÌ");
	g_VietnamChar[21].index = 0x00000420;
	strcpy(g_VietnamChar[21].chWord, "eì");
	g_VietnamChar[22].index = 0x00000440;
	strcpy(g_VietnamChar[22].chWord, "eÒ");
	g_VietnamChar[23].index = 0x00000480;
	strcpy(g_VietnamChar[23].chWord, "eÞ");
	g_VietnamChar[24].index = 0x00000500;
	strcpy(g_VietnamChar[24].chWord, "eò");
	g_VietnamChar[25].index = 0x00000411;
	strcpy(g_VietnamChar[25].chWord, "êÌ");
	g_VietnamChar[26].index = 0x00000421;
	strcpy(g_VietnamChar[26].chWord, "êì");
	g_VietnamChar[27].index = 0x00000441;
	strcpy(g_VietnamChar[27].chWord, "êÒ");
	g_VietnamChar[28].index = 0x00000481;
	strcpy(g_VietnamChar[28].chWord, "êÞ");
	g_VietnamChar[29].index = 0x00000501;
	strcpy(g_VietnamChar[29].chWord, "êò");

	// i
	g_VietnamChar[30].index = 0x00000800;
	strcpy(g_VietnamChar[30].chWord, "i");
	g_VietnamChar[31].index = 0x00000810;
	strcpy(g_VietnamChar[31].chWord, "iÌ");
	g_VietnamChar[32].index = 0x00000820;
	strcpy(g_VietnamChar[32].chWord, "iì");
	g_VietnamChar[33].index = 0x00000840;
	strcpy(g_VietnamChar[33].chWord, "iÒ");
	g_VietnamChar[34].index = 0x00000880;
	strcpy(g_VietnamChar[34].chWord, "iÞ");
	g_VietnamChar[35].index = 0x00000900;
	strcpy(g_VietnamChar[35].chWord, "iò");

	// o
	g_VietnamChar[36].index = 0x00001000;
	strcpy(g_VietnamChar[36].chWord, "o");
	g_VietnamChar[37].index = 0x00001001;
	strcpy(g_VietnamChar[37].chWord, "ô");
	g_VietnamChar[38].index = 0x00001002;
	strcpy(g_VietnamChar[38].chWord, "õ");
	g_VietnamChar[39].index = 0x00001010;
	strcpy(g_VietnamChar[39].chWord, "oÌ");
	g_VietnamChar[40].index = 0x00001020;
	strcpy(g_VietnamChar[40].chWord, "oì");
	g_VietnamChar[41].index = 0x00001040;
	strcpy(g_VietnamChar[41].chWord, "oÒ");
	g_VietnamChar[42].index = 0x00001080;
	strcpy(g_VietnamChar[42].chWord, "oÞ");
	g_VietnamChar[43].index = 0x00001100;
	strcpy(g_VietnamChar[43].chWord, "oò");
	g_VietnamChar[44].index = 0x00001011;
	strcpy(g_VietnamChar[44].chWord, "ôÌ");
	g_VietnamChar[45].index = 0x00001012;
	strcpy(g_VietnamChar[45].chWord, "õÌ");
	g_VietnamChar[46].index = 0x00001021;
	strcpy(g_VietnamChar[46].chWord, "ôì");
	g_VietnamChar[47].index = 0x00001022;
	strcpy(g_VietnamChar[47].chWord, "õì");
	g_VietnamChar[48].index = 0x00001041;
	strcpy(g_VietnamChar[48].chWord, "ôÒ");
	g_VietnamChar[49].index = 0x00001042;
	strcpy(g_VietnamChar[49].chWord, "õÒ");
	g_VietnamChar[50].index = 0x00001081;
	strcpy(g_VietnamChar[50].chWord, "ôÞ");
	g_VietnamChar[51].index = 0x00001082;
	strcpy(g_VietnamChar[51].chWord, "õÞ");
	g_VietnamChar[52].index = 0x00001101;
	strcpy(g_VietnamChar[52].chWord, "ôò");
	g_VietnamChar[53].index = 0x00001102;
	strcpy(g_VietnamChar[53].chWord, "õò");

	// u
	g_VietnamChar[54].index = 0x00002000;
	strcpy(g_VietnamChar[54].chWord, "u");
	g_VietnamChar[55].index = 0x00002002;
	strcpy(g_VietnamChar[55].chWord, "ý");
	g_VietnamChar[56].index = 0x00002010;
	strcpy(g_VietnamChar[56].chWord, "uÌ");
	g_VietnamChar[57].index = 0x00002020;
	strcpy(g_VietnamChar[57].chWord, "uì");
	g_VietnamChar[58].index = 0x00002040;
	strcpy(g_VietnamChar[58].chWord, "uÒ");
	g_VietnamChar[59].index = 0x00002080;
	strcpy(g_VietnamChar[59].chWord, "uÞ");
	g_VietnamChar[60].index = 0x00002100;
	strcpy(g_VietnamChar[60].chWord, "uò");
	g_VietnamChar[61].index = 0x00002012;
	strcpy(g_VietnamChar[61].chWord, "ýÌ");
	g_VietnamChar[62].index = 0x00002022;
	strcpy(g_VietnamChar[62].chWord, "ýì");
	g_VietnamChar[63].index = 0x00002042;
	strcpy(g_VietnamChar[63].chWord, "ýÒ");
	g_VietnamChar[64].index = 0x00002082;
	strcpy(g_VietnamChar[64].chWord, "ýÞ");
	g_VietnamChar[65].index = 0x00002102;
	strcpy(g_VietnamChar[65].chWord, "ýò");

	// y
	g_VietnamChar[66].index = 0x00004000;
	strcpy(g_VietnamChar[66].chWord, "y");
	g_VietnamChar[67].index = 0x00004010;
	strcpy(g_VietnamChar[67].chWord, "yÌ");
	g_VietnamChar[68].index = 0x00004020;
	strcpy(g_VietnamChar[68].chWord, "yì");
	g_VietnamChar[69].index = 0x00004040;
	strcpy(g_VietnamChar[69].chWord, "yÒ");
	g_VietnamChar[70].index = 0x00004080;
	strcpy(g_VietnamChar[70].chWord, "yÞ");
	g_VietnamChar[71].index = 0x00004100;
	strcpy(g_VietnamChar[71].chWord, "yò");

	// d
	g_VietnamChar[72].index = 0x00008000;
	strcpy(g_VietnamChar[72].chWord, "d");
	g_VietnamChar[73].index = 0x00008001;
	strcpy(g_VietnamChar[73].chWord, "ð");

	// w
	g_VietnamChar[74].index = 0x00010000;
	strcpy(g_VietnamChar[74].chWord, "w");

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// A
	g_VietnamChar[75].index = 0x10000200;
	strcpy(g_VietnamChar[75].chWord, "A");
	g_VietnamChar[76].index = 0x10000201;
	strcpy(g_VietnamChar[76].chWord, "Â");
	g_VietnamChar[77].index = 0x10000202;
	strcpy(g_VietnamChar[77].chWord, "Ã");
	g_VietnamChar[78].index = 0x10000210;
	strcpy(g_VietnamChar[78].chWord, "AÌ");
	g_VietnamChar[79].index = 0x10000220;
	strcpy(g_VietnamChar[79].chWord, "Aì");
	g_VietnamChar[80].index = 0x10000240;
	strcpy(g_VietnamChar[80].chWord, "AÒ");
	g_VietnamChar[81].index = 0x10000280;
	strcpy(g_VietnamChar[81].chWord, "AÞ");
	g_VietnamChar[82].index = 0x10000300;
	strcpy(g_VietnamChar[82].chWord, "Aò");
	g_VietnamChar[83].index = 0x10000211;
	strcpy(g_VietnamChar[83].chWord, "ÂÌ");
	g_VietnamChar[84].index = 0x10000212;
	strcpy(g_VietnamChar[84].chWord, "ÃÌ");
	g_VietnamChar[85].index = 0x10000221;
	strcpy(g_VietnamChar[85].chWord, "Âì");
	g_VietnamChar[86].index = 0x10000222;
	strcpy(g_VietnamChar[86].chWord, "Ãì");
	g_VietnamChar[87].index = 0x10000241;
	strcpy(g_VietnamChar[87].chWord, "ÂÒ");
	g_VietnamChar[88].index = 0x10000242;
	strcpy(g_VietnamChar[88].chWord, "ÃÒ");
	g_VietnamChar[89].index = 0x10000281;
	strcpy(g_VietnamChar[89].chWord, "ÂÞ");
	g_VietnamChar[90].index = 0x10000282;
	strcpy(g_VietnamChar[90].chWord, "ÃÞ");
	g_VietnamChar[91].index = 0x10000301;
	strcpy(g_VietnamChar[91].chWord, "Âò");
	g_VietnamChar[92].index = 0x10000302;
	strcpy(g_VietnamChar[92].chWord, "Ãò");

	// E
	g_VietnamChar[93].index = 0x10000400;
	strcpy(g_VietnamChar[93].chWord, "E");
	g_VietnamChar[94].index = 0x10000401;
	strcpy(g_VietnamChar[94].chWord, "Ê");
	g_VietnamChar[95].index = 0x10000410;
	strcpy(g_VietnamChar[95].chWord, "EÌ");
	g_VietnamChar[96].index = 0x10000420;
	strcpy(g_VietnamChar[96].chWord, "Eì");
	g_VietnamChar[97].index = 0x10000440;
	strcpy(g_VietnamChar[97].chWord, "EÒ");
	g_VietnamChar[98].index = 0x10000480;
	strcpy(g_VietnamChar[98].chWord, "EÞ");
	g_VietnamChar[99].index = 0x10000500;
	strcpy(g_VietnamChar[99].chWord, "Eò");
	g_VietnamChar[100].index = 0x10000411;
	strcpy(g_VietnamChar[100].chWord, "ÊÌ");
	g_VietnamChar[101].index = 0x10000421;
	strcpy(g_VietnamChar[101].chWord, "Êì");
	g_VietnamChar[102].index = 0x10000441;
	strcpy(g_VietnamChar[102].chWord, "ÊÒ");
	g_VietnamChar[103].index = 0x10000481;
	strcpy(g_VietnamChar[103].chWord, "ÊÞ");
	g_VietnamChar[104].index = 0x10000501;
	strcpy(g_VietnamChar[104].chWord, "Êò");

	// I
	g_VietnamChar[105].index = 0x10000800;
	strcpy(g_VietnamChar[105].chWord, "I");
	g_VietnamChar[106].index = 0x10000810;
	strcpy(g_VietnamChar[106].chWord, "IÌ");
	g_VietnamChar[107].index = 0x10000820;
	strcpy(g_VietnamChar[107].chWord, "Iì");
	g_VietnamChar[108].index = 0x10000840;
	strcpy(g_VietnamChar[108].chWord, "IÒ");
	g_VietnamChar[109].index = 0x10000880;
	strcpy(g_VietnamChar[109].chWord, "IÞ");
	g_VietnamChar[110].index = 0x10000900;
	strcpy(g_VietnamChar[110].chWord, "Iò");

	// O
	g_VietnamChar[111].index = 0x10001000;
	strcpy(g_VietnamChar[111].chWord, "O");
	g_VietnamChar[112].index = 0x10001001;
	strcpy(g_VietnamChar[112].chWord, "Ô");
	g_VietnamChar[113].index = 0x10001002;
	strcpy(g_VietnamChar[113].chWord, "Õ");
	g_VietnamChar[114].index = 0x10001010;
	strcpy(g_VietnamChar[114].chWord, "OÌ");
	g_VietnamChar[115].index = 0x10001020;
	strcpy(g_VietnamChar[115].chWord, "Oì");
	g_VietnamChar[116].index = 0x10001040;
	strcpy(g_VietnamChar[116].chWord, "OÒ");
	g_VietnamChar[117].index = 0x10001080;
	strcpy(g_VietnamChar[117].chWord, "OÞ");
	g_VietnamChar[118].index = 0x10001100;
	strcpy(g_VietnamChar[118].chWord, "Oò");
	g_VietnamChar[119].index = 0x10001011;
	strcpy(g_VietnamChar[119].chWord, "ÔÌ");
	g_VietnamChar[120].index = 0x10001012;
	strcpy(g_VietnamChar[120].chWord, "ÕÌ");
	g_VietnamChar[121].index = 0x10001021;
	strcpy(g_VietnamChar[121].chWord, "Ôì");
	g_VietnamChar[122].index = 0x10001022;
	strcpy(g_VietnamChar[122].chWord, "Õì");
	g_VietnamChar[123].index = 0x10001041;
	strcpy(g_VietnamChar[123].chWord, "ÔÒ");
	g_VietnamChar[124].index = 0x10001042;
	strcpy(g_VietnamChar[124].chWord, "ÕÒ");
	g_VietnamChar[125].index = 0x10001081;
	strcpy(g_VietnamChar[125].chWord, "ÔÞ");
	g_VietnamChar[126].index = 0x10001082;
	strcpy(g_VietnamChar[126].chWord, "ÕÞ");
	g_VietnamChar[127].index = 0x10001101;
	strcpy(g_VietnamChar[127].chWord, "Ôò");
	g_VietnamChar[128].index = 0x10001102;
	strcpy(g_VietnamChar[128].chWord, "Õò");

	// U
	g_VietnamChar[129].index = 0x10002000;
	strcpy(g_VietnamChar[129].chWord, "U");
	g_VietnamChar[130].index = 0x10002002;
	strcpy(g_VietnamChar[130].chWord, "Ý");
	g_VietnamChar[131].index = 0x10002010;
	strcpy(g_VietnamChar[131].chWord, "UÌ");
	g_VietnamChar[132].index = 0x10002020;
	strcpy(g_VietnamChar[132].chWord, "Uì");
	g_VietnamChar[133].index = 0x10002040;
	strcpy(g_VietnamChar[133].chWord, "UÒ");
	g_VietnamChar[134].index = 0x10002080;
	strcpy(g_VietnamChar[134].chWord, "UÞ");
	g_VietnamChar[135].index = 0x10002100;
	strcpy(g_VietnamChar[135].chWord, "Uò");
	g_VietnamChar[136].index = 0x10002012;
	strcpy(g_VietnamChar[136].chWord, "ÝÌ");
	g_VietnamChar[137].index = 0x10002022;
	strcpy(g_VietnamChar[137].chWord, "Ýì");
	g_VietnamChar[138].index = 0x10002042;
	strcpy(g_VietnamChar[138].chWord, "ÝÒ");
	g_VietnamChar[139].index = 0x10002082;
	strcpy(g_VietnamChar[139].chWord, "ÝÞ");
	g_VietnamChar[140].index = 0x10002102;
	strcpy(g_VietnamChar[140].chWord, "Ýò");

	// Y
	g_VietnamChar[141].index = 0x10004000;
	strcpy(g_VietnamChar[141].chWord, "Y");
	g_VietnamChar[142].index = 0x10004010;
	strcpy(g_VietnamChar[142].chWord, "YÌ");
	g_VietnamChar[143].index = 0x10004020;
	strcpy(g_VietnamChar[143].chWord, "Yì");
	g_VietnamChar[144].index = 0x10004040;
	strcpy(g_VietnamChar[144].chWord, "YÒ");
	g_VietnamChar[145].index = 0x10004080;
	strcpy(g_VietnamChar[145].chWord, "YÞ");
	g_VietnamChar[146].index = 0x10004100;
	strcpy(g_VietnamChar[146].chWord, "Yò");

	// D
	g_VietnamChar[147].index = 0x10008000;
	strcpy(g_VietnamChar[147].chWord, "D");
	g_VietnamChar[148].index = 0x10008001;
	strcpy(g_VietnamChar[148].chWord, "Ð");

	// W
	g_VietnamChar[149].index = 0x10010000;
	strcpy(g_VietnamChar[149].chWord, "W");

#endif
}
#endif // __VIETNAMCHAR_H__