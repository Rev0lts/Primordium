//LIST OF VEHICLE SELLS
new a_class_model[] = { //4-х дверные и Роскошные машины
	445,604,507,585,466,492,546,
	551,516,467,426,547,405,580,
	409,550,566,540,421,529,402,
	603
};

new b_class_model[] = { //2-х дверные и Компактные машины
	475,542,602,496,401,518,527,
	589,419,587,533,526,474,545,
	517,410,600,436,439,549,491
};

new lowride_class_model[] = { // Лоурайдеры
	536, 575, 534, 567, 535, 576, 412
};

new pickups_class_model[] = { //Пикапы и Фургоны
	459,422,482,605,/*530,*/418,/*572,*/
	582,413,440,543,/*583,*/478,554
};

new offroad_model[] = //Внедорожники и Универсалы
	{579, 400, 404, 489, 479, 442, 458};
	
new sport_model[] = { // Спортивные
	429, 415, 480, 562, 565, 559, 561,
	560, 506, 558, 555, 602, 603
};

new industrial_model[] = 
	{408, 413, 414, 420, 
	423, 431, 437, 438, 
	455, 456, 498, 499, 
	524, 525, 531, 552, 572, 
	578, 588, 403, 514, 515};

new motobike_model[] = 
	{461, 462, 463, 468, 471, 521, 581, 
	586, 481, 509, 510};
	
new boat_model[] =
	{446, 453, 454, 473, 484, 493};

new fly_model[] =
	{469, 487, 460, 512, 519, 593};

new other_model[] =
	{424, 457, 483, 508, 500};

new donate_model[] = 
	{568, 495, 541, 411, 522, 490, 
	470, 434, 451, 477};


//OTHER
enum DEALERSHIP
{
	cCarModel,
	cCarPrice,
	Float:cCarCompFuel,
	Float:cCarFuelUsage
}

