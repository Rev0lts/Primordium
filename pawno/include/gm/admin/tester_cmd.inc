CMD:testerhelp(playerid, params[])
{
    if (PlayerInfo[playerid][pHelper] == 0) return SendNotAdmin(playerid);
    SCM(playerid, COLOR_WHITE, "[TESTER]: {C3C3C3}/tduty /(t)chat /togtchat /freeze /an (/agr)eegraffiti");
    return 1;
}

CMD:tduty(playerid, params[])
{
    if (PlayerInfo[playerid][pHelper] < 1) return SendNotAdmin(playerid);
    if (IsAdminDuty(playerid)) return SendErrorMessage(playerid, "Você está de serviço como administrador.");

    if (IsTesterDuty(playerid) == 1)
    {
        StaffMSG(HELPER_DUTY, "AdmCmd: %s saiu do dever de tester.", GetAccName(playerid));
        pTemp[playerid][pTesterDuty] = 0;
        SetPlayerColor(playerid, TEAM_HIT_COLOR);
    }
    else
    {
        pTemp[playerid][pTesterDuty] = 1;
        StaffMSG(HELPER_DUTY, "AdmCmd: %s assumiu o dever de tester.", GetAccName(playerid));
        SetPlayerColor(playerid, HELPER_DUTY);
    }
    return 1;
}



ALTX:tchat("/t");
CMD:tchat(playerid, params[])
{
	if (!IsTester(playerid) && !CheckAdmin(playerid, ADMIN_LEVEL_1))	return SendNotAdmin(playerid);
    if (PlayerInfo[playerid][pSettings] & togTester)					return SendErrorMessage(playerid, "Para ligar o chat de tester use (/togtchat).");
	if (isnull(params))													return SendHelpMessage(playerid, "/(t)chat [texto]");

	new prefix[16] = "TESTER";

	if (PlayerInfo[playerid][pAdmin] > 0) prefix = "ADMIN";

	new mes[128];
	if (strlen(params) > MAXLEN)
	{
		new pos = MAXLEN;

		if (pos < MAXLEN-1) pos = MAXLEN;
		
		format(mes, sizeof(mes), "* [%s] %s(%i): %.*s ...", prefix, GetAccName(playerid), playerid, pos, params);
		SendStaffMessage(COLOR_LIGHTGREEN, mes);
		format(mes, sizeof(mes), "... %s", params[pos]);
		SendStaffMessage(COLOR_LIGHTGREEN, mes);
	}
	else
	{
		format(mes, sizeof(mes), "* [%s] %s(%i): %s", prefix, GetAccName(playerid), playerid, params);
		SendStaffMessage(COLOR_LIGHTGREEN, mes);
	}
	return 1;
}

CMD:togtchat(playerid, params[])
{
	if (!IsTester(playerid) && !CheckAdmin(playerid, ADMIN_LEVEL_1)) return SendNotAdmin(playerid);

    if (PlayerInfo[playerid][pSettings] & togTester)
	{
		PlayerInfo[playerid][pSettings] &= ~togTester;
    	SendSyntaxMessage(playerid, "You have enabled testers chat.");
	}
	else
	{
		PlayerInfo[playerid][pSettings] |= togTester;
		SendSyntaxMessage(playerid, "You have disabled the testers chat.");
	}
	return 1;
}

