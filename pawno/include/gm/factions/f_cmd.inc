CMD:editarspawn(playerid,params[])
{
	new factionid = PlayerInfo[playerid][pFaction];

	if (!isLeader(playerid) && !GetSpawnRight(playerid)) return SendErrorMessage(playerid, "Você não tem permissão para alterar a posição de spawn da facção.");

    GetPlayerPos(playerid, Factions[factionid][fPosX], Factions[factionid][fPosY], Factions[factionid][fPosZ]);
	Factions[factionid][fSpawnInt] = GetInterior(playerid);
	Factions[factionid][fSpawnVW] = GetWorld(playerid);

    new query[128 + 56];
    mysql_format(dbHandle, query, sizeof(query), "UPDATE `factions` SET `spawnx`=%f, `spawny`=%f, `spawnz`=%f, `spawnint`=%i, `spawnvw`=%i WHERE `id`= %i", Factions[factionid][fPosX], Factions[factionid][fPosY], Factions[factionid][fPosZ], Factions[factionid][fSpawnInt], Factions[factionid][fSpawnVW], Factions[factionid][fID]);
	mysql_tquery(dbHandle, query);

	SendSyntaxMessage(playerid, "Você definiu o spawn da facção %s para este local.", Factions[factionid][fName]);
	return 1;
}


CMD:editarnomecargo(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction], rank, name[32];

	if (!isLeader(playerid) && !GetRankNameRight(playerid))		return SendErrorMessage(playerid, "Você não tem permissão para alterar os nomes dos ranks.");
	if (sscanf(params,"ds[31]", rank, name))					return SendHelpMessage(playerid, "/editarnomecargo [cargo] [nome]");
	if (strlen(name) > 31)										return SendErrorMessage(playerid, "O nome do rank pode conter no máximo 31 caracteres.");
	if (rank < 1 || rank > Factions[faction][fRanks])			return SendErrorMessage(playerid, "Rank de 1 a %i.", Factions[faction][fRanks]);
	
	format(FactionRanks[faction][rank-1], 32, "%s", name);
	UpdateFactionRank(faction, rank);
	SendSyntaxMessage(playerid, "Você alterou o nome do rank [ID: %i] para '%s'.", rank, name);
	return 1;
}

CMD:fchat(playerid, params[])
{
	if (PlayerInfo[playerid][pFaction] == 0)			return SendErrorMessage(playerid, "Você deve estar em uma facção.");
    if (!isLeader(playerid) && !GetChatRight(playerid))	return SendErrorMessage(playerid, "Você não é um líder de facção.");

	new faction = PlayerInfo[playerid][pFaction];
	if (Factions[faction][fChatON] == 1)
	{
		SendFactionMessageF(faction, COLOR_LIGHTBLUE, " ** %s desativou o chat da facção.", GetNameEx(playerid));
		SendSyntaxMessage(playerid, "Você desativou o chat OOS da facção. Agora, apenas você pode escrever nele.");
		Factions[faction][fChatON] = 0;
	}
	else
	{
		SendFactionMessageF(faction, COLOR_LIGHTBLUE, " ** %s ativou o chat da facção.", GetNameEx(playerid));
		SendSyntaxMessage(playerid, "Você ativou o chat OOS da facção. Agora, todos os membros da facção podem escrever nele. ");
		Factions[faction][fChatON] = 1;
	}
	SQL_SetInteger("factions", "chaton", Factions[faction][fChatON], faction);
	return 1;
}


