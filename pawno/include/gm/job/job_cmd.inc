CMD:pegartrabalho(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed])
		return 1;

	new i = Job_Nearest(playerid);
	if (i == -1) return 1;

	new title[32+22];
	format(title, sizeof(title), "%s", GetJobName(JobsInfo[i][jobType]));

	static const strdlg[] = "%s\nConseguir um emprego\nAjuda através de comandos\n	\n{FFC46A}Deixar o emprego";
	new mes[sizeof(strdlg) + 36];

	if (JobsInfo[i][jobType] == JOB_PIZZA) {
		format(mes, sizeof(mes), strdlg, "Pode ser combinado com outro trabalho");
		format(title, sizeof(title), "%s - Trabalho Secundário", title);
	} else {
		format(mes, sizeof(mes), strdlg, "Não pode ser combinado com outro trabalho");
		format(title, sizeof(title), "%s - Trabalho Principal", title);
	}

	Dialog_Show(playerid, TakeJobMenu, DIALOG_STYLE_TABLIST_HEADERS, title, mes, "Selecionar", "Cancelar");
	return 1;
}

CMD:abandonartrabalho(playerid, params[])
{
	new job = PlayerInfo[playerid][pJob];

	if (job == JOBLESS) return SendErrorMessage(playerid, "Você está desempregado.");
	if (PlayerInfo[playerid][pPremium] < 2 && PlayerInfo[playerid][pCareer] < 2) return SendErrorMessage(playerid, "Você só pode deixar seu emprego duas horas após conseguir um emprego.");
	if (pTemp[playerid][pJob]) return SendErrorMessage(playerid, "Termine o trabalho primeiro, entregue o veículo de trabalho. (/unrcj)");

	PlayerInfo[playerid][pJob] = 0;
	PlayerInfo[playerid][pCareer] = 0;

	pTemp[playerid][pJob] = 0;
	pTemp[playerid][pJobTime] = 0;

	SQL_SetInteger("users", "job", 0, PlayerInfo[playerid][pID]);
	SQL_SetInteger("users", "career", 0, PlayerInfo[playerid][pID]);

	SendSyntaxMessage(playerid, "Você deixou o emprego %s%s", GetJobName(job), (job == JOB_TRUCKER)?(". Seu rank foi resetado."):("."));
	return 1;
}


CMD:sairempregosecundario(playerid, params[])
{
	new job = PlayerInfo[playerid][pSideJob];

	if (job == JOBLESS) return SendErrorMessage(playerid, "Você não tem emprego secundário.");

	SendSyntaxMessage(playerid, "Você deixou o emprego %s.", GetJobName(job));
	PlayerInfo[playerid][pSideJob] = 0;
	return 1;
}

