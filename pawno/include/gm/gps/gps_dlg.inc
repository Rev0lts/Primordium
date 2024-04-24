Dialog:DIALOG_GPS(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	switch(listitem) {
		case 0: { // localizações
			new substr[48],
                str[sizeof(substr)*20 + 20] = "Local\t Distância";

			for(new i; i < MAX_ENTRANCE; i++) {
				if (isempty(EntranceInfo[i][gpsText]) || !EntranceInfo[i][entranceOn]) continue;

				format(substr, sizeof(substr), "%.32s\t{33CC33}%i m", EntranceInfo[i][gpsText], GetPlayerDistanceToPointEx(playerid, EntranceInfo[i][eEnterX], EntranceInfo[i][eEnterY], EntranceInfo[i][eEnterZ]));
                format(str, sizeof(str), "%s\n%s", str, substr);
            }
			
            if (isempty(substr)) format(str, sizeof(str), "%s\n{FF6347}NÃO ENCONTRADO", str);

			Dialog_Show(playerid, DIALOG_GPS_LOCATION, DIALOG_STYLE_TABLIST_HEADERS, "GPS > Locais", str, "Selecionar", "Voltar");
		}
		case 1: { // negócios
			new substr[80],
                str[sizeof(substr)*MAX_BIZ_TYPES+30] = "Tipo\tNome\t Distância", 
				id, 
				Float:radius;

			for(new i; i < MAX_BIZ_TYPES; i++) {
				if ((id = GetClosestTypeBiz(playerid, i, radius)) == -1)	{
					format(substr, sizeof(substr), "%s\t{FF6347}NÃO ENCONTRADO", biz_types[i]);
				} else {
					format(substr, sizeof(substr), "%s\t%s\t{33CC33}%.0f m", biz_types[i], BizInfo[id][bizName], radius);
				}
                format(str, sizeof(str), "%s\n%s", str, substr);
			}

            if (isempty(substr)) format(str, sizeof(str), "%s\n{FF6347}NÃO ENCONTRADO", str);

			Dialog_Show(playerid, DIALOG_GPS_BIZ, DIALOG_STYLE_TABLIST_HEADERS, "GPS > Negócios", str, "Selecionar", "Voltar");
		}
		case 2: { // Trabalhos
			new substr[64],
                str[sizeof(substr)*MAX_BIZ_TYPES+20] = "Trabalho\tDistância", 
				Float:radius;

			for(new i = 1; i < MAX_JOBS_TYPES; i++) {
				if (GetClosestTypeJob(playerid, i, radius) == -1) {
					format(substr, sizeof(substr), "%s\t{FF6347}NÃO ENCONTRADO", jobs_names[i]);
				} else {
					format(substr, sizeof(substr), "%s\t{33CC33}%.0f m", jobs_names[i], radius);
				}
                format(str, sizeof(str), "%s\n%s", str, substr);
			}

            if (isempty(substr)) format(str, sizeof(str), "%s\n{FF6347}NÃO ENCONTRADO", str);

			Dialog_Show(playerid, DIALOG_GPS_JOBS, DIALOG_STYLE_TABLIST_HEADERS, "GPS > Trabalhos", str, "Selecionar", "Voltar");
		} 
	}
	return 1;
}


Dialog:DIALOG_GPS_LOCATION(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::gps(playerid, "");

	new id;
	for(new i; i < MAX_ENTRANCE; i++) {
		if (isempty(EntranceInfo[i][gpsText])) continue;
		
		if (listitem == id) {
			id = i;
			break;
		} else id++;
	}

	Waypoint_Set(playerid, EntranceInfo[id][eEnterX], EntranceInfo[id][eEnterY], EntranceInfo[id][eEnterZ]);
	SendClientMessageEx(playerid, COLOR_GREEN, "Localização '%s' marcada com sucesso no seu mapa. (/rcp - remover checkpoint)", EntranceInfo[id][gpsText]);
	PlayerPlaySound(playerid, 21001, 0.0, 0.0, 0.0);
	return 1;
}

Dialog:DIALOG_GPS_BIZ(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::gps(playerid, "");

	new id, Float:radius;
	if ((id = GetClosestTypeBiz(playerid, listitem, radius)) == -1) return cmd::gps(playerid, "");

	Waypoint_Set(playerid, BizInfo[id][bizPosX], BizInfo[id][bizPosY], BizInfo[id][bizPosZ]);
	SendClientMessageEx(playerid, COLOR_GREEN, "[%s] %s marcado com sucesso no seu mapa. (/rcp - remover checkpoint)", biz_types[listitem], BizInfo[id][bizName]);
	PlayerPlaySound(playerid, 21001, 0.0, 0.0, 0.0);
	return 1;
}

Dialog:DIALOG_GPS_JOBS(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::gps(playerid, "");
	
	listitem++;

	new id, Float:radius;
	if ((id = GetClosestTypeJob(playerid, listitem, radius)) == -1) return cmd::gps(playerid, "");

	Waypoint_Set(playerid, JobsInfo[id][jobX], JobsInfo[id][jobY], JobsInfo[id][jobZ]);
	SendClientMessageEx(playerid, COLOR_GREEN, "Trabalho '%s' marcado com sucesso no seu mapa. (/rcp - remover checkpoint)", jobs_names[listitem]);
	PlayerPlaySound(playerid, 21001, 0.0, 0.0, 0.0);
	return 1;
}