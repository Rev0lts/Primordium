#define compat=1
#pragma dynamic 8342

#pragma warning disable 214
#pragma warning disable 239
//Teste ind 
//////////////////////////////////////////////////////
//                                                  //
// 		MYSQL SETTIGNS:                             //
//		-	scriptfiles/mysqlsettings.ini           //
//                                                  //
//    	REGISTRATION TYPES:                			//
//      - In Game = Define it as 1                  //
//      - Only UCP = Define it as 0                 //
//                                                  //
//////////////////////////////////////////////////////

/*				coisas para testar

comando /cache ( dentro de casa )

*/

#define 	REGISTRATION 		(0)
#define 	GameModeVersion 	"1.02"
#define 	GameModeUpdate 		"27.09.2021"
#define 	SERVER_BONUS 		2000
#define 	GameModeText 		"RC-RP "GameModeVersion""
#define 	WeburlName 			"rc-rp.pt"
#define 	ProjectName 		"Red County Roleplay"
#define 	ServerName 			""ProjectName" ["WeburlName"]"
#define 	MAP_NAME 			"County"

//////////////////////////////////////////////////////
//                   INCLUDES                       //;
//////////////////////////////////////////////////////

#include 	<a_samp>
#include 	<crashdetect>
#include 	<dc_cmd>
#include 	<a_mysql>
#include 	<sscanf2>
#include 	<foreach>
#include 	<dc_foreach_veh>
#include 	<streamer>
#include 	<memory>
#include 	<PreviewModelDialog>
#include 	<eSelection>
#include 	<easyDialog>
#include 	<messagebox>
#include 	<3dmenu>
#include 	<PlayerToPlayer>
#include	<strlib>
#include 	<acuf>
#include 	<mxdate>
#include 	<timestamp>
#include 	<TOTP>
#include	<mapandreas>
#include    <mxINI>
#include    <evf>
#include 	<evi>
#include 	<nex-ac>

//////////////////////////////////////////////////////
//                   GAMEMODE                       //
//////////////////////////////////////////////////////

#include "../include/furniture_list.inc"
#include "../include/vehicle_sells.inc"
#include "gm/a_build/build.inc"