CMD:ajudatrabalho(playerid, params[])
{
	if (PlayerInfo[playerid][pJob] == JOBLESS) return SendErrorMessage(playerid, "Você está desempregado.");

	new mes[512];
	SendSyntaxMessage(playerid, "[VEÍCULO] Pegue um veículo de trabalho - /vtrabalho, devolva usando - /devolvervtrabalho(/dvt).");

	if (PlayerInfo[playerid][pSideJob] == JOB_PIZZA) SendSyntaxMessage(playerid, "[EMPREGO DE ENTREGA DE PIZZA] Entregue pizzas - /pizzajob. Digite /quitsidejob para sair.");

	if (PlayerInfo[playerid][pJob] == JOB_CARJACKER)
	{
		static const str_size[] = "{ffffff}Você está trabalhando como ladrão de carros. Horário de funcionamento: %i.\n\n/entregarcarro - entrega veículos roubado ao desmanche.\n /dropoff - se livrar do veículo.\n /leavemission - redefinir a missão.\n\n /vbl (n) oisy - hackeamento grosseiro de veículos.\n /vbl (q) uiet - hackeamento silencioso de veículos\n\n (/en) gine - iniciar a quebra do motor\n (/uns) cramble - descriptografar a palavra (quando quebrando o motor) ";
		format(mes, sizeof(mes), str_size, PlayerInfo[playerid][pCareer]);
	}

	else if (PlayerInfo[playerid][pJob] == JOB_TAXI)
	{
		static const str_size[] = "{ffffff}Você está trabalhando como motorista de táxi. Horário de funcionamento: %i.\n\n/taxi duty - iniciar /parar o trabalho.\n /taxi taxas - alterar a tarifa.\n /taxi aceitar - aceitar a chamada.\n /endereco - marcar o endereço no mapa\n /rua - descobrir a rua em que você está. ";

		format(mes, sizeof(mes), str_size, PlayerInfo[playerid][pCareer]);
	}
	if (PlayerInfo[playerid][pJob] == JOB_MECHANIC)
	{
		static const str_size[] = "{ffffff}Você está trabalhando como mecânico. Horário de funcionamento: %i.\n\n/refill - abastecer o veículo.\n /repair - reparar o veículo.\n /paintcar - pintar o veículo.\n /buycomp - comprar componentes.\n /mycomp - descobrir o número de componentes no veículo.\n /attach - rebocar o veículo. ";
		format(mes, sizeof(mes), str_size, PlayerInfo[playerid][pCareer]);
	}

	else if (PlayerInfo[playerid][pJob] == JOB_TRUCKER)
	{
		format(mes, sizeof(mes), "{ffffff}Você está trabalhando como caminhoneiro. Horário de funcionamento: %i.\n\n/cargo - trabalhe com a carga.\n /trailer - controle o reboque.\n /tpda - o menu do caminhoneiro.\n /industry - informações sobre a empresa.\n\n", PlayerInfo[playerid][pCareer]);
        switch(PlayerInfo[playerid][pCareer])
		{
		    case 0..11: strcat(mes, "Veículos disponíveis: Picador, Sadler, Bobcat, Walton, Yosemite.");
			case 12..23: strcat(mes, "Veículos disponíveis: Picador, Sadler, Bobcat, Walton, Yosemite, Pony, Berkley's RC Van, Burrito.");
		    case 24..31: strcat(mes, "Veículos disponíveis: Picador, Sadler, Bobcat, Walton, Yosemite, Pony, Berkley's RC Van, Burrito, Rumpo, Boxville.");
		    case 32..47:
			{
				strcat(mes, "Veículos disponíveis: Picador, Sadler, Bobcat, Walton, Yosemite, Pony, Berkley's RC Van, Burrito, Rumpo, Boxville.\n");
                strcat(mes, "Benson, Mule, DFT-30, Tanker, Securicar.");
			}
			case 48..58:
			{
				strcat(mes, "Veículos disponíveis: Picador, Sadler, Bobcat, Walton, Yosemite, Pony, Berkley's RC Van, Burrito, Rumpo, Boxville.\n");
                strcat(mes, "Benson, Mule, DFT-30, Tanker, Securicar, Yankee, Flatbed.");
			}
			default:
			{
				strcat(mes, "Veículos disponíveis: Picador, Sadler, Bobcat, Walton, Yosemite, Pony, Berkley's RC Van, Burrito.\n");
                strcat(mes, "Benson, Mule, DFT-30, Tanker, Securicar, Yankee, Flatbed, Ônibus.");
			}
		}
	}

	Dialog_Show(playerid, None, DIALOG_STYLE_MSGBOX, "{FFFFFF}Comandos de Trabalho", mes, "Retornar", "Fechar");

	return 1;
}