CMD:convidar(playerid, params[])
{
    if (PlayerInfo[playerid][pFaction] == 0)                        return SendErrorMessage(playerid, "Você deve estar em uma facção. ");
    if (!isLeader(playerid) && !GetInviteRight(playerid))            return SendErrorMessage(playerid, "Você não tem permissão para admitir pessoas na facção.");
    if (Factions[PlayerInfo[playerid][pFaction]][fJoinRank] == 0)    return SendErrorMessage(playerid, "Por favor, configure o rank de entrada na sua estrutura. ");

    new userid;

    if (sscanf(params,"u",userid))                                    return SendHelpMessage(playerid, "/convidar [ID/Parte do Nome]");
    if (!IsPlayerLogged(userid))                                    return SendNotConnected(playerid);
    if (userid == playerid)                                            return SendErrorMessage(playerid, "Você já está em uma facção.");
    if (PlayerInfo[userid][pFaction] != 0)                            return SendErrorMessage(playerid, "O jogador já está em uma facção.");
    if (pTemp[userid][pFactionPlayer] == playerid)                    return SendErrorMessage(playerid, "Você já convidou o jogador para se juntar à facção.");

    pTemp[userid][pFactionPlayer] = playerid;
    SetPVarInt(userid, #accept_time, gettime() + 30);

    SendSyntaxMessage(userid, "%s convidou você para se juntar à %s (Pressione Y para aceitar o convite)", GetNameWithMask(playerid), Factions[PlayerInfo[playerid][pFaction]][fName]);
    SendSyntaxMessage(playerid, "Você convidou %s para se juntar à sua facção.", GetNameWithMask(userid));
    return 1;
}

CMD:demitir(playerid, params[])
{
    if (PlayerInfo[playerid][pFaction] == 0)                        return SendErrorMessage(playerid, "Você deve estar em uma facção.");
    if (!isLeader(playerid) && !GetUninviteRight(playerid))            return SendErrorMessage(playerid, "Você não tem permissão para excluir membros da facção.");
    //if (Factions[PlayerInfo[playerid][pFaction]][fJoinRank] == 0)    return SendErrorMessage(playerid, "Por favor, configure o rank de entrada na sua estrutura. ");

    new userid;

    if (sscanf(params,"u",userid))                                    return SendHelpMessage(playerid, "/demitir [ID/Parte do Nome]");
    if (!IsPlayerLogged(userid))                                    return SendNotConnected(playerid);
    if (userid == playerid)                                            return SendErrorMessage(playerid, "Você não pode se demitir.");
    if (PlayerInfo[userid][pFaction] != PlayerInfo[playerid][pFaction])    return SendErrorMessage(playerid, "O jogador não é membro da sua organização.");

    if (!isLeader(playerid) && GetUninviteRight(playerid) && (isLeader(userid) || PlayerInfo[userid][pRank] >= PlayerInfo[playerid][pRank]))
    {
        return SendErrorMessage(playerid, "Você só pode demitir um jogador com um rank inferior ao seu.");
    }

    SendSyntaxMessage(userid, "%s te demitiu da facção %s.", GetNameEx(playerid), Factions[PlayerInfo[playerid][pFaction]][fName]);
    SendSyntaxMessage(playerid, "Você demitiu %s da sua facção.", GetNameEx(userid));

    RemovePlayerFaction(userid);
    SendFactionMessageF(PlayerInfo[playerid][pFaction], COLOR_LIGHTBLUE, "[Facção] %s foi demitido da facção pelo líder %s.", GetNameEx(userid), GetNameEx(playerid));
    return 1;
}


CMD:definircargo(playerid, params[])
{
    if (PlayerInfo[playerid][pFaction] == 0)                        return SendErrorMessage(playerid, "Você deve estar em uma facção.");
    if (!isLeader(playerid) && !GetRankRight(playerid))                return SendErrorMessage(playerid, "Você não tem permissão para alterar os ranks dos membros da facção.");
    if (Factions[PlayerInfo[playerid][pFaction]][fJoinRank] == 0)    return SendErrorMessage(playerid, "Por favor, configure os ranks na sua estrutura.");

    new userid, rank;

    if (sscanf(params,"ui",userid, rank))                                    return SendHelpMessage(playerid, "/definircargo [ID/Parte do Nome] [cargo]");
    if (!IsPlayerLogged(userid))                                            return SendNotConnected(playerid);
    if (userid == playerid)                                                    return SendErrorMessage(playerid, "Você não pode alterar seu próprio cargo.");
    if (PlayerInfo[userid][pFaction] != PlayerInfo[playerid][pFaction])        return SendErrorMessage(playerid, "O jogador não é membro da sua organização.");
    if (rank < 1 || rank > Factions[PlayerInfo[playerid][pFaction]][fRanks])    return SendErrorMessage(playerid, "Rank de 1 a %i.", Factions[PlayerInfo[playerid][pFaction]][fRanks]);
    if (!isLeader(playerid) && GetRankRight(playerid) && (isLeader(userid) || PlayerInfo[userid][pRank] >= PlayerInfo[playerid][pRank]))
    {
        return SendErrorMessage(playerid, "Você só pode alterar o cargo de jogadores com cargo inferior ao seu.");
    }
    if (PlayerInfo[playerid][pRank] < rank) return SendErrorMessage(playerid, "Você não pode atribuir um cargo mais alto que o seu a um jogador.");
             
    PlayerInfo[userid][pRank] = rank;
    SendSyntaxMessage(userid, "%s te promoveu para '%s'.", GetNameEx(playerid), GetRankName(userid));
    SendSyntaxMessage(playerid, "Você promoveu %s para '%s'.", GetNameEx(userid), GetRankName(userid));    
    return 1;
}

CMD:cargos(playerid, params[])
{
    new faction = PlayerInfo[playerid][pFaction], mes[1024] = "{FFFFFF}";

    if (!isLeader(playerid) && !GetRankRight(playerid))    return SendErrorMessage(playerid, "Você não tem permissão para visualizar os cargos de uma facção.");

    for(new i = 0; i != Factions[faction][fRanks]; i++)
    {
        format(mes, sizeof(mes), "%s%i. %s\n", mes, i+1, FactionRanks[faction][i]);
    }

    new title[32];
    format(title, sizeof(title), "{FFFFFF}Cargos da Facção [%i]", Factions[faction][fRanks]);
    Dialog_Show(playerid, None, DIALOG_STYLE_LIST, title, mes, "Fechar", "");
    return 1;
}

CMD:editarcargos(playerid, params[])
{
    if (!PlayerInfo[playerid][pFaction])                    return SendErrorMessage(playerid, "Você não é membro de uma facção.");
    if (!isLeader(playerid) && !GetRightsRight(playerid))     return SendErrorMessage(playerid, "Você não tem permissão para alterar os direitos de cargo.");

    new faction = PlayerInfo[playerid][pFaction];
    
    new    faction_ranks[1024] = EOS;
    for(new i = 0; i != Factions[faction][fRanks]; i++)
    {
        if (PlayerInfo[playerid][pRank] >= i && !isLeader(playerid))
        {
            format(faction_ranks, sizeof(faction_ranks), "%s%i\t%s\n", faction_ranks, i+1, FactionRanks[faction][i]);
        }
        else if (isLeader(playerid))
        {
            format(faction_ranks, sizeof(faction_ranks), "%s%i\t%s\n", faction_ranks, i+1, FactionRanks[faction][i]);
        }
    }
    Dialog_Show(playerid, FactionSettings, DIALOG_STYLE_TABLIST, "{FFFFFF}Configurações de Cargos", faction_ranks, ">>>", "Cancelar");
    return 1;
}

CMD:meusdireitos(playerid, params[])
{
    if (!PlayerInfo[playerid][pFaction])    return SendErrorMessage(playerid, "Você não é membro de uma facção.");
    return ShowMyRights(playerid);
}


CMD:vfrespawn(playerid, params[])
{
	if (!isLeader(playerid) && !GetTowRight(playerid))
	    return SendErrorMessage(playerid, "Você não tem permissão para enviar veículos não utilizados para o spawn.");

	new faction = PlayerInfo[playerid][pFaction];

	SendFactionMessageF(faction, COLOR_LIGHTRED, "%s %s respawnou todos os veículos não utilizados da facção.", GetRankName(playerid), GetNameEx(playerid));


    new vehicle, occupied[MAX_VEHICLES char];

    foreach(new i : Player)
	{
	    if ((vehicle = GetPlayerVehicleID(i)) == 0 || VehicleInfo[vehicle][carFaction] != faction) continue;

		occupied{vehicle} = 1;
	}

	foreach(new i : Vehicle)
	{
		if (occupied{i} == 1 || VehicleInfo[i][carFaction] != faction) continue;

		RespawnVehicle(i);
	}
	return 1;
}

ALTX:flist("/members","/membros");
CMD:flist(playerid, params[])
{
    if (PlayerInfo[playerid][pFaction] == 0) return SendErrorMessage(playerid, "Você não é membro de uma facção.");

    new mes[3072] = "{FFFFFF}Apelido\t{FFFFFF}ID\t{FFFFFF}Cargo";

    foreach(new i : Player)
    {
        if (IsPlayerLogged(i) == 0 || PlayerInfo[i][pFaction] != PlayerInfo[playerid][pFaction]) continue;
        format(mes, sizeof(mes), "%s\n%s\t%i\t%s", mes, GetNameEx(i), i, GetRankName(i));	
    }

    Dialog_Show(playerid, None, DIALOG_STYLE_TABLIST_HEADERS, "{FFFFFF}Membros da facção online", mes, "Fechar", "");
    return 1;
}


ALTX:departments("/d");
CMD:departments(playerid, params[])
{
	if (PlayerInfo[playerid][pMutedTime] > 0) return SendErrorMessage(playerid, "Seu chat local está bloqueado pelo sistema.");

	new faction[23];

	if (isnull(params)) return SendHelpMessage(playerid, "(/d)epartamentos [mensagem]");
	if (!IsFactionLegal(PlayerInfo[playerid][pFaction])) return SendErrorMessage(playerid, "A equipe é destinada a agências governamentais.");
	if (!PlayerInfo[playerid][pOnDuty]) return SendErrorMessage(playerid, "Primeiro você precisa entrar em serviço.");
	if (!IsPlayerHasItem(playerid, INV_RADIO)) return SendErrorMessage(playerid, "Você não possui um rádio comunicador.");

	format(faction, sizeof(faction), "%s", GetInitials(Factions[PlayerInfo[playerid][pFaction]][fName]));

	for(new i = 1; i != MAX_FACTIONS; i++)
	{
	    if (Factions[i][fON] == 0 || !IsFactionLegal(i)) continue;

        if (strlen(params) > MAXLEN)
		{
			new pos = MAXLEN;

			if (pos < MAXLEN-1) pos = MAXLEN;

			SendFactionMessage(i, COLOR_ALLDEPT, "** [%s] %s %s: %.*s ...", faction, GetRankName(playerid), GetNameWithMask(playerid), pos, params);
			SendFactionMessage(i, COLOR_ALLDEPT, "... %s **", params[pos]);
		}
		else
		{
			SendFactionMessage(i, COLOR_ALLDEPT, "** [%s] %s %s: %s **", faction, GetRankName(playerid), GetNameWithMask(playerid), params);
		}
	}

	if (strlen(params) > MAXLEN)
	{
		new pos = MAXLEN;

		if (pos < MAXLEN-1) pos = MAXLEN;

    	SendLocalMessage(5.0, playerid, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5, "(Rádio) %s diz: %.*s ...", GetNameWithMask(playerid), pos, params);
		SendLocalMessage(5.0, playerid, COLOR_FADE1,COLOR_FADE2,COLOR_FADE3,COLOR_FADE4,COLOR_FADE5, "... %s", params[pos]);
	}
	else
        SendLocalMessage(5.0, playerid, COLOR_FADE1,COLOR_FADE2,COLOR_FADE3,COLOR_FADE4,COLOR_FADE5, "(Rádio) %s diz: %s", GetNameWithMask(playerid), params);
	return 1;
}

ALTX:faction("/f");
CMD:faction(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction];
    
	if (PlayerInfo[playerid][pMutedTime] > 0) return SendErrorMessage(playerid, "O chat local está bloqueado pelo sistema.");
	if (isnull(params)) return SendHelpMessage(playerid, "(/f)acção [chat OOC]");
	if (PlayerInfo[playerid][pFaction] == 0) return SendErrorMessage(playerid, "Você deve estar em uma facção.");
	if (Factions[faction][fChatON] == 0 && !isLeader(playerid)) return SendErrorMessage(playerid, "O chat OOC da facção foi desativado.");
	if (PlayerInfo[playerid][pSettings] & togFaction) return SendErrorMessage(playerid, "Você desabilitou a exibição de mensagens da facção.");
	
	new id = PlayerInfo[playerid][pFaction];

	if (IsFactionLegal(faction))
	{
		if (strlen(params) > MAXLEN)
		{
			new pos = MAXLEN;

			if (pos < MAXLEN-1) pos = MAXLEN;

			SendFactionMessage(PlayerInfo[playerid][pFaction], COLOR_WHITE, "{%06x}** (( %s %s: %.*s ... )) **", Factions[id][fColor] >>> 8, GetRankName(playerid), GetNameEx(playerid), pos, params);
			SendFactionMessage(PlayerInfo[playerid][pFaction], COLOR_WHITE, "{%06x}... %s )) **", Factions[id][fColor] >>> 8, params[pos]);
		}
		else
		{
			SendFactionMessage(PlayerInfo[playerid][pFaction], COLOR_WHITE, "{%06x}** (( %s %s: %s )) **", Factions[id][fColor] >>> 8, GetRankName(playerid), GetNameEx(playerid), params);
		}
	}
	else
	{
		if (strlen(params) > MAXLEN)
		{
			new pos = MAXLEN;

			if (pos < MAXLEN-1) pos = MAXLEN;

			SendFactionMessage(PlayerInfo[playerid][pFaction], COLOR_WHITE, "{%06x}** (( %s: %.*s ... )) **", Factions[id][fColor] >>> 8, GetNameEx(playerid), pos, params);
			SendFactionMessage(PlayerInfo[playerid][pFaction], COLOR_WHITE, "{%06x}... %s )) **", Factions[id][fColor] >>> 8, params[pos]);
		}
		else
		{
			SendFactionMessage(PlayerInfo[playerid][pFaction], COLOR_WHITE, "{%06x}** (( %s: %s )) **", Factions[id][fColor] >>> 8, GetNameEx(playerid), params);
		}
	}
	return 1;
}