// model, price, fuel capacity, fuel usage
//static const 
new Float:vehicles_info[][DEALERSHIP] = {
	{409, 95000, 100.0, 6.0},// Stretch					//A-Class 
	{421, 40000, 80.0, 6.0}, // Washington
	{426, 32000, 70.0, 5.0}, // Premier
	{434, 50000, 70.0, 8.0}, // Hotknife
	{480, 40000, 60.0, 5.0}, // Comet
	{507, 37500, 85.0, 6.0}, // Elegant
	{533, 27500, 60.0, 7.0}, // Feltzer
	{545, 45000, 60.0, 6.0}, // Hustler
	{551, 23000, 80.0, 5.0}, // Merit
	{555, 38500, 80.0, 6.0}, // Windsor
	{560, 70000, 60.0, 9.0}, // Sultan
	{580, 120000, 80.0, 5.0}, // Stafford  					//A-Class
	{401, 10000, 60.0, 5.0}, // Bravura						//B-Class
	{405, 40000, 80.0, 5.0}, // Sentinel
	{410, 8000, 50.0, 5.0}, // Manana
	{419, 20000, 60.0, 5.0}, // Esperanto
	{436, 12000, 50.0, 5.0}, // Previon
	{439, 19000, 60.0, 5.0}, // Stallion
	{445, 40000, 80.0, 5.0}, // Admiral
	{458, 32500, 80.0, 5.0}, // Solair
	{466, 20000, 70.0, 6.0}, // Glendale
	{467, 22000, 70.0, 5.0}, // Oceanic
	{482, 30000, 130.0, 8.0}, // Burrito
	{491, 17000, 80.0, 6.0}, // Virgo
	{492, 17000, 70.0, 6.0}, // Greenwood
	{496, 27000, 80.0, 5.0}, // Blista Compact
	{516, 19000, 70.0, 5.0}, // Nebula
	{517, 14000, 70.0, 5.0}, // Majestic
	{518, 28000, 60.0, 5.0}, // Buccaneer
	{526, 35000, 70.0, 5.0}, // Fortune
	{527, 21000, 60.0, 5.0}, // Cadrona
	{529, 15000, 80.0, 6.0}, // Willard
	{534, 25000, 60.0, 5.0}, // Remington
	{535, 40000, 80.0, 8.0}, // Slamvan
	{536, 25000, 60.0, 5.0}, // Blade
	{540, 17000, 70.0, 5.0}, // Vincent
	{546, 16000, 60.0, 5.0}, // Intruder
	{547, 18000, 70.0, 5.0}, // Primo
	{550, 23000, 80.0, 5.0}, // Sunrise
	{561, 23000, 60.0, 8.0}, // Stratum
	{566, 20000, 80.0, 5.0}, // Tahoma
	{567, 23000, 60.0, 5.0}, // Savanna
	{575, 22000, 60.0, 5.0}, // Broadway
	{585, 20000, 80.0, 7.0}, // Emperor
	{589, 30000, 60.0, 6.0}, // Club 				//B-Class
	{404, 9000, 60.0, 5.0}, // Perennial				//С-Class
	{412, 17500, 60.0, 5.0}, // Voodoo
	{418, 16000, 90.0, 5.0}, // Moonbeam
	{474, 27000, 60.0, 5.0}, // Hermes
	{475, 25000, 80.0, 6.0}, // Sabre
	{479, 35000, 80.0, 5.0}, // Regina
	{542, 14300, 70.0, 5.0}, // Clover
	{543, 7500, 90.0, 5.0}, // Sadler
	{549, 15000, 80.0, 5.0}, // Tampa
	{576, 17500, 65.0, 5.0}, // Tornado
	{600, 6500, 90.0, 5.0}, // Picador
	{605, 3000, 90.0, 5.0}, // Sadler Shit
	{604, 5000, 70.0, 5.0}, // Glendale Shit 			//С-Class
	{402, 122000, 60.0, 7.0}, // Buffalo 					//Sport
	{411, 150000, 60.0, 10.0}, // Infernus
	{429, 90000, 55.0, 9.0}, // Banshee
	{451, 100000, 60.0, 9.0}, // Turismo
	{477, 45000, 60.0, 8.0}, // ZR-350
	{506, 105000, 55.0, 8.0}, // Super GT
	{541, 85000, 50.0, 10.0}, // Bullet
	{558, 37000, 60.0, 5.0}, // Uranus
	{559, 32500, 60.0, 7.0}, // Jester
	{562, 35000, 60.0, 9.0}, // Elegy
	{565, 35000, 60.0, 6.0}, // Flash
	{587, 32000, 60.0, 7.0}, // Euros
	{602, 70000, 60.0, 8.0}, // Alpha
	{415, 95000, 60.0, 9.0}, // Cheetah
	{603, 34000, 60.0, 8.0}, // Phoenix 				//Sport
	{400, 32500, 100.0, 7.0}, // Landstalker 			//Off-Road
	{470, 48000, 90.0, 7.0}, // Patriot
	{489, 50000, 110.0, 7.0}, // Rancher
	{490, 60000, 110.0, 7.0}, // FBI Rancher
	{495, 60000, 110.0, 7.0}, // Sandking
	{500, 30000, 100.0, 6.0}, // Mesa
	{554, 40000, 100.0, 5.0}, // Yosemite
	{579, 60000, 100.0, 8.0}, // Huntley				//Off-Road
	{461, 66000, 50.0, 8.0}, // PCJ-600					//MotoBike
	{462, 2500, 50.0, 4.0}, // Faggio
	{463, 33000, 60.0, 6.0}, // Freeway
	{468, 17500, 50.0, 5.0}, // Sanchez
	{471, 5000, 40.0, 4.0}, // Quad
	{521, 77000, 55.0, 8.0}, // FCR-900
	{522, 88000, 50.0, 10.0}, // NRG-500
	{581, 66000, 50.0, 8.0}, // BF-400
	{586, 10000, 60.0, 5.0}, // Wayfarer
	{481, 1250, 0.0, 0.0}, // BMX
	{509, 500, 0.0, 0.0}, // Bike
	{510, 900, 0.0, 0.0}, // Mountain Bike				//MotoBike
	{423, 11000, 80.0, 5.0}, // Mr. Whoopee				//Industrial
	{408, 25000, 150.0, 8.0}, // Trashmaster
	{413, 25000, 130.0, 6.0}, // Pony
	{414, 20000, 100.0, 6.0}, // Mule
	{420, 35000, 70.0, 5.0}, // Taxi
	{422, 17500, 105.0, 6.0}, // Bobcat
	{431, 27000, 160.0, 9.0}, // Bus
	{437, 32500, 165.0, 8.0}, // Coach
	{438, 25000, 70.0, 5.0}, // Cabbie
	{440, 20000, 135.0, 7.0}, // Rumpo
	{459, 20000, 135.0, 7.0}, // Berkley's RC Van
	{455, 35000, 165.0, 8.0}, // Flatbed
	{456, 35000, 165.0, 8.0}, // Yankee
	{478, 10700, 105.0, 5.0}, // Walton
	{498, 19400, 165.0, 7.0}, // Boxville
	{499, 19900, 165.0, 7.0}, // Benson
	{524, 34800, 165.0, 8.0}, // Cement Truck
	{525, 15000, 60.0, 5.0}, // Towtruck
	{531, 5400, 60.0, 5.0}, // Tractor
	{552, 17000, 100.0, 6.0}, // Utility Van
	{572, 4900, 40.0, 5.0}, // Mower
	{578, 30000, 165.0, 7.0}, // DFT-30
	{406, 350000, 300.0, 20.0}, // Dumper
	{514, 48000, 170.0, 10.0}, // Tanker
	{515, 48000, 170.0, 10.0}, // Roadtrain
	{588, 6000, 60.0, 5.0}, // Hotdog			//Industrial
	{424, 45800, 60.0, 9.0}, // BF Injection	 	//Other
	{442, 11800, 90.0, 5.0}, // Romero
	{457, 8400, 30.0, 4.0}, // Caddy
	{483, 12500, 60.0, 6.0}, // Camper
	{508, 13800, 80.0, 8.0}, // Journey
	{568, 61500, 50.0, 9.0}, // Bandito			//Other
	{446, 34900, 170.0, 5.0}, // Squallo			//Boats
	{453, 10700, 200.0, 6.0}, // Reefer
	{454, 78500, 190.0, 8.0}, // Tropic
	{473, 5500, 150.0, 5.0}, // Dinghy
	{484, 39500, 170.0, 5.0}, // Marquis
	{493, 69900, 180.0, 10.0}, // Jetmax			//Boats
	{469, 200500, 210.0, 8.0}, // Sparrow			//Air vehicle
	{487, 300000, 230.0, 8.0}, // Maverick
	{460, 115000, 400.0, 10.0}, // Skimmer
	{512, 114900, 400.0, 10.0}, // Cropduster
	{519, 600000, 350.0, 9.0}, // Shamal
	{593, 350000, 260.0, 7.0} //Dodo 			//Air vehicle

	//Beagle
	//Stuntplane
	//Dune
	//=====
	//Kart
};
	
