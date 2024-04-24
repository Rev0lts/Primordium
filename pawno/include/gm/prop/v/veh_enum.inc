enum OWNED_CARS_ENUM
{
	carID,
	carDate[24],
	carModel,
	carColor1,
	carColor2,
	Float:carParkX,
	Float:carParkY,
	Float:carParkZ,
	Float:carParkA,
	carPlate[24],
	carOwner[MAX_PLAYER_NAME],
	carOwnerID,
	carOwnerPlayer,
	carAlarm,
	carLock,
	carImmob,
	carInsurance,
	carDestroyed,
	Impounded,
	carDeath,
	carGps,
	Float:carFuel,
	carRegistered,
	Float:carBattery,

	bool:ePhysicalAttack,
	eDoorHealth,
	eDoorEffect,
	bool:vCooldown,
	Text3D:eVehicleLabel,
	
	/*trunkInv[MAX_TRUNK_INVENTORY],
	trunkInvQ[MAX_TRUNK_INVENTORY],
	trunkInvEx[MAX_TRUNK_INVENTORY],
	trunkInvS[MAX_TRUNK_INVENTORY],
	Float:trunkInvQFloat[MAX_TRUNK_INVENTORY],
	trunkInvName[MAX_TRUNK_INVENTORY],//[16],

	carInv[MAX_CAR_INVENTORY],
	carInvQ[MAX_CAR_INVENTORY],
	carInvEx[MAX_CAR_INVENTORY],
	carInvS[MAX_CAR_INVENTORY],
	Float:carInvQFloat[MAX_CAR_INVENTORY],
	carInvName[MAX_CAR_INVENTORY],//[16],*/

	carComponent[14],
	carPaintJob,
	carRadio,
	carRadioOn,
	carRadioURL[156],
	carTrunkLock,
	carHoodLock,
	carComps,
	carOn,
	carAttach[MAX_TRUNK_INVENTORY],
	carAttachObj[MAX_TRUNK_INVENTORY],
	Float:carAttachPosX[MAX_TRUNK_INVENTORY],
	Float:carAttachPosY[MAX_TRUNK_INVENTORY],
	Float:carAttachPosZ[MAX_TRUNK_INVENTORY],
	Float:carAttachRotX[MAX_TRUNK_INVENTORY],
	Float:carAttachRotY[MAX_TRUNK_INVENTORY],
	Float:carAttachRotZ[MAX_TRUNK_INVENTORY],
	carAttach3DtextUse,
	Float:carEngine,
	Float:carMilliage,
	Float:carArmour,
	carType,
	carFaction,
	carInt,
	carVW,
	carObjectiveTime,
	carLocked,
	carEngineOn,
	carAlarmTime,
	carSiren[5],
	carAdmin,
	carDriver,
	carRent,
	carTerminate,
	carHidedPlate,
	carFakePlate,
	carFakePlateS[24],

	carWanted,

	vCorp,
	
	carCargoObj[6],
	carCargoAmount[MAX_TRUCK_PRODUCT]
}
new VehicleInfo[MAX_VEHICLES][OWNED_CARS_ENUM];
new VehicleAlertNote[MAX_VEHICLES];

new default_VehicleInfo[OWNED_CARS_ENUM] = {0, ...};