ALTX:factionattach("/fa","/fitens");
CMD:factionattach(playerid, params[])
{
    if (GetNextToySlot(playerid) == -1) return SendErrorMessage(playerid, "Todos os slots de roupas estão ocupados.");

    if (!IsFactionLegal(PlayerInfo[playerid][pFaction])) return 1;

    if (IsPlayerFactionMedic(playerid) || IsPlayerFactionFire(playerid))
    {
        new mes[sizeof(attach_fd_model) * 64];
        for (new i = 0; i < sizeof(attach_fd_model); i++)
        {
            format(mes, sizeof(mes), "%s%i(0.0, 0.0, 0.0, 1.0)\t~w~%s\n", mes, attach_fd_model[i], attach_fd_name[i]);
        }
        Dialog_Show(playerid, MODEL_SELECTION_FA, DIALOG_STYLE_PREVIEW_MODEL, "Itens facção", mes, "Selecionar", "Fechar");
    }
    else if (IsPlayerFactionCity(playerid))
    {
        new mes[sizeof(attach_city_model) * 64];
        for (new i = 0; i < sizeof(attach_city_model); i++)
        {
            format(mes, sizeof(mes), "%s%i(0.0, 0.0, 0.0, 1.0)\t~w~%s\n", mes, attach_city_model[i], attach_city_name[i]);
        }
        Dialog_Show(playerid, MODEL_SELECTION_FA, DIALOG_STYLE_PREVIEW_MODEL, "Itens facção", mes, "Selecionar", "Fechar");
    }
    else if (IsPlayerFactionPolice(playerid))
    {
        new mes[sizeof(attach_police_model) * 64];
        for (new i = 0; i < sizeof(attach_police_model); i++)
        {
            format(mes, sizeof(mes), "%s%i(0.0, 0.0, 0.0, 1.0)\t~w~%s\n", mes, attach_police_model[i], attach_police_name[i]);
        }
        Dialog_Show(playerid, MODEL_SELECTION_FA, DIALOG_STYLE_PREVIEW_MODEL, "Itens facção", mes, "Selecionar", "Fechar");
    }

    return 1;
}

