CMD:mostrarcontatos(playerid, params[])
{
    if (PlayerInfo[playerid][pNumber] == 0)
        return SendErrorMessage(playerid, "Você não possui um telefone.");

    if (PlayerInfo[playerid][pPhoneOff] != 0)
        return SendErrorMessage(playerid, "O telefone está desligado.");

    new userid;
    if (sscanf(params, "u", userid)) return SendHelpMessage(playerid, "/mostrarcontatos [ID/Parte do Nome]");

    if (!IsPlayerLogged(userid))
        return SendNotConnected(playerid);

    if (!PlayerNearPlayer(3.0, playerid, userid))
        return SendNotNearPlayer(playerid);

    new count;
    for (new i = 0; i < MAX_PLAYER_CONTACTS; i++)
    {
        if (PlayerContacts[playerid][i][contactOn] == 0)
            continue;

        count++;
        break;
    }

    if (count == 0)
        return SendErrorMessage(playerid, "Você não possui contatos.");

    SendClientMessageEx(userid, COLOR_HELPS, "%s mostrou a você uma lista de meus contatos.", GetNameWithMask(playerid));
    SendClientMessageEx(playerid, COLOR_HELPS, "Você mostrou a %s uma lista de seus contatos.", GetNameWithMask(userid));

    count = 0;
    for (new i = 0; i < MAX_PLAYER_CONTACTS; i++)
    {
        if (PlayerContacts[playerid][i][contactOn] == 0)
            continue;

        count++;
        SendClientMessageEx(userid, COLOR_GRAD2, "%i. %s - número de telefone: %i", count, PlayerContacts[playerid][i][contactName], PlayerContacts[playerid][i][contactNumber]);
    }

    new mes[128];
    format(mes, sizeof(mes), "%s mostra os contatos do telefone de %s", GetNameWithMask(playerid), GetNameWithMask(userid));
    SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 15.0, 3000);
    return 1;
}

CMD:mostrarcontato(playerid, params[])
{
    if (PlayerInfo[playerid][pNumber] == 0)
        return SendErrorMessage(playerid, "Você não possui um telefone.");

    if (PlayerInfo[playerid][pPhoneOff] != 0)
        return SendErrorMessage(playerid, "O telefone está desligado.");

    new userid,
        contact[32];

    if (sscanf(params, "us[32]", userid, contact)) return SendHelpMessage(playerid, "/mostrarcontato [ID/Parte do Nome] [contato]");

    if (!IsPlayerLogged(userid))
        return SendNotConnected(playerid);

    if (!PlayerNearPlayer(3.0, playerid, userid))
        return SendNotNearPlayer(playerid);

    new count = -1;
    for (new i = 0; i < MAX_PLAYER_CONTACTS; i++)
    {
        if (PlayerContacts[playerid][i][contactOn] == 0 || !CompareStrings(contact, PlayerContacts[playerid][i][contactName]))
            continue;

        count = i;

        SendClientMessageEx(userid, COLOR_HELPS, "%s mostrou a você um contato - %s.", GetNameWithMask(playerid), PlayerContacts[playerid][i][contactName]);
        SendClientMessageEx(playerid, COLOR_HELPS, "Você mostrou a %s um contato - %s.", GetNameWithMask(userid), PlayerContacts[playerid][i][contactName]);

        SendClientMessageEx(userid, COLOR_GRAD2, "1. %s - número de telefone: %i", PlayerContacts[playerid][i][contactName], PlayerContacts[playerid][i][contactNumber]);

        new mes[128];
        format(mes, sizeof(mes), "%s mostra o contato - %s", GetNameWithMask(playerid), GetNameWithMask(userid));
        SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 15.0, 3000);
        break;
    }

    if (count == -1)
        return SendWarningMessage(playerid, "Este contato não foi encontrado.");

    return 1;
}