enum CARSIGN_ENUM
{
	Model,
	Float:positionX,
	Float:positionY,
	Float:positionZ
}
new CarSignPosition[][CARSIGN_ENUM] =
{
	{461, 0.3, -1.0, -0.3},
	{462, 0.3, -1.0, -0.3},
	{525, 1.167443, -3.043605, 0.180004},
	{543, 1.030224, -2.722537, -0.200004},
	{473, 1.193565, -1.414345, 0.420029},
	{463, 0.3, -1.0, -0.3},
	{440, 1.002595, -2.681906, -0.459998},
	{600, 1.091160, -2.746158, -0.120002},
	{491, 0.953455, -2.979441, -0.239999},
	{410, 0.955007, -2.228608, -0.080001},
	{516, 1.027337, -2.962487, -0.170002},
	{418, 1.103214, -2.570087, -0.464830},
	{586, 0.3, -1.0, -0.3},
	{467, 1.028320, -2.999034, -0.214818},
	{547, 1.020201, -2.584710, -0.120000},
	{401, 1.054817, -2.480719, -0.140003},
	{549, 1.082301, -2.583779, -0.080000},
	{436, 0.916299, -2.549531, -0.050001},
	{420, 1.0, -2.9, -0.3},
	{517, 1.034572, -2.839301, -0.210004},
	{589, 0.992253, -2.355031, 0.000000},
	{518, 0.900428, -2.861342, -0.260004},
	{527, 0.982744, -2.393449, -0.110004},
	{551, 1.061103, -3.165968, -0.150003},
	{439, 1.006824, -2.595001, -0.270002},
	{445, 1.021359, -2.832397, -0.279996},
	{478, 1.052814, -2.609067, -0.380001},
	{492, 0.978229, -2.940849, -0.180002},
	{422, 0.983972, -2.524313, -0.420005},
	{529, 1.115404, -2.667127, -0.040000},
	{405, 0.980890, -2.784532, -0.210003},
	{466, 1.045667, -2.966061, -0.260004},
	{546, 1.069141, -2.793050, -0.070000},
	{540, 1.091474, -2.847417, -0.269996},
	{474, 1.069592, -2.866971, -0.390014},
	{526, 0.983719, -2.478292, -0.192113},
	{500, 0.890960, -2.004438, -0.320005},
	{483, 0.853677, -2.782062, -0.690004},
	{545, 0.928996, -2.174747, -0.480005},
	{419, 0.894909, -2.980162, -0.230004},
	{561, 0.979001, -2.623326, -0.240000},
	{459, 1.053846, -2.747935, -0.310001},
	{421, 0.904305, -3.054692, -0.350002},
	{458, 1.028704, -2.880819, -0.281234},
	{550, 1.069714, -2.771331, -0.190003},
	{576, 1.106926, -3.187511, -0.189998},
	{412, 1.076371, -3.572624, -0.280000},
	{438, 1.156400, -2.742904, -0.490127},
	{533, 0.974355, -2.592291, -0.180003},
	{534, 0.902085, -2.936193, -0.235802},
	{585, 1.040636, -3.108255, 0.000011},
	{507, 1.120712, -3.145817, -0.244607},
	{554, 1.174269, -2.955613, -0.229997},
	{567, 1.151497, -2.914819, -0.323813},
	{536, 0.993514, -3.115452, -0.260004},
	{599, 1.156144, -2.711604, -0.280002},
	{575, 0.988381, -2.790904, -0.090000},
	{490, 1.160361, -3.164604, -0.280003},
	{479, 0.989108, -2.917149, -0.140010},
	{489, 1.156144, -2.711604, -0.280002},
	{505, 1.156144, -2.711604, -0.280002},
	{431, 1.333595, -5.849972, -0.349942},
	{528, 1.144452, -2.726179, -0.440945},
	{523, 0.3, -1.0, -0.3},
	{581, 0.3, -1.0, -0.3},
	{596, 1.0, -2.9, -0.3},
	{597, 1.0, -2.9, -0.3},
	{598, 1.0, -2.9, -0.3},
	{542, 0.906872, -2.986380, 0.060001},
	{475, 0.950730, -2.742572, -0.130002},
	{413, 1.046246, -2.721698, -0.320005},
	{496, 0.977342, -2.171343, -0.080001},
	{535, 1.130306, -2.585677, -0.260024},
	{508, 1.315136, -3.968503, -0.799797},
	{426, 1.0, -2.9, -0.3},
	{404, 0.904094, -2.716050, -0.130002},
	{562, 0.932509, -2.307183, 0.000000},
	{580, 1.111744, -2.988797, -0.220002},
	{427, 1.165557, -3.895762, -0.450003},
	{498, 1.212809, -3.118173, -0.430007},
	{482, 1.020809, -2.521172, -0.500000},
	{566, 1.010413, -3.070576, -0.210004},
	{587, 1.092979, -2.675873, -0.070001},
	{423, 1.077004, -2.316038, -0.530009},
	{588, 1.292603, -4.102272, -0.280002},
	{559, 1.018711, -2.410182, -0.030000},
	{579, 1.126602, -2.781873, -0.121123},
	{400, 0.969509, -2.338449, -0.340002},
	{453, 1.779353, -4.521831, 0.782691},
	{402, 0.962478, -2.753054, -0.110001},
	{558, 0.952415, -2.535459, 0.080000},
	{480, 0.920290, -2.553400, -0.200003},
	{408, 1.345347, -3.027748, -0.510008},
	{602, 1.057301, -2.615010, -0.160005},
	{565, 0.836606, -2.065968, -0.050016},
	{555, 0.776128, -2.399894, -0.186654},
	{484, 1.914409, -6.640307, 0.918600},
	{430, 1.741363, -3.972848, 0.732989},
	{409, 0.922155, -3.940322, -0.160002},
	{560, 0.976966, -2.092453, -0.149999},
	{454, 2.659766, -4.882382, 0.413978},
	{603, 0.995825, -2.742580, -0.250000},
	{415, 1.000831, -2.276099, -0.170001},
	{446, 1.756320, -4.015568, 0.610863},
	{563, 1.339340, -1.450655, -0.110008},
	{497, 0.870477, -1.143729, 0.000000},
	{417, 0.810983, -4.166908, 0.830009},
	{487, 0.870477, -1.143729, 0.000000},
	{593, 0.493165, -2.364330, -0.396965},
	{509, 0.3, -1.0, -0.3},
	{510, 0.3, -1.0, -0.3},
	{481, 0.3, -1.0, -0.3},
	{477, 1.076897, -2.785017, -0.060000},
	{468, 0.3, -1.0, -0.3},
	{521, 0.3, -1.0, -0.3},
	{522, 0.3, -1.0, -0.3},
	{471, 0.3, -1.0, -0.3},
	{424, 0.931385, -1.469463, 0.310007},
	{541, 1.0, -2.3, -0.3},
	{429, 0.963224, -2.496615, -0.140003},
	{506, 0.955852, -2.359090, -0.229991},
	{448, 0.3, -1.0, -0.3},
	{601, 1.299075, -3.126926, 0.790003},
	{176, 1.333595, -5.849972, -0.349942},
	{416, 1.260592, -3.708050, -0.590007},
	{544, 1.096374, -2.750413, -0.619934},
	{552, 1.274269, -2.964355, 0.000000},
	{407, 1.311471, -3.237637, -0.580261}
};