CMD:alterardistintivo(playerid, params[])
{
    new faction = PlayerInfo[playerid][pFaction];
    if (Factions[faction][fType] == FACTION_MAFIA || Factions[faction][fType] == FACTION_GANG) return SendErrorMessage(playerid, "Você deve ser o líder da facção estadual.");
    if (PlayerInfo[playerid][pRank] < 11) return SendErrorMessage(playerid, "Disponível a partir do cargo 11.");

    new userid, badge;

    if (sscanf(params, "ui", userid, badge)) return SendHelpMessage(playerid, "/alterardistintivo [ID/Parte do Nome] [número]");
    if (!IsPlayerLogged(userid)) return SendNotConnected(playerid);
    if (badge < 1 || badge > 9999999) return SendErrorMessage(playerid, "Número do distintivo de 1 a 9999999.");

    PlayerInfo[userid][pBadgeNumber] = badge;
    SendSyntaxMessage(playerid, "Você alterou o número do distintivo de %s para [%i].", GetNameEx(userid), badge);
    SendSyntaxMessage(userid, "%s alterou o seu número de distintivo para [%i].", GetNameEx(playerid), badge);

    return 1;
}

CMD:repararcarro(playerid, params[])
{
    if (IsPlayerAtCarfix(playerid) == 0) return SendErrorMessage(playerid, "Você deve estar em uma garagem ou base da organização.");

    new vehicle = GetPlayerVehicleID(playerid);

    if (!vehicle || VehicleInfo[vehicle][carFaction] != PlayerInfo[playerid][pFaction]) return SendErrorMessage(playerid, "Você não está em um veículo da facção.");

    FixVehicle(vehicle);
    VehicleInfo[vehicle][carFuel] = GetVehicleFuelCapacity(GetVehicleModel(vehicle));
    SendSyntaxMessage(playerid, "O veículo foi completamente reparado.");
    return 1;
}