ALTX:rentcarjob("/rcj","/vtrabalho");
CMD:rentcarjob(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed]) return 1;
	
	if (pTemp[playerid][pJob])					return SendErrorMessage(playerid, "Você já pegou um veículo de trabalho! Digite /unrentcarjob(/unrcj) para recuperá-lo. ");
	if (!IsPlayerInAnyVehicle(playerid))			return SendErrorMessage(playerid, "Você deve estar em um veículo de trabalho! ");
	
	new vehicle = GetPlayerVehicleID(playerid);

	if (!IsPizzaCar(vehicle) && PlayerInfo[playerid][pJob] == JOBLESS) return SendErrorMessage(playerid,"Você está desempregado.");
	if (GetPlayerState(playerid) != PLAYER_STATE_DRIVER)		  		  return SendErrorMessage(playerid, "Você deve estar dirigindo um veículo.");

	if (VehicleInfo[vehicle][carRent])			return SendErrorMessage(playerid, "Este veículo já está sendo usado por alguém.");
	if (pTemp[playerid][pRentveh])				return SendErrorMessage(playerid, "Você precisa cancelar o aluguel do veículo/unrentcarjob(/unrcj) !");

	if (IsTruckCar(vehicle) && PlayerInfo[playerid][pJob] == JOB_TRUCKER && GetAvailableTruck(playerid, GetVehicleModel(vehicle)))
	{
		pTemp[playerid][pJob] = 1;
		pTemp[playerid][pJobTime] = gettime();
	}
	else if (IsTaxiCar(vehicle) && PlayerInfo[playerid][pJob] == JOB_TAXI) {
		pTemp[playerid][pJob] = 1;
		pTemp[playerid][pJobTime] = gettime();
	}
	else if (IsMechanicCar(vehicle) && PlayerInfo[playerid][pJob] == JOB_MECHANIC) {
		pTemp[playerid][pJob] = 1;
	}
	else if (IsTrashMasterCar(vehicle) && PlayerInfo[playerid][pJob] == JOB_TRASHMASTER) {
		pTemp[playerid][pJob] = 1;
		Trash_InitPlayer(playerid);
	}
	else if (IsPizzaCar(vehicle) && PlayerInfo[playerid][pSideJob] == JOB_PIZZA) {
		pTemp[playerid][pJob] = 1;
	}
	else {
		SendErrorMessage(playerid, "Tipo de veículo inválido!");
		if (PlayerInfo[playerid][pJob] == JOB_TRUCKER) SendSyntaxMessage(playerid, "Você pode descobrir o veículo disponível para você usando /jobhelp !");
		return 1;
	}

	pTemp[playerid][pRentveh] = vehicle;
	VehicleInfo[vehicle][carRent] = PlayerInfo[playerid][pID];

	SendHelpMessage(playerid, "Digite /jobhelp para ver a lista de comandos disponíveis. ");
	SendSyntaxMessage(playerid, "Você pegou o veículo de trabalho %s. Comandos disponíveis: /unrentcarjob(/unrcj), /lock.", VehicleNames[GetVehicleModel(vehicle)-400]);
	SendSyntaxMessage(playerid, "Atenção! Se você não usar o veículo por 10 minutos, ele desaparecerá. ");

	return 1;
}

ALTX:unrentcarjob("/unrcj","/devolvervtrabalho","/dvt");
CMD:unrentcarjob(playerid, params[])
{
	if (!pTemp[playerid][pRentveh]) return 1;

    SendSyntaxMessage(playerid, "Você devolveu um veículo de trabalho.");
	EndJob(playerid);
	return 1;
}

EndJob(playerid) {
	new vehicle = pTemp[playerid][pRentveh];
	
	if (PlayerInfo[playerid][pJob] == JOB_TRASHMASTER && pTemp[playerid][pJob])
	{
		Trash_ResetPlayer(playerid, 1);
		EndTrashJob(playerid);
	}
	
	pTemp[playerid][pRentveh] = 0;
    VehicleInfo[vehicle][carRent] = 0;

	pTemp[playerid][pJob] = 0;
	pTemp[playerid][pJobTime] = 0;

    RespawnVehicle(vehicle);
	return 1;
}

CMD:comprardrogas(playerid, params[])
{
    new faction = PlayerInfo[playerid][pFaction];
    
    if (!PlayerInfo[playerid][pFaction]) return SendErrorMessage(playerid, "Você não é membro de uma facção oficial.");
    if (PlayerInfo[playerid][pSideJob] != JOB_DRUGDEALER) return SendErrorMessage(playerid, "Você não tem acesso à compra de drogas.");
    if (Factions[faction][fType] != FACTION_GANG && Factions[faction][fType] != FACTION_MAFIA) return 1;

    if (!PlayerToPoint(2.0, playerid, BLACKMARKET_X, BLACKMARKET_Y, BLACKMARKET_Z))
    {
        Waypoint_Set(playerid, BLACKMARKET_X, BLACKMARKET_Y, BLACKMARKET_Z);
        return SendErrorMessage(playerid, "Você deve estar no mercado negro.");
    }

    Dialog_Show(playerid, BuyDrugs, DIALOG_STYLE_TABLIST, "Comprar drogas", "Sementes de maconha\t50 $ por peça.\nLSD\t150 $ por grama.\nHeroina\t100 $ por grama.\nCocaína\t200 $ por grama.\nEfedrina\t50 $ por peça.", "Comprar", "Cancelar");
    return 1;
}