enum INSURANCE_ENUM
{
	insuranceModel,
	insurancePrice
}

new InsurancesPrice[][INSURANCE_ENUM] =
{
	{469, 4000},
   	{487, 6000},
   	{460, 7500},
   	{512, 4500},
   	{519, 9000},
   	{593, 7000},
   	{446, 1900},
   	{452, 1500},
   	{453, 900},
   	{454, 3000},
   	{473, 600},
   	{484, 2100},
   	{493, 2500},
   	{461, 900},
   	{462, 300},
   	{463, 650},
   	{468, 900},
   	{471, 900},
   	{521, 1100},
   	{522, 1500},
   	{581, 800},
   	{586, 700},
   	{409, 2000},
   	{421, 1200},
   	{426, 1200},
   	{434, 1500},
   	{480, 1500},
   	{507, 1350},
   	{533, 1350},
   	{545, 1350},
   	{551, 1100},
   	{555, 1500},
   	{560, 1900},
   	{580, 1700},
   	{401, 700},
   	{405, 900},
   	{410, 600},
   	{419, 650},
   	{436, 600},
   	{439, 700},
   	{445, 900},
   	{458, 700},
   	{466, 650},
   	{467, 650},
   	{482, 900},
   	{491, 800},
   	{492, 900},
   	{496, 900},
   	{516, 900},
	{517, 900},
   	{518, 700},
   	{526, 700},
   	{527, 600},
   	{529, 900},
   	{534, 750},
	{535, 900},
   	{536, 900},
   	{540, 900},
   	{546, 750},
   	{547, 600},
   	{550, 900},
   	{561, 900},
   	{566, 900},
   	{567, 900},
	{575, 900},
   	{585, 900},
   	{589, 900},
   	{404, 400},
   	{412, 450},
   	{418, 450},
   	{474, 450},
   	{475, 450},
   	{479, 450},
   	{542, 400},
   	{543, 400},
   	{549, 400},
   	{576, 450},
   	{600, 450},
   	{402, 2800},
   	{411, 4300},
   	{415, 4300},
   	{429, 3800},
   	{451, 4300},
   	{477, 3500},
   	{506, 3800},
   	{541, 3800},
   	{558, 2500},
   	{559, 3200},
   	{562, 3500},
   	{565, 2500},
   	{587, 2500},
   	{602, 2500},
   	{603, 2800},
   	{400, 950},
   	{470, 1800},
   	{489, 900},
   	{490, 2200},
   	{495, 1500},
   	{500, 900},
   	{554, 800},
   	{579, 1100},
   	{406, 1500},
   	{408, 1000},
   	{413, 800},
   	{414, 850},
   	{420, 700},
   	{422, 600},
   	{423, 700},
   	{431, 1000},
   	{437, 1000},
   	{438, 500},
   	{440, 800},
   	{455, 1500},
   	{456, 900},
   	{478, 600},
   	{498, 850},
   	{499, 900},
   	{524, 1500},
   	{525, 500},
   	{531, 400},
	{552, 800},
 	{578, 1000},
   	{588, 700},
   	{424, 1500},
   	{442, 700},
   	{457, 350},
   	{483, 750},
   	{508, 900},
   	{568, 2000},
   	{572, 300}
};