ALTX:bortlist("/bl","/removerobjetos");
CMD:bortlist(playerid, params[])
{
    new faction = PlayerInfo[playerid][pFaction];

    if (!IsFactionLegal(faction)) return SendErrorMessage(playerid, "Você deve ser um policial/agente/paramédico.");
    if (IsPlayerInAnyVehicle(playerid)) return SendErrorMessage(playerid, "Você não deve estar em um veículo.");
    if (!PlayerInfo[playerid][pOnDuty]) return SendErrorMessage(playerid, "Primeiro, você precisa estar de serviço.");

    new count, str[512] = "- Deletar todos os objetos de estrada\n";

    for (new i = 0; i != MAX_BORTES; i ++)
	{
		if (BortInfo[playerid][i][bortOn] == 0) continue;

        format(str, sizeof(str), "%s%i: %s\n", str, i+1, BortInfo[playerid][i][bortName]);
        count++;
	}

    if (!count) return SendErrorMessage(playerid, "Você não criou nenhum objeto de estrada.");

    Dialog_Show(playerid, BortList, DIALOG_STYLE_LIST, "{FFFFFF}Lista de objetos", str, "Selecionar", "Cancelar");
    return 1;
}

CMD:bort(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PhoneInfo[playerid][current_page] == PAGE_NOTIFICATION || PlayerInfo[playerid][pJailed] != 0)
		return SendErrorMessage(playerid, "Você não pode usar esse recurso agora.");
			
	new faction = PlayerInfo[playerid][pFaction];

	if (!IsFactionLegal(faction)) return SendErrorMessage(playerid, "Você deve ser um policial/agente/paramédico.");
	if (IsPlayerInAnyVehicle(playerid)) return SendErrorMessage(playerid, "Você não deve estar em um veículo.");
	if (!PlayerInfo[playerid][pOnDuty]) return SendErrorMessage(playerid, "Primeiro, você precisa estar de serviço.");

	if (IsFactionMedic(faction) || IsFactionFire(faction))
	{
	    new mes[sizeof(bort_list_fd) * 128];
	    for (new i = 0; i < sizeof(bort_list_fd); i++)
	    {
	        format(mes, sizeof(mes), "%s%i(0.0, 0.0, 0.0, 1.0)\t%s\n", mes, bort_list_fd[i], bort_names_fd[i]);
	    }
	    Dialog_Show(playerid, MODEL_SELECTION_BORT, DIALOG_STYLE_PREVIEW_MODEL, "_", mes, "Selecionar", "Fechar");
	}
	else if (IsFactionCity(faction))
	{
	    new mes[sizeof(bort_list_city) * 128];
	    for (new i = 0; i < sizeof(bort_list_city); i++)
	    {
	        format(mes, sizeof(mes), "%s%i(0.0, 0.0, 0.0, 1.0)\t%s\n", mes, bort_list_city[i], bort_names_city[i]);
	    }
	    Dialog_Show(playerid, MODEL_SELECTION_BORT, DIALOG_STYLE_PREVIEW_MODEL, "_", mes, "Selecionar", "Fechar");
	}
	else
	{
	    new mes[sizeof(bort_list) * 128];
	    for (new i = 0; i < sizeof(bort_list); i++)
	    {
	        format(mes, sizeof(mes), "%s%i(0.0, 0.0, 0.0, 1.0)\t%s\n", mes, bort_list[i], bort_names[i]);
	    }
	    Dialog_Show(playerid, MODEL_SELECTION_BORT, DIALOG_STYLE_PREVIEW_MODEL, "_", mes, "Selecionar", "Fechar");
	}
	return 1;
}