CMD:altofalante(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PhoneInfo[playerid][current_page] == PAGE_NOTIFICATION || PlayerInfo[playerid][pJailed] != 0)
		return SendErrorMessage(playerid, "Você não pode usar esta função agora.");

    if (PlayerInfo[playerid][pNumber] == 0)
		return SendErrorMessage(playerid, "Você não possui um telefone.");

	if (pTemp[playerid][pCallLine] == INVALID_PLAYER_ID || pTemp[playerid][pCallStatus] != 2)
	    return SendWarningMessage(playerid, "Você não está em uma ligação telefônica.");

	if (GetPVarInt(playerid, #loud_speak) == 0)
	{
		SetPVarInt(playerid, #loud_speak, 1);
		SM2(playerid, "Você ativou o alto-falante.");
	}
	else
	{
		SetPVarInt(playerid, #loud_speak, 0);
		SM2(playerid, "Você desativou o alto-falante.");
	}
	return 1;
}

ALTX:phone("/ph","/celular");
CMD:phone(playerid, params[])
{
	if (PlayerInfo[playerid][pNumber] == 0)
		return SendErrorMessage(playerid, "Você não tem um telefone.");
		
	if (GetPVarInt(playerid, #pDelay))	return 1;
	if (IsPlayerUseMDC(playerid)) 		return SendErrorMessage(playerid, "Feche o MDC!");

	if (IsPlayerPhoneCreated(playerid))
	{
		if (pTemp[playerid][pCallLine] != INVALID_PLAYER_ID || pTemp[playerid][pEmergency])
			cmd::hangup(playerid, "");
			
	    return Phone_Hide(playerid);
	}
	
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PhoneInfo[playerid][current_page] == PAGE_NOTIFICATION || PlayerInfo[playerid][pJailed] != 0)
		return SendErrorMessage(playerid, "Você não pode usar este recurso agora.");

	if (pTemp[playerid][pPayphone] != -1)
		return SendErrorMessage(playerid, "Você já está usando um orelhão.");
	
	if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS) || IsPlayerHoldingWeapon(playerid))
        return SendWarningMessage(playerid, "Suas mãos já estão ocupadas.");

	if (GetPVarInt(playerid, #selfie))
	    cmd::selfie(playerid, "");

	PhoneInfo[playerid][current_page] = PAGE_NONE;
	PhoneInfo[playerid][phoneCreated] = 0;
	Phone_Show(playerid);

	SelectTextDraw(playerid, COLOR_LIGHTRED);
	return 1;
}


ALTX:phonecursor("/pc","/cursor");
CMD:phonecursor(playerid, params[])
{
	if (IsPlayerPhoneCreated(playerid)) SelectTextDraw(playerid, COLOR_LIGHTRED);
	else SelectTextDraw(playerid, 255);
	return 1;
}

ALTX:sms("/text");
CMD:sms(playerid, params[])
{
    if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PhoneInfo[playerid][current_page] == PAGE_NOTIFICATION || PlayerInfo[playerid][pJailed] != 0)
		return SendErrorMessage(playerid, "Você não pode usar este recurso agora.");
	
	/*if (PlayerInfo[playerid][pJailed] != 0)
		return SCM(playerid, COLOR_WHITE, "Não é possível usar o celular na prisão.");*/

    if (PlayerInfo[playerid][pNumber] == 0)
		return SendErrorMessage(playerid, "Você não tem um celular.");

    if (PlayerInfo[playerid][pPhoneOff] != 0)
        return SendCellMessage(playerid, "O celular está desligado.");

    if (0 == GetPlayerNetwork(playerid))
	    return SendCellMessage(playerid, "Sem conexão.");

	if (PlayerInfo[playerid][pPhoneAirplane] == 1)
	    return SendCellMessage(playerid, "Chamadas e SMS não são possíveis - seu celular está no modo avião.");

	new number_str[24],
		text[128];

	if (sscanf(params,"s[24]s[128]", number_str, text))
		return SendHelpMessage(playerid, "/sms [número do celular / contato da agenda] [texto]");

	SmsNumber(playerid, number_str, text);
	return 1;
}



CMD:selfie(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PhoneInfo[playerid][current_page] == PAGE_NOTIFICATION || PlayerInfo[playerid][pJailed] != 0)
		return SendErrorMessage(playerid, "Você não pode usar este recurso agora.");

	if (!IsPlayerPhoneCreated(playerid) && !GetPVarInt(playerid, #selfie) && IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS))
        return SendWarningMessage(playerid, "Suas mãos já estão ocupadas.");

    if (!GetPVarInt(playerid, #selfie))
    {
		if (pTemp[playerid][pCallLine] != INVALID_PLAYER_ID || IsPlayerInAnyVehicle(playerid))
		    return SendErrorMessage(playerid, "Você não pode usar a câmera no momento.");

        new Float: x,
			Float: y,
			Float: z,
			Float: new_x,
			Float: new_y,
			Float: radius = 0.9,
			Float: rot;

        TogglePlayerControllable(playerid, 0);

	    GetPlayerPos(playerid, x, y, z);
	    GetPlayerFacingAngle(playerid, rot);
     	new_x = x + radius * floatcos(rot, degrees);
	    new_y = y + radius * floatsin(rot, degrees);

		RemovePlayerAttachedObject(playerid, ATTACH_HANDS);
	   	SetPlayerAttachedObject(playerid, ATTACH_HANDS, phone_model[PlayerInfo[playerid][pPhoneModel]], 6, 0.06, 0.01, 0.08, 180.0, 90.0, 0.0);

		SetPlayerCameraPos(playerid, new_x, new_y, z + 0.78);
		SetPlayerCameraLookAt(playerid, x, y, z + 0.60);
	    SetPlayerFacingAngle(playerid, rot -90.0);

		SetPVarFloat(playerid, #selfie_z, 0.60);

		ApplyAnimation(playerid, "PED", "gang_gunstand", 4.1, 1, 1, 1, 1, 1, 1);
  		SetPVarInt(playerid, #selfie, 1);

		SetPlayerArmedWeapon(playerid, 0);

        // As linhas comentadas parecem ser instruções em russo, mantive como comentário.
		
		InfoMSG(playerid, "Pressione ~y~F8~w~ para tirar um screenshot, ~y~F7~w~ para esconder o chat.~n~Segure ~r~W, A, S~w~ e ~r~D~w~ para controlar a camera, ~y~ENTER~w~ para voltar.~n~Use ~r~/headmove~w~ para rotacionar a cabeça do personagem.", 6);
		CancelSelectTextDraw(playerid);

		Phone_Hide(playerid, 1);
		return 1;
 	}


	TogglePlayerControllable(playerid, 1);
 	SetCameraBehindPlayer(playerid);

	RemovePlayerAttachedObject(playerid, ATTACH_HANDS);

	if (IsPlayerPhoneCreated(playerid))
 		SetPlayerAttachedObject(playerid, ATTACH_HANDS, phone_model[PlayerInfo[playerid][pPhoneModel]], 6, 0.111, -0.000999, 0.0, 77.9, -171.6, -3.40001, 1.0, 1.0, 1.0);

	ApplyAnimation(playerid, "PED", "ATM", 4.1, 0, 1, 1, 0, 1, 1);
 	DeletePVar(playerid, #selfie);
	
	new selfiepage = PAGE_HOME;
	if (PhoneInfo[playerid][current_page] != PAGE_NONE)
	{
		selfiepage = PhoneInfo[playerid][current_page];
		PhoneInfo[playerid][current_page] = PAGE_NONE;
	}

	Phone_Show(playerid, selfiepage);
	return 1;
}

ALTX:pickup("/p","/atender");
CMD:pickup(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PhoneInfo[playerid][current_page] == PAGE_NOTIFICATION || PlayerInfo[playerid][pJailed] != 0)
		return SendErrorMessage(playerid, "Você não pode usar este recurso agora.");

    if (PlayerInfo[playerid][pNumber] == 0)
		return SendErrorMessage(playerid, "Você não tem um celular.");

    new targetid = pTemp[playerid][pCallLine];

    if (!IsPlayerPhoneCreated(playerid) && targetid == INVALID_PLAYER_ID && IsPlayerAttachedObjectSlotUsed(playerid, 9))
        return SendWarningMessage(playerid, "Suas mãos já estão ocupadas.");

    if (pTemp[playerid][pCallStatus] == 2)
        return SendCellMessage(playerid, "Você já está em uma chamada.");

	if (targetid == INVALID_PLAYER_ID || pTemp[playerid][pCallStatus] != 1)
        return SendCellMessage(playerid, "Ninguém está ligando para você.");

	if (pTemp[targetid][pCallLine] != playerid)
	    return SendErrorMessage(playerid, "404. [F8 > BUG TRACKER]");

	if (pTemp[playerid][pPayphone] != -1)
		return SendErrorMessage(playerid, "Você está usando um orelhão.");
	//SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USECELLPHONE);

    pTemp[playerid][pCallIncoming] = 1;
    pTemp[targetid][pCallIncoming] = 0;

    pTemp[playerid][pCallStatus] = 2;
    pTemp[targetid][pCallStatus] = 2;

    pTemp[playerid][pCallTime] = 0;
    pTemp[targetid][pCallTime] = 0;

	if (pTemp[targetid][pPayphone] == -1)
	{
		if (!IsPlayerPhoneCreated(targetid)) Phone_Show(targetid, PAGE_CALL);
		else								ph_showpage(targetid, PAGE_CALL);
	}

    if (!IsPlayerPhoneCreated(playerid)) Phone_Show(playerid, PAGE_CALL);
	else								ph_showpage(playerid, PAGE_CALL);

	SM2(playerid,"Você atendeu a chamada.");
	SM2(targetid,"O assinante atendeu a chamada.");

    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USECELLPHONE);
	return 1;
}


ALTX:togphone("/togph");
CMD:togphone(playerid, params[])
{
	SendErrorMessage(playerid, "Use a interface do celular para desligá-lo. ");
	return 1;
}

ALTX:hangup("/hg","/finalizar");
CMD:hangup(playerid, params[])
{
	//if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PhoneInfo[playerid][current_page] == PAGE_NOTIFICATION || PlayerInfo[playerid][pJailed] != 0)
	//	return SendErrorMessage(playerid, "Você não pode usar este recurso agora.");
	
	if (PlayerInfo[playerid][pNumber] == 0 && pTemp[playerid][pPayphone] == -1) 		 return SendErrorMessage(playerid, "Você não tem um celular.");
	if (pTemp[playerid][pCallLine] == INVALID_PLAYER_ID && !pTemp[playerid][pEmergency]) return 1;
	
	if (pTemp[playerid][pPayphone] == -1) {
		TextDrawHideForPlayer(playerid, CallBox[playerid]);
		TextDrawHideForPlayer(playerid, PhNote[1][playerid]);
		PhoneInfo[playerid][phoneCallBox] = 0;	
	}
	
	if (pTemp[playerid][pEmergency]) SendCellMessage(playerid, "Você desligou a chamada.");
	
	new targetid = pTemp[playerid][pCallLine];

	if (!pTemp[playerid][pEmergency] && pTemp[playerid][pCallStatus] == 2)
	{
	    SendCellMessage(playerid, "Você terminou a conversa com  %s.", pTemp[playerid][pCallName]);
	    SendCellMessage(targetid, "Foram ouvidos bips curtos.");
		HangupCall(targetid);
 	}

	else if (!pTemp[playerid][pEmergency] && pTemp[playerid][pCallStatus] == 1)
	{
	    SendCellMessage(targetid, "Você rejeitou uma chamada de saída.");
	    SendCellMessage(playerid, "O celular parou de tocar.");
		HangupCall(targetid);

		/*for(new i = 0; i < sizeof(PhBtn); i++)
		{
			TextDrawColor(PhBtn[i][targetid], phone_btns_color[PlayerInfo[targetid][pPhoneModel]]);
			TextDrawBackgroundColor(PhBtn[i][targetid], phone_btns_color[PlayerInfo[targetid][pPhoneModel]]);
			if (IsPlayerPhoneCreated(targetid)) TextDrawShowForPlayer(targetid, PhBtn[i][targetid]);
		}*/
 	}

	HangupCall(playerid);
	return 1;
}

ALTX:call("/chamar");
CMD:call(playerid, params[])
{		
    if (PlayerInfo[playerid][pNumber] == 0)
		return SendErrorMessage(playerid, "Você não tem um celular.");

    if (PlayerInfo[playerid][pPhoneOff] != 0)
        return SendCellMessage(playerid, "O celular está desligado.");

	if (0 == GetPlayerNetwork(playerid))
	    return SendCellMessage(playerid, "Sem conexão.");

	if (PlayerInfo[playerid][pPhoneAirplane] == 1)
	    return SendCellMessage(playerid, "Chamadas e SMS não são possíveis - seu celular está no modo avião.");

    /*if (PlayerInfo[playerid][pJailed] != 0)
		return SCM(playerid, COLOR_WHITE, "Não é possível usar o celular na prisão.");*/
		
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PhoneInfo[playerid][current_page] == PAGE_NOTIFICATION || PlayerInfo[playerid][pJailed] != 0)
		return SendErrorMessage(playerid, "Você não pode usar este recurso agora.");

    if (pTemp[playerid][pCallLine] != INVALID_PLAYER_ID || pTemp[playerid][pEmergency])
	    return SendCellMessage(playerid, "Você tem uma chamada não finalizada. Digite '/hangup' para desligar.");

    if (!IsPlayerPhoneCreated(playerid) && IsPlayerAttachedObjectSlotUsed(playerid, 9))
        return SendWarningMessage(playerid, "Suas mãos já estão ocupadas.");

	if (pTemp[playerid][pPayphone] != -1)
		return SendErrorMessage(playerid, "Você já está usando um orelhão.");

	new number_str[32];

	if (sscanf(params,"s[32]", number_str))
	{
		SendHelpMessage(playerid, "/call [número / contato / lista (números de serviço)]");

		if (CompareStrings(number_str, "list"))
		{
			SM2(playerid, "Emergência: 911");
			SM2(playerid, "Polícia (para qualquer chamada geral): 991");
			SM2(playerid, "Táxi: 555");
			SM2(playerid, "Serviço de mecânico: 444");
		}
		return 1;
	}

	SetPlayerChatBubble(playerid, "tira o celular do bolso.", COLOR_PURPLE, 20.0, 4000);
	
	
	SetPlayerAttachedObject(playerid, ATTACH_HANDS, phone_model[PlayerInfo[playerid][pPhoneModel]], 6, 0.111, -0.000999, 0.0, 77.9, -171.6, -3.40001, 1.0, 1.0, 1.0);
	SetPlayerCellphoneAction(playerid, true);

	CallNumber(playerid, number_str);
	return 1;
}

ALTX:dropcell("/droparcelular");
CMD:dropcell(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PhoneInfo[playerid][current_page] == PAGE_NOTIFICATION || PlayerInfo[playerid][pJailed] != 0)
		return SendErrorMessage(playerid, "Você não pode usar este recurso agora.");

 	if (PlayerInfo[playerid][pNumber] == 0)
		return SendErrorMessage(playerid, "Você não tem um celular.");

	if (pTemp[playerid][pCallLine] != INVALID_PLAYER_ID || pTemp[playerid][pEmergency] != 0)
	    return SendErrorMessage(playerid, "Termine a chamada - /hangup.");

	if (IsPlayerPhoneCreated(playerid))
		Phone_Hide(playerid);

    new query[128];
   	mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `sms` WHERE `to_number` = %i", PlayerInfo[playerid][pNumber]);
	mysql_tquery(dbHandle, query);

	mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `player_contacts` WHERE `userid` = %i", PlayerInfo[playerid][pID]);
	mysql_tquery(dbHandle, query);

	for(new i; i != MAX_PLAYER_CONTACTS; i++)
	{
	    PlayerContacts[playerid][i][contactNumber] = 0;
		PlayerContacts[playerid][i][contactOn] = 0;
		PlayerContacts[playerid][i][contactID] = 0;
	}

	SendCellMessage(playerid, "Você jogou fora o seu celular.");
	SendHelpMessage(playerid, "Para que você possa usar seu celular novamente - compre um novo chip SIM.");

	SendNearbyMessage(Range_Normal, playerid, COLOR_PURPLE, "* %s jogou fora o celular.", GetNameWithMask(playerid));

	PlayerInfo[playerid][pNumber] = 0;
	PlayerInfo[playerid][pPhoneModel] = 0;
	return 1;
}