enum COMP_ENUM
{
	compID,
	compPrice,
}
new vehicle_component[8][29][COMP_ENUM] =
{
 	{//������
	    {1025, 1100},
		{1073, 1300},
		{1074, 1500},
		{1075, 1500},
		{1076, 1800},
		{1077, 1600},
		{1078, 2100},
		{1079, 1400},
		{1080, 2400},
		{1081, 1600},
		{1082, 1900},
		{1083, 1900},
		{1084, 1800},
		{1085, 1600},
		{1096, 1700},
		{1097, 1700},
		{1098, 900},
		{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
		{0,0},{0,0}
	},
	{//��������
	    {1000, 800},
		{1001, 980},
		{1002, 890},
		{1003, 940},
		{1014, 950},
		{1015, 930},
		{1016, 850},
		{1023, 900},
		{1049, 980},
		{1050, 800},
		{1058, 950},
		{1060, 780},
		{1138, 1450},
		{1139, 1200},
		{1146, 1000},
		{1147, 1450},
		{1158, 1300},
		{1162, 1200},
		{1163, 1100},
		{1164, 1300},
		{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
	},
	{//����������������
        {1004, 900},
		{1005, 750},
		{1011, 800},
		{1012, 850},
		{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
		{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
		{0,0},{0,0},{0,0},{0,0},{0,0}
	},
	{//�����
	    {1006, 650},
		{1032, 890},
		{1033, 950},
		{1035, 950},
		{1038, 890},
		{1053, 850},
		{1054, 780},
		{1055, 780},
		{1061, 850},
		{1067, 890},
		{1068, 950},
		{1088, 780},
		{1091, 850},
		{1103, 1300},
		{1128, 1550},
		{1130, 1550},
		{1131, 1300},
		{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
		{0,0},{0,0}
	},
	{//������� ����
		{1017, 300},
		{1027, 700},
		{1030, 800},
		{1039, 980},
		{1040, 850},
		{1051, 800},
		{1052, 700},
		{1062, 770},
		{1063, 750},
		{1071, 900},
		{1072, 850},
		{1094, 800},
		{1099, 500},
		{1093, 700},
		{1101, 850},
		{1102, 400},
		{1107, 400},
		{1120, 550},
		{1121, 800},
		{1124, 690},
		{1137, 400},
		{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
	},
	{//��������� �����
	    {1018, 800},
		{1019, 700},
		{1020, 580},
		{1021, 550},
		{1022, 550},
		{1028, 1000},
		{1029, 850},
		{1034, 1000},
		{1037, 850},
		{1043, 750},
		{1044, 650},
		{1045, 800},
		{1046, 900},
		{1059, 850},
		{1064, 900},
		{1065, 850},
		{1066, 1000},
		{1089, 800},
		{1092, 900},
		{1104, 800},
		{1105, 700},
		{1113, 1000},
		{1114, 900},
		{1126, 700},
		{1127, 800},
		{1129, 650},
		{1132, 750},
		{1135, 800},
		{1136, 700}
	},
	{//�������� ������
		{1117, 2250},
		{1152, 2650},
		{1153, 2800},
		{1155, 2800},
		{1157, 2650},
		{1160, 3100},
		{1165, 2600},
		{1166, 2800},
		{1169, 3200},
		{1170, 2800},
		{1171, 3200},
		{1172, 2800},
		{1173, 2800},
		{1174, 2250},
		{1175, 2100},
		{1179, 2400},
		{1181, 2300},
		{1182, 2350},
		{1185, 2200},
		{1188, 2500},
		{1189, 2350},
		{1190, 2150},
		{1191, 2250},
		{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
	},
	{//������ ������
		{1141, 3000},
		{1140, 2900},
		{1148, 3000},
		{1149, 3100},
		{1150, 2650},
		{1151, 2800},
		{1154, 2650},
		{1156, 2800},
		{1159, 2900},
		{1161, 2850},
		{1167, 2750},
		{1168, 2700},
		{1176, 2200},
		{1177, 2250},
		{1178, 2300},
		{1180, 2350},
		{1183, 2150},
		{1184, 2200},
		{1186, 2300},
		{1187, 2200},
		{1192, 2100},
		{1193, 2200},
		{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
	}
};

IsNotAEngineCar(vehid)
{
    new pveh = GetVehicleModel(vehid);
    if (pveh >= 509 && pveh <= 510 || pveh == 571 || pveh == 462 || pveh == 481
	|| pveh == 430 || pveh == 446 || pveh == 452 || pveh == 453 || pveh == 454
	|| pveh == 472 || pveh == 473 || pveh == 484 || pveh == 493 || pveh == 595
	|| pveh == 417 || pveh == 425 || pveh == 430 || pveh == 446 || pveh == 452
	|| pveh == 453 || pveh == 454 || pveh == 460 || pveh == 469 || pveh == 472
	|| pveh == 473 || pveh == 476 || pveh == 484 || pveh == 487 || pveh == 488
	|| pveh == 493 || pveh == 497 || pveh == 511 || pveh == 512 || pveh == 513
	|| pveh == 519 || pveh == 520 || pveh == 512 || pveh == 548 || pveh == 481
	|| pveh == 553 || pveh == 577 || pveh == 592 || pveh == 593 || pveh == 595)
    {
        return true;
    }
	return false;
}