ALTX:megaphone("/m");
CMD:megaphone(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction], vehicle = GetPlayerVehicleID(playerid);

	if (!IsFactionLegal(faction)) return SendErrorMessage(playerid, "Você deve estar na facção estatal.");
	if (vehicle == 0 || VehicleInfo[vehicle][carFaction] != PlayerInfo[playerid][pFaction]) return SendErrorMessage(playerid, "Você deve estar em um veículo da facção.");
	if (isnull(params)) return SendHelpMessage(playerid, "(/m)egafone [Mensagem]");

	SendNearbyMessage(Range_VLong, playerid, COLOR_YELLOWG, "[ %s %s:o< %s ]", GetRankName(playerid), GetNameWithMask(playerid), params);
	return 1;
}

CMD:mdc(playerid, params[])
{	
	if (IsPlayerPhoneCreated(playerid))	return SendErrorMessage(playerid, "Remova o celular!");

	if (!IsPlayerFactionPolice(playerid) &&
		!IsPlayerFactionMedic(playerid) &&
		!IsPlayerFactionFire(playerid))	return SendErrorMessage(playerid, "Você deve ser um policial ou paramédico.");

	new id = PlayerInfo[playerid][pFaction],
		vehicle = GetPlayerVehicleID(playerid);

	if (VehicleInfo[vehicle][carFaction] != id && 0 == IsPlayerInRangeOfPoint(playerid, 50.0, Factions[id][fPosX], Factions[id][fPosY], Factions[id][fPosZ]))
	{
		return SendErrorMessage(playerid, "Disponível apenas na sede ou em um veículo. ");
	}

	if (IsPlayerFactionPolice(playerid))
	{
		if (!PlayerInfo[playerid][pOnDuty])	return SendErrorMessage(playerid, "Primeiro você precisa estar de serviço.");

		if (IsPlayerUseMDC(playerid))
			return MDC_Hide(playerid);

		if (pTemp[playerid][pCCTV])
			return 1; // Não é possível usar MDC enquanto monitora CCTV

		MdcInfo[playerid][mdcPage] = PAGE_NONE;

		MDC_Show(playerid);

		SelectTextDraw(playerid, 255);//-1 //0x6d856eFF //-1061109505
		return 1;
	}

	if (IsPlayerFactionMedic(playerid) || IsPlayerFactionFire(playerid))
	{
		if (!PlayerInfo[playerid][pOnDuty])	return SendErrorMessage(playerid, "Primeiro você precisa estar de serviço. ");
		
		return Get_Calls(playerid);
	}
	return 1;
}


