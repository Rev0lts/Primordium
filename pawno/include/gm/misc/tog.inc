CMD:tog(playerid, params[])
{
	new option[16];
	if (sscanf(params, "s[16]", option))
	{
	    SendHelpMessage(playerid, "/tog [opções]");
	    SendSyntaxMessage(playerid, "nome hud ooc entrada ads gov");
	    SendSyntaxMessage(playerid, "pm adm f notocias afk entrar animchat");
	    return 1;
	}

	if (CompareStrings(option, "nome"))
	{
	    if (PlayerInfo[playerid][pSettings] & togName)
	    {
	        PlayerInfo[playerid][pSettings] &= ~togName;
	        SQL_SetInteger("users", "settings", PlayerInfo[playerid][pSettings], PlayerInfo[playerid][pID]);
	        foreach(new i : Player)
	        {
	            ShowPlayerNameTagForPlayer(playerid, i, 1);
	        }
	        SendSyntaxMessage(playerid, "Você habilitou a exibição dos apelidos dos jogadores.");
	    }
	    else
	    {
	        PlayerInfo[playerid][pSettings] |= togName;
	        SQL_SetInteger("users", "settings", PlayerInfo[playerid][pSettings], PlayerInfo[playerid][pID]);
	        foreach(new i : Player)
	        {
	            ShowPlayerNameTagForPlayer(playerid, i, 0);
	        }
	        SendSyntaxMessage(playerid, "Você desabilitou a exibição dos apelidos dos jogadores.");
	    }
	}
	else if (CompareStrings(option, "hud"))
	{
	    if (PlayerInfo[playerid][pSettings] & togHUD)
	    {
	        PlayerInfo[playerid][pSettings] &= ~togHUD;
	        SQL_SetInteger("users", "settings", PlayerInfo[playerid][pSettings], PlayerInfo[playerid][pID]);
	        ShowHudTextDraw(playerid);
	        SendSyntaxMessage(playerid, "Você ativou o HUD.");
	    }
	    else
	    {
	        PlayerInfo[playerid][pSettings] |= togHUD;
	        SQL_SetInteger("users", "settings", PlayerInfo[playerid][pSettings], PlayerInfo[playerid][pID]);
	        HideHudTextDraw(playerid);
	        SendSyntaxMessage(playerid, "Você desabilitou o HUD.");
	    }
	}
	else if (CompareStrings(option, "ooc"))
	{
	    if (PlayerInfo[playerid][pSettings] & togOOC)
	    {
	        PlayerInfo[playerid][pSettings] &= ~togOOC;
	        SQL_SetInteger("users", "settings", PlayerInfo[playerid][pSettings], PlayerInfo[playerid][pID]);
	        SendSyntaxMessage(playerid, "Você habilitou o chat OOC.");
	    }
	    else 
	    {
	        PlayerInfo[playerid][pSettings] |= togOOC;
	        SQL_SetInteger("users", "settings", PlayerInfo[playerid][pSettings], PlayerInfo[playerid][pID]);
	        SendSyntaxMessage(playerid, "Você desabilitou o chat OOC.");
	    }
	}
	else if (CompareStrings(option, "infoentrada"))
	{
	    if (PlayerInfo[playerid][pSettings] & togJoin)
	    {
	        PlayerInfo[playerid][pSettings] &= ~togJoin;
	        SQL_SetInteger("users", "settings", PlayerInfo[playerid][pSettings], PlayerInfo[playerid][pID]);
	        SendSyntaxMessage(playerid, "Você habilitou a exibição de informações sobre a entrada/saída de jogadores próximos.");
	    }
	    else 
	    {
	        PlayerInfo[playerid][pSettings] |= togJoin;
	        SQL_SetInteger("users", "settings", PlayerInfo[playerid][pSettings], PlayerInfo[playerid][pID]);
	        SendSyntaxMessage(playerid, "Você desabilitou a exibição de informações sobre a entrada/saída de jogadores próximos.");
	    }
	}
	else if (CompareStrings(option, "anuncios"))
	{
	    if (PlayerInfo[playerid][pSettings] & togAdv)
	    {
	        PlayerInfo[playerid][pSettings] &= ~togAdv;
	        SQL_SetInteger("users", "settings", PlayerInfo[playerid][pSettings], PlayerInfo[playerid][pID]);
	        SendSyntaxMessage(playerid, "Você habilitou a exibição de anúncios.");
	    }
	    else
	    {
	        PlayerInfo[playerid][pSettings] |= togAdv;
	        SQL_SetInteger("users", "settings", PlayerInfo[playerid][pSettings], PlayerInfo[playerid][pID]);
	        SendSyntaxMessage(playerid, "Você desabilitou a exibição de anúncios.");
	    }
	}

	else if (CompareStrings(option, "gov"))
	{
	    if (PlayerInfo[playerid][pSettings] & togGov)
	    {
	        PlayerInfo[playerid][pSettings] &= ~togGov;
	        SQL_SetInteger("users", "settings", PlayerInfo[playerid][pSettings], PlayerInfo[playerid][pID]);
	        SendSyntaxMessage(playerid, "Você habilitou a exibição das notícias do governo.");
	    }
	    else
	    {
	        PlayerInfo[playerid][pSettings] |= togGov;
	        SQL_SetInteger("users", "settings", PlayerInfo[playerid][pSettings], PlayerInfo[playerid][pID]);
	        SendSyntaxMessage(playerid, "Você desabilitou as notícias do governo.");
	    }
	}

	else if (CompareStrings(option, "pm"))
	{
	    if (PlayerInfo[playerid][pSettings] & togPM)
	    {
	        PlayerInfo[playerid][pSettings] &= ~togPM;
	        SQL_SetInteger("users", "settings", PlayerInfo[playerid][pSettings], PlayerInfo[playerid][pID]);
	        SendSyntaxMessage(playerid, "Você ativou a recepção de mensagens privadas.");
	    }
	    else 
	    {
	        PlayerInfo[playerid][pSettings] |= togPM;
	        SQL_SetInteger("users", "settings", PlayerInfo[playerid][pSettings], PlayerInfo[playerid][pID]);
	        SendSyntaxMessage(playerid, "Você desabilitou a recepção de mensagens privadas.");
	    }
	}

	else if (CompareStrings(option, "adm"))
	{
	    if (PlayerInfo[playerid][pSettings] & togAdmCmds)
	    {
	        PlayerInfo[playerid][pSettings] &= ~togAdmCmds;
	        SQL_SetInteger("users", "settings", PlayerInfo[playerid][pSettings], PlayerInfo[playerid][pID]);
	        SendSyntaxMessage(playerid, "Você habilitou a exibição de notificações de admin.");
	    }
	    else
	    {
	        PlayerInfo[playerid][pSettings] |= togAdmCmds;
	        SQL_SetInteger("users", "settings", PlayerInfo[playerid][pSettings], PlayerInfo[playerid][pID]);
	        SendSyntaxMessage(playerid, "Você desabilitou a exibição de notificações de admin.");
	    }
	}

	else if (CompareStrings(option, "f"))
	{
	    if (PlayerInfo[playerid][pSettings] & togFaction)
	    {
	        PlayerInfo[playerid][pSettings] &= ~togFaction;
	        SQL_SetInteger("users", "settings", PlayerInfo[playerid][pSettings], PlayerInfo[playerid][pID]);
	        SendSyntaxMessage(playerid, "Você habilitou a exibição de mensagens de facção.");
	    }
	    else 
	    {
	        PlayerInfo[playerid][pSettings] |= togFaction;
	        SQL_SetInteger("users", "settings", PlayerInfo[playerid][pSettings], PlayerInfo[playerid][pID]);
	        SendSyntaxMessage(playerid, "Você desabilitou a exibição de mensagens de facção.");
	    }
	}

	else if (CompareStrings(option, "noticias"))
	{
	    if (PlayerInfo[playerid][pSettings] & togNews)
	    {
	        PlayerInfo[playerid][pSettings] &= ~togNews;
	        SQL_SetInteger("users", "settings", PlayerInfo[playerid][pSettings], PlayerInfo[playerid][pID]);
	        SendSyntaxMessage(playerid, "Você habilitou a exibição de notícias do SAN.");
	    }
	    else 
	    {
	        PlayerInfo[playerid][pSettings] |= togNews;
	        SQL_SetInteger("users", "settings", PlayerInfo[playerid][pSettings], PlayerInfo[playerid][pID]);
	        SendSyntaxMessage(playerid, "Você desabilitou a exibição de notícias do SAN.");
	    }
	}


	else if (CompareStrings(option, "afk"))
	{
	    if (PlayerInfo[playerid][pSettings] & togAFK)
	    {
	        PlayerInfo[playerid][pSettings] &= ~togAFK;
	        SQL_SetInteger("users", "settings", PlayerInfo[playerid][pSettings], PlayerInfo[playerid][pID]);
	        SendSyntaxMessage(playerid, "Você habilitou o tempo gasto em AFK.");
	    }
	    else 
	    {
	        PlayerInfo[playerid][pSettings] |= togAFK;
	        SQL_SetInteger("users", "settings", PlayerInfo[playerid][pSettings], PlayerInfo[playerid][pID]);
	        SendSyntaxMessage(playerid, "Você desabilitou o tempo gasto em AFK.");
	    }
	}

	else if (CompareStrings(option, "entrar"))
	{
	    if (PlayerInfo[playerid][pSettings] & togEnter)
	    {
	        PlayerInfo[playerid][pSettings] &= ~togEnter;
	        SQL_SetInteger("users", "settings", PlayerInfo[playerid][pSettings], PlayerInfo[playerid][pID]);
	        SendSyntaxMessage(playerid, "Você habilitou a interação com a tecla 'F'.");
	    }
	    else 
	    {
	        PlayerInfo[playerid][pSettings] |= togEnter;
	        SQL_SetInteger("users", "settings", PlayerInfo[playerid][pSettings], PlayerInfo[playerid][pID]);
	        SendSyntaxMessage(playerid, "Você desabilitou a interação com a tecla 'F'.");
	    }
	}

	else if (CompareStrings(option, "servico"))
	{
	    if (PlayerInfo[playerid][pSettings] & togService)
	    {
	        PlayerInfo[playerid][pSettings] &= ~togService;
	        SQL_SetInteger("users", "settings", PlayerInfo[playerid][pSettings], PlayerInfo[playerid][pID]);
	        SendSyntaxMessage(playerid, "Você habilitou a exibição de serviços [RELATAR | AJUDA | SOLICITAÇÃO].");
	    }
	    else 
	    {
	        PlayerInfo[playerid][pSettings] |= togService;
	        SQL_SetInteger("users", "settings", PlayerInfo[playerid][pSettings], PlayerInfo[playerid][pID]);
	        SendSyntaxMessage(playerid, "Você desabilitou a exibição de serviços [RELATAR | AJUDA | SOLICITAÇÃO].");
	    }
	}
	else 
	{
	    SendHelpMessage(playerid, "/tog [opções]");
	    SendSyntaxMessage(playerid, "nome hud ooc entrar anuncios governo");
	    SendSyntaxMessage(playerid, "pm adm faccao noticias afk");
	}
	return 1;
}

HUD_OnPlayerStateChange(playerid, newstate, oldstate)
{
	if (newstate == PLAYER_STATE_DRIVER)
	{
		 for(new i=0; i < 24; i++)	PlayerTextDrawShow(playerid, speedTD[i][playerid]);
	}
	
	if (oldstate == PLAYER_STATE_DRIVER)
	{
		 for(new i=0; i<24; i++) PlayerTextDrawHide(playerid, speedTD[i][playerid]);
	}
	return 1;
}