new FishActor;
new electricianactor;
//new vehiculjob = GetPlayerVehicleID(playerid);
new Hatss[MAX_PLAYERS];
new urcainmasinaelectrician[MAX_PLAYERS];
// HATS
new Float:SkinCaps[311][6] = {
{0.135928, 0.002891, -0.008518, 0.000000, 0.000000, 347.188201},//Skin - 0
{0.136207, 0.006102, -0.013858, 0.612963, 0.000000, 0.000000}, //Skin - 1
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                 //Skin - 2
{0.167425, -0.006185, -0.004779, 0.612963, 0.000000, 0.000000}, //Skin - 3
{0.188546, -0.006185, -0.011507, 0.612963, 0.000000, 0.000000}, //Skin - 4
{0.188546, -0.006185, -0.011507, 0.612963, 0.000000, 0.000000}, //Skin - 5
{0.155785, 0.005998, -0.014326, 0.000000, 0.000000, 347.188201},//Skin - 6
{0.157787, 0.012994, -0.014590, 0.612963, 0.000000, 0.000000},  //Skin - 7
{0.131067, -0.013737, -0.008518, 0.000000, 0.000000, 347.188201},//Skin - 8
{0.118922, -0.015322, -0.008518, 0.000000, 0.000000, 347.188201},//Skin - 9
{0.125779, -0.001459, -0.008518, 0.000000, 0.000000, 347.188201},//Skin - 10
{0.129249, -0.014101, -0.008518, 0.000000, 0.000000, 347.188201},//Skin - 11
{0.139572, -0.003642, -0.002145, 0.612963, 0.000000, 10.335063}, //Skin - 12
{0.161076, -0.015624, -0.006768, 0.000000, 0.000000, 347.188201},//Skin - 13
{0.112204, -0.023196, -0.006768, 0.000000, 0.000000, 347.188201},//Skin - 14
{0.104552, -0.015122, -0.005275, 0.612963, 2.307199, 357.920410}, //Skin - 15
{0.150166, -0.008718, -0.006768, 0.000000, 0.000000, 347.188201},//Skin - 16
{0.149627, 0.002943, -0.005275, 0.612963, 2.307199, 357.920410}, //Skin - 17
{0.105319, 0.003517, -0.005275, 0.612963, 2.307199, 357.920410}, //Skin - 18
{0.153609, -0.003207, -0.007717, 0.000000, 0.000000, 357.608825},//Skin - 19
{0.143831, 0.001813, -0.010588, 0.000000, 0.000000, 357.608825}, //Skin - 20
{0.154598, -0.003549, -0.013304, 0.000000, 0.000000, 0.000000},  //Skin - 21
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                  //Skin - 22
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                   //Skin - 23
{0.127352, 0.009877, -0.006845, 0.726156, 359.666778, 348.825012},//Skin - 24
{0.124666, -0.029373, -0.006845, 0.726156, 359.666778, 329.940704},//Skin - 25
{0.152029, -0.018331, -0.003139, 0.000000, 358.344604, 348.467559}, //Skin - 26
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                    //Skin - 27
{0.128768, 0.041474, -0.007667, 0.726156, 359.666778, 355.429199}, //Skin - 28
{0.166457, -0.006228, -0.012669, 0.726156, 359.666778, 354.612152},//Skin - 29
{0.155160, -0.022985, -0.011249, 0.000000, 358.344604, 348.467559}, //Skin - 30
{0.096077, -0.023233, -0.009101, 0.726156, 359.666778, 343.094055},//Skin - 31
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                    //Skin - 32
{0.094003, -0.022985, -0.011249, 0.000000, 358.344604, 348.467559}, //Skin - 33
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                    //Skin - 34
{0.155565, 0.014400, -0.009101, 0.726156, 359.666778, 6.131487},   //Skin - 35
{0.156485, 0.013641, -0.009101, 0.726156, 359.666778, 6.131487},   //Skin - 36
{0.144815, 0.013641, -0.009374, 0.726156, 359.666778, 350.562103}, //Skin - 37
{0.113347, -0.006682, -0.009374, 0.726156, 359.666778, 350.562103},//Skin - 38
{0.147231, -0.014448, -0.004786, 0.726156, 359.666778, 357.303253},//Skin - 39
{0.133873, -0.019703, -0.000058, 0.000000, 358.344604, 348.467559},//Skin - 40
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                    //Skin - 41
{0.082446, 0.004202, -0.004786, 0.726156, 359.666778, 357.303253}, //Skin - 42
{0.104901, 0.004013, -0.004786, 0.726156, 359.666778, 342.983184}, //Skin - 43
{0.116172, -0.001954, -0.004786, 0.726156, 359.666778, 357.100677},//Skin - 44
{0.153321, 0.025744, -0.008666, 0.726156, 359.666778, 10.704365},  //Skin - 45
{0.160556, 0.007781, -0.010438, 0.726156, 359.666778, 0.991972},   //Skin - 46
{0.179010, -0.035613, -0.010438, 0.726156, 359.666778, 347.956573},//Skin - 47
{0.123363, 0.008694, -0.010438, 0.726156, 359.666778, 347.956573}, //Skin - 48
{0.167061, -0.037899, -0.010438, 0.726156, 359.666778, 347.775817},//Skin - 49
{0.164100, -0.040816, -0.011624, 357.030151, 358.344604, 342.811187},//Skin - 50
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                    //Skin - 51
{0.129040, 0.016069, -0.010438, 0.726156, 359.666778, 347.775817}, //Skin - 52
{0.129040, 0.016069, -0.006084, 0.726156, 359.666778, 347.775817}, //Skin - 53
{0.137743, -0.016369, -0.011731, 0.726156, 359.666778, 355.812011},//Skin - 54
{0.137743, -0.016369, -0.011731, 0.726156, 359.666778, 355.812011},//Skin - 55
{0.174539, -0.000662, -0.007289, 0.726156, 359.666778, 352.847045},//Skin - 56
{0.109382, -0.002955, -0.007289, 0.726156, 359.666778, 352.847045},//Skin - 57
{0.152276, -0.029331, -0.008357, 0.726156, 359.666778, 332.070648},//Skin - 58
{0.129599, -0.019172, -0.012204, 0.726156, 359.666778, 332.070648},//Skin - 59
{0.138485, -0.012405, -0.011567, 357.030151, 358.344604, 342.811187},//Skin - 60
{0.130350, 0.000897, -0.000747, 0.726156, 359.666778, 332.070648}, //Skin - 61
{0.150659, -0.035485, -0.006299, 0.726156, 359.666778, 341.617431},//Skin - 62
{0.119340, -0.006483, -0.006299, 0.726156, 359.666778, 341.617431},//Skin - 63
{0.110634, 0.009623, -0.011624, 357.030151, 358.344604, 342.811187},//Skin - 64
{0.133055, -0.000092, -0.006299, 0.726156, 359.666778, 341.617431},//Skin - 65
{0.129061, -0.006499, -0.006299, 0.726156, 359.666778, 341.617431},//Skin - 66
{0.127292, 0.010318, -0.006299, 0.726156, 359.666778, 341.617431}, //Skin - 67
{0.138791, -0.025311, -0.006299, 0.726156, 359.666778, 341.617431},//Skin - 68
{0.148132, 0.003970, -0.002304, 0.726156, 359.666778, 340.120025}, //Skin - 69
{0.129753, 0.006469, -0.006376, 0.726156, 359.666778, 354.029815}, //Skin - 70
{0.125663, 0.015428, -0.006376, 0.726156, 359.666778, 354.029815}, //Skin - 71
{0.125663, 0.015428, -0.009030, 0.726156, 359.666778, 354.029815}, //Skin - 72
{0.117674, 0.014567, -0.011567, 357.030151, 358.344604, 342.811187}, //Skin - 73
{0.135729, -0.018656, -0.013554, 0.000000, 0.000000, 337.893737},  //Skin - 74
{0.141888, -0.042810, -0.006206, 0.000000, 0.000000, 337.893737},  //Skin - 75
{0.134968, -0.020112, -0.003604, 357.030151, 358.344604, 342.811187},//Skin - 76
{0.124348, -0.017813, -0.006206, 0.000000, 0.000000, 346.786865},  //Skin - 77
{0.124348, 0.000583, -0.006206, 0.000000, 0.000000, 346.786865},   //Skin - 78
{0.102654, -0.010906, -0.006206, 0.000000, 0.000000, 346.786865},  //Skin - 79
{0.102654, -0.010906, -0.006206, 0.000000, 0.000000, 346.786865},  //Skin - 81
{0.167928, 0.031601, -0.006206, 0.000000, 0.000000, 17.955888},    //Skin - 82
{0.159998, 0.023540, -0.006206, 0.000000, 0.000000, 17.955888},    //Skin - 83
{0.169630, 0.019315, -0.006206, 0.000000, 0.000000, 17.955888},    //Skin - 84
{0.163052, -0.039735, -0.006206, 0.000000, 0.000000, 341.169891},  //Skin - 85
{0.122285, -0.020112, -0.003604, 357.030151, 358.344604, 342.811187},//Skin - 86
{0.144811, -0.007521, -0.014207, 0.000000, 0.000000, 341.169891},  //Skin - 87
{0.129932, -0.007521, -0.007289, 0.000000, 0.000000, 341.169891},  //Skin - 88
{0.151147, -0.038608, -0.009597, 0.000000, 0.000000, 343.694549},  //Skin - 89
{0.147416, -0.031632, -0.009597, 0.000000, 0.000000, 343.694549},  //Skin - 90
{0.157728, -0.009677, -0.009597, 0.000000, 0.000000, 0.934848},    //Skin - 91
{0.136577, -0.015592, -0.009597, 0.000000, 0.000000, 341.013824},  //Skin - 92
{0.143821, 0.000631, -0.008385, 0.000000, 0.000000, 358.808868},   //Skin - 93
{0.100521, 0.003151, -0.007624, 0.000000, 0.000000, 358.808868},   //Skin - 94
{0.122833, -0.006031, -0.007624, 0.000000, 0.000000, 358.808868},  //Skin - 95
{0.145296, 0.003959, -0.007624, 0.000000, 0.000000, 358.808868},   //Skin - 96
{0.141658, 0.016474, -0.007624, 0.000000, 0.000000, 9.683902},     //Skin - 97
{0.145276, -0.002846, -0.007624, 0.000000, 0.000000, 340.239593},  //Skin - 98
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                    //Skin - 99
{0.161745, -0.010244, -0.007624, 0.000000, 0.000000, 351.499267}, //Skin - 100
{0.151006, -0.030994, -0.005366, 0.000000, 0.000000, 340.428894}, //Skin - 101
{0.147111, 0.003794, -0.012433, 0.000000, 0.000000, 358.069244},  //Skin - 102
{0.154213, -0.052348, -0.003511, 356.299316, 0.000000, 336.751647},//Skin - 103
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                    //Skin - 104
{0.153814, -0.039614, -0.006756, 356.299316, 0.000000, 336.930084},//Skin - 105
{0.153638, -0.039614, -0.013630, 356.299316, 0.000000, 336.930084},//Skin - 106
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                    //Skin - 107
{0.140473, -0.026201, -0.000469, 0.390689, 355.405181, 335.554260},//Skin - 108
{0.140904, -0.007227, -0.008114, 0.390689, 355.405181, 335.554260},//Skin - 109
{0.140904, -0.007227, -0.008114, 0.390689, 355.405181, 335.554260},//Skin - 110
{0.134860, 0.001485, -0.010145, 0.390689, 358.632415, 347.730010},//Skin - 111
{0.124823, 0.001485, -0.009402, 0.390689, 358.632415, 347.730010},//Skin - 112
{0.157999, -0.012039, -0.006082, 0.390689, 358.632415, 347.730010},//Skin - 113
{0.144906, -0.005139, -0.009654, 0.390689, 358.632415, 336.830108},//Skin - 114
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                    //Skin - 116
{0.152829, -0.009735, -0.009654, 0.390689, 358.632415, 336.830108},//Skin - 117
{0.113804, 0.009252, -0.009654, 0.390689, 358.632415, 345.244384},//Skin - 118
{0.113804, 0.009252, -0.009654, 0.390689, 358.632415, 345.244384},//Skin - 119
{0.144202, -0.021273, -0.005915, 357.030151, 358.344604, 342.811187},//Skin - 120
{0.154275, -0.037484, -0.009654, 0.390689, 358.632415, 337.676666},//Skin - 121
{0.155674, -0.015613, -0.004339, 0.390689, 358.632415, 350.571228},//Skin - 122
{0.136433, -0.019181, -0.004339, 0.390689, 358.632415, 340.261840},//Skin - 123
{0.163258, -0.032386, -0.013128, 0.390689, 358.632415, 340.261840},//Skin - 124
{0.153242, -0.029651, -0.002434, 0.390689, 358.632415, 333.367614},//Skin - 125
{0.127978, -0.001961, -0.008867, 0.390689, 358.632415, 347.279052},//Skin - 126
{0.160856, -0.025356, -0.004428, 0.390689, 358.632415, 347.279052},//Skin - 127
{0.150266, -0.009032, -0.006781, 0.390689, 358.632415, 347.223754},//Skin - 128
{0.158060, 0.022907, -0.006781, 0.390689, 358.632415, 349.378875},//Skin - 129
{0.111739, 0.012673, -0.006781, 0.390689, 358.632415, 349.378875},//Skin - 130
{0.091638, -0.011600, -0.008686, 0.390689, 358.632415, 336.674468},//Skin - 131
{0.125788, 0.000635, -0.005915, 0.390689, 358.632415, 343.007751},//Skin - 132
{0.031324, -0.014154, -0.005915, 0.390689, 358.632415, 343.007751},//Skin - 133
{0.142321, 0.015417, -0.005915, 0.243191, 358.632415, 350.329559},//Skin - 133
{0.128780, -0.030750, 0.006687, 173.184967, 358.632415, 27.422966},//Skin - 134
{0.115882, -0.004931, -0.003807, 358.837646, 358.632415, 346.206237},//Skin - 135
{0.127531, -0.008916, -0.003807, 358.837646, 358.632415, 346.206237},//Skin - 136
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                  //Skin - 137
{0.148992, -0.017748, -0.006509, 358.837646, 358.632415, 350.742156},//Skin - 138
{0.148992, -0.017748, -0.006509, 358.837646, 358.632415, 350.742156},//Skin - 139
{0.147315, 0.001708, -0.006509, 358.837646, 358.632415, 354.390045},//Skin - 140
{0.144315, -0.013571, -0.006509, 358.837646, 358.632415, 354.390045},//Skin - 141
{0.144315, -0.002729, -0.010357, 358.837646, 358.632415, 354.390045},//Skin - 142
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                  //Skin - 143
{0.177516, -0.070868, -0.009114, 358.837646, 358.632415, 331.679321},//Skin - 144
{0.139578, -0.008750, -0.004405, 358.837646, 358.632415, 343.319335},//Skin - 145
{0.139578, -0.014406, -0.004405, 358.837646, 358.632415, 343.319335},//Skin - 146
{0.115592, -0.010754, -0.004405, 358.837646, 358.632415, 343.319335},//Skin - 147
{0.150735, -0.000459, -0.004405, 358.837646, 358.632415, 9.362450},//Skin - 148
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                  //Skin - 149
{0.149485, -0.008709, -0.006168, 358.837646, 358.632415, 2.276566},//Skin - 150
{0.168162, -0.009708, -0.012160, 359.504821, 4.442328, 355.348114},//Skin - 151
{0.156369, -0.024521, -0.012160, 359.504821, 0.415596, 355.348114},//Skin - 152
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                  //Skin - 153
{0.119297, -0.016080, -0.010776, 359.504821, 0.415596, 341.522827},//Skin - 154
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                  //Skin - 155
{0.172295, -0.065549, -0.007187, 359.504821, 0.415596, 336.175567},//Skin - 156
{0.126340, -0.030764, -0.007187, 359.504821, 0.415596, 336.175567},//Skin - 157
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                  //Skin - 158
{0.154280, 0.002166, -0.010436, 359.504821, 0.415596, 357.792144},//Skin - 159
{0.121469, -0.007383, -0.010436, 359.504821, 0.415596, 341.538574},//Skin - 160
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                  //Skin - 161
{0.139495, -0.007383, -0.010436, 359.504821, 0.415596, 341.538574},//Skin - 162
{0.113212, -0.005302, -0.010436, 359.504821, 0.415596, 341.538574},//Skin - 163
{0.120208, 0.003533, -0.010436, 359.504821, 0.415596, 341.538574},//Skin - 164
{0.135111, 0.005091, -0.006407, 359.504821, 0.415596, 352.954559},//Skin - 165
{0.122118, 0.005091, -0.006407, 359.504821, 0.415596, 352.954559},//Skin - 166
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                  //Skin - 167
{0.125229, 0.005091, -0.013084, 359.504821, 0.415596, 352.954559},//Skin - 168
{0.153451, -0.018119, -0.013276, 359.504821, 0.415596, 358.219451},//Skin - 169
{0.141395, -0.009131, -0.013276, 359.504821, 0.415596, 347.866027},//Skin - 170
{0.157631, -0.028753, -0.006450, 359.504821, 0.415596, 339.935516},//Skin - 171
{0.152687, -0.027057, -0.007731, 359.504821, 0.415596, 344.054809},//Skin - 172
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                  //Skin - 173
{0.165146, 0.015393, -0.007731, 359.504821, 0.415596, 344.001678},//Skin - 174
{0.162788, -0.019696, -0.007731, 359.504821, 0.415596, 344.001678},//Skin - 175
{0.157728, -0.027188, -0.012891, 359.504821, 0.415596, 345.804748},//Skin - 176
{0.187507, 0.010472, -0.012891, 359.504821, 0.415596, 12.315887},//Skin - 177
{0.153901, -0.027720, -0.007884, 359.504821, 0.415596, 344.553527},//Skin - 178
{0.137445, -0.009757, -0.012987, 359.504821, 0.415596, 344.553527},//Skin - 179
{0.173041, -0.006323, -0.012987, 359.504821, 0.415596, 3.267552},//Skin - 180
{0.143467, 0.016897, -0.007831, 359.504821, 0.415596, 349.504974},//Skin - 181
{0.114480, 0.006202, -0.007831, 359.504821, 0.415596, 349.504974},//Skin - 182
{0.114480, 0.008813, -0.007831, 359.504821, 0.415596, 349.504974},//Skin - 183
{0.128122, -0.012152, -0.013144, 359.504821, 0.415596, 336.326538},//Skin - 184
{0.156171, 0.007268, -0.013144, 359.504821, 0.415596, 10.805211},//Skin - 185
{0.156409, -0.034861, -0.007927, 359.504821, 0.415596, 336.978668},//Skin - 186
{0.118034, -0.024105, -0.002947, 359.504821, 0.415596, 336.978668},//Skin - 187
{0.128686, -0.029632, -0.002947, 358.201873, 0.415596, 329.325042},//Skin - 188
{0.172639, -0.026749, -0.012705, 358.201873, 0.415596, 349.092590},//Skin - 189
{0.180897, -0.026749, -0.007224, 358.201873, 0.415596, 349.092590},//Skin - 190
{0.180897, -0.026749, -0.007224, 358.201873, 0.415596, 349.092590},//Skin - 191
{0.178725, -0.010278, -0.007224, 358.201873, 0.415596, 354.053405},//Skin - 192
{0.172020, -0.010278, -0.010734, 358.201873, 0.415596, 354.053405},//Skin - 193
{0.172020, -0.010278, -0.010734, 358.201873, 0.415596, 354.053405},//Skin - 194
{0.176089, -0.032526, -0.005110, 358.201873, 0.415596, 341.814422},//Skin - 195
{0.118042, 0.007002, -0.005110, 358.201873, 0.415596, 341.814422},//Skin - 196
{0.143840, -0.042712, -0.007556, 358.201873, 0.415596, 341.814422},//Skin - 197
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                  //Skin - 198
{0.148060, -0.032384, -0.009754, 358.201873, 0.415596, 333.484924},//Skin - 199
{0.148060, -0.032384, -0.009754, 358.201873, 0.415596, 333.484924},//Skin - 200
{0.140799, 0.025145, -0.009754, 358.201873, 0.415596, 5.040688},//Skin - 201
{0.140799, 0.015851, -0.009754, 358.201873, 0.415596, 349.796478},//Skin - 202
{0.140799, -0.004372, -0.013685, 358.201873, 0.415596, 349.796478},//Skin - 203
{0.154274, 0.006245, -0.013685, 358.201873, 0.415596, 2.035465},//Skin - 204
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                  //Skin - 205
{0.154274, 0.016669, -0.013685, 358.201873, 0.415596, 2.035465},//Skin - 206
{0.106604, 0.004805, -0.011840, 358.201873, 0.415596, 2.035465},//Skin - 207
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                  //Skin - 208
{0.148378, -0.003807, -0.011840, 358.201873, 0.415596, 2.035465},//Skin - 209
{0.113854, 0.001969, -0.011840, 358.201873, 0.415596, 343.826263},//Skin - 210
{0.149539, -0.028623, -0.009621, 358.201873, 0.415596, 331.587280},//Skin - 211
{0.104024, -0.014549, -0.009621, 358.201873, 0.415596, 331.587280},//Skin - 212
{0.145820, -0.029160, -0.009621, 358.201873, 0.415596, 331.587280},//Skin - 213
{0.148646, -0.008515, -0.009621, 358.201873, 0.415596, 1.360260},//Skin - 214
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                  //Skin - 215
{0.148646, -0.005935, -0.004058, 358.201873, 0.415596, 1.360260},//Skin - 216
{0.148646, -0.015611, -0.004058, 358.201873, 0.415596, 340.374938},//Skin - 217
{0.133952, -0.030138, -0.009880, 358.201873, 0.415596, 340.374938},//Skin - 218
{0.140503, -0.033425, -0.005693, 358.201873, 0.415596, 340.374938},//Skin - 219
{0.114608, 0.009020, -0.009135, 358.201873, 0.415596, 352.932006},//Skin - 220
{0.186516, -0.044762, -0.009135, 358.201873, 0.415596, 344.217132},//Skin - 221
{0.186516, -0.044762, -0.009135, 358.201873, 0.415596, 344.217132},//Skin - 222
{0.179908, -0.010779, -0.009135, 358.201873, 0.415596, 344.217132},//Skin - 223
{0.156689, -0.015437, -0.009135, 358.201873, 0.415596, 352.741638},//Skin - 224
{0.156689, -0.015437, -0.009135, 358.201873, 0.415596, 352.741638},//Skin - 225
{0.134990, -0.034685, -0.009135, 358.201873, 0.415596, 340.812927},//Skin - 226
{0.151760, 0.002680, -0.009135, 358.201873, 0.415596, 340.812927},//Skin - 227
{0.167410, -0.028664, -0.009135, 358.201873, 0.415596, 340.250427},//Skin - 228
{0.127699, -0.015571, -0.006103, 358.201873, 0.415596, 347.232238},//Skin - 229
{0.100555, -0.007753, -0.006103, 358.201873, 0.415596, 347.232238},//Skin - 230
{0.126940, 0.016886, -0.006103, 358.201873, 0.415596, 347.232238},//Skin - 231
{0.132949, -0.017515, -0.008594, 358.201873, 0.415596, 347.232238},//Skin - 232
{0.146124, -0.008425, -0.008594, 358.201873, 0.415596, 347.232238},//Skin - 233
{0.125714, -0.021018, -0.008594, 358.201873, 0.415596, 347.232238},//Skin - 234
{0.084982, -0.009809, -0.008594, 358.201873, 0.415596, 347.232238},//Skin - 235
{0.114669, -0.005190, -0.008594, 358.201873, 0.415596, 351.301177},//Skin - 236
{0.123264, -0.014946, -0.008594, 358.201873, 0.415596, 351.301177},//Skin - 237
{0.146656, -0.023925, -0.006749, 358.201873, 0.415596, 334.356781},//Skin - 238
{0.133769, -0.007373, -0.006749, 358.201873, 0.415596, 343.105895},//Skin - 239
{0.165378, -0.020173, -0.005869, 358.201873, 0.415596, 348.352233},//Skin - 240
{0.143331, -0.133577, -0.011472, 358.201873, 0.415596, 312.328857},//Skin - 241
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                  //Skin - 242
{0.098602, 0.002084, -0.011472, 358.201873, 0.415596, 348.195495},//Skin - 243
{0.124240, -0.011682, -0.006423, 358.201873, 0.415596, 341.555999},//Skin - 244
{0.158155, -0.044311, -0.005439, 358.201873, 0.415596, 336.024902},//Skin - 245
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                  //Skin - 246
{0.164307, -0.040974, -0.006797, 358.201873, 0.415596, 337.067047},//Skin - 247
{0.191578, -0.040435, -0.010605, 358.201873, 0.415596, 340.908203},//Skin - 248
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                  //Skin - 249
{0.135208, -0.015243, -0.011916, 358.201873, 0.415596, 340.908203},//Skin - 250
{0.134272, -0.027377, -0.006035, 358.201873, 0.415596, 333.416168},//Skin - 251
{0.158813, -0.038977, -0.006035, 358.201873, 0.415596, 336.013519},//Skin - 252
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                  //Skin - 253
{0.165106, -0.048880, -0.009719, 358.201873, 0.415596, 331.050933},//Skin - 254
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                  //Skin - 255
{0.142586, 0.020829, -0.008549, 358.201873, 0.415596, 2.765411},//Skin - 256
{0.134018, -0.024462, -0.008549, 358.201873, 0.415596, 339.642486},//Skin - 257
{0.147750, -0.042854, -0.008114, 0.951334, 0.415596, 330.441131},//Skin - 258
{0.147750, -0.042854, -0.008114, 0.951334, 0.415596, 330.441131},//Skin - 259
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                  //Skin - 260
{0.134710, 0.006798, -0.008114, 358.188964, 0.415596, 352.703582},//Skin - 261
{0.111691, 0.006798, -0.008114, 358.188964, 0.415596, 352.703582},//Skin - 262
{0.146077, -0.005195, -0.008114, 358.188964, 0.415596, 3.866970},//Skin - 263
{0.135858, -0.157842, -0.008114, 358.188964, 0.415596, 314.852203},//Skin - 264
{0.127964, 0.000132, -0.008114, 358.188964, 0.415596, 352.699432},//Skin - 265
{0.127964, -0.002646, -0.008114, 358.188964, 0.415596, 352.699432},//Skin - 266
{0.132329, -0.014261, -0.007384, 1.504234, 0.415596, 352.699432},//Skin - 267
{0.145951, -0.043442, -0.010053, 1.504234, 0.415596, 320.469390},//Skin - 268
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                  //Skin - 269
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                  //Skin - 270
{0.141851, -0.034538, -0.010580, 1.504234, 0.415596, 340.349456},//Skin - 271
{0.136473, -0.057088, -0.008204, 1.504234, 0.415596, 318.134399},//Skin - 272
{0.140332, -0.013011, -0.004432, 354.106964, 357.498840, 352.802062}, //Skin - 273
{0.124270, 0.003252, -0.008204, 1.504234, 0.415596, 346.744995},//Skin - 274
{0.131583, 0.007682, -0.008204, 1.504234, 0.415596, 346.744995},//Skin - 275
{0.131583, 0.007682, -0.008204, 1.504234, 0.415596, 346.744995},//Skin - 276
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                  //Skin - 277
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                  //Skin - 278
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                  //Skin - 279
{0.131583, 0.007682, -0.008204, 1.504234, 0.415596, 346.744995},//Skin - 280
{0.131583, 0.007682, -0.008204, 1.504234, 0.415596, 346.744995},//Skin - 281
{0.140515, 0.009018, -0.008204, 1.504234, 0.415596, 346.744995},//Skin - 282
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                  //Skin - 283
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                  //Skin - 284
{0.189940, -0.004297, 0.001481, 354.106964, 357.498840, 352.802062},//Skin - 285
{0.140515, 0.001933, -0.008204, 1.504234, 0.415596, 346.744995}, //Skin - 286
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                  //Skin - 287
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                  //Skin - 288
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                  //Skin - 289
{0.128789, -0.014062, -0.007850, 1.504234, 0.415596, 340.341094},//Skin - 290
{0.158929, -0.027358, -0.010655, 1.504234, 0.415596, 337.298858},//Skin - 291
{0.113309, -0.012434, -0.010655, 1.504234, 0.415596, 337.298858},//Skin - 292
{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},                                  //Skin - 293
{0.158438, -0.023891, -0.007217, 1.504234, 0.415596, 337.298858},//Skin - 294
{0.145000, -0.032054, -0.007217, 1.504234, 0.415596, 336.385589},//Skin - 295
{0.139293, -0.069554, -0.010619, 1.504234, 0.415596, 320.746429},//Skin - 296
{0.148252, -0.066463, -0.010619, 1.504234, 0.415596, 320.729705},//Skin - 297
{0.126423, -0.066463, -0.010619, 1.504234, 0.415596, 320.729705},//Skin - 298
{0.144949, -0.040691, -0.008599, 1.504234, 0.415596, 320.729705},//Skin - 299
{0.131583, 0.007682, -0.008204, 1.504234, 0.415596, 346.744995},//Skin - 300
{0.131583, 0.007682, -0.008204, 1.504234, 0.415596, 346.744995},//Skin - 301
{0.131583, 0.007682, -0.008204, 1.504234, 0.415596, 346.744995},//Skin - 302
{0.131583, 0.007682, -0.008204, 1.504234, 0.415596, 346.744995},//Skin - 303
{0.131583, 0.007682, -0.008204, 1.504234, 0.415596, 346.744995},//Skin - 304
{0.131583, 0.007682, -0.008204, 1.504234, 0.415596, 346.744995},//Skin - 305
{0.146124, -0.008425, -0.008594, 358.201873, 0.415596, 347.232238},//Skin - 306
{0.146124, -0.008425, -0.008594, 358.201873, 0.415596, 347.232238},//Skin - 307
{0.146124, -0.008425, -0.008594, 358.201873, 0.415596, 347.232238},//Skin - 308
{0.146124, -0.008425, -0.008594, 358.201873, 0.415596, 347.232238},//Skin - 309
{0.140515, 0.009018, -0.008204, 1.504234, 0.415596, 346.744995},//Skin - 310
{0.140515, 0.009018, -0.008204, 1.504234, 0.415596, 346.744995} //Skin - 311

};

