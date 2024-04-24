CMD:pegartrabalho(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed])
		return 1;

	new i = Job_Nearest(playerid);
	if (i == -1) return 1;

	new title[32+22];
	format(title, sizeof(title), "%s", GetJobName(JobsInfo[i][jobType]));

	static const strdlg[] = "%s\nConseguir um emprego\nAjuda atrav�s de comandos\n	\n{FFC46A}Deixar o emprego";
	new mes[sizeof(strdlg) + 36];

	if (JobsInfo[i][jobType] == JOB_PIZZA) {
		format(mes, sizeof(mes), strdlg, "Pode ser combinado com outro trabalho");
		format(title, sizeof(title), "%s - Trabalho Secund�rio", title);
	} else {
		format(mes, sizeof(mes), strdlg, "N�o pode ser combinado com outro trabalho");
		format(title, sizeof(title), "%s - Trabalho Principal", title);
	}

	Dialog_Show(playerid, TakeJobMenu, DIALOG_STYLE_TABLIST_HEADERS, title, mes, "Selecionar", "Cancelar");
	return 1;
}

CMD:abandonartrabalho(playerid, params[])
{
	new job = PlayerInfo[playerid][pJob];

	if (job == JOBLESS) return SendErrorMessage(playerid, "Voc� est� desempregado.");
	if (PlayerInfo[playerid][pPremium] < 2 && PlayerInfo[playerid][pCareer] < 2) return SendErrorMessage(playerid, "Voc� s� pode deixar seu emprego duas horas ap�s conseguir um emprego.");
	if (pTemp[playerid][pJob]) return SendErrorMessage(playerid, "Termine o trabalho primeiro, entregue o ve�culo de trabalho. (/unrcj)");

	PlayerInfo[playerid][pJob] = 0;
	PlayerInfo[playerid][pCareer] = 0;

	pTemp[playerid][pJob] = 0;
	pTemp[playerid][pJobTime] = 0;

	SQL_SetInteger("users", "job", 0, PlayerInfo[playerid][pID]);
	SQL_SetInteger("users", "career", 0, PlayerInfo[playerid][pID]);

	SendSyntaxMessage(playerid, "Voc� deixou o emprego %s%s", GetJobName(job), (job == JOB_TRUCKER)?(". Seu rank foi resetado."):("."));
	return 1;
}


CMD:sairempregosecundario(playerid, params[])
{
	new job = PlayerInfo[playerid][pSideJob];

	if (job == JOBLESS) return SendErrorMessage(playerid, "Voc� n�o tem emprego secund�rio.");

	SendSyntaxMessage(playerid, "Voc� deixou o emprego %s.", GetJobName(job));
	PlayerInfo[playerid][pSideJob] = 0;
	return 1;
}