CMD:mostrardistintivo(playerid, params[])
{
	new userid;
	
	if (!PlayerInfo[playerid][pFaction] || !IsFactionLegal(PlayerInfo[playerid][pFaction]))	return SendErrorMessage(playerid, "Você não é membro de uma estrutura de poder. ");
	if (sscanf(params, "u", userid))														return SendHelpMessage(playerid, "/mostrardistintivo [ID/Parte do Nome]");
	if (!IsPlayerLogged(userid))															return SendNotConnected(playerid);
	if (!PlayerNearPlayer(3.0, playerid, userid))											return SendNotNearPlayer(playerid);

	SCM(userid, COLOR_POLICECHAT, "_______________________");
	SendClientMessageEx(userid, COLOR_WHITE, Factions[PlayerInfo[playerid][pFaction]][fName]);
	SendClientMessageEx(userid, COLOR_WHITE, "%s - %s", GetRankName(playerid), GetNameWithMask(playerid));
	SendClientMessageEx(userid, COLOR_WHITE, "Distintivo: %i", PlayerInfo[playerid][pBadgeNumber]);
	SCM(userid, COLOR_POLICECHAT, "_______________________");

	SendNearbyMessage(Range_Normal, playerid, COLOR_PURPLE, "* %s mostra seu distintivo a %s", GetNameWithMask(playerid), GetNameWithMask(userid));
	return 1;
}