//////////////////////////////////////////////////////
//                SERTVER STARTING                  //
//////////////////////////////////////////////////////

main()
{
	print(" "ProjectName" Versao - "GameModeVersion"");
	print(" Ultimo update - "GameModeUpdate"");
	return 1;
}

public OnGameModeInit()
{
	Map_OnGameModeInit();
	SQL_OnGameModeInit();
	Server_OnGameModeInit();
	Job_OnGameModeInit();
	TDraw_OnGameModeInit();
	Taxi_OnGameModeInit();
	ItemDrop_Init();
	ChemistryDrop_Init();
	Chemistry_Init();
	Label_OnGameModeInit();
	Tower_OnGameModeInit();
	Garage_OnGameModeInit();
	Enterance_OnGameModeInit();
    Gate_OnGameModeInit();
	ATM_OnGameModeInit();
    Tele_OnGameModeInit();
    Obj_OnGameModeInit();
    CCTV_OnGameModeInit();
	ChopShop_OnGameModeInit();
	SC_OnGameModeInit();
    House_OnGameModeInit();
    Apart_OnGameModeInit();
    Biz_OnGameModeInit();
    Food_OnGameModeInit();
    APB_OnGameModeInit();
    Factions_OnGameModeInit();
    Radio_OnGameModeInit();
    Drugs_OnGameModeInit();
    Trucker_OnGameModeInit();
    PayPhone_OnGameModeInit();
    Marks_OnGameModeInit();
    Graffity_OnGameModeInit();
	Halloween_OnGameModeInit();
	Ship_Init();
	Patrul_Init();
    Fish_Init();
    Toll_Init();
	SetCurrentTime();
	Interior_OnGameModeInit();
	Timer_OnGameModeInit();
	License_OnGameModeInit();
	Donate_OnGameModeInit();
	LoadPayPhoneTD();
	Advert_OnGameModeInit();
	BillBoard_OnGameModeInit();
	Park_OnGameModeInit();
	RadarHud_OnGameModeInit();

	//StoreTD();
	Street_OnGameModeInit();

	SetTimer("Cars_OnGameModeInit", 5000, false);
	SetTimer("Vehicle_OnGameModeInit", 5000, false);
	
	UploadAntiCheatSettings();
	
	
	FishActor = CreateActor(30, 1427.8522,-208.0056,7.8505, 184.6293);
	electricianactor = CreateActor(255, 1228.1548,182.5512,20.2784,335.5103);
	return 1;
}