stock GetVehiclePrice(model)
{
	for(new i = 0; i < sizeof(vehicles_info); i++)
	{
		if (model == vehicles_info[i][cCarModel])
			return vehicles_info[i][cCarPrice];
	}
	return 0;
}

stock Float:GetVehicleFuelCapacity(model)
{
	for(new i = 0; i < sizeof(vehicles_info); i++)
	{
		if (model == vehicles_info[i][cCarModel])
			return vehicles_info[i][cCarCompFuel];
	}
	
	return 60.0;
}

stock Float:GetVehicleFuelUsage(model)
{
	for(new i = 0; i < sizeof(vehicles_info); i++)
	{
		if (model == vehicles_info[i][cCarModel])
			return vehicles_info[i][cCarFuelUsage];
	}
	return 6.0;
}

stock Float:GetVehicleMaxHealth(modelid)
{
	new Float:vvalue = 0.0;

	switch(modelid)
	{
		case 462, 463, 521, 522, 581, 586, 461: vvalue = 750.0;
		case 468, 471: vvalue = 700.0;
		case 481, 509, 510: vvalue = 1000.0;
		case 439, 480, 533, 555: vvalue = 650.0;
		case 549, 604, 605: vvalue = 600.0;
		default: vvalue = 750.0;
	}

	return vvalue;
}

stock IsADonateModel(model)
{
	for(new i = 0; i < sizeof(donate_model); i++)
		if (donate_model[i] == model)
			return true;
	return false;
}