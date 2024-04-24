Player_OnPlayerText(playerid, text[])
{
	new str[144], vehicle = GetPlayerVehicleID(playerid);

	if (pTemp[playerid][pDrugTime]) format(text, 160, "%s", MulStringHandler(text));

	if (pTemp[playerid][pEmergency] == 544)
	{
	    if (!strlen(text) || CheckTextDrawString(text))
	    {
	        SCM(playerid, COLOR_YELLOWG, "Despachante (celular): Infelizmente, não entendi você, por favor, repita. ");
	        return 0;
	    }

	    SendMechanicHelp(playerid, text);

	    SCM(playerid, COLOR_YELLOWG, "Despachante (celular): Nossos funcionários foram notificados do seu chamado, aguarde no local. ");

	    pTemp[playerid][pEmergency] = 0;
	    pTemp[playerid][pCallStatus] = 0;
	    SendCellMessage(playerid, "Eles desligaram.");

	    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_STOPUSECELLPHONE);
	    if (pTemp[playerid][pPayphone] != -1) cancelPayphone(playerid);
	    else
	    {
	        RemovePlayerAttachedObject(playerid, ATTACH_HANDS);
	        Phone_Hide(playerid);
	    }
	    return 0;
	}
	else if (pTemp[playerid][pEmergency] == 555)
	{
	    if (!strlen(text) || CheckTextDrawString(text))
	    {
	        SCM(playerid, COLOR_YELLOWG, "Despachante (celular): Infelizmente, não entendi você, por favor, repita. ");
	        return 0;
	    }

	    SendTaxiRequest(playerid, text);

	    SCM(playerid, COLOR_YELLOWG, "Despachante (celular): Nossos motoristas foram notificados do seu chamado, aguarde no local. ");

	    SetPVarInt(playerid, "NeedTaxi", 1);

	    pTemp[playerid][pEmergency] = 0;
	    pTemp[playerid][pCallStatus] = 0;
	    SendCellMessage(playerid, "Eles desligaram.");

	    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_STOPUSECELLPHONE);
	    if (pTemp[playerid][pPayphone] != -1) cancelPayphone(playerid);
	    else
	    {
	        RemovePlayerAttachedObject(playerid, ATTACH_HANDS);
	        Phone_Hide(playerid);
	    }
	    return 0;
	}
	else if (pTemp[playerid][pEmergency] == 911)
	{
	    if (!strlen(text) || CheckTextDrawString(text))
	    {
	        SCM(playerid, COLOR_YELLOWG, "Operador 911 (celular): Infelizmente, não entendi você, por favor, repita. ");
	        return 0;
	    }

	    if (strval(text) != 1 && strval(text) != 2 && strval(text) != 3)
	    {
	        SCM(playerid, COLOR_YELLOWG, "Operador 911 (celular): Repita ... Qual departamento você precisa? ");
	        return 0;
	    }

	    SCM(playerid, COLOR_YELLOWG, "Operador 911 (celular): Por favor, descreva sua localização e situação. ");
	    pTemp[playerid][pEmergency] = 911 + strval(text);
	    return 0;
	}

	else if (pTemp[playerid][pEmergency] == 912)
	{
	    if (!strlen(text) || CheckTextDrawString(text))
	    {
	        SCM(playerid, COLOR_YELLOWG, "Operador 911 (celular): Infelizmente, não entendi. Por favor, repita o que aconteceu? ");
	        return 0;
	    }        

	    new location[MAX_ZONE_NAME];
	    GetPlayer2DZone(playerid, location, MAX_ZONE_NAME);

	    SendFactionMessageF(FACTION_POLICE, COLOR_LIGHTBLUE, "|__________Chamada de Emergência__________|");

	    if (pTemp[playerid][pPayphone] == -1)
	    {
	        SendFactionMessageF(FACTION_POLICE, COLOR_LIGHTBLUE, "Chamada de: %s, Tel: %i", GetNameWithMask(playerid), PlayerInfo[playerid][pNumber]);
	    }
	    else
	    {
	        SendFactionMessageF(FACTION_POLICE, COLOR_LIGHTBLUE, "Chamada de: Cabine Telefônica, Tel: %i", PayphoneInfo[pTemp[playerid][pPayphone]][phNumber]);
	    }

	    SendFactionMessageF(FACTION_POLICE, COLOR_LIGHTBLUE, "Solicita: Polícia");
	    SendFactionMessageF(FACTION_POLICE, COLOR_LIGHTBLUE, "Localização: %s", location);
	    if (strlen(text) > MAXLEN)
	    {
	        SendFactionMessageF(FACTION_POLICE, COLOR_LIGHTBLUE, "Situação: %.*s ...", MAXLEN, text);
	        SendFactionMessageF(FACTION_POLICE, COLOR_LIGHTBLUE, "Situação: ... %s", text[MAXLEN]);
	    }
	    else
	        SendFactionMessageF(FACTION_POLICE, COLOR_LIGHTBLUE, "Situação: %s", text);

	    Add_Call(playerid, 1, text);

	    SCM(playerid, COLOR_YELLOWG, "Operador 911 (celular): Notificamos todas as subdivisões em sua área, por favor, permaneça onde está. ");
	    
	    pTemp[playerid][pEmergency] = 0;
	    pTemp[playerid][pCallStatus] = 0;
	    SendCellMessage(playerid, "Eles desligaram.");

	    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_STOPUSECELLPHONE);
	    if (pTemp[playerid][pPayphone] != -1) cancelPayphone(playerid);
	    else
	    {
	        RemovePlayerAttachedObject(playerid, ATTACH_HANDS);
	        Phone_Hide(playerid);
	    }
	    return 0;
	}
	else if (pTemp[playerid][pEmergency] == 913)
	{
	    if (!strlen(text) || CheckTextDrawString(text))
	    {
	        SCM(playerid, COLOR_YELLOWG, "Operador 911 (celular): Infelizmente, não entendi. Por favor, repita o que aconteceu?");
	        return 0;
	    }
	    new location[MAX_ZONE_NAME];
	    GetPlayer2DZone(playerid, location, MAX_ZONE_NAME);

	    for(new i = FACTION_MEDICS; i <= FACTION_FIRE; i++)
	    {
	        SendFactionMessageF(i, LSFD_DUTY, "|__________Chamada de Emergência__________|"); //������
	        if (pTemp[playerid][pPayphone] == -1)
	        {
	            SendFactionMessageF(i, LSFD_DUTY, "Chamada de: %s, Tel: %i", GetNameWithMask(playerid), PlayerInfo[playerid][pNumber]);
	        }
	        else
	        {
	            SendFactionMessageF(i, LSFD_DUTY, "Chamada de: Cabine Telefônica, Tel: %i", PayphoneInfo[pTemp[playerid][pPayphone]][phNumber]);
	        }
	        SendFactionMessageF(i, LSFD_DUTY, "Solicita: Policia, Ambulancia / Bombeiros");
	        SendFactionMessageF(i, LSFD_DUTY, "Localização: %s", location);
	        if (strlen(text) > MAXLEN)
	        {
	            SendFactionMessageF(i, LSFD_DUTY, "Situação: %.*s ...", MAXLEN, text);
	            SendFactionMessageF(i, LSFD_DUTY, "Situação: ... %s", text[MAXLEN]);
	        }
	        else
	            SendFactionMessageF(i, LSFD_DUTY, "Situação: %s", text);
	    }

	    Add_Call(playerid, 2, text);

	    SCM(playerid, COLOR_YELLOWG, "Operador 911 (celular): Notificamos todas as equipes em sua área, por favor, permaneça onde está. ");
	    
	    pTemp[playerid][pEmergency] = 0;
	    pTemp[playerid][pCallStatus] = 0;
	    SendCellMessage(playerid, "Eles desligaram.");

	    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_STOPUSECELLPHONE);
	    if (pTemp[playerid][pPayphone] != -1) cancelPayphone(playerid);
	    else
	    {
	        RemovePlayerAttachedObject(playerid, ATTACH_HANDS);
	        Phone_Hide(playerid);
	    }
	    return 0;
	}

	else if (pTemp[playerid][pEmergency] == 914)
	{
	    if (!strlen(text) || CheckTextDrawString(text))
	    {
	        SCM(playerid, COLOR_YELLOWG, "Operador 911 (celular): Infelizmente, não entendi. Por favor, repita o que aconteceu? ");
	        return 0;
	    }

	    new location[MAX_ZONE_NAME];
	    GetPlayer2DZone(playerid, location, MAX_ZONE_NAME);
	    
	    for(new i = FACTION_POLICE; i <= FACTION_FIRE; i++)
	    {
	        SendFactionMessage(i, i == FACTION_POLICE ? COLOR_LIGHTBLUE : LSFD_DUTY, "|__________Chamada de Emergência__________|");
	        printf("-1");
	        if (pTemp[playerid][pPayphone] == -1)
	        {
	            printf("0");
	            SendFactionMessage(i, i == FACTION_POLICE ? COLOR_LIGHTBLUE : LSFD_DUTY, "Chamada de: %s, Tel: %i", "hoe", 1);//, GetNameWithMask(playerid), PlayerInfo[playerid][pNumber]);
	            printf("1");
	        }
	        else
	        {
	            SendFactionMessage(i, i == FACTION_POLICE ? COLOR_LIGHTBLUE : LSFD_DUTY, "Chamada de: Cabine Telefônica, Tel: %i", PayphoneInfo[pTemp[playerid][pPayphone]][phNumber]);
	        }
	        SendFactionMessage(i, i == FACTION_POLICE ? COLOR_LIGHTBLUE : LSFD_DUTY, "Solicita: Polícia, Ambulância / Bombeiros");
	        SendFactionMessage(i, i == FACTION_POLICE ? COLOR_LIGHTBLUE : LSFD_DUTY, "Localização: %s", location);
	        if (strlen(text) > MAXLEN)
	        {
	            SendFactionMessage(i, i == FACTION_POLICE ? COLOR_LIGHTBLUE : LSFD_DUTY, "Situação: %.*s ...", MAXLEN, text);
	            SendFactionMessage(i, i == FACTION_POLICE ? COLOR_LIGHTBLUE : LSFD_DUTY, "Situação: ... %s", text[MAXLEN]);
	        }
	        else
	            SendFactionMessage(i, i == FACTION_POLICE ? COLOR_LIGHTBLUE : LSFD_DUTY, "Situação: %s", text);
	    }

	    Add_Call(playerid, 3, text);

	    SCM(playerid, COLOR_YELLOWG, "Operador 911 (celular): Notificamos todas as subdivisões em sua área, por favor, permaneça onde está. ");
	    
	    pTemp[playerid][pEmergency] = 0;
	    pTemp[playerid][pCallStatus] = 0;
	    SendCellMessage(playerid, "Eles desligaram.");

	    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_STOPUSECELLPHONE);
	    if (pTemp[playerid][pPayphone] != -1) cancelPayphone(playerid);
	    else
	    {
	        RemovePlayerAttachedObject(playerid, ATTACH_HANDS);
	        Phone_Hide(playerid);
	    }
	    return 0;
	}
	else if (pTemp[playerid][pEmergency] == 991)
	{
	    if (!strlen(text) || CheckTextDrawString(text))
	    {
	        SCM(playerid, COLOR_YELLOWG, "Operador 911 (celular): Infelizmente, não entendi. ");
	        return 0;
	    }

	    new location[MAX_ZONE_NAME];
	    GetPlayer2DZone(playerid, location, MAX_ZONE_NAME);
	    foreach(new i : Player)
	    {
	        if (!IsPlayerLogged(i) || !IsPlayerFactionPolice(i) || (PlayerInfo[i][pSettings] & togFaction))
	            continue;

	        SendClientMessage(i, COLOR_POLICECHAT, "|_____________Chamada de Emergência_____________|");
	        if (pTemp[playerid][pPayphone] == -1)
	        {
	            SendFactionMessage(i, COLOR_POLICECHAT, "Chamada de: %s, Tel: %i", GetNameWithMask(playerid), PlayerInfo[playerid][pNumber]);
	        }
	        else
	        {
	            SendFactionMessage(i, COLOR_POLICECHAT, "Chamada de: Cabine Telefônica, Tel: %i", PayphoneInfo[pTemp[playerid][pPayphone]][phNumber]);
	        }
	        SendClientMessageEx(i, COLOR_POLICECHAT, "Localização: %s", location);
	        if (strlen(text) > MAXLEN)
	        {
	            SendClientMessageEx(i, COLOR_POLICECHAT, "Situação: %.*s ...", MAXLEN, text);
	            SendClientMessageEx(i, COLOR_POLICECHAT, "Situação: ... %s", text[MAXLEN]);
	        }
	        else
	            SendClientMessageEx(i, COLOR_POLICECHAT, "Situação: %s", text);
	    }
	    GetPlayerPos(playerid, pTemp[playerid][pCallPosX], pTemp[playerid][pCallPosY], pTemp[playerid][pCallPosZ]);
	    
	    SendFactionMessageF(FACTION_POLICE, COLOR_POLICECHAT, "DICA:{FFFFFF} Para aceitar a chamada, digite /ane %i", playerid);
	    
	    pTemp[playerid][pCallOn] = 1;
	    
	    SCM(playerid, COLOR_YELLOWG, "Despachante (celular): Notificamos todas as patrulhas em sua área, por favor, permaneça onde está. ");
	    
	    pTemp[playerid][pEmergency] = 0;
	    pTemp[playerid][pCallStatus] = 0;
	    SendCellMessage(playerid, "Eles desligaram.");

	    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_STOPUSECELLPHONE);
	    if (pTemp[playerid][pPayphone] != -1) cancelPayphone(playerid);
	    else
	    {
	        RemovePlayerAttachedObject(playerid, ATTACH_HANDS);
	        Phone_Hide(playerid);
	    }
	    return 0;
	}


	if (pTemp[playerid][pInterview] != INVALID_PLAYER_ID)
	{
	    if (strlen(text) > MAXLEN)
	    {
	        format(str, sizeof(str), "** [SAN] Entrevistado %s: %.*s ...", GetNameWithMask(playerid), MAXLEN, text);
	        NewsChat(0x04FB5AAA, str);
	        format(str, sizeof(str), "... %s", text[MAXLEN]);
	        NewsChat(0x04FB5AAA, str);
	    }
	    else
	    {
	        format(str, sizeof(str), "** [SAN] Entrevistado %s: %s", GetNameWithMask(playerid), text);
	        NewsChat(0x04FB5AAA, str);
	    }
	    return 0;
	}
	else if (pTemp[playerid][pNews] == 1)
	{
	    if (strlen(text) > MAXLEN)
	    {
	        format(str, sizeof(str), "** [SAN] Editor %s: %.*s ...", GetNameWithMask(playerid), MAXLEN, text);
	        NewsChat(0x04FB5AAA, str);
	        format(str, sizeof(str), "... %s", text[MAXLEN]);
	        NewsChat(0x04FB5AAA, str);
	    }
	    else
	    {
	        format(str, sizeof(str), "** [SAN] Editor %s: %s", GetNameWithMask(playerid), text);
	        NewsChat(0x04FB5AAA, str);
	    }
	    return 0;
	}


	if (IsPlayerInAnyVehicle(playerid) && !IsADontWindow(vehicle))
	{
	    if (0 != IsWindowsClose(GetPlayerVehicleID(playerid))) {
	        if (strlen(text) > MAXLEN - 15)
	        {
	            format(str, sizeof(str), "%s diz (veículo %s): %.*s ...", GetNameWithMask(playerid), ((pTemp[playerid][pCallLine] != INVALID_PLAYER_ID && pTemp[playerid][pCallStatus] == 2) || pTemp[playerid][pEmergency])?(" / celular"):(""), MAXLEN - 15, text);
	            SendVehicleMessage(GetPlayerVehicleID(playerid), COLOR_FADE1, str);
	            format(str, sizeof(str), "... %s", text[MAXLEN - 15]);
	            SendVehicleMessage(GetPlayerVehicleID(playerid), COLOR_FADE1, str);
	        }
	        else
	        {
	            format(str, sizeof(str), "%s diz (veículo %s): %s", GetNameWithMask(playerid), ((pTemp[playerid][pCallLine] != INVALID_PLAYER_ID && pTemp[playerid][pCallStatus] == 2) || pTemp[playerid][pEmergency])?(" / celular"):(""), text);
	            SendVehicleMessage(GetPlayerVehicleID(playerid), COLOR_FADE1, str);
	        }
	    }
	    else
	    {
	        if (strlen(text) > MAXLEN - 15)
	        {
	            format(str, sizeof(str), "%s diz (veículo %s): %.*s ...", GetNameWithMask(playerid), ((pTemp[playerid][pCallLine] != INVALID_PLAYER_ID && pTemp[playerid][pCallStatus] == 2) || pTemp[playerid][pEmergency])?(" / celular"):(""), MAXLEN - 15, text);
	            SendLocalMessage(Range_VShort, playerid, COLOR_FADE1, COLOR_FADE1, COLOR_FADE1, COLOR_FADE1, COLOR_FADE1, str);
	            format(str, sizeof(str), "... %s", text[MAXLEN - 15]);
	            SendLocalMessage(Range_VShort, playerid, COLOR_FADE1, COLOR_FADE1, COLOR_FADE1, COLOR_FADE1, COLOR_FADE1, str);
	        }
	        else
	        {
	            format(str, sizeof(str), "%s diz (veículo %s): %s", GetNameWithMask(playerid), ((pTemp[playerid][pCallLine] != INVALID_PLAYER_ID && pTemp[playerid][pCallStatus] == 2) || pTemp[playerid][pEmergency])?(" / celular"):(""), text);
	            SendLocalMessage(Range_VShort, playerid, COLOR_FADE1, COLOR_FADE1, COLOR_FADE1, COLOR_FADE1, COLOR_FADE1, str);
	        }
	    }
	}

	else
	{
		if (strlen(text) > MAXLEN)
		{
		    if ((pTemp[playerid][pCallLine] != INVALID_PLAYER_ID && pTemp[playerid][pCallStatus] == 2) || pTemp[playerid][pEmergency])
		    {
		        SendLocalMessage(Range_Normal, playerid, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5, "%s diz (celular): %.*s ...", GetNameWithMask(playerid), MAXLEN, text);
		        SendLocalMessage(Range_Normal, playerid, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5, "... %s", text[MAXLEN]);
		    }
		    else
		    {
		        SendLocalMessage(Range_Normal, playerid, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5, "%s diz: %.*s ...", GetNameWithMask(playerid), MAXLEN, text);
		        SendLocalMessage(Range_Normal, playerid, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5, "... %s", text[MAXLEN]);
		    }
		}
		else
		{
		    if ((pTemp[playerid][pCallLine] != INVALID_PLAYER_ID && pTemp[playerid][pCallStatus] == 2) || pTemp[playerid][pEmergency])
		        SendLocalMessage(Range_Normal, playerid, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5, "%s diz (celular): %s", GetNameWithMask(playerid), text);
		    else
		        SendLocalMessage(Range_Normal, playerid, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5, "%s diz: %s", GetNameWithMask(playerid), text);
		}
	}
	ChatAnimation(playerid, strlen(text));

	if (pTemp[playerid][pCallLine] != INVALID_PLAYER_ID && pTemp[playerid][pCallStatus] == 2)
	{
	    new targetid = pTemp[playerid][pCallLine];
	    if (IsPlayerLogged(targetid) == 0)
	    {
	        HangupCall(playerid);
	        return 0;
	    }
		if (strlen(text) > MAXLEN)
		{
		    SendClientMessageEx(targetid, COLOR_YELLOWG, "%s diz (celular): %.*s ...", pTemp[targetid][pCallName], MAXLEN, text);
		    SendClientMessageEx(targetid, COLOR_YELLOWG, "%s diz (celular): ... %s", pTemp[targetid][pCallName], text[MAXLEN]);

		    if (GetPVarInt(targetid, #loud_speak) == 1)
		    {
		        SendLocalMessage(10.0, targetid, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5, "%s diz (celular): %.*s ...", pTemp[targetid][pCallName], MAXLEN, text);
		        SendLocalMessage(10.0, targetid, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5, "%s diz (celular): ... %s", pTemp[targetid][pCallName], text[MAXLEN]);
		    }
		}
		else
		{
		    SendClientMessageEx(targetid, COLOR_YELLOWG, "%s diz (celular): %s", pTemp[targetid][pCallName], text);

		    if (GetPVarInt(targetid, #loud_speak) == 1)
		        SendLocalMessage(10.0, targetid, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5, "%s diz (celular): %s", pTemp[targetid][pCallName], text);
		}
	}
    return 1;
}


Phone_OnPlayerUpdate(playerid)
{
    if (GetPVarInt(playerid, #selfie))
	{
		new keys[3];
		GetPlayerKeys(playerid, keys[0], keys[1], keys[2]);

	    if (keys[1] == KEY_UP)
	    {
	        new Float: pz =
	        	GetPVarFloat(playerid, #selfie_z);

			pz += 0.01;
			if (pz < 0.70)
			{
			    new Float: rot;
     			GetPlayerFacingAngle(playerid, rot);

                SetPVarFloat(playerid, #selfie_z, pz);

		        new Float: x,
					Float: y,
					Float: z,
					Float: new_x,
					Float: new_y,
					Float: radius = 0.9;

			    GetPlayerPos(playerid, x, y, z);
		        new_x = x + radius * floatcos(rot - 270.0, degrees);
		        new_y = y + radius * floatsin(rot - 270.0, degrees);

				SetPlayerCameraPos(playerid, new_x, new_y, z + 0.78);
				SetPlayerCameraLookAt(playerid, x, y, z + pz);
				SetPlayerFacingAngle(playerid, rot);
			}
	    }
	    else if (keys[1] == KEY_DOWN)
	    {
	        new Float: pz =
	        	GetPVarFloat(playerid, #selfie_z);

			pz -= 0.01;
			if (pz > 0.50)
			{
			    new Float: rot;
     			GetPlayerFacingAngle(playerid, rot);

                SetPVarFloat(playerid, #selfie_z, pz);

		        new Float: x,
					Float: y,
					Float: z,
					Float: new_x,
					Float: new_y,
					Float: radius = 0.9;

			    GetPlayerPos(playerid, x, y, z);
		        new_x = x + radius * floatcos(rot - 270.0, degrees);
		        new_y = y + radius * floatsin(rot - 270.0, degrees);

				SetPlayerCameraPos(playerid, new_x, new_y, z + 0.78);
				SetPlayerCameraLookAt(playerid, x, y, z + pz);
				SetPlayerFacingAngle(playerid, rot);
			}
	    }
	    if (keys[2] == KEY_LEFT)
	    {
	        new Float: rot;
	        GetPlayerFacingAngle(playerid, rot);

			rot += 1.0;
			if (rot >= 359.0) rot = 0.0;

	        new Float: x,
				Float: y,
				Float: z,
				Float: new_x,
				Float: new_y,
				Float: radius = 0.9;

		    GetPlayerPos(playerid, x, y, z);
	        new_x = x + radius * floatcos(rot - 270.0, degrees);
	        new_y = y + radius * floatsin(rot - 270.0, degrees);

			SetPlayerCameraPos(playerid, new_x, new_y, z + 0.78);
			SetPlayerCameraLookAt(playerid, x, y, z + GetPVarFloat(playerid, #selfie_z));
			SetPlayerFacingAngle(playerid, rot);
	    }
	    else if (keys[2] == KEY_RIGHT)
	    {
	        new Float: rot;
	        GetPlayerFacingAngle(playerid, rot);

	        rot -= 1.0;
			if (rot <= 1.0) rot = 359.0;

	        new Float: x,
				Float: y,
				Float: z,
				Float: new_x,
				Float: new_y,
				Float: radius = 0.9;

		    GetPlayerPos(playerid, x, y, z);
	        new_x = x + radius * floatcos(rot - 270.0, degrees);
	        new_y = y + radius * floatsin(rot - 270.0, degrees);

			SetPlayerCameraPos(playerid, new_x, new_y, z + 0.78);
			SetPlayerCameraLookAt(playerid, x, y, z + GetPVarFloat(playerid, #selfie_z));
			SetPlayerFacingAngle(playerid, rot);
	    }
	}
    return 1;
}

Ph_OnPlayerClickTextDraw(playerid, Text:clickedid)
{
	if (IsPlayerPhoneCreated(playerid) && 
		PhoneInfo[playerid][current_page] != PAGE_NOTIFICATION)
		
	{
		if (clickedid == PhOFF[playerid])
		{
			Phone_Switch(playerid);
			return 1;
		}

		if (PlayerInfo[playerid][pPhoneOff] == 0)
		{
			if (clickedid == PhNewMSG[playerid])
			{
				ph_showpage(playerid, PAGE_NEW_MESSAGES);

				if (PhoneInfo[playerid][current_page] == PAGE_HOME)
				{
					for(new d = 0; d < sizeof(PhoneTDdisplay); d++) TextDrawHideForPlayer(playerid, PhoneTDdisplay[d][playerid]);
				}
				return 1;
			}

			if (clickedid == Ph_selfie[playerid])
			{
				cmd::selfie(playerid, "");
				return 1;
			}

			for(new i = 0; i < sizeof(PhBtn); i++)
			{
				if (clickedid == PhBtn[i][playerid])
				{
					switch(PhoneInfo[playerid][current_page])
					{
						case PAGE_HOME:
						{
							switch (i)
							{
								case 0: ph_showpage(playerid, PAGE_MENU_DEFAULT);
								case 1: Phone_Hide(playerid);
							}
							return 1;
						}
						case PAGE_MENU_DEFAULT:
						{
							switch (i)
							{
								case 0: return 1;						 // MENU
								case 1: ph_showpage(playerid, PAGE_HOME);// BACK
							}
							return 1;
						}
						case PAGE_MENU_CONTACTS:
						{
							switch (i)
							{
								case 0: return 1;								 // MENU
								case 1: ph_showpage(playerid, PAGE_MENU_DEFAULT);// BACK
							}
							return 1;
						}
						case PAGE_MENU_CALLS:
						{
							switch (i)
							{
								case 0: return 1;						 		// MENU
								case 1: ph_showpage(playerid, PAGE_MENU_DEFAULT);// BACK
							}
							return 1;
						}
						case PAGE_MENU_SMS:
						{
							switch (i)
							{
								case 0: return 1;								 // MENU
								case 1: ph_showpage(playerid, PAGE_MENU_DEFAULT);// BACK
							}
							return 1;
						}
						case PAGE_MENU_SETTINGS:
						{
							switch (i)
							{
								case 0: return 1;								 // MENU
								case 1: ph_showpage(playerid, PAGE_MENU_DEFAULT);// BACK
							}
							return 1;
						}
						case PAGE_CONTACT_INFO:
						{
							switch (i)
							{
								case 0: return 1;							      // MENU
								case 1: ph_showpage(playerid, PAGE_LIST_CONTACTS);// BACK
							}
							return 1;
						}
						case PAGE_LIST_CALLS:
						{
							switch (i)
							{
								case 0: return 1;						 		// MENU
								case 1: ph_showpage(playerid, PAGE_MENU_CALLS);// BACK
							}
							return 1;
						}
						case PAGE_LIST_CONTACTS:
						{
							switch(i)
							{
								case 0: return 1;
								case 1: ph_showpage(playerid, PAGE_MENU_CONTACTS);
							}
						}
						case PAGE_LIST_CONTACTS_FOR_CALL:
						{
							switch (i)
							{
								case 0: return 1;						 		// MENU
								case 1: ph_showpage(playerid, PAGE_MENU_CALLS);// BACK
							}
							return 1;
						}
						case PAGE_LIST_ARCH:
						{
							switch (i)
							{
								case 0: return 1;						 	  // MENU
								case 1: ph_showpage(playerid, PAGE_MENU_SMS);// BACK
							}
							return 1;
						}
						case PAGE_LIST_SMS:
						{
							switch (i)
							{
								case 0: return 1;						 	// MENU
								case 1:
								{
									for(new j = 0; j < sizeof(PhoneMenuBg); j++)
									{
										TextDrawColor(PhoneMenuBg[j][playerid], phone_panel[PlayerInfo[playerid][pPhoneStyle]]);
										TextDrawBackgroundColor(PhoneMenuBg[j][playerid], phone_panel[PlayerInfo[playerid][pPhoneStyle]]);
										TextDrawShowForPlayer(playerid, PhoneMenuBg[j][playerid]);
									}
									ph_showpage(playerid, PAGE_MENU_SMS);// BACK
								}
							}
							return 1;
						}

						case PAGE_NEW_MESSAGES:
						{
							switch (i)
							{
								case 0: return 1;						 	// MENU
								case 1:
								{
									for(new j = 0; j < sizeof(PhoneMenuBg); j++)
									{
										TextDrawColor(PhoneMenuBg[j][playerid], phone_panel[PlayerInfo[playerid][pPhoneStyle]]);
										TextDrawBackgroundColor(PhoneMenuBg[j][playerid], phone_panel[PlayerInfo[playerid][pPhoneStyle]]);
										TextDrawShowForPlayer(playerid, PhoneMenuBg[j][playerid]);
									}
									ph_showpage(playerid, PAGE_HOME);// BACK
								}
							}
							return 1;
						}

						case PAGE_LIST_CONTACTS_FOR_SMS:
						{
							switch (i)
							{
								case 0: return 1;							 // MENU
								case 1: ph_showpage(playerid, PAGE_MENU_SMS);// BACK
							}
							return 1;
						}
						case PAGE_CALL:
						{
							switch (i)
							{
								case 0: ph_showpage(playerid, PAGE_MENU_DEFAULT);// MENU
								case 1: cmd::hangup(playerid, "");				// BACK
							}
							return 1;
						}
						case PAGE_INCOMING_CALL:
						{
							switch (i)
							{
								case 0: cmd::pickup(playerid, "");// MENU
								case 1: cmd::hangup(playerid, "");// BACK
							}
							return 1;
						}
						case PAGE_OUTCOMING_CALL:
						{
							switch (i)
							{
								case 0: return 1;
								case 1: cmd::hangup(playerid, "");// BACK
							}
							return 1;
						}
						case PAGE_PHONE_STYLE:
						{
							switch (i)
							{
								case 0: return 1;									// MENU
								case 1: ph_showpage(playerid, PAGE_MENU_SETTINGS);// BACK
							}
							return 1;
						}
						case PAGE_PHONE_MELODY:
						{
							switch (i)
							{
								case 0: return 1;						// MENU
								case 1:
								{
									PlayerPlaySound(playerid, 1184, 0.0, 0.0, 0.0);
									ph_showpage(playerid, PAGE_MENU_SETTINGS);
								}
							}
							return 1;
						}
					}
				}
			}
			
			for(new j = 0; j < sizeof(PhoneMenuBg); j++)
			{
				if (clickedid == PhoneMenuBg[j][playerid])
				{
					switch(PhoneInfo[playerid][current_page])
					{
						case PAGE_MENU_DEFAULT:
						{
							switch (j)
							{
								case 0: ph_showpage(playerid, PAGE_MENU_CONTACTS);
								case 1: ph_showpage(playerid, PAGE_MENU_CALLS);
								case 2:	ph_showpage(playerid, PAGE_MENU_SMS);
								case 3:	ph_showpage(playerid, PAGE_MENU_SETTINGS);
							}
							return 1;
						}
						case PAGE_MENU_CONTACTS:
						{
						    switch (j)
						    {
						        case 0: ph_showpage(playerid, PAGE_LIST_CONTACTS);
						        case 1: Dialog_Show(playerid, AddContact, DIALOG_STYLE_INPUT, "Adicionar contato", "Novo contato\n\n\tDigite o número do contato:", ">>>", "Cancelar");
						        case 2: return 1;
						        case 3: return 1;
						    }
						    return 1;
						}
						case PAGE_MENU_CALLS:
						{
						    switch (j)
						    {
						        case 0: ph_showpage(playerid, PAGE_LIST_CONTACTS_FOR_CALL);
						        case 1: Dialog_Show(playerid, DialogCall, DIALOG_STYLE_INPUT, "Chamada","Chamando um número de celular\n\n\tDigite o número/nome do contato:", ">>>", "Cancelar");
						        case 2: ph_showpage(playerid, PAGE_LIST_CALLS);
						        case 3: return 1;
						    }
						    return 1;
						}
						case PAGE_MENU_SMS:
						{
						    switch (j)
						    {
						        case 0: ph_showpage(playerid, PAGE_LIST_CONTACTS_FOR_SMS);
						        case 1: Dialog_Show(playerid, SendSMS, DIALOG_STYLE_INPUT, "Enviar mensagem", "Nova mensagem\n\n\tDigite o número/nome do contato:", ">>>", "Cancelar");
						        case 2: ph_showpage(playerid, PAGE_LIST_SMS);
						        case 3: ph_showpage(playerid, PAGE_LIST_ARCH);
						    }
						    return 1;
						}
						case PAGE_MENU_SETTINGS:
						{
							switch (j)
							{
								case 0: Change_Airplane(playerid);
								case 1:	Change_Sound(playerid);
								case 2: ph_showpage(playerid, PAGE_PHONE_STYLE);
								case 3: ph_showpage(playerid, PAGE_PHONE_MELODY);
							}
							return 1;
						}
						case PAGE_LIST_CONTACTS:
						{
							SetPVarInt(playerid, #select_contact, GetPVarInt(playerid, #phonelist)+j);
							ph_showpage(playerid, PAGE_CONTACT_INFO);
							return 1;
						}
						case PAGE_CONTACT_INFO:
						{
							switch (j)
							{
								case 0: return 1;
								case 1:	
								{
									new mes[MAX_PHONE_STR];
									format(mes, sizeof(mes), "%i", PlayerContacts[playerid][GetPVarInt(playerid, #select_contact)][contactName]);
									cmd::call(playerid, mes);
								}
								case 2: 
								{
								    new mes[MAX_PHONE_STR+62];
								    SetPVarString(playerid, #sms_number, PlayerContacts[playerid][GetPVarInt(playerid, #select_contact)][contactName]);
								    format(mes, sizeof(mes), "Mensagem >>> {33AA33}%s{a9c4e4}\n\n\tDigite sua mensagem:", PlayerContacts[playerid][GetPVarInt(playerid, #select_contact)][contactName]);
								    Dialog_Show(playerid, SendSMS2, DIALOG_STYLE_INPUT, "Escrever uma mensagem", mes, "Enviar mensagem", "Cancelar");
								}
								case 3:
								{
								    new mes[74];
								    format(mes, sizeof(mes), "Deseja excluir o contato - {33AA33}%s (%i)", PlayerContacts[playerid][GetPVarInt(playerid, #select_contact)][contactName], PlayerContacts[playerid][GetPVarInt(playerid, #select_contact)][contactNumber]);
								    Dialog_Show(playerid, DeleteContact, DIALOG_STYLE_MSGBOX, "Excluir contato", mes, ">>>", "Cancelar");
								}

							}
							return 1;
						}
						case PAGE_LIST_CONTACTS_FOR_CALL:
						{
							new mes[MAX_PHONE_STR];
							format(mes, sizeof(mes), "%i", PlayerContacts[playerid][GetPVarInt(playerid, #phonelist)+j][contactName]);
							cmd::call(playerid, mes);
							return 1;
						}
						case PAGE_LIST_CONTACTS_FOR_SMS:
						{
						    new mes[MAX_PHONE_STR+62];
						    format(mes, sizeof(mes), "Mensagem >>> {33AA33}%s{a9c4e4}\n\n\tDigite sua mensagem:", PlayerContacts[playerid][GetPVarInt(playerid, #phonelist)+j][contactName]);
						    Dialog_Show(playerid, SendSMS2, DIALOG_STYLE_INPUT, "Enviar mensagem", mes, "Enviar mensagem", "Cancelar");
						    return 1;
						}

						case PAGE_LIST_SMS:
						{
							new query[128];
							mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `sms` WHERE `id`=%i", phListMenu[playerid][GetPVarInt(playerid, #phonelist)+j][phoneListed]);
							mysql_tquery(dbHandle, query, "ShowSMS", "i", playerid);//,phListMenu[playerid][GetPVarInt(playerid, #phonelist)+j][phoneListed]);
							return 1;
						}
						case PAGE_NEW_MESSAGES:
						{
							new query[128];
							mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `sms` WHERE `id`=%i", phListMenu[playerid][GetPVarInt(playerid, #phonelist)+j][phoneListed]);
							mysql_tquery(dbHandle, query, "ShowSMS", "i", playerid);//,phListMenu[playerid][GetPVarInt(playerid, #phonelist)+j][phoneListed]);
							return 1;
						}
						case PAGE_LIST_CALLS:
						{
							new query[128];
							mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `history_calls` WHERE `id` = %i",phListMenu[playerid][GetPVarInt(playerid, #phonelist)+j][phoneListed]);
							mysql_tquery(dbHandle, query, "ShowHistoryInfo", "i", playerid);//,phListMenu[playerid][GetPVarInt(playerid, #phonelist)+j][phoneListed]);
							return 1;
						}
						case PAGE_LIST_ARCH:
						{
							new query[128];
							mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `sms` WHERE `id`=%i  AND `archieved` = 1",phListMenu[playerid][GetPVarInt(playerid, #phonelist)+j][phoneListed]);
							mysql_tquery(dbHandle, query, "ShowSMS", "i", playerid);//,phListMenu[playerid][GetPVarInt(playerid, #phonelist)+j][phoneListed]);
							return 1;
						}
						case PAGE_PHONE_STYLE:
						{
							Switch_Style(playerid, j);
							return 1;
						}
						case PAGE_PHONE_MELODY:
						{
							Switch_Melody(playerid, j);
							return 1;
						}
					}			
				}
			}

			if (clickedid == PhArrow[0][playerid]) // LEFT
			{
				return 1;
			}

			if (clickedid == PhArrow[1][playerid]) //RIGHT
			{
				return 1;
			}

			if (clickedid == PhArrow[2][playerid]) // UP
			{
				if ( PhoneInfo[playerid][current_page] != PAGE_LIST_CONTACTS &&
					PhoneInfo[playerid][current_page] != PAGE_LIST_CONTACTS_FOR_CALL &&
					PhoneInfo[playerid][current_page] != PAGE_LIST_CONTACTS_FOR_SMS &&
					PhoneInfo[playerid][current_page] != PAGE_LIST_ARCH &&
					PhoneInfo[playerid][current_page] != PAGE_NEW_MESSAGES &&
					PhoneInfo[playerid][current_page] != PAGE_LIST_SMS &&
					PhoneInfo[playerid][current_page] != PAGE_LIST_CALLS ) return 1;
					
				if (GetPVarInt(playerid, #phonelist) == 0) 	return 1;
				
				SetPVarInt(playerid, #phonelist, GetPVarInt(playerid, #phonelist) - 1);
				ShowPhoneList(playerid, GetPVarInt(playerid, #phonelist));
				return 1;
			}
			
			if (clickedid == PhArrow[3][playerid]) // DOWN
			{
				if ( PhoneInfo[playerid][current_page] != PAGE_LIST_CONTACTS &&
					PhoneInfo[playerid][current_page] != PAGE_LIST_CONTACTS_FOR_CALL &&
					PhoneInfo[playerid][current_page] != PAGE_LIST_CONTACTS_FOR_SMS &&
					PhoneInfo[playerid][current_page] != PAGE_LIST_ARCH &&
					PhoneInfo[playerid][current_page] != PAGE_NEW_MESSAGES &&
					PhoneInfo[playerid][current_page] != PAGE_LIST_SMS &&
					PhoneInfo[playerid][current_page] != PAGE_LIST_CALLS ) return 1;

				if (GetPVarInt(playerid, #maxphonelist) < 4) return 1;
				if (GetPVarInt(playerid, #phonelist) == GetPVarInt(playerid, #maxphonelist)-4) return 1;

				SetPVarInt(playerid, #phonelist, GetPVarInt(playerid, #phonelist) + 1);
				ShowPhoneList(playerid, GetPVarInt(playerid, #phonelist));
				return 1;
			}

			if (clickedid == CallBox[playerid]) 
			{
				if (!(pTemp[playerid][pCallLine] != INVALID_PLAYER_ID || pTemp[playerid][pEmergency])) return 1;

				PhoneInfo[playerid][phoneCallBox] = 0;
				TextDrawHideForPlayer(playerid, CallBox[playerid]);

				ph_showpage(playerid, PAGE_CALL);
				return 1;
			}
		}
	}
	return 1;
}