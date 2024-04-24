ALTX:checkint("/verinterior");
CMD:checkint(playerid, params[])
{
    new houseid;

    if ((houseid = GetNearOutsideHouse(playerid)) == -1) return SendErrorMessage(playerid, "Não há casas à venda perto de você.");
    if (HouseInfo[houseid][hOwnerID])                     return SendErrorMessage(playerid, "Esta casa já foi comprada.");
    
    if (!GetWorld(playerid) && !GetInterior(playerid))     GetPlayerPos(playerid, PlayerInfo[playerid][pLocationPos][0], PlayerInfo[playerid][pLocationPos][1], PlayerInfo[playerid][pLocationPos][2]);

    SetPlayerPos(playerid,HouseInfo[houseid][hExitX],HouseInfo[houseid][hExitY],HouseInfo[houseid][hExitZ]);
    SetInterior(playerid,HouseInfo[houseid][hExitInt]);
    SetWorld(playerid,HouseInfo[houseid][hExitVW]);
    pTemp[playerid][pHouseEntered] = houseid;
    
    SetPVarInt(playerid, #check_int, 30);
    SendSyntaxMessage(playerid, "Você está visualizando o interior. Você tem 30 segundos.");
    return 1;
}


ALTX:house("/h","/casa");
CMD:house(playerid, params[])
{
	new option[16], secoption[128];

	if (sscanf(params, "s[16]S()[128]", option, secoption))
	{
	    SendHelpMessage(playerid, "/c(asa) [opções]");
	    SendSyntaxMessage(playerid, "comprar | menu | melhorar | sacar | depositar | vender | venderpara | portadosfundos");

	    if (!randomEx(0,5)) {
	        SendHelpMessage(playerid, "Se você precisa adicionar uma garagem / mudar a posição da entrada / saída principal ou adicional");
	        SendHelpMessage(playerid, "Consulte o fórum na seção \"Propriedade\"!");
	    }		
	     
	    return 1;
	}

	if (CompareStrings(option, "comprar"))
	{
	    new houseid;

	    if ((houseid = GetNearOutsideHouse(playerid)) == -1)                return SendErrorMessage(playerid, "Você não está perto de uma casa à venda.");
	    if (HouseInfo[houseid][hOwnerID])                                   return SendErrorMessage(playerid, "Esta casa já pertence a alguém.");
	    if (GetPlayerHouses(playerid) >= GetMaxPlayerHouse(playerid))       return SendErrorMessage(playerid, "Você não pode comprar mais casas.");
	    if (PlayerInfo[playerid][pLevel] < HouseInfo[houseid][hLevelbuy])   return SendErrorMessage(playerid, "Para comprar esta casa você precisa do nível %i.", HouseInfo[houseid][hLevelbuy]);
	    if (GetCash(playerid) < HouseInfo[houseid][hPrice])                 return SendNotCash(playerid);

	    HouseInfo[houseid][hOwnerID] = PlayerInfo[playerid][pID];
	    HouseInfo[houseid][hLocked] = 1;
	    HouseInfo[houseid][hCash] = 0;
	    format(HouseInfo[houseid][hOwner], MAX_PLAYER_NAME, GetNameEx(playerid));

	    new money = PlayerInfo[playerid][pCash];

	    GiveCash(playerid, -HouseInfo[houseid][hPrice]);
	    SendSyntaxMessage(playerid, "Você comprou uma casa[ID: %i] por $%i.", houseid, HouseInfo[houseid][hPrice]);
	    
	    for(new i = 0; i < MAX_GARAGES; i++)
	    {
	        if (!GarageInfo[i][garageID] || GarageInfo[i][garageHouse] != HouseInfo[houseid][hID]) continue;

	        SendSyntaxMessage(playerid, "Sua casa tem uma garagem [ID: %i].", i);
	        GarageInfo[i][garageOwner] = PlayerInfo[playerid][pID];
	        format(GarageInfo[i][garageOwnerName], 24, "%s", GetNameEx(playerid));
	    
	        SQL_SetInteger("garage", "owner", GarageInfo[i][garageOwner], GarageInfo[i][garageID]);
	        SQL_SetString("users", "name", GarageInfo[i][garageOwnerName], GarageInfo[i][garageOwner]);
	    }

	    if (HouseInfo[houseid][hExtX][0] && HouseInfo[houseid][hExtY][0] && HouseInfo[houseid][hExtZ][0]) {
	        SendSyntaxMessage(playerid, "Sua casa tem uma porta dos fundos. Você precisa instalar uma saída no interior. ( /c portadosfundos )");
	    }

	    SendHelpMessage(playerid, "Use /ajudacasa para te ajudar a gerenciar sua casa. ");
	    Save_House(houseid);

	    new mes[256];
	    format(mes, sizeof(mes), "%s [MYSQL: %i | IP: %s | Dinheiro(ERA): %i | Dinheiro(FICOU): %i] comprou uma casa [H_MYSQL: %i] por -- $%i", GetName(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), money, GetCash(playerid), 
	    HouseInfo[houseid][hID], HouseInfo[houseid][hPrice]);
	    insert_log(mes, PROPERTY_LOG, 3, PlayerInfo[playerid][pAccID]);
	    return 1;
	}

	else if (CompareStrings(option, "hmenu"))	return cmd::hmenu(playerid, "");
	else if (CompareStrings(option, "melhorar"))
	{
		new houseid;

		if ((houseid = GetNearInsideHouse(playerid)) == -1)				return SendErrorMessage(playerid, "Você deve estar dentro da casa.");
		if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta casa não pertence a você.");

		new parameters[16], arg[112];

		if (sscanf(secoption, "s[16]S()[112]", parameters, arg))
		{
			SendHelpMessage(playerid, "/casa melhorar [opção]");
			SendSyntaxMessage(playerid, "rádio {33CC33}[$500]{FFFFFF} | tranca | alarme | armario {33CC33}[$600]{FFFFFF} | porta | consertarporta | curar");
			return 1;
		}

		if (CompareStrings(parameters, "radio"))
		{
			if (HouseInfo[houseid][hRadio])		return SendErrorMessage(playerid, "Já existe um rádio instalado nesta casa.");
			if (GetCash(playerid) < 500)		return SendErrorMessage(playerid, "Você não tem dinheiro suficiente. Você precisa de $ 500.");

			HouseInfo[houseid][hRadio] = 1;
			SQL_SetInteger("houses", "radio", HouseInfo[houseid][hRadio], HouseInfo[houseid][hID]);
			GiveCash(playerid, -500);
			SendSyntaxMessage(playerid, "Você comprou um rádio por $500. Use /setarestacao.");
			return 1;
		}

		else if (CompareStrings(parameters, "tranca"))
		{
			new level;

			if (sscanf(arg, "i", level))
			{
				SendHelpMessage(playerid, "/casa melhorar tranca [nível (1-5)]");
				SendSyntaxMessage(playerid, "Cada nível custa $ 250 a mais que o anterior [1 nível - $250 | 5 nível - $1250].");
				return 1;
			}
			if (level < 1 || level > 5)				return SendErrorMessage(playerid, "Nível do cadeado de 1 a 5.");
			if (GetCash(playerid) < 250 * level)	return SendErrorMessage(playerid, "O preço de um cadeado nível %i é $%i.", level, level * 250);
			if (HouseInfo[houseid][hLock] >= level)	return SendErrorMessage(playerid, "Você já tem um cadeado nível %i.", HouseInfo[houseid][hLock]);

			HouseInfo[houseid][hLock] = level;
			SQL_SetInteger("houses", "lock", HouseInfo[houseid][hLock], HouseInfo[houseid][hID]);
			GiveCash(playerid, -level * 250);
			SendSyntaxMessage(playerid, "Você comprou um cadeado nível %i por $%i.", HouseInfo[houseid][hLock], level * 250);
			return 1;
		}

		else if (CompareStrings(parameters, "alarme"))
		{
			new level;

			if (sscanf(arg, "i", level))
			{
				SendHelpMessage(playerid, "/casa melhorar alarme [nível (1-3)]");
				SendSyntaxMessage(playerid, "Cada nível custa $500 a mais que o anterior [1 nível - $500 | 3 nível - $1500].");
				return 1;
			}
			if (level < 1 || level > 3)					return SendErrorMessage(playerid, "Nível do alarme de 1 a 3.");
			if (GetCash(playerid) < 500 * level)		return SendErrorMessage(playerid, "O preço do alarme nível %i é $%i.", level, level * 500);
			if (HouseInfo[houseid][hAlarm] >= level)	return SendErrorMessage(playerid, "Você já instalou um alarme nível %i.", HouseInfo[houseid][hAlarm]);

			HouseInfo[houseid][hAlarm] = level;
			SQL_SetInteger("houses", "alarm", HouseInfo[houseid][hAlarm], HouseInfo[houseid][hID]);
			GiveCash(playerid, -level * 500);
			SendSyntaxMessage(playerid, "Você comprou um alarme nível %i por $%i.", HouseInfo[houseid][hAlarm], level * 500);
			return 1;
		}		
		else if (CompareStrings(parameters, "porta"))
		{
			new level;

			if (sscanf(arg, "i", level))
			{
				SendHelpMessage(playerid, "/casa melhorar porta [nível (1-2)]");
				SendSyntaxMessage(playerid, "Cada nível custa $ 750 a mais que o anterior [1 nível - $750 | 2 nível - $1500].");
				return 1;
			}
			if (level < 1 || level > 2)				return SendErrorMessage(playerid, "Nível da porta de 1 a 2.");
			if (GetCash(playerid) < 750 * level)	return SendErrorMessage(playerid, "O preço da porta de nível %i é $%i.", level, level * 750);
			if (HouseInfo[houseid][hDoor] >= level)	return SendErrorMessage(playerid, "Você já tem uma porta de nível %i instalada.", HouseInfo[houseid][hDoor]);

			HouseInfo[houseid][hDoor] = level;
			SQL_SetInteger("houses", "door", HouseInfo[houseid][hDoor], HouseInfo[houseid][hID]);
			GiveCash(playerid, -level * 750);
			SendSyntaxMessage(playerid, "Você comprou uma porta de nível %i por $%i.", HouseInfo[houseid][hDoor], level * 750);
			return 1;
		}

		else if (CompareStrings(parameters, "armario"))
		{
			if (HouseInfo[houseid][hCupboard])	return SendErrorMessage(playerid, "Você já tem um armário.");
			if (GetCash(playerid) < 600)			return SendErrorMessage(playerid, "O preço do armário é $600.");

			HouseInfo[houseid][hCupboard] = 1;
			SQL_SetInteger("houses", "cupboard", HouseInfo[houseid][hCupboard], HouseInfo[houseid][hID]);
			GiveCash(playerid, -600);
			SendSyntaxMessage(playerid, "Você comprou um armário por $600.");
			SendSyntaxMessage(playerid, "Digite /roupas para usar.");
			return 1;
		}


		else if (CompareStrings(parameters, "repararporta"))
		{
			if (!HouseInfo[houseid][hDDoor])	return SendErrorMessage(playerid, "Sua porta da frente não está danificada.");
			if (GetCash(playerid) < 300)		return SendErrorMessage(playerid, "O preço do conserto da porta é $300.");

			HouseInfo[houseid][hDDoor] = 0;
			SQL_SetInteger("houses", "ddoor", HouseInfo[houseid][hDDoor], HouseInfo[houseid][hID]);
			UpdateHouseDoor(houseid);
			GiveCash(playerid, -300);
			SendSyntaxMessage(playerid, "Você consertou a porta da frente por $300.");
			return 1;
		}

		else if (CompareStrings(parameters, "curar"))
		{
			new level;
			
			if (sscanf(arg, "i", level))
			{
				SendHelpMessage(playerid, "/casa melhorar curar [nível (1-5)]");
				SendSyntaxMessage(playerid, "Cada nível custa $ 400 a mais que o anterior [1 nível - $400 | 5 nível - $2000].");
				SendSyntaxMessage(playerid, "Cada nível aumenta a saúde restaurada em 20 HP [1 nível - +20 HP | 5 nível - +100 HP].");
				SendSyntaxMessage(playerid, "Cada nível aumenta o atraso do comando em 20 segundos [1 nível - 60 seg. | 5 nível - 140 seg.].");
				return 1;
			}
			if (HouseInfo[houseid][hHeal])			return SendErrorMessage(playerid, "A casa já possui um kit de primeiros socorros.");
			if (level < 1 || level > 5)				return SendErrorMessage(playerid, "Nível do kit de primeiros socorros de 1 a 5.");
			if (GetCash(playerid) < level * 400)	return SendErrorMessage(playerid, "O preço do kit de primeiros socorros de nível %i é $%i.", level, level * 400);

			HouseInfo[houseid][hHeal] = level;
			HouseInfo[houseid][hHealAmount] = 3;
			SQL_SetInteger("houses", "heal", HouseInfo[houseid][hHeal], HouseInfo[houseid][hID]);
			SQL_SetInteger("houses", "heal_amount", HouseInfo[houseid][hHealAmount], HouseInfo[houseid][hID]);
			GiveCash(playerid, -level * 400);
			SendSyntaxMessage(playerid, "Você comprou um kit de primeiros socorros de nível %i por $%i.", HouseInfo[houseid][hHeal], level * 400);
			return 1;
		}
	}

	else if (CompareStrings(option, "sacar"))
	{
		new houseid; 

		if ((houseid = GetNearInsideHouse(playerid)) == -1)				return SendErrorMessage(playerid, "Você deve estar dentro da casa.");
		if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta casa não pertence a você.");

		new amount;

		if (sscanf(secoption, "i", amount))
		{
			SendHelpMessage(playerid, "/casa sacar [dinheiro]");
			SendSyntaxMessage(playerid, "No cofre tem $%i.", HouseInfo[houseid][hCash]);
			return 1;
		}
		if (amount > HouseInfo[houseid][hCash] || amount < 1)	return SendErrorMessage(playerid, "Não há dinheiro suficiente no cofre.");

		HouseInfo[houseid][hCash] -= amount;
		SQL_SetInteger("houses", "cash", HouseInfo[houseid][hCash], HouseInfo[houseid][hID]);
		GiveCash(playerid, amount);
		SendSyntaxMessage(playerid, "Você retirou $%i do cofre. Restam $%i.", amount, HouseInfo[houseid][hCash]);
		return 1;
	}

	else if (CompareStrings(option, "depositar"))
	{
		new houseid;

		if ((houseid = GetNearInsideHouse(playerid)) == -1)				return SendErrorMessage(playerid, "Você deve estar dentro da casa.");
		if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta casa não pertence a você.");

		new amount;

		if (sscanf(secoption, "i", amount))
		{
			SendHelpMessage(playerid, "/casa depositar [dinheiro]");
			SendSyntaxMessage(playerid, "No cofre tem $%i.", HouseInfo[houseid][hCash]);
			return 1;
		}
		if (amount > GetCash(playerid) || amount < 1)	return SendNotCash(playerid);

		GiveCash(playerid, -amount);
		HouseInfo[houseid][hCash] += amount;
		SQL_SetInteger("houses", "cash", HouseInfo[houseid][hCash], HouseInfo[houseid][hID]);
		SendSyntaxMessage(playerid, "Você depositou $%i no cofre. Saldo: $%i.", amount, HouseInfo[houseid][hCash]);
		return 1;
	}

	else if (CompareStrings(option, "venderpara"))
	{
		new userid, price;

		if (sscanf(secoption, "ui", userid, price))			return SendHelpMessage(playerid, "/casa venderpara [ID/Parte do Nome] [preço]");

		new houseid;

		if ((houseid = GetNearOutsideHouse(playerid)) == -1)			return SendErrorMessage(playerid, "Você não está perto de casa.");
		if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta casa não pertence a você.");
		if (!IsPlayerLogged(userid))									return SendNotConnected(playerid);
		if (!PlayerNearPlayer(3.0, playerid, userid))					return SendNotNearPlayer(playerid);
		if (price < 0 || price > 100000000)								return SendErrorMessage(playerid, "Preço de venda de $0 a $100000000.");

		pTemp[userid][pHousePrice] = price;
		pTemp[userid][pHousePlayer] = playerid;
		pTemp[userid][pHouseHouse] = houseid;
		SetPVarInt(userid, #accept_time, gettime() + 30);
		SendSyntaxMessage(playerid, "Você sugeriu a %s comprar sua casa por $%i.", GetNameEx(userid), price);
		SendSyntaxMessage(userid, "%s quer vender-lhe uma casa por $%i. Pressione Y para comprar.", GetNameEx(playerid), price);
	}

	if (CompareStrings(option, "portatrazeira"))
	{
		new houseid = GetNearInsideHouse(playerid);
		
		if (houseid == -1) return SendErrorMessage(playerid, "Você deve estar na sua casa!");

		if (GetInterior(playerid) != HouseInfo[houseid][hExitInt] || GetWorld(playerid) != HouseInfo[houseid][hExitVW]) {
			return SendErrorMessage(playerid, "Você deve estar na sua casa!");
		}

		if (!HouseInfo[houseid][hExtX][0] && !HouseInfo[houseid][hExtY][0] && !HouseInfo[houseid][hExtZ][0]) {
			return SendErrorMessage(playerid, "A casa não tem ENTRADA traseira! (Consulte o fórum na seção de propriedades)");
		}
		
		new Float: posX, Float: posY, Float: posZ;
		GetPlayerPos(playerid, posX, posY, posZ);

		HouseInfo[houseid][hExtX][1] = posX;
		HouseInfo[houseid][hExtY][1] = posY;
		HouseInfo[houseid][hExtZ][1] = posZ;

		HouseInfo[houseid][hExtVW][1] = GetPlayerVirtualWorld(playerid);
		HouseInfo[houseid][hExtInt][1] = GetPlayerInterior(playerid);

		Save_House(houseid);
		SendSyntaxMessage(playerid, "Você definiu com sucesso uma posição de SAÍDA adicional para a sua casa.");
	}

	return 1;
}
ALTX:hmenu("/cmenu","/casamenu");
CMD:hmenu(playerid, params[])
{
    new houseid;

	if ((houseid = GetNearHouse(playerid)) == -1)					return SendErrorMessage(playerid, "Você não está perto de casa.");
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta casa não pertence a você.");

	static const str_size[] = "%s trava da porta\n\
								Melhorias na casa\n\
								Gestão de móveis\n\
								Inquilinos\n\
								Inventário da casa\n\
								Cofre com dinheiro\n\
								Um guarda-roupa\n\
								Vender uma casa";
	new mes[sizeof(str_size) - 4 + 7 + 1 + 1];
 	
	format(mes, sizeof(mes), str_size, (HouseInfo[houseid][hLocked] != 0) ? ("Abrir") : ("Fechar"));
	Dialog_Show(playerid, HouseMenu, DIALOG_STYLE_LIST, "Menu da Casa", mes, "Selecionar", "Cancelar");
	return 1;
}

ALTX:rentroom("/alugarquarto");
CMD:rentroom(playerid, params[])
{
    new houseid;
	if ((houseid = GetNearInsideHouse(playerid)) == -1)				return SendErrorMessage(playerid, "Você deve estar dentro de sua casa.");
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta casa não pertence a você.");
	    
    new userid, price;

	if (sscanf(params,"ud", userid, price))							return SendHelpMessage(playerid, "/alugarquarto [ID/Parte do Nome] [custo de vida por hora]");
    if (userid == playerid)											return 1;	
	if (!IsPlayerLogged(userid))									return SendNotConnected(playerid);
    if (!PlayerNearPlayer(3.0, playerid, userid))					return SendNotNearPlayer(playerid);
	if (price < 0 || price > 500)									return SendErrorMessage(playerid, "Custo de vida de $0 a $500.");
    if (GetPlayerHouses(userid) || PlayerInfo[userid][pRentKey])	return SendErrorMessage(playerid, "O jogador já comprou/alugou uma casa.");
	if (GetCash(userid) < price)									return SendErrorMessage(playerid, "O jogador não tem dinheiro suficiente.");

    new query[128];
	mysql_format(dbHandle, query, sizeof(query), "SELECT `id` FROM `users` WHERE `rent_key`=%i", HouseInfo[houseid][hID]);
	mysql_tquery(dbHandle, query, "OfferRentRoom", "iii", playerid, userid, price);
	return 1;
}


ALTX:unrent("/despejar");
CMD:unrent(playerid, params[])
{
	if (PlayerInfo[playerid][pRentKey] < 1)	return SendErrorMessage(playerid, "Você não está alugando uma casa.");

	PlayerInfo[playerid][pRentKey] = 0;
	SQL_SetInteger("users", "rent_key", 0, PlayerInfo[playerid][pID]);
	SendSyntaxMessage(playerid, "Você cancelou o seu aluguel da casa.");
	return 1;
}


ALTX:heal("/vida");
CMD:heal(playerid, params[])
{
	new id, userid;

	if ((id = GetNearInsideHouse(playerid)) != -1 && PlayerInfo[playerid][pID] == HouseInfo[id][hOwnerID])
	{
	    if (HouseInfo[id][hHeal] == 0)					return SendErrorMessage(playerid, "Não há kit de primeiros socorros na casa.");
	    if (pTemp[playerid][pHealTime] > gettime())		return SendErrorMessage(playerid, "Você pode usar o kit de primeiros socorros em %i segundos.", pTemp[playerid][pHealTime]-gettime());
	    if (sscanf(params,"u", userid))					return SendHelpMessage(playerid, "/curar [ID/Parte do Nome]");
        if (!IsPlayerLogged(userid))					return SendNotConnected(playerid);
        if (!PlayerNearPlayer(3.0, playerid, userid))	return SendNotNearPlayer(playerid);
        if (PlayerInfo[userid][pInjured])				return SendErrorMessage(playerid, "O jogador está morto/desmaiado/ferido.");
        if (GetPlayerHealthEx(userid) >= 100.0)			return SendErrorMessage(playerid, "O jogador está saudável.");

		if (playerid != userid)
		{
        	SendSyntaxMessage(playerid, "Você usou um kit de primeiros socorros (+%i HP) no jogador %s. Aguarde %i segundos para usar o comando novamente.", (HouseInfo[id][hHeal] * 20), GetNameWithMask(userid), 40 + (HouseInfo[id][hHeal] * 20));
			SendSyntaxMessage(userid, "%s usou o kit de primeiros socorros em você. (+%i HP)", GetNameWithMask(playerid), (HouseInfo[id][hHeal] * 20));
		}
		else
		{
		    SendSyntaxMessage(playerid, "Você usou um kit de primeiros socorros (+%i HP) em si mesmo. Aguarde %i segundos para usar o comando novamente.", (HouseInfo[id][hHeal] * 20), 40 + (HouseInfo[id][hHeal] * 20));
		}

		ResetDamages(userid);
		
        StopPlayerAnims(userid);
		UnFreezePlayer(userid);

		pTemp[playerid][pHealTime] = gettime() + 40 + (HouseInfo[id][hHeal] * 20);

        new Float:hp;

		AntiCheatGetHealth(playerid, hp);
	    SetPlayerHealth(playerid, hp + float(HouseInfo[id][hHeal]) * 20.0);

		if (GetPlayerHealthEx(userid) > 100.0)  SetPlayerHealth(userid, 100.0);

		HouseInfo[id][hHealAmount]--;
		if (!HouseInfo[id][hHealAmount]) HouseInfo[id][hHeal] = 0;
		Save_House(id);
		return 1;
	}

	else if (IsPlayerFactionMedic(playerid) || IsPlayerFactionFire(playerid)) //FD Heal
	{
		if (sscanf(params,"u", userid))					return SendHelpMessage(playerid, "/curar [ID/Parte do Nome]");
		if (!PlayerInfo[playerid][pOnDuty])				return SendErrorMessage(playerid, "Você deve estar em serviço.");
		if (!IsPlayerLogged(userid))					return SendNotConnected(playerid);
		if (userid == playerid)							return 1;
		if (!PlayerNearPlayer(3.0, playerid, userid))	return SendNotNearPlayer(playerid);
		if (pTemp[userid][pHealPlayer] == playerid)		return SendErrorMessage(playerid, "Você já ofereceu tratamento a este jogador.");
		if (PlayerInfo[userid][pInjured] != 0)			return SendErrorMessage(playerid, "Este jogador está na fase de ferimento/morte.");
		if (GetPlayerHealthEx(userid) >= 100.0)			return SendErrorMessage(playerid, "O jogador está saudável.");

		pTemp[userid][pHealPlayer] = playerid;
		SetPVarInt(userid, #accept_time, gettime() + 30);
		SendSyntaxMessage(userid, "%s quer curar você. Pressione Y para aceitar a oferta.", GetNameWithMask(playerid));
		SendSyntaxMessage(playerid, "Você ofereceu tratamento %s.", GetNameWithMask(userid));
	}
	
	else return SendErrorMessage(playerid, "Você não está na casa.");

	return 1;
}

ALTX:grantbuild("/permissaomobilia");
CMD:grantbuild(playerid, params[])
{
    new userid;

	if (sscanf(params,"u", userid))					return SendHelpMessage(playerid, "/permissaomobilia [ID/Parte do Nome]");
    if (!IsPlayerLogged(userid))					return SendNotConnected(playerid);
	if (userid == playerid)							return 1;
	if (!PlayerNearPlayer(3.0, playerid, userid))	return SendNotNearPlayer(playerid);

    new houseid;

	if ((houseid = GetNearInsideHouse(playerid)) == -1)				return SendErrorMessage(playerid, "Você deve estar dentro da casa.");
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid,"Esta casa não pertence a você.");

	if (pTemp[userid][pBuildHouse] == HouseInfo[houseid][hID])
	{
	    SendSyntaxMessage(playerid, "Você retirou o acesso à edição de móveis do jogador %s.", GetNameWithMask(userid));
	    SendSyntaxMessage(userid, "%s retirou seu acesso à edição de móveis.", GetNameWithMask(playerid));
	    pTemp[userid][pBuildHouse] = 0;
	    return 1;
	}
	SendSyntaxMessage(playerid, "Você concedeu acesso à edição de móveis ao jogador %s.", GetNameWithMask(userid));
 	SendSyntaxMessage(userid, "%s te concedeu acesso à lista de edição para colaboração.", GetNameWithMask(playerid));
    pTemp[userid][pBuildHouse] = HouseInfo[houseid][hID];
	return 1;
}

CMD:cache(playerid, params[])
{
    new houseid;

	if ((houseid = GetNearInsideHouse(playerid)) == -1)				return SendErrorMessage(playerid, "Você deve estar dentro da casa.");
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Esta casa não pertence a você.");

 	GetPlayerPos(playerid, HouseInfo[houseid][hCheckPosX], HouseInfo[houseid][hCheckPosY], HouseInfo[houseid][hCheckPosZ]);
	
 	Save_House(houseid);
 	SendSyntaxMessage(playerid, "Você alterou a localização do esconderijo.");
	return 1;
}


ALTX:hbreaklock("/hbl", "/hbreak","/arrombarporta");
CMD:hbreaklock(playerid, params[])
{
	if (!PoliceOnline())	return SendErrorMessage(playerid, "Deve haver pelo menos %i policiais no jogo.", NEED_COPS_TO_CRIME);

	new houseid = GetNearHouse(playerid);
	if (houseid == -1)						 		  return SendErrorMessage(playerid, "Você deve estar perto da entrada da casa.");
	if (!IsPlayerOutsideHouse(playerid, houseid, 3.0)) return SendErrorMessage(playerid,"Você deve estar perto da entrada da casa.");
	if (HouseInfo[houseid][hOwnerID] == 0)   		  return SendErrorMessage(playerid,"Apenas casas compradas podem ser arrombadas.");
	if (HouseInfo[houseid][hLocked] == 0)   		  return SendErrorMessage(playerid,"As portas da casa estão abertas.");
	if (JackingLock[playerid] != 0)         		  return SendErrorMessage(playerid, "Você já está arrombando as portas.");
	if (IsPlayerCrouching(playerid) == false) 		  return SendErrorMessage(playerid, "Você deve estar agachado.");

	foreach(new i : Player) {
		if (BreakingHouse[i] == houseid) return SendErrorMessage(playerid, "Alguém já está arrombando as portas desta casa.");
	}

	new param[10];
	if (sscanf(params, "s[10]", param)) {
		SendHelpMessage(playerid, "/arrombarporta [quieto] ou [barulhento]");
		SendSyntaxMessage(playerid, "(q)uieto - arrombamento silencioso da porta da casa com um grampo. (mais demorado)");
		SendSyntaxMessage(playerid, "(b)arulhento - arrombamento alto da casa com um pé de cabra, ativa o alarme, se houver. (mais rápido)");
		return 1;
	}	

	if (CompareStrings(param, "q") || CompareStrings(param, "quieto"))
	{
		if (!IsPlayerHasItem(playerid, INV_PICKLOCK)) return SendErrorMessage(playerid, "Você deve ter uma chave mestra.");
		
		pTemp[playerid][JackingType] = 3;
		BreakingHouse[playerid] = houseid;
		
		new htime;
		
		switch(HouseInfo[houseid][hDoor]) {
			case 0: htime = 250+HouseInfo[houseid][hLock]*250;
			case 1: htime = 250+HouseInfo[houseid][hLock]*250 + floatround((250+HouseInfo[houseid][hLock]*250) * 0.05);
			case 2: htime = 250+HouseInfo[houseid][hLock]*250 + floatround((250+HouseInfo[houseid][hLock]*250) * 0.15);
		}

		static const str_size[] = "{"COL_GREY"}Você quer começar a arrombar as portas de casa? Você precisa de %i segundos.";
		new mes[sizeof(str_size) + 6 + 70];
		format(mes, sizeof(mes), str_size, (!HouseInfo[houseid][hApart])?(GetHouseZone(houseid)):(GetApartmentZone(HouseInfo[houseid][hApart])), htime);
        Dialog_Show(playerid, StartBreakingLock, DIALOG_STYLE_MSGBOX, "{FFFFFF}Arrombando portas", mes, "Começar", "Cancelar");
	}
	else if (CompareStrings(param, "b") || CompareStrings(param, "barulhento"))
	{
		if (!CheckHanditem(playerid,18634)) return SendErrorMessage(playerid, "Você deve ter um pé de cabra em mãos. (/roupa)");
		pTemp[playerid][JackingType] = 4;
		BreakingHouse[playerid] = houseid;

		new htime;
		
		switch(HouseInfo[houseid][hDoor]) {
			case 0: htime = floatround((250+HouseInfo[houseid][hLock]*250)*0.6);
			case 1: htime = floatround((250+HouseInfo[houseid][hLock]*250 + floatround((250+HouseInfo[houseid][hLock]*250) * 0.05))*0.6);
			case 2: htime = floatround((250+HouseInfo[houseid][hLock]*250 + floatround((250+HouseInfo[houseid][hLock]*250) * 0.15))*0.6);
		}

		static const str_size[] = "{"COL_GREY"}Você quer começar a arrombar as portas de casa %s, %i? Você precisa de %i segundos.";
		new mes[sizeof(str_size) + 76];
		format(mes, sizeof(mes), str_size, (!HouseInfo[houseid][hApart])?(GetHouseZone(houseid)):(GetApartmentZone(HouseInfo[houseid][hApart])), htime);
        Dialog_Show(playerid, StartBreakingLock, DIALOG_STYLE_MSGBOX, "{FFFFFF}Arrombando portas", mes, "Começar", "Cancelar");
	} else {
		SendHelpMessage(playerid, "/arrombarporta [quieto] ou [barulhento]");
		SendSyntaxMessage(playerid, "(q)uieto - arrombamento silencioso da porta da casa com um grampo. (mais demorado)");
		SendSyntaxMessage(playerid, "(b)arulhento - arrombamento alto da casa com um pé de cabra, ativa o alarme, se houver. (mais rápido)");
		return 1;
	}

	return 1;
}