CMD:ajudatrabalho(playerid, params[])
{
	if (PlayerInfo[playerid][pJob] == JOBLESS) return SendErrorMessage(playerid, "Voc� est� desempregado.");

	new mes[512];
	SendSyntaxMessage(playerid, "[VE�CULO] Pegue um ve�culo de trabalho - /vtrabalho, devolva usando - /devolvervtrabalho(/dvt).");

	if (PlayerInfo[playerid][pSideJob] == JOB_PIZZA) SendSyntaxMessage(playerid, "[EMPREGO DE ENTREGA DE PIZZA] Entregue pizzas - /pizzajob. Digite /quitsidejob para sair.");

	if (PlayerInfo[playerid][pJob] == JOB_CARJACKER)
	{
		static const str_size[] = "{ffffff}Voc� est� trabalhando como ladr�o de carros. Hor�rio de funcionamento: %i.\n\n/entregarcarro - entrega ve�culos roubado ao desmanche.\n /dropoff - se livrar do ve�culo.\n /leavemission - redefinir a miss�o.\n\n /vbl (n) oisy - hackeamento grosseiro de ve�culos.\n /vbl (q) uiet - hackeamento silencioso de ve�culos\n\n (/en) gine - iniciar a quebra do motor\n (/uns) cramble - descriptografar a palavra (quando quebrando o motor) ";
		format(mes, sizeof(mes), str_size, PlayerInfo[playerid][pCareer]);
	}

	else if (PlayerInfo[playerid][pJob] == JOB_TAXI)
	{
		static const str_size[] = "{ffffff}Voc� est� trabalhando como motorista de t�xi. Hor�rio de funcionamento: %i.\n\n/taxi duty - iniciar /parar o trabalho.\n /taxi taxas - alterar a tarifa.\n /taxi aceitar - aceitar a chamada.\n /endereco - marcar o endere�o no mapa\n /rua - descobrir a rua em que voc� est�. ";

		format(mes, sizeof(mes), str_size, PlayerInfo[playerid][pCareer]);
	}
	if (PlayerInfo[playerid][pJob] == JOB_MECHANIC)
	{
		static const str_size[] = "{ffffff}Voc� est� trabalhando como mec�nico. Hor�rio de funcionamento: %i.\n\n/refill - abastecer o ve�culo.\n /repair - reparar o ve�culo.\n /paintcar - pintar o ve�culo.\n /buycomp - comprar componentes.\n /mycomp - descobrir o n�mero de componentes no ve�culo.\n /attach - rebocar o ve�culo. ";
		format(mes, sizeof(mes), str_size, PlayerInfo[playerid][pCareer]);
	}

	else if (PlayerInfo[playerid][pJob] == JOB_TRUCKER)
	{
		format(mes, sizeof(mes), "{ffffff}Voc� est� trabalhando como caminhoneiro. Hor�rio de funcionamento: %i.\n\n/cargo - trabalhe com a carga.\n /trailer - controle o reboque.\n /tpda - o menu do caminhoneiro.\n /industry - informa��es sobre a empresa.\n\n", PlayerInfo[playerid][pCareer]);
        switch(PlayerInfo[playerid][pCareer])
		{
		    case 0..11: strcat(mes, "Ve�culos dispon�veis: Picador, Sadler, Bobcat, Walton, Yosemite.");
			case 12..23: strcat(mes, "Ve�culos dispon�veis: Picador, Sadler, Bobcat, Walton, Yosemite, Pony, Berkley's RC Van, Burrito.");
		    case 24..31: strcat(mes, "Ve�culos dispon�veis: Picador, Sadler, Bobcat, Walton, Yosemite, Pony, Berkley's RC Van, Burrito, Rumpo, Boxville.");
		    case 32..47:
			{
				strcat(mes, "Ve�culos dispon�veis: Picador, Sadler, Bobcat, Walton, Yosemite, Pony, Berkley's RC Van, Burrito, Rumpo, Boxville.\n");
                strcat(mes, "Benson, Mule, DFT-30, Tanker, Securicar.");
			}
			case 48..58:
			{
				strcat(mes, "Ve�culos dispon�veis: Picador, Sadler, Bobcat, Walton, Yosemite, Pony, Berkley's RC Van, Burrito, Rumpo, Boxville.\n");
                strcat(mes, "Benson, Mule, DFT-30, Tanker, Securicar, Yankee, Flatbed.");
			}
			default:
			{
				strcat(mes, "Ve�culos dispon�veis: Picador, Sadler, Bobcat, Walton, Yosemite, Pony, Berkley's RC Van, Burrito.\n");
                strcat(mes, "Benson, Mule, DFT-30, Tanker, Securicar, Yankee, Flatbed, �nibus.");
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
	
	if (pTemp[playerid][pJob])					return SendErrorMessage(playerid, "Voc� j� pegou um ve�culo de trabalho! Digite /unrentcarjob(/unrcj) para recuper�-lo. ");
	if (!IsPlayerInAnyVehicle(playerid))			return SendErrorMessage(playerid, "Voc� deve estar em um ve�culo de trabalho! ");
	
	new vehicle = GetPlayerVehicleID(playerid);

	if (!IsPizzaCar(vehicle) && PlayerInfo[playerid][pJob] == JOBLESS) return SendErrorMessage(playerid,"Voc� est� desempregado.");
	if (GetPlayerState(playerid) != PLAYER_STATE_DRIVER)		  		  return SendErrorMessage(playerid, "Voc� deve estar dirigindo um ve�culo.");

	if (VehicleInfo[vehicle][carRent])			return SendErrorMessage(playerid, "Este ve�culo j� est� sendo usado por algu�m.");
	if (pTemp[playerid][pRentveh])				return SendErrorMessage(playerid, "Voc� precisa cancelar o aluguel do ve�culo/unrentcarjob(/unrcj) !");

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
		SendErrorMessage(playerid, "Tipo de ve�culo inv�lido!");
		if (PlayerInfo[playerid][pJob] == JOB_TRUCKER) SendSyntaxMessage(playerid, "Voc� pode descobrir o ve�culo dispon�vel para voc� usando /jobhelp !");
		return 1;
	}

	pTemp[playerid][pRentveh] = vehicle;
	VehicleInfo[vehicle][carRent] = PlayerInfo[playerid][pID];

	SendHelpMessage(playerid, "Digite /jobhelp para ver a lista de comandos dispon�veis. ");
	SendSyntaxMessage(playerid, "Voc� pegou o ve�culo de trabalho %s. Comandos dispon�veis: /unrentcarjob(/unrcj), /lock.", VehicleNames[GetVehicleModel(vehicle)-400]);
	SendSyntaxMessage(playerid, "Aten��o! Se voc� n�o usar o ve�culo por 10 minutos, ele desaparecer�. ");

	return 1;
}

ALTX:unrentcarjob("/unrcj","/devolvervtrabalho","/dvt");
CMD:unrentcarjob(playerid, params[])
{
	if (!pTemp[playerid][pRentveh]) return 1;

    SendSyntaxMessage(playerid, "Voc� devolveu um ve�culo de trabalho.");
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
    
    if (!PlayerInfo[playerid][pFaction]) return SendErrorMessage(playerid, "Voc� n�o � membro de uma fac��o oficial.");
    if (PlayerInfo[playerid][pSideJob] != JOB_DRUGDEALER) return SendErrorMessage(playerid, "Voc� n�o tem acesso � compra de drogas.");
    if (Factions[faction][fType] != FACTION_GANG && Factions[faction][fType] != FACTION_MAFIA) return 1;

    if (!PlayerToPoint(2.0, playerid, BLACKMARKET_X, BLACKMARKET_Y, BLACKMARKET_Z))
    {
        Waypoint_Set(playerid, BLACKMARKET_X, BLACKMARKET_Y, BLACKMARKET_Z);
        return SendErrorMessage(playerid, "Voc� deve estar no mercado negro.");
    }

    Dialog_Show(playerid, BuyDrugs, DIALOG_STYLE_TABLIST, "Comprar drogas", "Sementes de maconha\t50 $ por pe�a.\nLSD\t150 $ por grama.\nHeroina\t100 $ por grama.\nCoca�na\t200 $ por grama.\nEfedrina\t50 $ por pe�a.", "Comprar", "Cancelar");
    return 1;
}


CMD:comprararma(playerid, params[])
{
    new bizid = -1;

    if ((bizid = GetNearInsideBiz(playerid)) != -1 && BizInfo[bizid][bizType] == BIZ_AMMO) return BuyGunMenu(playerid);

    new faction = PlayerInfo[playerid][pFaction];

    if (!PlayerInfo[playerid][pFaction]) return SendErrorMessage(playerid, "Voc� n�o � membro de uma fac��o oficial.");
    if (PlayerInfo[playerid][pSideJob] != JOB_GUNDEALER) return SendErrorMessage(playerid, "Voc� n�o tem acesso � compra de armas.");
    if (Factions[faction][fType] != FACTION_GANG && Factions[faction][fType] != FACTION_MAFIA) return 1;

    if (!PlayerToPoint(2.0, playerid, BLACKMARKET_X, BLACKMARKET_Y, BLACKMARKET_Z))
    {
        Waypoint_Set(playerid, BLACKMARKET_X, BLACKMARKET_Y, BLACKMARKET_Z);
        return SendErrorMessage(playerid, "Voc� deve estar no mercado negro.");
    }

    new title[56];
    format(title, sizeof(title), "No saldo da sua fac��o - %i de experi�ncia.", Factions[faction][fPoint]);
    Dialog_Show(playerid, BuyGunMain, DIALOG_STYLE_LIST, title, "- Arma\n- Muni��o", "Selecionar", "Cancelar");
    return 1;
}


CMD:roubarradio(playerid, params[])
{
    new vehicle = GetPlayerVehicleID(playerid);

    if (!vehicle || !IsDriver(playerid)) return SendErrorMessage(playerid, "Voc� deve estar no banco do motorista.");
    if (!VehicleInfo[vehicle][carOwnerID]) return SendErrorMessage(playerid, "O r�dio s� pode ser roubado de ve�culos pessoais.");
    if (!VehicleInfo[vehicle][carRadio]) return SendErrorMessage(playerid, "Este ve�culo n�o tem r�dio.");
    if (IsPlayerHasItem(playerid, INV_CARSOUND)) return SendErrorMessage(playerid, "Voc� j� possui um r�dio em seu invent�rio.");
    if (GiveInvItem(playerid, INV_CARSOUND, 1) == -1) return SendErrorMessage(playerid, "Invent�rio cheio.");

    VehicleInfo[vehicle][carRadio] = 0;

    new mes[128];
    format(mes, sizeof(mes), "retira o r�dio do(a) %s.", VehicleNames[GetVehicleModel(vehicle)-400]);
    cmd::me(playerid, mes);
    return 1;
}


CMD:trabalhofaccao(playerid, params[])
{
    if (!PlayerInfo[playerid][pFaction]) return SendErrorMessage(playerid, "Voc� deve ser membro de uma fac��o oficial de gangster ou m�fia.");
    if (Factions[PlayerInfo[playerid][pFaction]][fType] != FACTION_GANG && Factions[PlayerInfo[playerid][pFaction]][fType] != FACTION_MAFIA) return SendErrorMessage(playerid, "Voc� deve estar em uma fac��o oficial de gangster ou m�fia.");
    if (!isLeader(playerid)) return SendErrorMessage(playerid, "Este comando est� dispon�vel apenas para o l�der da fac��o oficial.");

    new userid, level;

    if (sscanf(params, "ud", userid, level)) return SendHelpMessage(playerid, "/trabalhofaccao [ID/Parte do Nome] [0 - revogar acesso | 1 - acesso � compra de drogas | 2 - acesso � compra de armas] ");

    if (!IsPlayerLogged(userid)) return SendNotConnected(playerid);
    if (PlayerInfo[userid][pFaction] != PlayerInfo[playerid][pFaction]) return SendErrorMessage(playerid, "O jogador n�o � membro da sua fac��o.");
    if (level > 2 || level < 0) return SendErrorMessage(playerid, "O par�metro deve estar entre 0 e 2.");

    if (!level)
    {
        if (PlayerInfo[userid][pSideJob] == 0) return SendErrorMessage(playerid, "O jogador n�o tinha acesso a nenhum trabalho.");

        SendSyntaxMessage(userid, "O l�der da fac��o %s revogou seu acesso ao trabalho %s.", GetNameEx(playerid), GetJobName(PlayerInfo[userid][pSideJob]));
        SendSyntaxMessage(playerid, "Voc� revogou o acesso ao trabalho de %s %s.", GetNameEx(userid), GetJobName(PlayerInfo[userid][pSideJob]));
        PlayerInfo[userid][pSideJob] = 0;
        return 1;
    }

    PlayerInfo[userid][pSideJob] = level + 3;
    SendSyntaxMessage(userid, "O l�der da fac��o %s concedeu a voc� acesso ao trabalho %s.", GetNameEx(playerid), GetJobName(PlayerInfo[userid][pSideJob]));
    SendSyntaxMessage(playerid, "Voc� concedeu acesso a %s ao trabalho %s.", GetNameEx(userid), GetJobName(PlayerInfo[userid][pSideJob]));
    return 1;
}