CMD:colocarviatura(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction], vehicle = GetPlayerVehicleID(playerid), userid, seat;

	if (!IsPlayerFactionPolice(playerid) ||
		!IsPlayerFactionMedic(playerid) ||
		!IsPlayerFactionFire(playerid))						return SendErrorMessage(playerid, "Você deve ser um policial ou paramédico. ");
    if (!vehicle || IsABoat(vehicle) || IsABiker(vehicle))	return SendErrorMessage(playerid, "Você deve estar em um veículo. ");

	new maxseat = GetMaxPassengers(GetVehicleModel(vehicle));

	if (sscanf(params,"ud", userid, seat))			return SendHelpMessage(playerid, "/colocarviatura [ID/Parte do Nome] [assento (1-%i)]", maxseat);
	if (!IsPlayerLogged(userid))						return SendNotConnected(playerid);
	if (seat > maxseat || seat < 1)					return SendSyntaxMessage(playerid, "Assento de 1 a %i.", maxseat);
	if (userid == playerid)							return 1;
	if (!PlayerNearPlayer(10.0, playerid, userid))	return SendNotNearPlayer(playerid);
	if (PlayerInfo[userid][pInjured])				return SendErrorMessage(playerid, "Você não pode transportar um jogador ferido. ");
	if (IsPlayerInAnyVehicle(userid))				return SendErrorMessage(playerid,"O jogador já está em um veículo.");
    if (!PutPlayerInVehicle(userid, vehicle, seat))	return SendErrorMessage(playerid, "Falha ao colocar o jogador no veículo.");

	if (IsFactionMedic(faction) || IsFactionFire(faction))
	{
		StopPlayerAnims(userid);
		SendSyntaxMessage(playerid, "Você colocou %s em um veículo.", GetNameEx(userid));
		SendSyntaxMessage(userid, "Você foi colocado por %s em um veículo.", GetNameEx(playerid));
	}
	else
	{
	    SendSyntaxMessage(playerid, "Você puxou %s para dentro de um veículo.", GetNameEx(userid));
		SendSyntaxMessage(userid, "Você foi puxado por %s para dentro de um veículo.", GetNameEx(playerid));
	}
	return 1;
}


/*
CMD:removerjogador(playerid, params[])
{
	//new faction = PlayerInfo[playerid][pFaction], vehicle = GetPlayerVehicleID(playerid), userid, seat;
	//if (faction != LSPD && faction != SASD && faction != LSFD)	return SendErrorMessage(playerid, "Âû äîëæíû áûòü ïîëèöåéñêèì/ïàðàìåäèêîì.");

	new userid;

	if (sscanf(params,"u", userid))					return SendHelpMessage(playerid, "/removerjogador [ID/Parte do Nome]");
	if (!IsPlayerLogged(userid))						return SendNotConnected(playerid);
	if (userid == playerid)							return 1;
	if (!PlayerNearPlayer(10.0, playerid, userid))	return SendNotNearPlayer(playerid);
	if (!IsPlayerInAnyVehicle(userid))				return SendErrorMessage(playerid, "O jogador não está dentro de um veículo.");
	if (PlayerInfo[userid][pInjured] == 0 &&
		pTemp[userid][pTaserTime] == 0)				return SendErrorMessage(playerid, "O jogador não está ferido/nocauteado/imobilizado.");
	
	new vehid = GetPlayerVehicleID(userid);
	if (VehicleInfo[vehid][carLock] != 0)			return SendErrorMessage(playerid, "O veículo já está trancado. Use /ram !");
	
	new mes[128];
	format(mes, sizeof(mes), "Você removeu %s do veículo.", GetNameWithMask(userid));
	cmd::me(playerid, mes);
	
	SavePlayerPos(playerid);

	SetPlayerPosEx(userid, PlayerInfo[playerid][pPosX],PlayerInfo[playerid][pPosY],PlayerInfo[playerid][pPosZ]);
	SavePlayerPos(userid);

	ApplyAnimation(userid, "PED", "KO_SHOT_STOM", 4.0, 1, 0, 0, 0, 0, 1);

	UnFreezePlayer(userid);
	return 1;
}*/