ALTX:agreegraffiti("/agr");
CMD:agreegraffiti(playerid, params[])
{
    new option[16], parameters[128];
    if (!IsTester(playerid) && !CheckAdmin(playerid, ADMIN_LEVEL_1)) return SendNotAdmin(playerid);
    if (sscanf(params, "s[16]S()[127]", option, parameters))
    {
        SendHelpMessage(playerid, "(/agr)eegraffiti [opções]");
        if (CheckAdmin(playerid, ADMIN_LEVEL_1))
        {
            return SendSyntaxMessage(playerid, "listar / ir / mostrar / aceitar / recusar / info / destruir");
        }
        else
        {
            return SendSyntaxMessage(playerid, "listar / ir / mostrar / aceitar / recusar");
        }
    }
    if (CompareStrings(option, "listar"))
    {
        return Graffity_GetAcceptList(playerid);
    }
    else if (CompareStrings(option, "ir"))
    {
        new grafid;

        if (sscanf(parameters, "d", grafid)) return SendHelpMessage(playerid, "/agreegraffitigoto [ID do graffiti]");
        if (grafid < 0 || grafid >= MAX_GRAFFITIES || !E_GRAFFITY[grafid][gON]) return SendErrorMessage(playerid, "Nenhum graffiti com este ID encontrado.");
        if (IsTester(playerid) && !CheckAdmin(playerid, ADMIN_LEVEL_1) && E_GRAFFITY[grafid][gAccepted]) return SendErrorMessage(playerid, "Este graffiti já foi aprovado. Não é possível teleportar para ele.");

        SetPlayerPosEx(playerid, E_GRAFFITY[grafid][gGotoX], E_GRAFFITY[grafid][gGotoY], E_GRAFFITY[grafid][gGotoZ]);
        SetInterior(playerid, E_GRAFFITY[grafid][gInterior]);
        SetWorld(playerid, E_GRAFFITY[grafid][gVW]);
    }
    else if (CompareStrings(option, "mostrar"))
    {
        new g_id = Graffity_Nearest(playerid);

        if (g_id == -1) return SendErrorMessage(playerid, "Não há nenhum graffiti perto de você.");
        if (E_GRAFFITY[g_id][gAccepted]) return SendErrorMessage(playerid, "O graffiti não precisa de consideração.");
        if (E_GRAFFITY[g_id][gEditing]) return SendErrorMessage(playerid, "O graffiti está sendo editado no momento. Por favor, aguarde um pouco.");
        if (IsValidDynamicObject(E_GRAFFITY[g_id][gObject])) return SendErrorMessage(playerid, "O graffiti já existe.");

        E_GRAFFITY[g_id][gObject] = CreateDynamicObject(GRAFFITY_OBJECT_ID, E_GRAFFITY[g_id][gPosX], E_GRAFFITY[g_id][gPosY], E_GRAFFITY[g_id][gPosZ], E_GRAFFITY[g_id][gRotX], E_GRAFFITY[g_id][gRotY], E_GRAFFITY[g_id][gRotZ], E_GRAFFITY[g_id][gVW], E_GRAFFITY[g_id][gInterior]);
        SetDynamicObjectMaterial(E_GRAFFITY[g_id][gObject], 0, 0, "nenhum", "nenhum", 0);
        SetDynamicObjectMaterialText(E_GRAFFITY[g_id][gObject], 0, E_GRAFFITY[g_id][gText], OBJECT_MATERIAL_SIZE_256x32, E_GRAFFITY[g_id][gFont], E_GRAFFITY[g_id][gFontSize], E_GRAFFITY[g_id][gBold], E_GRAFFITY[g_id][gColor], E_GRAFFITY[g_id][gBackColor], 0);
        SetPVarInt(playerid, "Tester:Graffity", g_id+1);
        SetPVarInt(playerid, "Tester:GraffityTime", 10);
        SendSyntaxMessage(playerid, "Após 10 segundos, o graffiti desaparecerá.");
    }
    else if (CompareStrings(option, "aceitar"))
    {
        new g_id = Graffity_Nearest(playerid);

        if (g_id == -1) return SendErrorMessage(playerid, "Não há nenhum graffiti perto de você.");
        if (E_GRAFFITY[g_id][gAccepted]) return SendErrorMessage(playerid, "O graffiti não precisa de consideração.");
        if (E_GRAFFITY[g_id][gEditing]) return SendErrorMessage(playerid, "O graffiti está sendo editado no momento. Por favor, aguarde um pouco.");

        Graffity_Accept(g_id, playerid);
        return 1;
    }
    else if (CompareStrings(option, "recusar"))
    {
        new g_id = Graffity_Nearest(playerid);

        if (g_id == -1) return SendErrorMessage(playerid, "Não há nenhum graffiti perto de você.");
        if (E_GRAFFITY[g_id][gAccepted]) return SendErrorMessage(playerid, "O graffiti não precisa de consideração.");
        if (E_GRAFFITY[g_id][gEditing]) return SendErrorMessage(playerid, "O graffiti está sendo editado no momento. Por favor, aguarde um pouco.");

        Graffity_Decline(g_id, playerid);
        return 1;
    }
    else if (CompareStrings(option, "info"))
    {
        new g_id = Graffity_Nearest(playerid);

        if (IsTester(playerid) && !CheckAdmin(playerid, ADMIN_LEVEL_1)) return SendNotAdmin(playerid);
        if (g_id == -1) return SendErrorMessage(playerid, "Não há nenhum graffiti perto de você.");
        if (E_GRAFFITY[g_id][gEditing]) return SendErrorMessage(playerid, "O graffiti está sendo editado no momento. Por favor, aguarde um pouco.");

        new msg[1024] = "{FFFFFF}", msgcat[128];
        format(msg, sizeof(msg), "{FFFFFF}ID do graffiti\t%d\nCriador\t%s\nData de criação\t%s\nFonte\t%s\n", g_id, E_GRAFFITY[g_id][gCreator], E_GRAFFITY[g_id][gCreateDate], E_GRAFFITY[g_id][gFont]);
        if (E_GRAFFITY[g_id][gAccepted])
        {
            format(msgcat, sizeof(msgcat), "	\nAceito\t%s\nData de aprovação\t%s\n", E_GRAFFITY[g_id][gAcceptor], E_GRAFFITY[g_id][gAcceptDate]);
        }
        strcat(msg, msgcat);

        Dialog_Show(playerid, None, DIALOG_STYLE_TABLIST, "{FFFFFF}Informações sobre o graffiti", msg, "Fechar", "");
    }
    else if (CompareStrings(option, "destruir"))
    {
        new g_id;

        if (IsTester(playerid) && !CheckAdmin(playerid, ADMIN_LEVEL_1)) return SendNotAdmin(playerid);
        if (sscanf(parameters, "d", g_id)) return SendErrorMessage(playerid, "/agreegraffitidestroy [ID do graffiti]");
        if (g_id < 0 || g_id >= MAX_GRAFFITIES || !E_GRAFFITY[g_id][gON]) return SendErrorMessage(playerid, "Nenhum graffiti com este ID encontrado.");
        if (E_GRAFFITY[g_id][gEditing]) return SendErrorMessage(playerid, "O graffiti está sendo editado no momento. Por favor, aguarde um pouco.");
        if (!E_GRAFFITY[g_id][gAccepted]) return SendErrorMessage(playerid, "O graffiti ainda não foi aprovado. Se você quiser removê-lo, use /gr recusar.");

        Graffity_Destroy(g_id);
        SendSyntaxMessage(playerid, "Você removeu o graffiti [ID: %i].", g_id);
    }
    else
    {
        SendHelpMessage(playerid, "/agreegraffiti [opções]");
        if (CheckAdmin(playerid, ADMIN_LEVEL_1))
        {
            return SendSyntaxMessage(playerid, "listar / ir / mostrar / aceitar / recusar / info / destruir");
        }
        else
        {
            return SendSyntaxMessage(playerid, "listar / ir / mostrar / aceitar / recusar");
        }
    }
    return 1;
}