public OnGameModeExit()
{
	mysql_tquery(dbHandle, "UPDATE `users` SET `online`=0 WHERE `online` = 1");
	ChemistrySave();
	for(new i; i < MAX_BIZ; i++) Save_Business(i);
	for(new i; i < MAX_HOUSES; i++) Save_House(i);
	
	mysql_close(dbHandle);
	DestroyActor(FishActor);
	DestroyActor(electricianactor);
	return 1;
}

forward OnLogsExitInsert();
public OnLogsExitInsert()
{
    logs_exit = cache_insert_id();
    return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
	if (IsPlayerNPC(playerid))
		return 1;

    if (IsPlayerLogged(playerid))
	{
     	SetSpawnInfoEx(playerid, PlayerInfo[playerid][pPosX],PlayerInfo[playerid][pPosY],PlayerInfo[playerid][pPosZ]);
    	SpawnPlayer(playerid);
		return 1;
	}
	return 1;
}

public OnPlayerConnect(playerid)
{
	Login_OnPlayerConnect(playerid);
	Anticheat_OnPlayerConnect(playerid);
	
	Mapping_OnPlayerConnect(playerid);
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	Login_OnPlayerDisconnect(playerid, reason);
	Graffity_OnPlayerDisconnect(playerid);
	Police_OnPlayerDisconnect(playerid);
	DeletePVar(playerid, "electricianJob");
	return 1;
}

public OnVehicleSirenStateChange(playerid, vehicleid, newstate)
{
	return 1;
}

public OnPlayerSpawn(playerid)
{
	Login_OnPlayerSpawn(playerid);
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
    Police_OnPlayerDeath(playerid);
	Death_OnPlayerDeath(playerid, killerid, reason);
	Graffity_OnPlayerDeath(playerid);
	return 1;
}

public OnVehicleSpawn(vehicleid)
{
    Vehicle_OnVehicleSpawn(vehicleid);
	return 1;
}