CMD:comprararma(playerid, params[])
{
    new bizid = -1;

    if ((bizid = GetNearInsideBiz(playerid)) != -1 && BizInfo[bizid][bizType] == BIZ_AMMO) return BuyGunMenu(playerid);

    new faction = PlayerInfo[playerid][pFaction];

    if (!PlayerInfo[playerid][pFaction]) return SendErrorMessage(playerid, "Você não é membro de uma facção oficial.");
    if (PlayerInfo[playerid][pSideJob] != JOB_GUNDEALER) return SendErrorMessage(playerid, "Você não tem acesso à compra de armas.");
    if (Factions[faction][fType] != FACTION_GANG && Factions[faction][fType] != FACTION_MAFIA) return 1;

    if (!PlayerToPoint(2.0, playerid, BLACKMARKET_X, BLACKMARKET_Y, BLACKMARKET_Z))
    {
        Waypoint_Set(playerid, BLACKMARKET_X, BLACKMARKET_Y, BLACKMARKET_Z);
        return SendErrorMessage(playerid, "Você deve estar no mercado negro.");
    }

    new title[56];
    format(title, sizeof(title), "No saldo da sua facção - %i de experiência.", Factions[faction][fPoint]);
    Dialog_Show(playerid, BuyGunMain, DIALOG_STYLE_LIST, title, "- Arma\n- Munição", "Selecionar", "Cancelar");
    return 1;
}


CMD:roubarradio(playerid, params[])
{
    new vehicle = GetPlayerVehicleID(playerid);

    if (!vehicle || !IsDriver(playerid)) return SendErrorMessage(playerid, "Você deve estar no banco do motorista.");
    if (!VehicleInfo[vehicle][carOwnerID]) return SendErrorMessage(playerid, "O rádio só pode ser roubado de veículos pessoais.");
    if (!VehicleInfo[vehicle][carRadio]) return SendErrorMessage(playerid, "Este veículo não tem rádio.");
    if (IsPlayerHasItem(playerid, INV_CARSOUND)) return SendErrorMessage(playerid, "Você já possui um rádio em seu inventário.");
    if (GiveInvItem(playerid, INV_CARSOUND, 1) == -1) return SendErrorMessage(playerid, "Inventário cheio.");

    VehicleInfo[vehicle][carRadio] = 0;

    new mes[128];
    format(mes, sizeof(mes), "retira o rádio do(a) %s.", VehicleNames[GetVehicleModel(vehicle)-400]);
    cmd::me(playerid, mes);
    return 1;
}


CMD:trabalhofaccao(playerid, params[])
{
    if (!PlayerInfo[playerid][pFaction]) return SendErrorMessage(playerid, "Você deve ser membro de uma facção oficial de gangster ou máfia.");
    if (Factions[PlayerInfo[playerid][pFaction]][fType] != FACTION_GANG && Factions[PlayerInfo[playerid][pFaction]][fType] != FACTION_MAFIA) return SendErrorMessage(playerid, "Você deve estar em uma facção oficial de gangster ou máfia.");
    if (!isLeader(playerid)) return SendErrorMessage(playerid, "Este comando está disponível apenas para o líder da facção oficial.");

    new userid, level;

    if (sscanf(params, "ud", userid, level)) return SendHelpMessage(playerid, "/trabalhofaccao [ID/Parte do Nome] [0 - revogar acesso | 1 - acesso à compra de drogas | 2 - acesso à compra de armas] ");

    if (!IsPlayerLogged(userid)) return SendNotConnected(playerid);
    if (PlayerInfo[userid][pFaction] != PlayerInfo[playerid][pFaction]) return SendErrorMessage(playerid, "O jogador não é membro da sua facção.");
    if (level > 2 || level < 0) return SendErrorMessage(playerid, "O parâmetro deve estar entre 0 e 2.");

    if (!level)
    {
        if (PlayerInfo[userid][pSideJob] == 0) return SendErrorMessage(playerid, "O jogador não tinha acesso a nenhum trabalho.");

        SendSyntaxMessage(userid, "O líder da facção %s revogou seu acesso ao trabalho %s.", GetNameEx(playerid), GetJobName(PlayerInfo[userid][pSideJob]));
        SendSyntaxMessage(playerid, "Você revogou o acesso ao trabalho de %s %s.", GetNameEx(userid), GetJobName(PlayerInfo[userid][pSideJob]));
        PlayerInfo[userid][pSideJob] = 0;
        return 1;
    }

    PlayerInfo[userid][pSideJob] = level + 3;
    SendSyntaxMessage(userid, "O líder da facção %s concedeu a você acesso ao trabalho %s.", GetNameEx(playerid), GetJobName(PlayerInfo[userid][pSideJob]));
    SendSyntaxMessage(playerid, "Você concedeu acesso a %s ao trabalho %s.", GetNameEx(userid), GetJobName(PlayerInfo[userid][pSideJob]));
    return 1;
}