public OnVehicleDeath(vehicleid, killerid)
{
    Police_OnVehicleDeath(vehicleid);
	Vehicle_OnVehicleDeath(vehicleid, killerid);
	Radar_OnVehicleDeath(vehicleid);
	return 1;
}

public OnPlayerCommandPerformed(playerid, cmdtext[], success)
{
	if (success == -1)	return SendErrorMessage(playerid, "Comando \"%s\" não existe. use {ffc46a}/ajuda{FFFFFF} ou {ffc46a}/assistencia{FFFFFF}.", cmdtext);

	return 1;
}

public OnPlayerCommandReceived(playerid, cmdtext[])
{
	if (!IsPlayerLogged(playerid)) return 0;
	return 1;
}

public OnPlayerText(playerid, text[])
{
	if (!IsPlayerLogged(playerid)) 			return 0;
	if (GetPVarInt(playerid, #buing_phone))	return 0;
	
	if (PlayerInfo[playerid][pMutedTime] > 0)
	{
		SendErrorMessage(playerid, "Seu chat local está bloqueado.");
		return 0;
	}
	if (text[0] == '.')
	{
		SendErrorMessage(playerid, "Comando \"%s\" não existe. use {ffc46a}/ajuda{FFFFFF} ou {ffc46a}/assistencia{FFFFFF}.");
	    return 0;
	}
	if (PlayerInfo[playerid][pInjured] == 2)
	{
		SendErrorMessage(playerid, "Você não pode digitar enquanto estiver morto.");
		return 0;
	}
	
	Player_OnPlayerText(playerid, text);
	return 0;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
	return 0;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if (dialogid == DIALOG_CONFIRM_SYS) ConfirmDialog_Response(playerid, response);
	
	Drugs_OnDialogResponse(playerid, dialogid, response, listitem);
	return 1;
}



public OnPlayerStateChange(playerid, newstate, oldstate)
{
    if ((newstate == 2 && oldstate == 3) || (newstate == 3 && oldstate == 2)) 	return Kick(playerid);
	if (IsPlayerNPC(playerid)) 													return 1;

	Box_OnPlayerStateChange(playerid, newstate);
	Weapon_OnPlayerStateChange(playerid, newstate);
	Taxi_OnPlayerStateChange(playerid, newstate);
	Vehicle_OnPlayerStateChange(playerid, newstate, oldstate);
	HUD_OnPlayerStateChange(playerid, newstate, oldstate);
	Lic_OnPlayerStateChange(playerid, newstate);
	Drugs_OnPlayerStateChange(playerid, newstate);
	Radar_OnPlayerStateChange(playerid, newstate, oldstate);
	Trash_OnPlayerStateChange(playerid, newstate, oldstate);
	return 1;
}
public OnPlayerEnterCheckpoint(playerid)
{
    if (GetPVarInt(playerid, #SWATROPE) && PlayerInfo[playerid][pSwatDuty])
    {
        DeletePVar(playerid, #SWATROPE);
        DeletePVar(playerid, #CHOPID);
        
        ClearAnimations(playerid);
        TogglePlayerControllable(playerid,0);
        TogglePlayerControllable(playerid,1);
        
        DisablePlayerCheckpoint(playerid);

		for (new i = 0; i < MAX_ROPE_LENGTH; i++) {
            DestroyObject(RopesInfo[playerid][i]);
        }
    }
	if(GetPVarInt(playerid, "electricianJob")) { 
		if(IsPlayerInAnyVehicle(playerid)) return SendErrorMessage(playerid, "Você deve sair do veículo primeiro.");
		cmd::reparacurentu(playerid, "");
		//SendClientMessage(playerid, -1, "Bun");
		DisableWaypoint(playerid);
		new rand3 = random(sizeof(repair_zones));
		SetPlayerCheckpoint(playerid, repair_zones[rand3][0], repair_zones[rand3][1], repair_zones[rand3][2], 2.5);
		//if()
		return 1;
	}
	return 1;
}

public OnPlayerEnterRaceCheckpoint(playerid)
{
	return 1;
}

public OnPlayerLeaveRaceCheckpoint(playerid)
{
	return 1;
}

public OnRconCommand(cmd[])
{
	printf(cmd);
	return 1;
}

public OnPlayerRequestSpawn(playerid)
{
	if (IsPlayerNPC(playerid))
		return 1;
	    
	return 0;
}

public OnDynamicObjectMoved(objectid)
{
    Ship_OnDynamicObjectMoved(objectid);

	return 1;
}


public OnObjectMoved(objectid)
{
	return 1;
}

public OnPlayerObjectMoved(playerid, objectid)
{
	return 1;
}

public OnPlayerPickUpPickup(playerid, pickupid)
{
	return 1;
}

public OnVehicleMod(playerid, vehicleid, componentid)
{
	return 1;
}

public OnVehiclePaintjob(playerid, vehicleid, paintjobid)
{
	return 1;
}

public OnVehicleRespray(playerid, vehicleid, color1, color2)
{
	return 1;
}

public OnVehicleDamageStatusUpdate(vehicleid, playerid)
{
	V_OnVehicleDamageStatusUpdate(vehicleid);
	return 1;
}

public OnPlayerSelectedMenuRow(playerid, row)
{
	House_OnPlayerSelectedMenuRow(playerid, row);
	return 1;
}

public OnPlayerExitedMenu(playerid)
{
    if (pTemp[playerid][pInteriorBiz])	ShowMenuForPlayer(buy_interior, playerid);
	
	return 1;
}

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
	return 1;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	Admin_OnPlayerKeyStateChange(playerid, newkeys);
	Attach_OnPlayerKeyStateChange(playerid, newkeys);
	Drugs_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
	Keys_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
	Skate_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
	return 1;
}

public OnRconLoginAttempt(ip[], password[], success)
{
	return 1;
}

public OnPlayerUpdate(playerid)
{
	Player_OnPlayerUpdate(playerid);
	Vehicle_OnPlayerUpdate(playerid);
	Phone_OnPlayerUpdate(playerid);
	Corpse_OnPlayerUpdate(playerid);
	return 1;
}

public OnPlayerStreamIn(playerid, forplayerid)
{
    if ((PlayerInfo[forplayerid][pSettings] & togName) || maskOn{playerid})	ShowPlayerNameTagForPlayer(forplayerid, playerid, 0);

	return 1;
}

public OnPlayerStreamOut(playerid, forplayerid)
{
	if (IsPlayerLogged(playerid) && maskOn{playerid})	ShowPlayerNameTagForPlayer(playerid, playerid, false);

	return 1;
}

public OnVehicleStreamIn(vehicleid, forplayerid)
{
	if (VehicleInfo[vehicleid][carLocked]) SetVehicleParamsForPlayer(vehicleid,forplayerid,0,1)
	;
	return 1;
}

public OnVehicleStreamOut(vehicleid, forplayerid)
{
    SetVehicleParamsForPlayer(vehicleid,forplayerid,0,0);
	return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, source)
{
	return 1;
}

public OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart)
{
	//Weapon_OnPlayerGiveDamage(playerid, damagedid, amount, weaponid, bodypart);
	return 1;
}

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
    Weapon_OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart);
	return 1;
}

forward OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ);
public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
    Weapon_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, fX, fY, fZ);
    Fire_OnPlayerWeaponShot(playerid);
    CCTV_OnPlayerWeaponShot(playerid, weaponid);
	Shells_OnPlayerWeaponShot(playerid);
	return 1;
}

public OnPlayerChange3DMenuBox(playerid,MenuID,boxid,list,boxes)
{
	return 1;
}

public OnPlayerSelect3DMenuBox(playerid,MenuID,boxid,list,boxes)
{
	Furn_OnPlayerSelect3DMenuBox(playerid, MenuID, boxid, list, boxes);
    
	return 1;
}

public OnPlayerSelectDynamicObject(playerid, STREAMER_TAG_OBJECT objectid, modelid, Float:x, Float:y, Float:z)
{
	Ob_OnPlayerSelectDynamicObject(playerid, objectid);
	Fu_OnPlayerSelectDynamicObject(playerid, objectid);
	
	return 1;
}

public OnPlayerEditObject(playerid, playerobject, objectid, response, Float:fX, Float:fY, Float:fZ, Float:fRotX, Float:fRotY, Float:fRotZ)
{
	return 1;
}

public OnPlayerEditDynamicObject(playerid, objectid, response, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz)
{
	/*
	0 - EDIT_RESPONSE_CANCEL   // player cancelled (ESC)
	1 - EDIT_RESPONSE_FINAL    // player clicked on save
	2 - EDIT_RESPONSE_UPDATE   // player moved the object (edition did not stop at all)
	*/
    if (GetPVarInt(playerid, #edit_street)) 	 		St_OnPlayerEditDynamicObject(playerid, objectid, response, x, y, z, rx, ry, rz);
 	else if (GetPVarInt(playerid, #edit_food))  		Food_OnPlayerEditDynamicObject(playerid, objectid, response, x, y, z, rx, ry, rz);
	else if (GetPVarInt(playerid, #edit_item))  		Item_OnPlayerEditDynamicObject(playerid, objectid, response, x, y, z, rx, ry, rz);
	else if (GetPVarInt(playerid, #edit_drug))  		Drug_OnPlayerEditDynamicObject(playerid, objectid, response, x, y, z, rx, ry, rz);
	else if (GetPVarInt(playerid, #edit_atm))   		ATM_OnPlayerEditDynamicObject(playerid, objectid, response, x, y, z, rz);
	else if (GetPVarInt(playerid, #edit_trash))   		Trash_OnPlayerEditDynamicObject(playerid, objectid, response, x, y, z, rz);
	else if (GetPVarInt(playerid, #edit_pp))   			PP_OnPlayerEditDynamicObject(playerid, objectid, response, x, y, z, rz);
	else if (GetPVarInt(playerid, #edit_pm))   			PM_OnPlayerEditDynamicObject(playerid, objectid, response, x, y, z, rz);
	else if (pTemp[playerid][pEditBort]) 				Fact_OnPlayerEditDynamicObject(playerid, objectid, response, x, y, z, rx, ry, rz);
	else if (GetPVarInt(playerid, #veh_editor))			Veh_OnPlayerEditDynamicObject(playerid, objectid, response, x, y, z, rx, ry, rz);
	else if (GetPVarInt(playerid, #edit_object))		Obj_OnPlayerEditDynamicObject(playerid, objectid, response, x, y, z, rx, ry, rz);
	else if (GetPVarInt(playerid, #edit_cctv))			CCTV_OnPlayerEditDynamicObject(playerid, objectid, response, x, y, z, rx, ry, rz);
	else if (GetPVarInt(playerid, #edit_tower))			Tow_OnPlayerEditDynamicObject(playerid, objectid, response, x, y, z, rx, ry, rz);
	else if (GetPVarInt(playerid, #edit_gate) ||
			GetPVarInt(playerid, #2_edit_gate))			Gate_OnPlayerEditDynamicObject(playerid, objectid, response, x, y, z, rx, ry, rz);
	else if (GetPVarInt(playerid, "edit_furniture"))	Bu_OnPlayerEditDynamicObject(playerid, objectid, response, x, y, z, rx, ry, rz);
	else if (GetPVarInt(playerid, "Graffity:EditPos"))	Graf_OnPlayerEditDynamicObject(playerid, objectid, response, x, y, z, rx, ry, rz);
	else if (GetPVarInt(playerid, "SC:Edit"))			SC_OnPlayerEditDynamicObject(playerid, objectid, response, x, y, z, rx, ry, rz);
	else if (GetPVarInt(playerid, #EditChopShop))		ChSh_OnPlayerEditDynamicObject(playerid, objectid, response, x, y, z, rx, ry, rz);
	else if (GetPVarInt(playerid, "BB:Edit"))			BB_OnPlayerEditDynamicObject(playerid, objectid, response, x, y, z, rx, ry, rz);
	else if (GetPVarInt(playerid, #CorpsEdit))			Corpse_OnPlayerEdit(playerid, objectid, response, x, y, z, rz);
	return 1;
}

public OnPlayerEnterDynamicRaceCP(playerid, checkpointid)
{
	Lic_OnPlayerEnterDynamicRaceCP(playerid, checkpointid);
	return 1;
}

public OnPlayerEnterDynamicCP(playerid, checkpointid)
{
	//Vehicle_OnPlayerEnterDynamicCP(playerid, checkpointid);
	GPS_OnPlayerEnterDynamicCP(playerid, checkpointid);
	House_OnPlayerEnterDynamicCP(playerid, checkpointid);
	Garage_OnPlayerEnterDynamicCP(playerid, checkpointid);
	Apart_OnPlayerEnterDynamicCP(playerid, checkpointid);
	Trash_OnPlayerEnterDynamicCP(playerid, checkpointid);
	return 1;
}
public OnPlayerLeaveDynamicCP(playerid, checkpointid)
{
	return 1;
}

public OnPlayerPickUpDynamicPickup(playerid, pickupid)
{
	Biz_OnPlayerPickUpDynamicPickup(playerid, pickupid);
	Ent_OnPlayerPickUpDynamicPickup(playerid, pickupid);
	Lab_OnPlayerPickUpDynamicPickup(playerid, pickupid);
	Fo_OnPlayerPickUpDynamicPickup(playerid, pickupid);
	return 1;
}

public OnPlayerEditAttachedObject(playerid, response, index, modelid, boneid, Float:fOffsetX, Float:fOffsetY, Float:fOffsetZ, Float:fRotX, Float:fRotY, Float:fRotZ, Float:fScaleX, Float:fScaleY, Float:fScaleZ)
{
    Attach_EditAttachedObject(playerid, response, index, modelid, boneid, Float:fOffsetX, Float:fOffsetY, Float:fOffsetZ, Float:fRotX, Float:fRotY, Float:fRotZ, Float:fScaleX, Float:fScaleY, Float:fScaleZ);
	Faction_EditAttachedObject(playerid, response, index, modelid, boneid, Float:fOffsetX, Float:fOffsetY, Float:fOffsetZ, Float:fRotX, Float:fRotY, Float:fRotZ, Float:fScaleX, Float:fScaleY, Float:fScaleZ);
	Weapon_EditAttachedObject(playerid, response, index, boneid, Float:fOffsetX, Float:fOffsetY, Float:fOffsetZ, Float:fRotX, Float:fRotY, Float:fRotZ);
	return 1;
}

public OnPlayerEnterDynamicArea(playerid, areaid)
{
	if (!IsPlayerLogged(playerid)) return 0;

	for(new i; i < Iter_Count(boomboxIter); i++) 	 if (areaid == BoomboxInfo[i][bArea])  Box_OnPlayerEnterArea(playerid, areaid);
	for(new i; i < Iter_Count(speedcamObjects); i++) if (areaid == speedcam[i][sc_areaid]) SpeedCam_OnPlayerEnterArea(playerid, areaid);
	
	return 1;
}

public OnPlayerLeaveDynamicArea(playerid, areaid)
{
	Box_OnPlayerLeaveArea(playerid, areaid);
	return 1;
}

public OnPlayerClickTextDraw(playerid, Text:clickedid)
{
	if (clickedid == Text:INVALID_TEXT_DRAW)
	{
		if (pTemp[playerid][pPayphone] != -1)
			return cancelPayphone(playerid);
		
		else if (GetPVarInt(playerid, #spawnveh_id))
		{
			CancelSelectTextDraw(playerid);
			DeletePVar(playerid, #spawnveh_id);

			PlayerTextDrawHide(playerid, SpawnVeh_Model[playerid]);
			PlayerTextDrawHide(playerid, SpawnVeh_Box[playerid]);
			for(new i = 0; i < 2; i++) PlayerTextDrawHide(playerid, SpawnVeh_Arrows[playerid][i]);
			for(new e = 0; e < 3; e++) PlayerTextDrawHide(playerid, SpawnVeh_Base[playerid][e]);
		}
	}
	
 	if (clickedid == jobsmenu[12])
    {
         //SendClientMessage(playerid, 0xFFFFFFAA, "12");
		 TextDrawHideForPlayer(playerid, jobsmenu[12]);
		 TextDrawShowForPlayer(playerid, jobsmenu[7]);
		 TextDrawShowForPlayer(playerid, jobsmenu[8]);
		 TextDrawShowForPlayer(playerid, jobsmenu[9]);
	     TextDrawShowForPlayer(playerid, jobsmenu[10]);
         TextDrawShowForPlayer(playerid, jobsmenu[11]);
         TextDrawShowForPlayer(playerid, jobsmenu[0]);
         return 1;
    }
    if (clickedid == jobsmenu[8])
    {
    	TextDrawHideForPlayer(playerid, jobsmenu[0]);
        TextDrawHideForPlayer(playerid, jobsmenu[1]);
		TextDrawHideForPlayer(playerid, jobsmenu[2]);
        TextDrawHideForPlayer(playerid, jobsmenu[3]);
		TextDrawHideForPlayer(playerid, jobsmenu[4]);
		TextDrawHideForPlayer(playerid, jobsmenu[5]);
		TextDrawHideForPlayer(playerid, jobsmenu[6]);
        TextDrawHideForPlayer(playerid, jobsmenu[7]);
        TextDrawHideForPlayer(playerid, jobsmenu[8]);
        TextDrawHideForPlayer(playerid, jobsmenu[9]);
        TextDrawHideForPlayer(playerid, jobsmenu[10]);
        TextDrawHideForPlayer(playerid, jobsmenu[11]);
        SetCameraBehindPlayer(playerid);
		CancelSelectTextDraw(playerid);
        SendErrorMessage(playerid, "Cancelado.");
        return 1;
    }
    if (clickedid == jobsmenu[0])
    {
    	TextDrawHideForPlayer(playerid, jobsmenu[0]);
        TextDrawHideForPlayer(playerid, jobsmenu[1]);
		TextDrawHideForPlayer(playerid, jobsmenu[2]);
        TextDrawHideForPlayer(playerid, jobsmenu[3]);
		TextDrawHideForPlayer(playerid, jobsmenu[4]);
		TextDrawHideForPlayer(playerid, jobsmenu[5]);
		TextDrawHideForPlayer(playerid, jobsmenu[6]);
        TextDrawHideForPlayer(playerid, jobsmenu[7]);
        TextDrawHideForPlayer(playerid, jobsmenu[8]);
        TextDrawHideForPlayer(playerid, jobsmenu[9]);
        TextDrawHideForPlayer(playerid, jobsmenu[10]);
        TextDrawHideForPlayer(playerid, jobsmenu[11]);
        SetCameraBehindPlayer(playerid);
		CancelSelectTextDraw(playerid);
		if(IsPlayerInRangeOfPoint(playerid, 1.5, 1427.8522,-208.0056,7.8505)) {
        SendClientMessage(playerid, -1, "Sebastian diz: De agora em diante você é pescador.");
		SendSyntaxMessage(playerid, "Use o comando {ffc46a}/ajuda{FFFFFF}, e selecione 'Pescador'"); 
		}
		if(IsPlayerInRangeOfPoint(playerid, 3.5, 1228.1548,182.5512,20.2784)) {
			new vehiculjob = GetPlayerVehicleID(playerid);
			SendClientMessage(playerid, -1, "Stephan spune: De agora em diante você é eletricista.");
			SendWarningMessage(playerid, "Caso seu carro seja destruído, você será parado imediatamente e o dinheiro será retirado de sua conta para reparos.");
			SendWarningMessage(playerid, "Para parar, use /parartrabalho");
			SendSyntaxMessage(playerid, "Use o comando {ffc46a}/ajuda{FFFFFF}, e selecione 'Eletricista'");
			SetPlayerSkin(playerid, 50);
			vehiculjob = CreateVehicle(552, 1210.5264,141.1469,20.2006,246.1348, 1, 1, -1); 
			PutPlayerInVehicle(playerid, vehiculjob, 0);
			new rand3 = random(sizeof(repair_zones));
			SetPlayerCheckpoint(playerid, repair_zones[rand3][0], repair_zones[rand3][1], repair_zones[rand3][2], 2.5);
			SetPVarInt(playerid, "electricianJob", !GetPVarInt(playerid, "electricianJob"));
			SetPVarInt(playerid, "vehCollision", !GetPVarInt(playerid, "vehCollision"));
			DisableRemoteVehicleCollisions(playerid, GetPVarInt(playerid, "vehCollision"));
			new Float:health;
			new veh = GetPlayerVehicleID(playerid);
			GetVehicleHealth(veh, health);
			if(health < 700) { 
			DestroyVehicle(vehiculjob);
			SetPlayerSkin(playerid, PlayerInfo[playerid][pSkin]);
			SendWarningMessage(playerid, "O carro foi enviado para a HQ dos eletricistas."); 
			}
		}
        return 1;
    }
	
	if(clickedid == fishTD[1]) { 
		TextDrawHideForPlayer(playerid, fishTD[1]);
		TextDrawShowForPlayer(playerid, fishTD[2]);
		//SendClientMessage(playerid, -1, "peste 1, afisez 2");
		return 1;
	}
	if(clickedid == fishTD[2]) {
		TextDrawHideForPlayer(playerid, fishTD[2]);
		TextDrawShowForPlayer(playerid, fishTD[3]);
	//	SendClientMessage(playerid, -1, "peste 2, afisez 3");
		return 1;
	}
	if(clickedid == fishTD[3]) {
		CancelSelectTextDraw(playerid);
		//SendClientMessage(playerid, -1, "peste 3, nu mai afisez");
		for(new i= 0; i < 10; i++) TextDrawHideForPlayer(playerid, fishTD[i]);
		StopPlayerAnims(playerid);
		if (random(2) == 0)
				{
				    new randlb = randomEx(15, 30),
						rand = random(sizeof(fish_names));

					SM2(playerid, "Você pescou um(a) %s pesando: %i kg.", fish_names[rand], randlb);
					if (PlayerInfo[playerid][pFishCount] + randlb <= 1000)
						PlayerInfo[playerid][pFishCount] += randlb;
					else
					    PlayerInfo[playerid][pFishCount] = 1000;
				}
				else
					SM1(playerid, "Você não pegou nada, tente novamente. ");
		return 1;
	}

	if(clickedid == electricianTD[3] && electricianTD[5]) {
		//SendClientMessage(playerid, -1, "3 & 5");
		TextDrawShowForPlayer(playerid, electricianTD[2]);
		TextDrawShowForPlayer(playerid, electricianTD[6]);
		TextDrawShowForPlayer(playerid, electricianTD[7]);
		return 1;
	}
	if(clickedid == electricianTD[6] && electricianTD[7]) {
		//SendClientMessage(playerid, -1, "6 & 7");
		TextDrawShowForPlayer(playerid, electricianTD[4]);
		TextDrawShowForPlayer(playerid, electricianTD[9]);
		TextDrawShowForPlayer(playerid, electricianTD[10]);
	}
	if(clickedid == electricianTD[9] && electricianTD[10]) {
	//	SendClientMessage(playerid, -1, "9 & 10");
		TextDrawShowForPlayer(playerid, electricianTD[8]);
		TextDrawShowForPlayer(playerid, electricianTD[12]);
		TextDrawShowForPlayer(playerid, electricianTD[13]);
	}
	if(clickedid == electricianTD[12] && electricianTD[13]) {
	// SendClientMessage(playerid, -1, "12 & 13, inchid");
		TextDrawShowForPlayer(playerid, electricianTD[11]);
		SM2(playerid, "A energia voltou a funcionar graças a você.");
		GameTextForPlayer(playerid, "Reparo feito", 5000, 1);
		for(new i= 0; i < 14; i++) TextDrawHideForPlayer(playerid, electricianTD[i]);
		new money = 100 + random(350);
		GivePlayerMoney(playerid, money);
		CancelSelectTextDraw(playerid);
	}

	TD_OnPlayerClickTextDraw(playerid, clickedid);
	PP_OnPlayerClickTextDraw(playerid, clickedid);
	Ph_OnPlayerClickTextDraw(playerid, clickedid);
	return 1;
}
public OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
	//Ph_OnPlayerClickPlayerTextDraw(playerid, playertextid);
	Fo_OnPlayerClickPlayerTextDraw(playerid, playertextid);
	PP_OnPlayerClickPlayerTextDraw(playerid, playertextid);
	Vh_OnPlayerClickPlayerTextDraw(playerid, playertextid);
	MDC_OnPlayerClickPlayerTextDraw(playerid, playertextid);
	return 1;
}

public OnPlayerLeaveCheckpoint(playerid)
{
	return 1;
}

public OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
	Admin_OnPlayerClickMap(playerid, fX, fY, fZ);
	return 1;
}

public OnQueryError(errorid, error[], callback[], query[], connectionHandle)
{
	SQL_OnQueryError(errorid, error, callback, query);
	return 1;
}

public OnUnoccupiedVehicleUpdate(vehicleid, playerid, passenger_seat, Float:new_x, Float:new_y, Float:new_z, Float:vel_x, Float:vel_y, Float:vel_z)
{
    return 1;
}

public OnPlayerShootDynamicObject(playerid, weaponid, STREAMER_TAG_OBJECT objectid, Float:x, Float:y, Float:z)
{
	CCTV_OnPlayerShootDynamicObject(playerid, weaponid, objectid);
	return 1;
}



public OnPlayerExitVehicle(playerid, vehicleid)
{
	Weapon_OnPlayerExitVehicle(playerid, vehicleid);
	Mechanic_OnPlayerExitVehicle(playerid);
	License_OnPlayerExitVehicle(playerid, vehicleid);
	Vehicle_OnPlayerExitVehicle(playerid, vehicleid);
	Box_OnPlayerExitVehicle(playerid);
	Death_OnPlayerExitVehicle(playerid, vehicleid);
	Police_OnPlayerExitVehicle(playerid);
	if(GetPVarInt(playerid, "electricianJob")) { 
		SendWarningMessage(playerid, "Você tem 2 minutos para voltar para o carro."); 
		urcainmasinaelectrician[playerid] = SetTimer("ElectricianTimer", 12000, false);
		return 1;
	}
	return 1;
}



this::ElectricianTimer(playerid)
{
	new vehiculjob = GetPlayerVehicleID(playerid);
	for(new i= 0; i < 14; i++) TextDrawHideForPlayer(playerid, electricianTD[i]);
	DestroyVehicle(vehiculjob);
	SetPlayerSkin(playerid, PlayerInfo[playerid][pSkin]);
	SendWarningMessage(playerid, "O veículo foi enviado para o HQ dos eletricistas."); 
	DisableWaypoint(playerid);
	printf("S-A SFARSIT TIMERU");
	DeletePVar(playerid, "electricianJob");
	return 1;
}


public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
    if (GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_CUFFED || GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_CARRY)	ClearAnimations(playerid);
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed]) return SendErrorMessage(playerid, "Você não pode este recurso agora.");
	
	Police_OnPlayerEnterVehicle(playerid, vehicleid, ispassenger);
	Corpse_OnPlayerEnterVehicle(playerid);

	if(GetPVarInt(playerid, "electricianJob")) {  
		KillTimer(urcainmasinaelectrician[playerid]); 
		return 1; 
	}
	return 1;
}


CMD:stopwork(playerid, params[]) {
	new vehiculjob = GetPlayerVehicleID(playerid);
	for(new i= 0; i < 14; i++) TextDrawHideForPlayer(playerid, electricianTD[i]);
	DestroyVehicle(vehiculjob);
	SetPlayerSkin(playerid, PlayerInfo[playerid][pSkin]);
	SendWarningMessage(playerid, "O veículo foi enviado para o HQ dos eletricistas."); 
	DisableWaypoint(playerid);
	DeletePVar(playerid, "electricianJob");
	return 1;
}

CMD:halloweenhat(playerid, params[])
{
        new hid, skin = GetPlayerSkin(playerid);
        if(sscanf(params, "d", hid)) return SendClientMessage(playerid, COLOR_ORANGE, "{FFD600}Usage:{FFFFFF} /halloweenhat <0/1>");
        if(hid == 0) { RemovePlayerAttachedObject(playerid, 0); Hatss[playerid] = 0; }
        else if(hid == 1) { SetPlayerAttachedObject(playerid, 0, 19528, 2, SkinCaps[skin][0], SkinCaps[skin][1], SkinCaps[skin][2], SkinCaps[skin][3],SkinCaps[skin][4], SkinCaps[skin][5], 1.000000, 1.000000, 1.000000); Hatss[playerid] = 1; }
        //else if(hid == 2) { SetPlayerAttachedObject(playerid, 0, 19320, 2, SkinCaps[skin][0], SkinCaps[skin][1], SkinCaps[skin][2], SkinCaps[skin][3],SkinCaps[skin][4], SkinCaps[skin][5], 1.000000, 1.000000, 1.000000); Hatss[playerid] = 1; }

		return 1;
}
    

CMD:svf(playerid, params[]) {
//pickDialog	new model, type, faction, plate[24], color1, color2, Float:x, Float:y, Float:z, Float:a, vw, interior;
	new faction = PlayerInfo[playerid][pFaction];
	if (IsFactionPolice(faction) || IsFactionDocs(faction)) Dialog_Show(playerid, LSPD, DIALOG_STYLE_LIST, "Veículos", "LSPD Car\nSFPD Car\nLVPD Car\nPolice Ranger\nHPV-1000\nPolice Maverick", ">>", ""); //else return SendErrorMessage(playerid, "Te caut eu cand o sa intrii in PD")//LSPD
	if (IsFactionMedic(faction) && IsFactionFire(faction)) Dialog_Show(playerid, EMS, DIALOG_STYLE_LIST, "Veículos", "Coisas vermelhas", ">>", ""); // else return SendErrorMessage(playerid, "Te caut eu cand o sa intrii in EMS"); //EMS

	//CreateFactionVehicle(true, 411, x, y, z, a, color1, color2, type, faction, plate, vw, interior)
	//SendClientMessage(playerid, -1, "Vehicle created [model %i] [plates %s] [type %i] [Faction %i] [vw %i] [int %i].", model, plate, type, faction, vw, interior);
}	   
//2338.9741,246.1243,26.1270
Dialog:LSPD(playerid, response, listitem, inputtext[]) {
	if(!response) return 1;
	switch(listitem) {
		case 0: {
			new lspdcar;
			lspdcar = CreateFactionVehicle(true, 596, 2338.9741,246.1243,26.1270,178.8572, 1, 0, 0, 1, "LSPD", 0, 0);
			PutPlayerInVehicle(playerid, lspdcar, 0);		
			//SendClientMessage(playerid, -1, "merge, esti scripter bun frate, te felicit");
	    	new query[128 + 128];
			mysql_format(dbHandle, query, sizeof(query), "INSERT INTO `cars` (`date`,`model`,`x`,`y`,`z`,`fa`,`color1`,`color2`,`type`,`faction`,`plate`,`vw`,`int`) VALUES ('%s','596', '2338.9741', '246.1243', '26.1270', '178.8572', '1', '0', '0', '1', 'LSPD', '0', '0')",
			GetFullDate());
			mysql_tquery(dbHandle, query, "OnVehicleInsert", "d", lspdcar);
		}
		case 1: {
			new sfpdcar;
			sfpdcar = CreateFactionVehicle(true, 597, 2338.9741,246.1243,26.1270,178.8572, 1, 0, 0, 1, "LSPD", 0, 0);
			PutPlayerInVehicle(playerid, sfpdcar, 0);		
			//SendClientMessage(playerid, -1, "merge, esti scripter bun frate, te felicit");
	    	new query[128 + 128];
			mysql_format(dbHandle, query, sizeof(query), "INSERT INTO `cars` (`date`,`model`,`x`,`y`,`z`,`fa`,`color1`,`color2`,`type`,`faction`,`plate`,`vw`,`int`) VALUES ('%s','597', '2338.9741', '246.1243', '26.1270', '178.8572', '1', '0', '0', '1', 'LSPD', '0', '0')",
			GetFullDate());
			mysql_tquery(dbHandle, query, "OnVehicleInsert", "d", sfpdcar);
		}
		case 2: {
			new lvpdcar;
			lvpdcar = CreateFactionVehicle(true, 598, 2338.9741,246.1243,26.1270,178.8572, 1, 0, 0, 1, "LSPD", 0, 0);
			PutPlayerInVehicle(playerid, lvpdcar, 0);		
			//SendClientMessage(playerid, -1, "merge, esti scripter bun frate, te felicit");
	    	new query[128 + 128];
			mysql_format(dbHandle, query, sizeof(query), "INSERT INTO `cars` (`date`,`model`,`x`,`y`,`z`,`fa`,`color1`,`color2`,`type`,`faction`,`plate`,`vw`,`int`) VALUES ('%s','598', '2338.9741', '246.1243', '26.1270', '178.8572', '1', '0', '0', '1', 'LSPD', '0', '0')",
			GetFullDate());
			mysql_tquery(dbHandle, query, "OnVehicleInsert", "d", lvpdcar);
		}
		case 3: {
			new policeranger;
			policeranger = CreateFactionVehicle(true, 599, 2338.9741,246.1243,26.1270,178.8572, 1, 0, 0, 1, "LSPD", 0, 0);
			PutPlayerInVehicle(playerid, policeranger, 0);		
			//SendClientMessage(playerid, -1, "merge, esti scripter bun frate, te felicit");
	    	new query[128 + 128];
			mysql_format(dbHandle, query, sizeof(query), "INSERT INTO `cars` (`date`,`model`,`x`,`y`,`z`,`fa`,`color1`,`color2`,`type`,`faction`,`plate`,`vw`,`int`) VALUES ('%s','599', '2338.9741', '246.1243', '26.1270', '178.8572', '1', '0', '0', '1', 'LSPD', '0', '0')",
			GetFullDate());
			mysql_tquery(dbHandle, query, "OnVehicleInsert", "d", policeranger);
		}
		case 4: {
			new hpv;
			hpv = CreateFactionVehicle(true, 523, 2338.9741,246.1243,26.1270,178.8572, 1, 0, 0, 1, "LSPD", 0, 0);
			PutPlayerInVehicle(playerid, hpv, 0);		
			//SendClientMessage(playerid, -1, "merge, esti scripter bun frate, te felicit");
	    	new query[128 + 128];
			mysql_format(dbHandle, query, sizeof(query), "INSERT INTO `cars` (`date`,`model`,`x`,`y`,`z`,`fa`,`color1`,`color2`,`type`,`faction`,`plate`,`vw`,`int`) VALUES ('%s','523', '2338.9741', '246.1243', '26.1270', '178.8572', '1', '0', '0', '1', 'LSPD', '0', '0')",
			GetFullDate());
			mysql_tquery(dbHandle, query, "OnVehicleInsert", "d", hpv);
		}
		case 5: {
			new maverick;
			maverick = CreateFactionVehicle(true, 497, 2360.9404,233.1178,34.9690,88.2200, 1, 0, 0, 1, "LSPD", 0, 0);
			PutPlayerInVehicle(playerid, maverick, 0);		
			//SendClientMessage(playerid, -1, "merge, esti scripter bun frate, te felicit");
	    	new query[128 + 128];
			mysql_format(dbHandle, query, sizeof(query), "INSERT INTO `cars` (`date`,`model`,`x`,`y`,`z`,`fa`,`color1`,`color2`,`type`,`faction`,`plate`,`vw`,`int`) VALUES ('%s','497', '2360.9404', '233.1178', '34.9690', '88.2200', '1', '0', '0', '1', 'LSPD', '0', '0')",
			GetFullDate());
			mysql_tquery(dbHandle, query, "OnVehicleInsert", "d", maverick);
		}
	}
	return 1;
}

Dialog:EMS(playerid, response, listitem, inputtext[]) {
	if(!response) return 1;
	switch(listitem) {
		case 0: {
			SendClientMessage(playerid, -1, "DEV MSG TEST");
		}
	}
	return 1;
}


CMD:dvf(playerid, params[]) { 
	new faction = PlayerInfo[playerid][pFaction];
	if (!IsFactionPolice(faction) || !IsFactionDocs(faction)) return SendErrorMessage(playerid, "Você não faz parte desta facção.");
	if (!IsFactionMedic(faction) && !IsFactionFire(faction)) return SendErrorMessage(playerid, "Você não faz parte desta facção.");
	new vehicle = GetPlayerVehicleID(playerid);
	if(IsPlayerInVehicle(playerid, GetPlayerVehicleID(playerid))) {
			new query[128];
			mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `cars` WHERE `id` = %i", GetPlayerVehicleID(playerid));
			mysql_tquery(dbHandle, query); 
	}
	DestroyVehicle(vehicle);
	return 1;
}


CMD:electricianhelp(playerid, params[]) {
	SendErrorMessage(playerid, "Eletricista");
	SendSyntaxMessage(playerid, "Descrição blá blá blá você está atualizando blá blá blá");
	SendSyntaxMessage(playerid, "Se você não tem corrente, torne-se um eletricista de verdade.");
	return 1;
}
