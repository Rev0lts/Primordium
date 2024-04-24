CMD:streamer(playerid, params[])
{
    new objects;

    if (!CheckAdmin(playerid, HIGH_ADMIN_LEVEL))	SendNotAdmin(playerid);

    if (sscanf(params, "i", objects))
    {
        SendHelpMessage(playerid, "/streamer [número de objetos]");
        SendSyntaxMessage(playerid, "Número atual de objetos: %i", Streamer_GetVisibleItems(STREAMER_TYPE_OBJECT));
        return 1;
    }
    if (objects < 1 || objects > 1499)	return SendErrorMessage(playerid, "Número de objetos deve estar entre 1 e 1499.");

    new Float:x, Float:y, Float:z;

    Streamer_SetVisibleItems(STREAMER_TYPE_OBJECT, objects);

    foreach(new i : Player)
    {
        if (!IsPlayerLogged(i))	continue;

        GetPlayerPos(i, x, y, z);
        Streamer_DestroyAllVisibleItems(playerid, STREAMER_TYPE_OBJECT);
        Streamer_UpdateEx(i, x, y, z, -1, -1, STREAMER_TYPE_OBJECT);
    }
    return 1;
}

CMD:resetcode(playerid, params[])
{
    if (!CheckAdmin(playerid, HIGH_ADMIN_LEVEL))	return SendNotAdmin(playerid);

    new userid;

    if (sscanf(params, "u", userid))					return SendHelpMessage(playerid, "/resetcode [ID/Parte do Nome]");
    if (userid == INVALID_PLAYER_ID)					return SendErrorMessage(playerid, "Jogador não encontrado.");
    if (!PlayerInfo[userid][pAdmin])					return SendErrorMessage(playerid, "O jogador não é um administrador.");
    if (isempty(PlayerInfo[userid][pAdminCode]))		return SendErrorMessage(playerid, "A senha do administrador ainda não foi definida.");

    if (IsAdminDuty(userid)) cmd::aduty(userid, "");
    ResetAdminCode(userid);
    SendAdmMessageToPlayer(playerid, "Você redefiniu sua senha para entrar em serviço de administrador para %s [ID: %i].", GetName(userid), userid);
    return 1;
}

CMD:setstat(playerid, params[])
{
	new userid, option[16], secoption[64];

	if (!CheckAdmin(playerid, HIGH_ADMIN_LEVEL))	SendNotAdmin(playerid);

	if (sscanf(params, "us[16]S()[10]", userid, option, secoption))
	{
		SendHelpMessage(playerid, "/setstat [ID/Parte do Nome] [opção] [valor]");
		SendSyntaxMessage(playerid, "nivel/sex/banco/horas/dinheiro/acessolabdrogas"); //exp
		SendSyntaxMessage(playerid, "carlic/armalic/aviaolic/taxilic/horastrabalho");
		return 1;
	}

	if (!IsPlayerLogged(userid)) return SendNotConnected(playerid);

	if (CompareStrings(option, "nivel"))
	{
		new level;

		if (sscanf(secoption, "i", level))	return SendHelpMessage(playerid, "/setstat [ID/Parte do Nome] nivel [nivel]");
		if (level <= 1 || level > 2)		return SendErrorMessage(playerid, "Nivel 1 a 2.");

		PlayerInfo[userid][pLevel] = level;
		SQL_SetInteger("users", "level", PlayerInfo[userid][pLevel], PlayerInfo[userid][pID]);
		SetPlayerScore(userid, PlayerInfo[userid][pLevel]);
		SendAdmMessage("%s alterou o nível de %s [ID: %i] para %i.", GetNameEx(playerid), GetNameEx(userid), userid, level);
		SendAdmMessageToPlayer(userid, "Administrador %s alterou seu nível para %i.", GetAccName(playerid), level);
		return 1;
	}

	if (CompareStrings(option, "acessolabdrogas"))
	{
		new level;

		if (sscanf(secoption, "i", level))	return SendHelpMessage(playerid, "/setstat [ID/Parte do Nome] acessolabdrogas [0-1]");
		if (level <= 0 || level > 1)		return SendErrorMessage(playerid, "Acesso 0 ou 1.");

		PlayerInfo[userid][pDrugLabAccess] = level;
		SQL_SetInteger("users", "drugaccess", PlayerInfo[userid][pDrugLabAccess], PlayerInfo[userid][pID]);
		SendAdmMessage("%s alterou o acesso do laboratório de drogas de %s [ID: %i] para %i.", GetNameEx(playerid), GetNameEx(userid), userid, level);
		SendAdmMessageToPlayer(userid, "Administrador %s alterou seu acesso do laboratório de drogas para %i.", GetAccName(playerid), level);
		return 1;
	}

	else if (CompareStrings(option, "sexo"))
	{
		new sex;

		if (sscanf(secoption, "i", sex))	return SendHelpMessage(playerid, "/setstat [ID/Parte do Nome] sexo [1 - masculino | 2 - feminino]");
		if (sex < 1 || sex > 2) 			return SendErrorMessage(playerid, "[1 - masculino | 2 - feminino].");

		PlayerInfo[userid][pSex] = sex;
		SQL_SetInteger("users", "sex", PlayerInfo[userid][pSex], PlayerInfo[userid][pID]);

		new sex_naming[16];

		if (sex == 1)	format(sex_naming, sizeof(sex_naming), "Masculino");
		else 			format(sex_naming, sizeof(sex_naming), "Feminino");

		SendAdmMessage("%s alterou o genero de %s [ID: %i] para %s.", GetNameEx(playerid), GetNameEx(userid), userid, sex_naming);
		SendAdmMessageToPlayer(userid, "Administrador %s alterou seu genero para %s.", GetAccName(playerid), sex_naming);
		return 1;
	}

	else if (CompareStrings(option, "banco"))
	{
		new abank;

		if (sscanf(secoption, "i", abank))	return SendHelpMessage(playerid, "/setstat [ID/Parte do Nome] banco [valor]");
		if (abank < 0 || abank > 50000000)	return SendErrorMessage(playerid, "Insira um valor de $0 até $50,000,000.");

		PlayerInfo[userid][pBank] = abank;
		SQL_SetInteger("users", "bank", PlayerInfo[userid][pBank], PlayerInfo[userid][pID]);
		SendAdmMessage("%s alterou a quantidade de dinheiro no banco de %s [ID: %i] para $%i.", GetNameEx(playerid), GetNameEx(userid), userid, abank);
		SendAdmMessageToPlayer(userid, "O administrador %s alterou o seu dinheiro no banco para $%i.", GetAccName(playerid), abank);

		new mes[128];
		format(mes, sizeof(mes), "[/givemoney] O administrador %s concedeu ao jogador %s a quantia de $%i.", GetAccName(playerid), GetNameEx(userid), abank);
		insert_log(mes, ADMIN_LOG, 8, PlayerInfo[userid][pAccID], PlayerInfo[playerid][pAccID]);
		return 1;
	}

	else if (CompareStrings(option, "horas"))
	{
		new hor;

		if (sscanf(secoption, "i", hor))	return SendHelpMessage(playerid, "/setstat [ID/Parte do Nome] horas [número de horas no jogo]");
		if (hor < 0 || hor > 8192)			return SendErrorMessage(playerid, "O número de horas no jogo deve estar entre 0 e 8192.");

		PlayerInfo[userid][pHours] = hor;
		SQL_SetInteger("users", "hours", PlayerInfo[userid][pHours], PlayerInfo[userid][pID]);
		SendAdmMessage("%s alterou as horas no jogo de %s [ID: %i] para %i.", GetNameEx(playerid), GetNameEx(userid), userid, hor);
		SendAdmMessageToPlayer(userid, "O administrador %s alterou suas horas no jogo para %i.", GetAccName(playerid), hor);
		return 1;
	}

	else if (CompareStrings(option, "carlic"))
	{
		new lic;
		
		if (sscanf(secoption, "i", lic)) return SendHelpMessage(playerid, "/setstat [ID/Parte do Nome] carlic [0 - retirar | 1 - conceder]");
		if (lic < 0 || lic > 1)			 return SendErrorMessage(playerid, "Valor deve ser de 0 a 1.");

		PlayerInfo[userid][pCarLic] = lic;
		SQL_SetInteger("users", "carlic", PlayerInfo[userid][pCarLic], PlayerInfo[userid][pID]);

		new lic_name[16];

		if (PlayerInfo[userid][pCarLic])	format(lic_name, sizeof(lic_name), "concedeu");
		else 								format(lic_name, sizeof(lic_name), "retirou");

		SendAdmMessage("%s %s direitos %s [ID: %i].", GetNameEx(playerid), lic_name, GetNameEx(userid), userid);
		SendAdmMessageToPlayer(userid, "Administrador %s %s %s direitos.", GetAccName(playerid), lic_name, (PlayerInfo[userid][pCarLic]) ? ("Seu") : ("Você"));
		return 1;
	}

	else if (CompareStrings(option, "aviaolic"))
	{
		new lic;

		if (sscanf(secoption, "i", lic)) return SendHelpMessage(playerid, "/setstat [ID/Parte do Nome] flylic [0 - retirou | 1 - concedeu | 2 - Blacklist]");
		if (lic < 0 || lic > 2)			 return SendSyntaxMessage(playerid, "Valor deve estar entre 0 e 2.");

		PlayerInfo[userid][pFlyLic] = lic;
		SQL_SetInteger("users", "flylic", PlayerInfo[userid][pFlyLic], PlayerInfo[userid][pID]);

		new lic_name[20];

		switch(PlayerInfo[userid][pFlyLic])
		{
		    case 0: format(lic_name, sizeof(lic_name), "retirou");
		    case 1: format(lic_name, sizeof(lic_name), "concedeu");
		    case 2: format(lic_name, sizeof(lic_name), "adicionou à lista negra");
		}

		SendAdmMessage("%s %s a licença de piloto de %s [ID: %i].", GetNameEx(playerid), lic_name, GetNameEx(userid), userid);
		SendAdmMessageToPlayer(userid, "O administrador %s %s sua licença de piloto.", GetAccName(playerid), lic_name, (PlayerInfo[userid][pFlyLic]) ? ("Sua") : ("Você"));
		return 1;
	}

	else if (CompareStrings(option, "taxilic"))
	{
		new lic;

		if (sscanf(secoption, "i", lic))	return SendHelpMessage(playerid, "/setstat [ID/Parte do Nome] taxilic [0 - retirou | 1 - concedeu | 2 - lista negra]");
		if (lic < 0 || lic > 2)			return SendSyntaxMessage(playerid, "Valor deve estar entre 0 e 2.");

		PlayerInfo[userid][pTaxiLic] = lic;
		SQL_SetInteger("users", "taxilic", PlayerInfo[userid][pTaxiLic], PlayerInfo[userid][pID]);

		new lic_name[20];

		switch(PlayerInfo[userid][pTaxiLic])
		{
		    case 0: format(lic_name, sizeof(lic_name), "retirou");
		    case 1: format(lic_name, sizeof(lic_name), "concedeu");
		    case 2: format(lic_name, sizeof(lic_name), "adicionou à lista negra");
		}

		SendAdmMessage("%s %s licença de taxista de %s [ID: %i].", GetNameEx(playerid), lic_name, GetNameEx(userid), userid);
		SendAdmMessageToPlayer(userid, "O administrador %s %s sua licença de taxista.", GetAccName(playerid), lic_name, (PlayerInfo[userid][pTaxiLic]) ? ("Sua") : ("Você"));
		return 1;
	}

	else if (CompareStrings(option, "armalic"))
	{
		new lic;

		if (sscanf(secoption, "i", lic))	return SendHelpMessage(playerid, "/setstat [ID/Parte do Nome] weplic [0 - retirou | 1 - concedeu]");
		if (lic < 0 || lic > 1)			return SendSyntaxMessage(playerid, "Valor deve estar entre 0 e 1.");

		PlayerInfo[userid][pWepLic] = lic;
		SQL_SetInteger("users", "weplic", PlayerInfo[userid][pWepLic], PlayerInfo[userid][pID]);

		new lic_name[16];

		if (PlayerInfo[userid][pWepLic])	format(lic_name, sizeof(lic_name), "concedeu");
		else							format(lic_name, sizeof(lic_name), "retirou");

		SendAdmMessage("%s %s licença de armas de fogo de %s [ID: %i].", GetNameEx(playerid), lic_name, GetNameEx(userid), userid);
		SendAdmMessageToPlayer(userid, "O administrador %s %s sua licença de armas de fogo.", GetAccName(playerid), lic_name, (PlayerInfo[userid][pWepLic]) ? ("Sua") : ("Você"));
		return 1;
	}

	else if (CompareStrings(option, "horastrabalho"))
	{
		new whours;

		if (whours < 0 || whours > 256)		return SendErrorMessage(playerid, "Horas de carreira devem estar entre 0 e 256.");

		PlayerInfo[userid][pCareer] = whours;
		SQL_SetInteger("users", "career", PlayerInfo[userid][pCareer], PlayerInfo[userid][pID]);
		SendAdmMessage("%s alterou as horas de carreira de %s [ID: %i] para %i.", GetNameEx(playerid), GetNameEx(userid), userid, whours);
		SendAdmMessageToPlayer(playerid, "O administrador %s alterou suas horas de carreira para %i.", GetAccName(playerid), whours);
	}
	return 1;
}

CMD:gmx(playerid, params[])
{
    new time;

    if (!CheckAdmin(playerid, HIGH_ADMIN_LEVEL)) return SendNotAdmin(playerid);
    if (restart_time)
    {
        SAMALL("%s cancelou a reinicialização do servidor.", GetAccName(playerid));

        restart_time = 0;
        return 1;
    }

    if (sscanf(params, "i", time)) 	return SendHelpMessage(playerid, "/gmx [tempo em segundos]");
    if (time < 10 || time > 300)	return SendErrorMessage(playerid, "Tempo de reinicialização de 10 a 300 segundos.");

    restart_time = time;

    SAMALL("%s iniciou o temporizador para reiniciar após %i segundos.", GetAccName(playerid), time);
    return 1;
}

CMD:copstocrime(playerid, params[])
{
    new amount;
    if (!CheckAdmin(playerid, HIGH_ADMIN_LEVEL)) return SendNotAdmin(playerid);
    if (sscanf(params, "d", amount))			return SendHelpMessage(playerid, "/copstocrime [quantidade]");
    if (amount < 0)								return SendErrorMessage(playerid, "Número negativo!");

    NEED_COPS_TO_CRIME = amount;
    SendAdmMessageToAll("O administrador %s alterou o limite de policiais para crimes para %i.", GetAccName(playerid), amount);
    return 1;
}

CMD:firetocrime(playerid, params[])
{
    new amount;
    if (!CheckAdmin(playerid, HIGH_ADMIN_LEVEL)) return SendNotAdmin(playerid);
    if (sscanf(params, "d", amount))			return SendHelpMessage(playerid, "/firetocrime [quantidade]");
    if (amount < 0)								return SendErrorMessage(playerid, "Número negativo!");

    NEED_FIRE_TO_CRIME = amount;
    SendAdmMessageToAll("O administrador %s alterou o limite de bombeiros para crimes para %i.", GetAccName(playerid), amount);
    return 1;
}

CMD:admin(playerid, params[])
{
    new userid, level;

    if (!CheckAdmin(playerid, HIGH_ADMIN_LEVEL))	return SendNotAdmin(playerid);
    if (sscanf(params, "ud", userid, level))		return SendHelpMessage(playerid, "/admin [ID/Parte do Nome] [nível]");
    if (!IsPlayerLogged(userid))					return SendNotConnected(playerid);
    if (userid == playerid)							return 1;
    if (!level && !PlayerInfo[userid][pAdmin])		return SendErrorMessage(playerid, "O jogador não possui direitos de administração.");
    
    if (PlayerInfo[userid][pAdmin] == HIGH_ADMIN_LEVEL) {
        if (level < 0 || level > HIGH_ADMIN_LEVEL-1)	return SendErrorMessage(playerid, "Nível de administração de 0 a 3.");
    } else if (PlayerInfo[userid][pAdmin] == DEVELOPER_LEVEL) {
        if (level < 0 || level > HIGH_ADMIN_LEVEL)		return SendErrorMessage(playerid, "Nível de administração de 0 a 4.");
    }
    
    if (!level)
    {
        if (IsAdminDuty(userid)) cmd::aduty(userid, "");
        
        SendAdmMessage("%s retirou os direitos de administração de %s.", GetAccName(playerid), GetAccName(userid));
        SendAdmMessageToPlayer(userid, "%s retirou os seus direitos de administração.", GetAccName(playerid));
        PlayerInfo[userid][pAdmin] = 0;
        SQL_SetInteger("accounts", "admin", PlayerInfo[userid][pAdmin], PlayerInfo[userid][pAccID]);		
        return 1;
    }

    PlayerInfo[userid][pAdmin] = level;
    SQL_SetInteger("accounts", "admin", PlayerInfo[userid][pAdmin], PlayerInfo[userid][pAccID]);
    SendAdmMessage("%s concedeu a %s o nível de acesso %i.", GetAccName(playerid), GetNameEx(userid), level);
    SendAdmMessageToPlayer(userid, "%s concedeu a você o nível de acesso %i.", GetAccName(playerid), level);
    return 1;
}

CMD:makepremium(playerid, params[])
{
    new userid, rank;

    if (!CheckAdmin(playerid, HIGH_ADMIN_LEVEL))	return SendNotAdmin(playerid);
    if (sscanf(params, "ui", userid, rank))		return SendHelpMessage(playerid, "/makepremium [ID/Parte do Nome] [0-3]");
    if (!IsPlayerLogged(userid))					return SendNotConnected(playerid);
    if (rank < 0 || rank > 3)					return SendErrorMessage(playerid, "Nível de conta Premium de 0 a 3.");

    new status_premium[4][32] = {"normal", "bronze", "prata", "ouro"};

    PlayerInfo[userid][pPremium] = rank;
    PlayerInfo[userid][pPremiumTime] = gettime() + 30 * 86400;
    SQL_SetInteger("users", "vip", PlayerInfo[userid][pPremium], PlayerInfo[userid][pID]);
    SQL_SetInteger("users", "vip_time", PlayerInfo[userid][pPremiumTime], PlayerInfo[userid][pID]);
    SQL_SetFloat("users", "spawnhealth", 75.0, PlayerInfo[userid][pID]);

    SendAdmMessage("%s instalou a conta %s %s.", GetAccName(playerid), GetAccName(userid), status_premium[rank]);
    SendAdmMessageToPlayer(userid, "O administrador %s instalou em sua conta o status %s.", GetAccName(playerid), status_premium[rank]);
    return 1;
}

CMD:tester(playerid, params[])
{
    new userid, level;

    if (!CheckAdmin(playerid, HIGH_ADMIN_LEVEL))	return SendNotAdmin(playerid);
    if (sscanf(params, "ui", userid, level))		return SendHelpMessage(playerid, "/tester [ID/Parte do Nome] [0 - remover | 1 - atribuir]");
    if (!IsPlayerLogged(playerid))					return SendNotConnected(playerid);
    if (level < 0 || level > 1)						return SendErrorMessage(playerid, "Nível deve ser 0 ou 1.");
    if (!level && !PlayerInfo[userid][pHelper])		return SendErrorMessage(playerid, "Este jogador não possui poderes de tester.");
    if (level && PlayerInfo[userid][pHelper])		return SendErrorMessage(playerid, "Este jogador já é um tester.");

    if (!level)
    {
        if (IsTesterDuty(userid)) cmd::tduty(userid, "");

        PlayerInfo[userid][pHelper] = level;
        SQL_SetInteger("accounts", "helper", PlayerInfo[userid][pHelper], PlayerInfo[userid][pAccID]);
        SendAdmMessage("%s retirou os poderes de tester de %s.", GetAccName(playerid), GetNameEx(userid));
        SendAdmMessageToPlayer(userid, "O administrador %s retirou seus poderes de tester.", GetAccName(playerid));
        return 1;
    }

    PlayerInfo[userid][pHelper] = level;
    SQL_SetInteger("accounts", "helper", PlayerInfo[userid][pHelper], PlayerInfo[userid][pAccID]);
    SendAdmMessage("%s concedeu a %s os poderes de tester.", GetAccName(playerid), GetNameEx(userid));
    SendAdmMessageToPlayer(userid, "O administrador %s concedeu a você os poderes de tester.", GetAccName(playerid));
    return 1;
}

CMD:givemoney(playerid, params[])
{
    new userid, amount;

    if (!CheckAdmin(playerid, HIGH_ADMIN_LEVEL))	return SendNotAdmin(playerid);
    if (sscanf(params, "ui", userid, amount))	return SendHelpMessage(playerid, "/givemoney [ID/Parte do Nome] [quantia]");
    if (!IsPlayerLogged(userid))					return SendNotConnected(playerid);
    if (amount < 0 || amount > 50000000)			return SendErrorMessage(playerid, "Quantia de $0 a $50000000.");

    GiveCash(userid, amount);
    
    new mes[128];
    format(mes, sizeof(mes), "[/givemoney] O administrador %s deu a %s - dinheiro $%i.", GetAccName(playerid), GetNameEx(userid), amount);
    insert_log(mes, ADMIN_LOG, 8, PlayerInfo[userid][pAccID], PlayerInfo[playerid][pAccID]);

    if (amount < 0)
    {
        SendAdmMessage("%s retirou de %s $%i.", GetAccName(playerid), GetNameEx(userid), amount);
        SendAdmMessageToPlayer(userid, "%s retirou de você $%i.", GetAccName(playerid), amount);
        return 1;
    }
    else
    {
        SendAdmMessage("%s concedeu a %s $%i.", GetAccName(playerid), GetNameEx(userid), amount);
        SendAdmMessageToPlayer(userid, "%s concedeu a você $%i.", GetAccName(playerid), amount);
    }
    return 1;
}

ALTX:aobject("/aobj");
CMD:aobject(playerid, params[])
{
	if (!CheckAdmin(playerid, HIGH_ADMIN_LEVEL))	return SendNotAdmin(playerid);

	new option[16], secoption[128];

	if (sscanf(params, "s[16]S()[128]", option, secoption))
	{
	    SendHelpMessage(playerid, "(/aobj)eto [opções]");
	    SendSyntaxMessage(playerid, "criar/remover/editar/editarobjetos/irpara");
	    return 1;
	}

	if (CompareStrings(option, "criar"))
	{
	    new modelid;

	    if (sscanf(secoption, "d", modelid))	return SendHelpMessage(playerid, "(/aobj)eto criar [ID do objeto]");

	    new objectid = -1;

	    for (new i = 0; i < MAX_OBJECTS; i++)
	    {
	        if (ObjectsInfo[i][oID] || ObjectsInfo[i][oObjectOn]) continue;

	        objectid = i;
	        break;
	    }

	    if (objectid == -1)	return SendErrorMessage(playerid, "O número máximo de objetos a ser criado foi atingido.");

	    GetPlayerPos(playerid, ObjectsInfo[objectid][oPosX], ObjectsInfo[objectid][oPosY], ObjectsInfo[objectid][oPosZ]);
	    
	    ObjectsInfo[objectid][oModel] = modelid;
	    ObjectsInfo[objectid][oPosRX] = 
	    ObjectsInfo[objectid][oPosRY] = 
	    ObjectsInfo[objectid][oPosRZ] = 0.0;
	    ObjectsInfo[objectid][oInt] = GetInterior(playerid);
	    ObjectsInfo[objectid][oVW] = GetWorld(playerid);
	    ObjectsInfo[objectid][oObject] = CreateDynamicObject(modelid, ObjectsInfo[objectid][oPosX], ObjectsInfo[objectid][oPosY], ObjectsInfo[objectid][oPosZ], 0.0, 0.0, 0.0, ObjectsInfo[objectid][oVW], ObjectsInfo[objectid][oInt], -1, 200.0);

	    if (ObjectsInfo[objectid][oObject] == INVALID_OBJECT_ID)	return SendErrorMessage(playerid, "Falha ao criar o objeto.");

	    ObjectsInfo[objectid][oObjectOn] = 1;

	    AddObjectToFile(objectid, modelid, ObjectsInfo[objectid][oInt], ObjectsInfo[objectid][oVW], ObjectsInfo[objectid][oPosX], ObjectsInfo[objectid][oPosY], ObjectsInfo[objectid][oPosZ]);
	    SendAdmMessageToPlayer(playerid, "Você criou um objeto [ID: %i | MODELO: %i].", objectid, modelid);
	    return 1;
	}

	else if (CompareStrings(option, "remover"))
	{
	    new objectid;

	    if (sscanf(secoption, "i", objectid))		return SendHelpMessage(playerid, "(/aobj)eto remover [ID do objeto]");
	    if (ObjectsInfo[objectid][oObjectOn] != 1)	return SendErrorMessage(playerid, "Objeto não encontrado.");

	    new query[128];
	    format(query, sizeof(query), "DELETE FROM `objects` WHERE `id` = %i", ObjectsInfo[objectid][oID]);
	    mysql_tquery(dbHandle, query);

	    if (IsValidDynamicObject(ObjectsInfo[objectid][oObject]))	DestroyDynamicObject(ObjectsInfo[objectid][oObject]);

	    ObjectsInfo[objectid][oPosX] = 
	    ObjectsInfo[objectid][oPosY] = 
	    ObjectsInfo[objectid][oPosZ] = 
	    ObjectsInfo[objectid][oPosRX] = 
	    ObjectsInfo[objectid][oPosRY] = 
	    ObjectsInfo[objectid][oPosRZ] = 0.0;
	    ObjectsInfo[objectid][oObjectOn] =
	    ObjectsInfo[objectid][oID] = 0;
	    
	    SendAdmMessageToPlayer(playerid, "Você removeu com sucesso o objeto. [ID: %i]", objectid);
	    return 1;
	}

	else if (CompareStrings(option, "editar"))
	{
		new id, parameters[16], arg[112];

		if (sscanf(secoption, "is[12]S()[112]", parameters, arg))
		{
		    SendHelpMessage(playerid, "(/aobj)eto editar [ID do objeto] [opções]");
		    SendSyntaxMessage(playerid, "pos/textura/apagartextura");
		    return 1;
		}

		if (id < 0 || id > MAX_OBJECTS - 1 || ObjectsInfo[id][oObjectOn] != 1)	return SendErrorMessage(playerid, "Objeto não encontrado.");

		if (CompareStrings(parameters, "pos"))
		{
		    SetPVarInt(playerid, #edit_object, id + 1);
		    EditDynamicObject(playerid, ObjectsInfo[id][oObject]);
		    return 1;
		}

		else if (CompareStrings(parameters, "textura"))
		{
		    new slot, model, txdname[32], texturename[32];

		    if (sscanf(arg, "iis[32]s[32]", slot, model, txdname, texturename))
		    {
		        SendHelpMessage(playerid, "(/aobj)eto editar [ID do objeto] textura [slot de substituição (0-15)] [ID do modelo] [nome do TXD] [nome da textura]");
		        SendSyntaxMessage(playerid, "A lista de texturas com imagens está disponível no site: http://texture.xyin.ws");
		        return 1;
		    }
		    
		    if (slot < 0 || slot > 15) return SendErrorMessage(playerid, "Slot de 0 a 15.");
		    if (model < 0) return SendErrorMessage(playerid, "O ID do modelo deve ser maior que 0.");
		    if (strlen(txdname) >= 32) return SendErrorMessage(playerid, "O nome do TXD não pode ter mais de 32 caracteres.");
		    if (strlen(texturename) >= 32) return SendErrorMessage(playerid, "Nome da textura, máximo de 32 caracteres.");
		    
		    if (!SetDynamicObjectMaterial(ObjectsInfo[id][oObject], slot, model, txdname, texturename, 0)) return SendErrorMessage(playerid, "Falha ao alterar a textura do objeto.");

		    new query[128];
		    mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `objects_texture` WHERE `objectid` = %i AND `slot` = %i", ObjectsInfo[id][oID], slot);
		    mysql_tquery(dbHandle, query, "OnObjectTextureInsert", "iiiiss", playerid, id, slot, model, txdname, texturename);
		    return 1;
		}

		else if (CompareStrings(parameters, "deltextura"))
		{
		    if (isnull(arg)) return SendHelpMessage(playerid, "(/aobj)eto editar [ID do objeto] deltextura [slot (0-15)]");

		    new slot = strval(arg);

		    if (slot < 0 || slot > 15) return SendErrorMessage(playerid, "Slot de 0 a 15.");

		    if (!SetDynamicObjectMaterial(ObjectsInfo[id][oObject], slot, -1, "none", "none", 0)) return SendErrorMessage(playerid, "Falha ao remover a textura do objeto.");

		    new query[128];
		    format(query, sizeof(query), "DELETE FROM `objects_texture` WHERE `objectid` = %i AND `slot` = %i", ObjectsInfo[id][oID], slot);
		    mysql_tquery(dbHandle, query);

		    SendAdmMessageToPlayer(playerid, "Você removeu a textura do objeto [ID: %i] [SLOT: %i].", id, slot);
		    return 1;
		}

	}

	else if (CompareStrings(option, "ir"))
	{
	    new objectid;

	    if (sscanf(secoption, "i", objectid)) return SendHelpMessage(playerid, "(/aobj)eto irpara [ID do objeto]");
	    if (objectid < 0 || objectid >= MAX_OBJECTS || ObjectsInfo[objectid][oObjectOn] != 1) return SendErrorMessage(playerid, "Objeto não encontrado.");

	    SetPlayerPosEx(playerid, ObjectsInfo[objectid][oPosX], ObjectsInfo[objectid][oPosY], ObjectsInfo[objectid][oPosZ]);
	    SetInterior(playerid, ObjectsInfo[objectid][oInt]);
	    SetWorld(playerid, ObjectsInfo[objectid][oVW]);
	    SendSpecialMessage(playerid, "Você foi teleportado para o objeto [ID: %i].", objectid);
	    return 1;
	}

	else if (CompareStrings(option, "editarobjetos"))
	{
		SetPVarInt(playerid, #edit_objects, 1);
		SelectObject(playerid);
		SendAdmMessageToPlayer(playerid, "Seleciona o que deseja editar clicando nele.");
		return 1;
	}

	else 
	{
		SendHelpMessage(playerid, "(/aobj)ect [opção]");
		SendSyntaxMessage(playerid, "criar/remover/editar/editarobjetos/ir");
	}
	return 1;
}

CMD:atower(playerid, params[])
{
	if (!CheckAdmin(playerid, HIGH_ADMIN_LEVEL))	return SendNotAdmin(playerid);

	new option[16], secoption[128];

	if (sscanf(params, "s[16]S()[128]", option, secoption))
	{
		SendHelpMessage(playerid, "/atower [opção]");
		SendSyntaxMessage(playerid, "criar/remover/editar/ir");
		return 1;
	}	

	if (CompareStrings(option, "criar"))
	{
		if (GetWorld(playerid) || GetInterior(playerid))	return SendErrorMessage(playerid, "Você não pode criar torreres em interior.");

		new type;

		if (sscanf(secoption, "i", type))	return SendHelpMessage(playerid, "/atower criar [0 - pequena | 1 - grande] ");
		if (type < 0 || type > 1)			return SendErrorMessage(playerid, "Selecione um tipo de terre entre 0 e 1.");
		new towerid = -1;

		for(new i; i < MAX_TOWERS; i++)
		{
			if (TowerInfo[i][towerOn]) continue;

			towerid = i;
			break;
		}

		if (towerid == -1)	return SendErrorMessage(playerid, "O servidor atingiu o limite de torres que podem ser criada.");

		new Float:x, Float:y, Float:z, Float:angle;

		GetPlayerPos(playerid, x, y, z);
		GetPlayerFacingAngle(playerid, angle);	
		x += 1.0 * floatsin(-angle, degrees);
		y += 1.0 * floatsin(-angle, degrees);
		z -= 1.6;

		TowerInfo[towerid][towerModel] = (type) ? (3763) : (3030);
		TowerInfo[towerid][towerPosX] = x;
		TowerInfo[towerid][towerPosY] = y;
		TowerInfo[towerid][towerPosZ] = z;
		TowerInfo[towerid][towerPosRX] = 0;
		TowerInfo[towerid][towerPosRY] = 0;
		TowerInfo[towerid][towerPosRZ] = angle;
		TowerInfo[towerid][towerOn] = 1;
		TowerInfo[towerid][towerObject] = CreateDynamicObject(TowerInfo[towerid][towerModel], x, y, z, 0.0, 0.0, angle, 0, 0);

		new query[128];
		mysql_format(dbHandle, query, sizeof(query), "INSERT INTO `towers` (`model`, `x`, `y`, `z`, `rz`) VALUES (%i, %f, %f, %f, %f)", type, x, y, z, angle);
		mysql_tquery(dbHandle, query, "OnTowerInsert", "i", towerid);

		new tower_name[16];

		if (!type)	format(tower_name, sizeof(tower_name), "pequena");
		else 		format(tower_name, sizeof(tower_name), "grande");

		SendAdmMessageToPlayer(playerid, "Você criou uma torre de comunicações %s [ID:%i].", tower_name, towerid);
		return 1;
	}

	else if (CompareStrings(option, "remover"))
	{
		new towerid;

		if (sscanf(secoption, "d", towerid))										return SendHelpMessage(playerid, "/atower remover [Torre ID]");
		if (towerid < 0 || towerid > MAX_TOWERS || !TowerInfo[towerid][towerOn])	return SendErrorMessage(playerid, "Torre não encontrada.");
		
		if (IsValidDynamicObject(TowerInfo[towerid][towerObject])) DestroyDynamicObject(TowerInfo[towerid][towerObject]);

		new query[128];
		mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `towers` WHERE `id` = %i", TowerInfo[towerid][towerID]);
		mysql_tquery(dbHandle, query);

		TowerInfo[towerid][towerOn] = 
		TowerInfo[towerid][towerID] = 0;
		SendAdmMessageToPlayer(playerid, "A torre [ID:%i] foi removida.", towerid);
		return 1;
	}

	else if (CompareStrings(option, "editar"))
	{
		new towerid, parameters[16], arg[112];

		if (sscanf(secoption, "d", towerid))
		{
			SendHelpMessage(playerid, "/atower editar [Torre ID] [parametros] ");
			SendSyntaxMessage(playerid, "pos / tipo");
			return 1;
		}

		if (towerid < 0 || towerid > MAX_TOWERS || !TowerInfo[towerid][towerID])	return SendErrorMessage (playerid, "Torre não encontrada.");

		if (CompareStrings(parameters, "pos"))
		{
			if (GetWorld(playerid) || GetInterior(playerid))																							return SendErrorMessage (playerid, "Você não pode editar torres em interior.");
			if (!IsPlayerInRangeOfPoint(playerid, 10.0, TowerInfo[towerid][towerPosX], TowerInfo[towerid][towerPosY], TowerInfo[towerid][towerPosZ]))	return SendErrorMessage (playerid, "Você deve estar perto de uma torre.");
			if (GetPVarInt(playerid, #edit_tower) != 0)																									return SendErrorMessage (playerid, "Você já está editando uma torre.");

			SetPVarInt(playerid, #edit_tower, towerid + 1);
			EditDynamicObject(playerid, TowerInfo[towerid][towerObject]);
			return 1;
		}
		else if (CompareStrings(parameters, "tipo"))
		{
			new type;
			if (sscanf (arg, "i", type)) return SendHelpMessage (playerid, "/atower edit [Torre ID] tipo [0 - pequena | 1 - grande]");
			if (type <0 || type> 1) return SendErrorMessage (playerid, "Selecione um tipo de torre entre 0 e 1.");

			TowerInfo[towerid][towerModel] = (type) ? (3763) : (3030);
			SQL_SetInteger("towers", "model", TowerInfo[towerid][towerModel], TowerInfo[towerid][towerID]);

			if (IsValidDynamicObject(TowerInfo[towerid][towerObject])) DestroyDynamicObject(TowerInfo[towerid][towerObject]);

			TowerInfo[towerid][towerObject] = CreateDynamicObject(TowerInfo[towerid][towerModel], TowerInfo[towerid][towerPosX], TowerInfo[towerid][towerPosY], TowerInfo[towerid][towerPosZ], TowerInfo[towerid][towerPosRX],
			TowerInfo[towerid][towerPosRY], TowerInfo[towerid][towerPosRZ], 0, 0);
			SendAdmMessageToPlayer(playerid,"Você alterou o tipo da torre [ID:%i].", towerid);
			return 1;
		}
	}

	else if (CompareStrings(option, "ir"))
	{
		new towerid; 

		if (sscanf(secoption, "d", towerid))										return SendHelpMessage(playerid, "/atower ir [Torre ID] ");
		if (towerid < 0 || towerid > MAX_TOWERS || !TowerInfo[towerid][towerID])	return SendErrorMessage(playerid, "Torre não encontrada.");

		SetWorld(playerid, 0);
		SetInterior(playerid, 0);
		SetPlayerPosEx(playerid, TowerInfo[towerid][towerPosX], TowerInfo[towerid][towerPosY], TowerInfo[towerid][towerPosZ]);
		SendSpecialMessage (playerid, "Você se teleportou para torre [ID: %i].", towerid);
	}
	return 1;
}

CMD:departship(playerid, params[])
{
    if (!CheckAdmin(playerid, HIGH_ADMIN_LEVEL)) return SendNotAdmin(playerid);

	new depart;

	if (sscanf(params,"i", depart))	return SendHelpMessage(playerid, "/departship	[Tempo de envio em segundos] ");

	ship_depart = gettime() + depart;
	return 1;
}

CMD:timetruck(playerid, params[])
{
    if (!CheckAdmin(playerid, HIGH_ADMIN_LEVEL)) return SendNotAdmin(playerid);

	new depart;

	if (sscanf(params,"i", depart))	return SendHelpMessage(playerid, "/timetruck [tempo em segundos] ");

	time_truck = depart;
	return 1;
}

CMD:nexttship(playerid, params[])
{
    if (!CheckAdmin(playerid, HIGH_ADMIN_LEVEL)) return SendNotAdmin(playerid);

	new depart;

	if (sscanf(params,"i", depart))	return SendHelpMessage(playerid, "/nexttship [Tempo de retorno em segundos] ");

	ship_next = gettime() + depart;
	return 1;
}

CMD:agarage(playerid, params[])
{
	if (!CheckAdmin(playerid, HIGH_ADMIN_LEVEL))	return SendNotAdmin(playerid);

	new option[16], secoption[128];

	if (sscanf(params, "s[16]S()[128]", option, secoption))
	{
	    SendHelpMessage(playerid, "/agarage [opção]");
	    SendSyntaxMessage(playerid, "criar/remover/editar/ir/gempresa/gcasa");
	    return 1;
	}

	if (CompareStrings(option, "criar"))
	{
	    new type, price;

	    if (sscanf(secoption, "ii", type, price)) return SendHelpMessage(playerid, "/agarage criar [tipo] [preço]");
	    if (type < 1 || type > 9) return SendErrorMessage(playerid, "Tipo de 1 a 9.");
	    if (price <= 0 || price > 1000000) return SendErrorMessage(playerid, "Preço de $1 a $1,000,000.");
	    if (GetInterior(playerid) || GetWorld(playerid)) return SendErrorMessage(playerid, "Você deve estar ao ar livre.");
	    if (IsPlayerInAnyVehicle(playerid)) return SendErrorMessage(playerid, "Você deve estar fora do veículo.");
	    if (Garage_Create(playerid, type, price) == -1) return SendErrorMessage(playerid, "O número máximo de garagens foi atingido.");

	    return 1;
	}

	else if (CompareStrings(option, "remover"))
	{
	    new id;

	    if (sscanf(secoption, "i", id))								return SendHelpMessage(playerid, "/agarage remove [ID da garagem]");
	    if (id < 0 || id > MAX_GARAGES || !GarageInfo[id][garageID])	return SendErrorMessage(playerid, "Nenhuma garagem encontrada.");

	    new query[128];
	    mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `garage` WHERE `id` = %i", GarageInfo[id][garageID]);
	    mysql_tquery(dbHandle, query);

	    if (IsValidDynamicCP(GarageInfo[id][garageCP]))	DestroyDynamicCP(GarageInfo[id][garageCP]);

	    GarageInfo[id][garageID] = 0;
	    SendAdmMessageToPlayer(playerid, "Você removeu a garagem [ID:%i].", id);
	    return 1;
	}

	else if (CompareStrings(option, "editar"))
	{
		new id, parameters[16], arg[112];

		if (sscanf(secoption, "is[12]S()[112]", id, parameters, arg))
		{
			SendHelpMessage(playerid, "/agarage editar [ID da garagem] [opção]");
			SendSyntaxMessage(playerid, "pos vpos int preco trancar");
			return 1;
		}

		if (id < 0 || id >= MAX_GARAGES || !GarageInfo[id][garageID])	return SendErrorMessage (playerid, "Garagem não encontrada.");

		if (CompareStrings(parameters, "pos"))
		{
			if (IsPlayerInAnyVehicle(playerid)) return SendErrorMessage(playerid, "Você deve sair do veículo.");
			if (GetInterior(playerid) || GetWorld(playerid)) return SendErrorMessage(playerid, "Você deve estar ao ar livre.");

			GetPlayerPos(playerid, GarageInfo[id][garagePlayer_Enter][0], GarageInfo[id][garagePlayer_Enter][1], GarageInfo[id][garagePlayer_Enter][2]);
			GarageInfo[id][garageEnterInt] = 
			GarageInfo[id][garageEnterVW] = 0;

			if (IsValidDynamicCP(GarageInfo[id][garageCP]))	DestroyDynamicCP(GarageInfo[id][garageCP]);

			GarageInfo[id][garageCP] = CreateDynamicCP(GarageInfo[id][garagePlayer_Enter][0], GarageInfo[id][garagePlayer_Enter][1], GarageInfo[id][garagePlayer_Enter][2], 0.9, 0, 0, -1, 3.0);
			SendAdmMessageToPlayer (playerid, "Você alterou a entrada da garagem [ID:%i].", id);

			SQL_SetFloat("garage", "player_enter_x", GarageInfo[id][garagePlayer_Enter][0], GarageInfo[id][garageID]);
			SQL_SetFloat("garage", "player_enter_y", GarageInfo[id][garagePlayer_Enter][1], GarageInfo[id][garageID]);
			SQL_SetFloat("garage", "player_enter_z", GarageInfo[id][garagePlayer_Enter][2], GarageInfo[id][garageID]);
			SQL_SetInteger("garage", "enter_int", 0, GarageInfo[id][garageID]);
			SQL_SetInteger("garage", "enter_vw", 0, GarageInfo[id][garageID]);
			return 1;
		}
		
		else if (CompareStrings(parameters, "vpos"))
		{
			if (!IsPlayerInAnyVehicle(playerid))				return SendErrorMessage (playerid, "Você deve estar dentro de um veículo.");
			if (GetInterior (playerid) || GetWorld (playerid)) return SendErrorMessage (playerid, "Você deve estar fora de um interior.");

			GetVehiclePos(GetPlayerVehicleID(playerid), GarageInfo[id][garageVeh_Enter][0], GarageInfo[id][garageVeh_Enter][1], GarageInfo[id][garageVeh_Enter][2]);
			GetVehicleZAngle(GetPlayerVehicleID(playerid), GarageInfo[id][garageVeh_Enter][3]);
			SendAdmMessageToPlayer (playerid, "Você alterou a pos de entrada da garagem [ID:%i].", id);

			SQL_SetFloat("garage", "veh_enter_x", GarageInfo[id][garageVeh_Enter][0], GarageInfo[id][garageID]);
			SQL_SetFloat("garage", "veh_enter_y", GarageInfo[id][garageVeh_Enter][1], GarageInfo[id][garageID]);
			SQL_SetFloat("garage", "veh_enter_z", GarageInfo[id][garageVeh_Enter][2], GarageInfo[id][garageID]);
			SQL_SetFloat("garage", "veh_enter_fa", GarageInfo[id][garageVeh_Enter][3], GarageInfo[id][garageID]);
			return 1;
		}

		else if (CompareStrings(parameters, "int"))
		{
			new type;

			if (sscanf(arg, "i", type))						return SendHelpMessage(playerid, "/agarage editar [Garagem ID] int [tipo] ");
			if (type <1 || type> 9) return SendErrorMessage (playerid, "Escolha um tipo de 1 e 9.");
			if (GetInterior(playerid) || GetWorld(playerid))	return SendErrorMessage(playerid, "Você deve estar fora de um interior. ");

			GarageInfo[id][garagePlayer_Exit][0] = garage_player_pos[type-1][0];
			GarageInfo[id][garagePlayer_Exit][1] = garage_player_pos[type-1][1];
			GarageInfo[id][garagePlayer_Exit][2] = garage_player_pos[type-1][2];
			GarageInfo[id][garageVeh_Exit][0] = garage_veh_pos[type-1][0];
			GarageInfo[id][garageVeh_Exit][1] = garage_veh_pos[type-1][1];
			GarageInfo[id][garageVeh_Exit][2] = garage_veh_pos[type-1][2];
			GarageInfo[id][garageVeh_Exit][3] = garage_veh_pos[type-1][3];
			GarageInfo[id][garageExitInt] = garage_int[type-1];
			SendAdmMessageToPlayer(playerid, "Você alterou o interior da garagem [ID: %i] para %i.", id, type);

			SQL_SetFloat("garage", "player_exit_x", GarageInfo[id][garagePlayer_Exit][0], GarageInfo[id][garageID]);
			SQL_SetFloat("garage", "player_exit_y", GarageInfo[id][garagePlayer_Exit][1], GarageInfo[id][garageID]);
			SQL_SetFloat("garage", "player_exit_z", GarageInfo[id][garagePlayer_Exit][2], GarageInfo[id][garageID]);
			SQL_SetFloat("garage", "veh_exit_x", GarageInfo[id][garageVeh_Exit][0], GarageInfo[id][garageID]);
			SQL_SetFloat("garage", "veh_exit_y", GarageInfo[id][garageVeh_Exit][1], GarageInfo[id][garageID]);
			SQL_SetFloat("garage", "veh_exit_z", GarageInfo[id][garageVeh_Exit][2], GarageInfo[id][garageID]);
			SQL_SetFloat("garage", "veh_exit_fa", GarageInfo[id][garageVeh_Exit][3], GarageInfo[id][garageID]);
			SQL_SetInteger("garage", "exit_int", GarageInfo[id][garageExitInt], GarageInfo[id][garageID]);
			return 1;
		}

		else if (CompareStrings(parameters, "preco"))
		{
			new price;

			if (sscanf(arg, "i", price))			return SendHelpMessage(playerid, "/agarage editar [Garagem ID] preco [preco]");
			if (price <= 0 || price > 1000000)	return SendErrorMessage(playerid, "Preço de  $1 à $1000000.");

			GarageInfo[id][garagePrice] = price;
			SQL_SetInteger("garage", "price", GarageInfo[id][garagePrice], GarageInfo[id][garageID]);
			SendAdmMessageToPlayer(playerid, "Você alterou o valor da garagem [ID: %i] para $%i.", id, price);
			return 1;
		}

		else if (CompareStrings(parameters, "trancado"))
		{
			new locked;

			if (sscanf(arg, "i", locked))	return SendHelpMessage(playerid, "/agarage edit [Garagem ID] trancado [0 - aberto | 1 - fechado]");
			if (locked < 0 || locked > 1)	return SendErrorMessage(playerid, "Selecione um valor entre 0 e 1.");

			GarageInfo[id][garageLocked] = locked;
			SQL_SetInteger("garage", "locked", GarageInfo[id][garageLocked], GarageInfo[id][garageID]);
			SendAdmMessageToPlayer(playerid, "Você %s a garagem [ID: %i].", (locked) ? ("trancou") : ("abriu"));
			return 1;
		}

		else 
		{
			SendHelpMessage(playerid, "/agarage editar [Garagem ID] [opção]");
			SendSyntaxMessage(playerid, "pos vpos int preco trancar");
		}
	}

	else if (CompareStrings(option, "ir"))
	{
		new id;

		if (sscanf(secoption, "i", id))									return SendHelpMessage(playerid, "/agarage ir [Garagem ID]");
		if (id < 0 || id >= MAX_GARAGES || !GarageInfo[id][garageID])	return SendErrorMessage(playerid, "Garagem não encontrada.");

		SetPlayerPosEx(playerid, GarageInfo[id][garagePlayer_Enter][0], GarageInfo[id][garagePlayer_Enter][1], GarageInfo[id][garagePlayer_Enter][2]);
		SetInterior(playerid, GarageInfo[id][garageEnterInt]);
		SetWorld(playerid, GarageInfo[id][garageEnterVW]);
		return 1;
	}

	else if (CompareStrings(option, "gempresa"))
	{
		new type, id, price;

		if (sscanf(secoption, "iii", type, price, id))			return SendHelpMessage(playerid, "/agarage gempresa [tipo] [preço] [empresa ID] ");
		if (type <1 || type> 9) return SendErrorMessage (playerid, "Escolha um tipo entre 1 e 9.");
		if (price <= 0 || price> 1000000) return SendErrorMessage (playerid, "Selecione um valor de $1 a $ 1000000.");
		if (id <0 || id>= MAX_BIZ ||! BizInfo [id] [bizID]) return SendErrorMessage (playerid, "Empresa não encontrada.");
		if (Garage_Create(playerid, type, price, 2, id) == -1)	return SendErrorMessage(playerid, "O servidor atingiu o limite de garagens que podem ser criadas");

		return 1;
	}

	else if (CompareStrings(option, "gcasa"))
	{
		new type, id, price;
		
		if (sscanf(secoption, "iii", type, price, id))			return SendHelpMessage(playerid, "/agarage gcasa [tipo] [preço] [Casa ID]");
		if (type < 1 || type > 9)								return SendErrorMessage(playerid, "Escolha um tipo entre 1 e 9.");
		if (price <= 0 || price > 1000000)						return SendErrorMessage(playerid, "Selecione um valor de $1 a $ 1000000.");
		if (id < 0 || id >= MAX_HOUSES || !HouseInfo[id][hID])	return SendErrorMessage(playerid, "Casa não encontrada.");
		if (Garage_Create(playerid, type, price, 1, id) == -1)	return SendErrorMessage(playerid, "O servidor atingiu o limite de garagens que podem ser criadas");

		return 1;
	}

	else 
	{
		SendHelpMessage(playerid, "/agarage [opção]");
		SendSyntaxMessage(playerid, "criar/remover/editar/ir/gempresa/gcasa");
	}
	return 1;
}

CMD:changefight(playerid, params[])
{
    new pid;

    if (!CheckAdmin(playerid, HIGH_ADMIN_LEVEL)) return SendNotAdmin(playerid);
    if (sscanf(params, "u", pid)) return SendHelpMessage(playerid, "/changefight [ID/Parte do Nome]");
    if (!IsPlayerLogged(pid)) return SendNotConnected(playerid);

    SetPVarInt(playerid, "ChangeFightTo", pid);
    Dialog_Show(playerid, ChangeFight, DIALOG_STYLE_LIST, "{FFFFFF}Alterar estilo de luta", "Normal\nBoxe\nKung Fu\nCabeçada no Joelho\nChute Agarrado\nCotovelada", "Alterar", "Cancelar");
    return 1;
}

CMD:adminhide(playerid, params[])
{
	if (!CheckAdmin(playerid, HIGH_ADMIN_LEVEL)) return SendNotAdmin(playerid);

    PlayerInfo[playerid][pAdminHide] = (PlayerInfo[playerid][pAdminHide]) ? (0) : (1);
    SendAdmMessageToPlayer(playerid, "Você %s a si mesmo como um administrador.", (PlayerInfo[playerid][pAdminHide]) ? ("ocultou") : ("mostrou"));

	if (PlayerInfo[playerid][pAdminHide] && IsAdminDuty(playerid)) SetPlayerColor(playerid, TEAM_HIT_COLOR);
	else if (!PlayerInfo[playerid][pAdminHide] && IsAdminDuty(playerid)) SetPlayerColor(playerid, TEAM_ADMIN_COLOR);

	return 1;
}


ALTX:afoodstand("afood");
CMD:afoodstand(playerid, params[])
{
	if (!CheckAdmin(playerid, HIGH_ADMIN_LEVEL))	return SendNotAdmin(playerid);

	new option[16], secoption[128];

	if (sscanf(params, "s[16]S()[128]", option, secoption))
	{
		SendHelpMessage(playerid, "(/food)stand [opção]");
		SendSyntaxMessage(playerid, "criar/remover/ir");
		return 1;
	}

	if (CompareStrings(option, "criar"))
	{
		new type;

		if (sscanf(secoption, "i", type)) return SendHelpMessage(playerid, "(/food) stand criar [0 - sem objeto | 1 - com objeto]");
		if (type < 0 || type > 1) return SendErrorMessage(playerid, "Tipo de 0 a 1.");
		if (IsPlayerInAnyVehicle(playerid)) return SendErrorMessage(playerid, "Você deve sair do veículo.");

		new standid = -1;

		for(new i; i < MAX_FOOD_STAND; i++)
		{
			if (FoodStand[i][fStandOn]) continue;

			standid = i;
			break;
		}

		if (standid == -1)	return SendErrorMessage(playerid, "O servidor atingiu o limite de pontos de foods.");

		GetPlayerPos(playerid, FoodStand[standid][fPosX], FoodStand[standid][fPosY], FoodStand[standid][fPosZ]);
		
		FoodStand[standid][fPosRX] = 
		FoodStand[standid][fPosRY] = 
		FoodStand[standid][fPosRZ] = 0.0;
		FoodStand[standid][fVW] = GetWorld(playerid);
		FoodStand[standid][fInt] = GetInterior(playerid);
		FoodStand[standid][fType] = type;

		if (type)
		{
			FoodStand[standid][fObject] = CreateDynamicObject(1340, FoodStand[standid][fPosX], FoodStand[standid][fPosY], FoodStand[standid][fPosZ], 0.0, 0.0, 0.0, FoodStand[standid][fVW], FoodStand[standid][fInt], -1, 200.0);
		}

		//FoodStand[standid][fLabel] = CreateDynamic3DTextLabel(""EMBED_CYAN"/buyfood", -1, FoodStand[standid][fPosX] + 1.5, FoodStand[standid][fPosY], FoodStand[standid][fPosZ], 2.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, FoodStand[standid][fVW], FoodStand[standid][fInt], -1, 100.0);
		FoodStand[standid][fPickUp] = CreateDynamicPickup(1239, 1, FoodStand[standid][fPosX] + 1.5, FoodStand[standid][fPosY], FoodStand[standid][fPosZ], FoodStand[standid][fVW], FoodStand[standid][fInt], -1, 100.0);
		FoodStand[standid][fStandOn] = 1;
		AddFoodStandToFile(standid, FoodStand[standid][fType], FoodStand[standid][fPosX], FoodStand[standid][fPosY], FoodStand[standid][fPosZ], FoodStand[standid][fVW], FoodStand[standid][fInt]);
		SendAdmMessageToPlayer(playerid, "Você criou uma tenda de comida [ID: %i].", standid);
		return 1;
	}

	else if (CompareStrings(option, "remover"))
	{
		new standid;

		if (sscanf(secoption, "i", standid))											return SendHelpMessage(playerid, "(/food)stand remover [Tenda ID]");
		if (standid < 0 || standid >= MAX_FOOD_STAND || !FoodStand[standid][fStandOn])	return SendErrorMessage(playerid,"Tenda não encontrada.");

		if (FoodStand[standid][fType] && IsValidDynamicObject(FoodStand[standid][fObject]))
		{
			DestroyDynamicObject(FoodStand[standid][fObject]);
			FoodStand[standid][fObject] = INVALID_OBJECT_ID;
		}

		/*if (IsValidDynamic3DTextLabel(FoodStand[standid][fLabel]))
		{
			DestroyDynamic3DTextLabel(FoodStand[standid][fLabel]);
			FoodStand[standid][fLabel] = Text3D:INVALID_3DTEXT_ID;
		}*/

		if (IsValidDynamicPickup(FoodStand[standid][fPickUp]))	DestroyDynamicPickup(FoodStand[standid][fPickUp]);

		new query[128];
		mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `foodstand` WHERE `id` = %i", FoodStand[standid][fID]);
		mysql_tquery(dbHandle, query);

		FoodStand[standid][fStandOn] = 
		FoodStand[standid][fID] = 0;
		SendAdmMessageToPlayer(playerid, "Tenda de comida [ID: %i] apagada.", standid);
		return 1;
	}

	else if (CompareStrings(option, "ir"))
	{
		new standid;

		if (sscanf(secoption, "i", standid))											return SendHelpMessage(playerid, "(/food)stand ir [Tenda ID]");
		if (standid < 0 || standid >= MAX_FOOD_STAND || !FoodStand[standid][fStandOn])	return SendErrorMessage(playerid, "Tenda não encontrada. ");

		SetPlayerPosEx(playerid, FoodStand[standid][fPosX], FoodStand[standid][fPosY], FoodStand[standid][fPosZ]);
		SetWorld(playerid, FoodStand[standid][fVW]);
		SetInterior(playerid, FoodStand[standid][fInt]);
	}

	else 
	{
		SendHelpMessage(playerid, "(/food)stand [opção]");
		SendSyntaxMessage(playerid, "criar/remover/ir");
	}
	return 1;
}

CMD:ajob(playerid, params[])
{
	if (!CheckAdmin(playerid, HIGH_ADMIN_LEVEL))	return SendNotAdmin(playerid);

	new option[16], secoption[128];

	if (sscanf(params, "s[16]S()[128]", option, secoption))
	{
		SendHelpMessage(playerid, "/ajob [opção]");
		SendSyntaxMessage(playerid, "criar/remover/ir");
	}

	if (CompareStrings(option, "criar"))
	{
		new type;

		if (sscanf(secoption, "i", type))
		{
			SendHelpMessage(playerid, "/ajob criar [tipo de emprego]");
			SendSyntaxMessage(playerid, "1 - caminhoneiro | 2 - mecanico | 3 - taxista | 6 - entregador de pizza | 7 - ladrão de carro");
			SendSyntaxMessage(playerid, "8 - lenhador | 9 - lixeiro | 10 - minerador");
			return 1;
		}
		if (type < 0 || type > 10)			return SendErrorMessage(playerid, "Tipo de emprego inválido.");
		if (IsPlayerInAnyVehicle (playerid)) return SendErrorMessage (playerid, "Você deve estar fora de um veículo.");

		new jobid = -1;

		for(new i = 0; i < MAX_JOBS; i++)
		{
			if (JobsInfo[i][jobOn])	continue;

			jobid = i;
			continue;
		}

		if (jobid == -1)	return SendErrorMessage(playerid, "O servidor atingiu o limite máximo de empregos que podem ser criado.");

		GetPlayerPos(playerid, JobsInfo[jobid][jobX], JobsInfo[jobid][jobY], JobsInfo[jobid][jobZ]);
		
		JobsInfo[jobid][jobVW] = GetWorld(playerid);
		JobsInfo[jobid][jobInt] = GetInterior(playerid);
		JobsInfo[jobid][jobType] = type;
		JobsInfo[jobid][jobOn] = 1;

		JobsInfo[jobid][jobPickUp] = CreateDynamicPickup(1314, 1, JobsInfo[jobid][jobX], JobsInfo[jobid][jobY], JobsInfo[jobid][jobZ], JobsInfo[jobid][jobVW], JobsInfo[jobid][jobInt], -1, 100.0);

		new jobtext[32];

		switch(type)
		{
			case JOB_MECHANIC:	jobtext = "Mecânico";
			case JOB_TRUCKER:	jobtext = "Caminhoneiro";
			case JOB_TAXI:		jobtext = "Taxista";
			case JOB_PIZZA:		jobtext = "Entregador de Pizza";
			case JOB_CARJACKER: jobtext = "Ladrão de carro";
		}

		AddJobToFile(jobid, JobsInfo[jobid][jobType], JobsInfo[jobid][jobX], JobsInfo[jobid][jobY], JobsInfo[jobid][jobZ], JobsInfo[jobid][jobInt], JobsInfo[jobid][jobVW]);
		SendAdmMessageToPlayer(playerid, "Você criou o emprego de %s [ID: %i].", jobtext, jobid);
		return 1;
	}

	else if (CompareStrings(option, "remover"))
	{
		new jobid;

		if (sscanf(secoption, "i", jobid))								return SendHelpMessage(playerid, "/ajob remover [Emprego ID]");
		if (jobid < 0 || jobid >= MAX_JOBS || !JobsInfo[jobid][jobOn])	return SendErrorMessage(playerid, "Emprego não encontrado.");

		if (IsValidDynamicPickup(JobsInfo[jobid][jobPickUp]))		DestroyDynamicPickup(JobsInfo[jobid][jobPickUp]);

		new query[128];
		mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `jobs` WHERE `id` = %i", JobsInfo[jobid][jobID]);
		mysql_tquery(dbHandle, query);

		JobsInfo[jobid][jobID] = 
		JobsInfo[jobid][jobOn] = 0;
		SendAdmMessageToPlayer(playerid, "Você apagou o emprego [ID: %i].", jobid);
		return 1;
	}

	else if (CompareStrings(option, "ir"))
	{
		new jobid;

		if (sscanf(secoption, "i", jobid))								return SendHelpMessage(playerid, "/ajob ir [Emprego ID] ");
		if (jobid <0 || jobid>= MAX_JOBS ||! JobsInfo [jobid] [jobOn]) return SendErrorMessage (playerid, "Emprego não encontrado.");

		SetPlayerPosEx(playerid, JobsInfo[jobid][jobX], JobsInfo[jobid][jobY], JobsInfo[jobid][jobZ]);
		SetInterior(playerid, JobsInfo[jobid][jobInt]);
		SetWorld(playerid, JobsInfo[jobid][jobVW]);
	}

	else 
	{
		SendHelpMessage(playerid, "/ajob [opção]");
		SendSyntaxMessage(playerid, "criar/remover/ir");
	}
	return 1;
}

ALTX:asc("/aspeedcam");
CMD:asc(playerid, params[])
{
	if (!CheckAdmin(playerid, HIGH_ADMIN_LEVEL))	return SendNotAdmin(playerid);

	new option[16], secoption[128];

	if (sscanf(params, "s[16]S()[128]", option, secoption))
	{
		SendHelpMessage(playerid, "/asc [opção]");
		SendHelpMessage(playerid, "criar remover editar lista");
	}

	if (CompareStrings(option, "criar"))
	{
		new sp_limit, sp_zone[64], id = SC_GetFreeID();
		if (id == -1)									return SendErrorMessage (playerid, "Todos os slots de radar de velocidade estão ocupados!");
		if (sscanf(secoption, "d", sp_limit))			return SendHelpMessage (playerid, "/asc criar [limite de velocidade] ");
		if (sp_limit <= 0 || sp_limit> SC_MAX_SPEED)	return SendErrorMessage (playerid, "Você deve inserir um limite de velocidade de 1 à %d.", SC_MAX_SPEED);
		
        GetPlayerPos(playerid, speedcam[id][sc_x], speedcam[id][sc_y], speedcam[id][sc_z]);
        GetPlayer2DZone(playerid, sp_zone, sizeof(sp_zone));

		format(speedcam[id][sc_name], 64, "%s 12%i", sp_zone, id);

		new Float:x, Float:y, Float:z;
		GetPlayerPos(playerid, x, y, z); 

        speedcam[id][sc_speed] = sp_limit;
        speedcam[id][sc_x] = x;
        speedcam[id][sc_y] = y+1;
        speedcam[id][sc_z] = z;
		speedcam[id][sc_rx] = 0;
        speedcam[id][sc_ry] = 0;
        speedcam[id][sc_rz] = 0;
        speedcam[id][sc_int] = GetPlayerInterior(playerid);
        speedcam[id][sc_vw] = GetPlayerVirtualWorld(playerid);
        speedcam[id][sc_object] = CreateDynamicObject(18880, speedcam[id][sc_x], speedcam[id][sc_y], speedcam[id][sc_z], speedcam[id][sc_rx], speedcam[id][sc_ry], speedcam[id][sc_rz], speedcam[id][sc_vw], speedcam[id][sc_int]);
		Iter_Add(speedcamObjects, id);

		speedcam[id][sc_areaid] = CreateDynamicSphere(speedcam[id][sc_x], speedcam[id][sc_y], speedcam[id][sc_z], SC_DEF_RADIUS, speedcam[id][sc_vw], speedcam[id][sc_int]);

		AddSpeedcamToFile(id, sp_limit, speedcam[id][sc_name], speedcam[id][sc_x], speedcam[id][sc_y], speedcam[id][sc_z], 0.0, 0.0, 0.0, speedcam[id][sc_int], speedcam[id][sc_vw]);
        SendAdmMessageToPlayer(playerid, "Você criou um radar de velocidade [ID: %i] %s | com limite de: %d.", id, speedcam[id][sc_name], sp_limit);
		return 1;
	}

	else if (CompareStrings(option, "remover"))
	{
		new sp_near = GetNearestSpeed(playerid, 20.0);
        
        if (sp_near == -1)                                   return SendErrorMessage(playerid, "Não há radares de velocidade neste raio.");

        if (IsValidDynamicObject(speedcam[sp_near][sc_object]))
        {
            DestroyDynamicObject(speedcam[sp_near][sc_object]);
            speedcam[sp_near][sc_object] = INVALID_OBJECT_ID;
        }

        new sql_query[128];

        SendAdmMessageToPlayer(playerid, "Você removeu o radar de velocidade [ID: %i | %s].", sp_near, speedcam[sp_near][sc_name]);

        speedcam[sp_near][sc_name] = EOS;
        Iter_Remove(speedcamObjects, sp_near);
		
        mysql_format(dbHandle, sql_query, sizeof(sql_query), "DELETE FROM `speedcams` WHERE `id` = %i", sp_near);
        mysql_tquery(dbHandle, sql_query);
        return 1;
	}

	else if (CompareStrings(option, "editar"))
    {
        new sp_near = GetNearestSpeed(playerid, 15.0), thirdoption[64], fouroption[64];

        if (sp_near == -1)	return SendErrorMessage(playerid, "Não há radares de velocidade neste raio.");     

        if (sscanf(secoption, "s[64]S()[64]", thirdoption, fouroption))        
        {
            SendHelpMessage(playerid, "/sc editar [opção]");
            SendHelpMessage(playerid, "pos limite");
            return 1;
        }
        else if (CompareStrings(thirdoption, "limite"))
        {
            new sp_limit;

            if (sscanf(fouroption, "d", sp_limit))           return SendHelpMessage(playerid, "/sc editar limite [velocidade]");
            if (sp_limit <= 0 || sp_limit > SC_MAX_SPEED) 	return SendErrorMessage(playerid, "Você deve inserir um limite de velocidade de 1 à %d.", SC_MAX_SPEED);

            speedcam[sp_near][sc_speed] = sp_limit;
            SQL_SetInteger("speedcams", "speed", speedcam[sp_near][sc_speed], speedcam[sp_near][sc_id]);
            SendAdmMessageToPlayer(playerid, "Você alterou o limite de velocidade do radar [ID: %i] %s para %d.", sp_near, speedcam[sp_near][sc_name], sp_limit);
            return 1;
        }

        else if (CompareStrings(thirdoption, "pos"))
        {
            SetPVarInt(playerid, "SC:ID", sp_near + 1);
            SetPVarInt(playerid, "SC:Edit", 1);
            EditDynamicObject(playerid, speedcam[sp_near][sc_object]);
            return 1;
        }
    }

    else if (CompareStrings(option, "list"))
    {
		new dialog_header[96] = "{FFFFFF}Radar de Velocidade\t{FFFFFF}Radar de Velocidade ID\t{FFFFFF}Limite de velocidade\n", dialog_body[3072];

		format(dialog_body, sizeof(dialog_body), "%s", dialog_header);

		for(new i = 0; i < Iter_Count(speedcamObjects); i++)
		{
			format(dialog_body, sizeof(dialog_body), "%s%s\t%i\t%d\n", dialog_body, speedcam[i][sc_name], i, speedcam[i][sc_speed]);
		}

		Dialog_Show(playerid, None, DIALOG_STYLE_TABLIST_HEADERS, "{FFFFFF}Lista de radares de velocidade", dialog_body, "Fechar", "");
    }  
	return 1;
}

CMD:acctv(playerid, params[])
{
	if (!CheckAdmin(playerid, HIGH_ADMIN_LEVEL))	return SendNotAdmin(playerid);
	
	new option[16], secoption[128];

	if (sscanf(params, "s[16]S()[128]", option, secoption)) return SendHelpMessage(playerid, "/acctv [criar/remover/editar/lista/repall/ir]");

	if (CompareStrings(option, "criar"))
	{
		new name[32];

		if (sscanf(secoption, "s[32]", name))		return SendHelpMessage(playerid, "/acctv criar [nome]");
		if (strlen (name) <4 || strlen (name)> 30) return SendErrorMessage (playerid, "O nome deve ter de 4 a 30 caracteres.");
		if (! CheckCCTVEquals (name)) return SendErrorMessage (playerid, "Este nome de camera já existe.");

		new nearcam = GetNearestCamera(playerid, 40.0);

		if (nearcam!= -1) return SendErrorMessage (playerid, "A câmera já está operando neste raio.");

		new id = -1;

		for(new i = 0; i < MAX_CCTV; i++)
		{
			if (CCTVInfo[i][tvObjectOn])	continue;

			id = i;
			break;
		}

		if (id == -1)	return SendErrorMessage(playerid, "O servidor atingiu o limite máximo de CCTVs que podem ser criadas.");
		format(CCTVInfo[id][tvName], 32, "%s", name);
		GetPlayerPos(playerid, CCTVInfo[id][tvPosX], CCTVInfo[id][tvPosY], CCTVInfo[id][tvPosZ]);

		CCTVInfo[id][tvPosRX] = 
		CCTVInfo[id][tvPosRY] = 
		CCTVInfo[id][tvPosRZ] = 0.0;
		CCTVInfo[id][tvInt] = GetInterior(playerid);
		CCTVInfo[id][tvVW] = GetWorld(playerid);
		CCTVInfo[id][tvObject] = CreateDynamicObject(1886, CCTVInfo[id][tvPosX], CCTVInfo[id][tvPosY], CCTVInfo[id][tvPosZ], 0.0, 0.0, 0.0, CCTVInfo[id][tvVW], CCTVInfo[id][tvInt]);
		CCTVInfo[id][tvCollision] = CreateDynamicObject(3800, CCTVInfo[id][tvPosX], CCTVInfo[id][tvPosY], CCTVInfo[id][tvPosZ]-1.0, CCTVInfo[id][tvPosRX], CCTVInfo[id][tvPosRY], CCTVInfo[id][tvPosRZ], CCTVInfo[id][tvVW], CCTVInfo[id][tvInt]);
		SetDynamicObjectMaterial(CCTVInfo[id][tvCollision], 0, 16644, "a51_detailstuff", "roucghstonebrtb", 0x00000000);
		CCTVInfo[id][tvObjectOn] = 1;
		AddCameraToFile(id, CCTVInfo[id][tvInt], CCTVInfo[id][tvVW], CCTVInfo[id][tvPosX], CCTVInfo[id][tvPosY], CCTVInfo[id][tvPosZ], 0.0, 0.0, 0.0, name);
		SendAdmMessageToPlayer(playerid, "Você criou uma camera %s [ID: %i].", CCTVInfo[id][tvName], id);
		return 1;
	}

	else if (CompareStrings(option, "remover"))
	{
		new id;

		if (sscanf(secoption, "i", id))								return SendHelpMessage(playerid, "/acctv remover [Camera ID]");
		if (id < 0 || id >= MAX_CCTV || !CCTVInfo[id][tvObjectOn])	return SendErrorMessage(playerid, "CCTV não encontrada.");

		new query[128];
		mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `cctvs` WHERE `id` = %i", CCTVInfo[id][tvID]);
		mysql_tquery(dbHandle, query);

		if (IsValidDynamicObject(CCTVInfo[id][tvObject]))
		{
			DestroyDynamicObject(CCTVInfo[id][tvObject]);
			DestroyDynamicObject(CCTVInfo[id][tvCollision]);
			CCTVInfo[id][tvObject] = INVALID_OBJECT_ID;
			CCTVInfo[id][tvCollision] = INVALID_OBJECT_ID;
		}
		
		CCTVInfo[id][tvID] = 
		CCTVInfo[id][tvObjectOn] = 0;
		SendAdmMessageToPlayer(playerid, "Você removeu a camera [ID: %i].", id);
		return 1;
	}

	else if (CompareStrings(option, "editar"))
	{
		new id, parameters[16], arg[112];

		if (sscanf(secoption, "is[16]S()[112]", id, parameters, arg)) return SendHelpMessage(playerid, "/acctv editar [ID CCTVs] [pos/nome]");

		if (id < 0 || id >= MAX_CCTV || !CCTVInfo[id][tvObjectOn] || !IsValidDynamicObject(CCTVInfo[id][tvObjectOn]))	return SendErrorMessage (playerid, "CCTV não encontrada.");

		if (CompareStrings(parameters, "pos"))
		{
			if (GetPVarInt(playerid, #edit_cctv))																			return SendErrorMessage(playerid, "Você já estar editando uma CCTV.");
			if (!IsPlayerInRangeOfPoint(playerid, 5.0, CCTVInfo[id][tvPosX], CCTVInfo[id][tvPosY], CCTVInfo[id][tvPosZ]))	return SendErrorMessage(playerid, "Você deve estar perto de uma CCTV.");

			SetPVarInt(playerid, #edit_cctv, id + 1);
			EditDynamicObject(playerid, CCTVInfo[id][tvObject]);
			return 1;
		}

		else if (CompareStrings(parameters, "nome"))
		{
			new name[32];

			if (sscanf(arg, "s[32]", name))				return SendHelpMessage(playerid, "/acctv editar [ID CCTVs] nome [nome]");
			if (strlen(name) < 4 || strlen(name) > 30)	return SendErrorMessage(playerid, "O nome do CCTVs deve ter de 4 a 30 caracteres. ");
			if (!CheckCCTVEquals(name))					return SendErrorMessage(playerid, "Esse nome já existe em outro CCTV.");

			format(CCTVInfo[id][tvName], 32, "%s", name);
			SQL_SetString("cctvs", "info", CCTVInfo[id][tvName], CCTVInfo[id][tvID]);
			SendAdmMessageToPlayer(playerid, "Você alterou o nome do CCTV [ID: %i] para %s.", id, CCTVInfo[id][tvName]);
			return 1;
		}
	}
	
	else if (CompareStrings(option, "lista"))
	{
		new dialog_header[96] = "{FFFFFF}CCTV nome\t{FFFFFF}ID\tStatus\n", dialog_body[3072];

		format(dialog_body, sizeof(dialog_body), "%s", dialog_header);

		for(new i = 0; i < MAX_CCTV; i++)
		{
			if (!CCTVInfo[i][tvObjectOn]) continue;

			new break_str[24];

            if (CCTVInfo[i][tvBreak])    format(break_str, sizeof(break_str), "{CC0000}Offline");
            else                        format(break_str, sizeof(break_str), "{00B200}Online");

			format(dialog_body, sizeof(dialog_body), "%s%s\t%i\t%s\n", dialog_body, CCTVInfo[i][tvName], i, break_str);
		}

		Dialog_Show(playerid, None, DIALOG_STYLE_TABLIST_HEADERS, "{FFFFFF} CCTV Lista ", dialog_body, "Fechar", "");
		return 1;
	}

	else if (CompareStrings(option, "repall"))
	{
		for(new i = 0; i < MAX_CCTV; i++)
		{
			if (!CCTVInfo[i][tvObjectOn])                        continue;
        	if (!IsValidDynamicObject(CCTVInfo[i][tvObject])) continue;
			if (!CCTVInfo[i][tvBreak])							continue;

			SetCameraBreakState(i, 0);	
		}

		SendAdmMessageToPlayer(playerid,"Você reparou todas as cameras CCTVs.");
	}

	else if (CompareStrings(option, "ir"))
	{
		new id;

		if (sscanf(secoption, "i", id))								return SendHelpMessage(playerid, "/acctv ir [ID cctv]");
		if (id < 0 || id > MAX_CCTV || !CCTVInfo[id][tvObjectOn])	return SendErrorMessage(playerid, "CCTV não encontrada.");

		SetPlayerPosEx(playerid, CCTVInfo[id][tvPosX], CCTVInfo[id][tvPosY], CCTVInfo[id][tvPosZ]);
		SetWorld(playerid, CCTVInfo[id][tvVW]);
		SetInterior(playerid, CCTVInfo[id][tvInt]);
		return 1;
	}
	
	else SendHelpMessage(playerid, "/acctv [criar/remover/editar/lista/repall/ir]");
	
	return 1;
}

CMD:aatm(playerid, params[])
{
	if (!CheckAdmin(playerid, HIGH_ADMIN_LEVEL))	return SendNotAdmin(playerid);

	new option[16], secoption[128];
	
	if (sscanf(params, "s[16]S()[128]", option, secoption)) return SendHelpMessage(playerid, "/aatm [criar/remover/editar/ir]");

	if (CompareStrings(option, "criar"))
	{
		new id = -1;

		for(new i; i < MAX_ATMS; i++)
		{
			if (ATMInfo[i][atmID]) continue;

			id = i;
			break;
		}

		if (id == -1)	return SendErrorMessage (playerid, "O servidor atingiu o limite maximo de ATMS que podem ser criadas.");

		new Float:x, Float:y, Float:z, Float:angle;

		GetPlayerPos(playerid, x, y, z);
		GetPlayerFacingAngle(playerid, angle);

		x += 1.0 * floatsin(-angle, degrees);
		y += 1.0 * floatcos(-angle, degrees);

		ATMInfo[id][atmPosX] = x;
		ATMInfo[id][atmPosY] = y;
		ATMInfo[id][atmPosZ] = z - 0.4;
		ATMInfo[id][atmPosRZ] = angle;
		ATMInfo[id][atmInt] = GetInterior(playerid);
		ATMInfo[id][atmVW] = GetWorld(playerid);
		ATMInfo[id][atmObject] = CreateDynamicObject(2942, ATMInfo[id][atmPosX], ATMInfo[id][atmPosY], ATMInfo[id][atmPosZ], 0.0, 0.0, ATMInfo[id][atmPosRZ], ATMInfo[id][atmVW], ATMInfo[id][atmInt]);
		
		SetPVarInt(playerid, #edit_atm, id + 1);
		EditDynamicObject(playerid, ATMInfo[id][atmObject]);

		AddATMToFile(id, ATMInfo[id][atmPosX], ATMInfo[id][atmPosY], ATMInfo[id][atmPosZ], ATMInfo[id][atmPosRZ], ATMInfo[id][atmInt], ATMInfo[id][atmVW]);
		SendAdmMessageToPlayer(playerid, "Você criou uma ATM [ID: %i].", id);
		return 1;
	}

	else if (CompareStrings(option, "remover"))
	{
		new id;
		
		if (sscanf(secoption, "i", id))							return SendHelpMessage(playerid, "/aatm remover [ID ATM]");
		if (id < 0 || id >= MAX_ATMS || !ATMInfo[id][atmID])	return SendErrorMessage(playerid, "ATM não encontrada.");

		if (IsValidDynamicObject(ATMInfo[id][atmObject]))
		{
			DestroyDynamicObject(ATMInfo[id][atmObject]);
			ATMInfo[id][atmObject] = INVALID_OBJECT_ID;
		}

		new query[128];
		mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `atms` WHERE `id` = %i", ATMInfo[id][atmID]);
		mysql_tquery(dbHandle, query);

		ATMInfo[id][atmID] = 0;
		SendAdmMessageToPlayer(playerid, "Você removeu a ATM [ID: %i].", id);
		return 1;
	}

	else if (CompareStrings(option, "editar"))
	{
		new id;

		if (sscanf(secoption, "d", id))						return SendHelpMessage(playerid, "/aatm editar [ID ATM]");
		if (id < 0 || id > MAX_ATMS || !ATMInfo[id][atmID])	return SendErrorMessage (playerid, "ATM não encontrada.");
		if (ATM_Nearest (playerid)!= id) 					return SendErrorMessage (playerid, "Você deve estar perto de uma ATM.");

		SetPVarInt(playerid, #edit_atm, id + 1);
		EditDynamicObject(playerid, ATMInfo[id][atmObject]);
		return 1;
	}

	else if (CompareStrings(option, "ir"))
	{
		new id;

		if (sscanf(secoption, "i", id))						return SendHelpMessage(playerid, "/aatm ir [ATM ID]");
		if (id < 0 || id > MAX_ATMS || !ATMInfo[id][atmID])	return SendErrorMessage(playerid, "ATM não encontrada. ");

		SetPlayerPosEx(playerid, ATMInfo[id][atmPosX], ATMInfo[id][atmPosY], ATMInfo[id][atmPosZ]);
		SetWorld(playerid, ATMInfo[id][atmVW]);
		SetInterior(playerid, ATMInfo[id][atmInt]);
		return 1;
	}
	
	else
	{
		SendHelpMessage(playerid, "/aatm [criar/remover/editar/ir]");
	}
	return 1;
}

CMD:agate(playerid, params[])
{
	new option[16], secoption[128], modelid, faction, gateid, Float:o_rad, toption;

	if (!CheckAdmin(playerid, HIGH_ADMIN_LEVEL)) return SendNotAdmin(playerid);
	if (sscanf(params, "s[16]S()[128]", option, secoption))
	{
		SendHelpMessage(playerid, "/agate [criar/remover/editar/ir]");
		return 1;
	}

	if (CompareStrings(option, "criar"))
	{
		if (sscanf(secoption, "ii", modelid, faction)) 	return SendHelpMessage(playerid, "/agate criar [ID object] [ID facção (/afaction lista)]");
		if (faction < 0 || faction > MAX_FACTIONS-1) 	return SendErrorMessage(playerid, "ID de facção inválido.");

		new gate = -1;
		for(new i; i < MAX_GATES; i++)
		{
			if (Gates[i][gateID] != 0 || Gates[i][gateObjectOn] != 0) continue;
			gate = i;
			break;
		}

		if (gate == -1) return SendErrorMessage(playerid, "O servidor atingiu o limite máximo de portões que podem ser criados.");

		GetPlayerPos(playerid, Gates[gate][gatePosX], Gates[gate][gatePosY], Gates[gate][gatePosZ]);
		GetPlayerPos(playerid, Gates[gate][gateMoveX], Gates[gate][gateMoveY], Gates[gate][gateMoveZ]);

		Gates[gate][gateObject] = CreateDynamicObject(modelid, Gates[gate][gatePosX], Gates[gate][gatePosY], Gates[gate][gatePosZ], 0.0, 0.0, 0.0, GetWorld(playerid), GetInterior(playerid), -1, 200.0);

		if (Gates[gate][gateObject] == INVALID_OBJECT_ID) return SendErrorMessage(playerid,"O objeto não foi criado.");

		Gates[gate][gateSpeed] = 0.5;
		Gates[gate][gateRadius] = 5.0;
		Gates[gate][gateModel] = modelid;
		Gates[gate][gateMoveRX] = 
		Gates[gate][gateMoveRY] = 
		Gates[gate][gateMoveRZ] = 
		Gates[gate][gatePosRX] = 
		Gates[gate][gatePosRY] = 
		Gates[gate][gatePosRZ] = 0.0;
		Gates[gate][gateInt] = GetInterior(playerid);
		Gates[gate][gateVW] = GetWorld(playerid);
		Gates[gate][gateSound] = 0;
		Gates[gate][gateFaction] = faction;
		Gates[gate][gateOpened] = 0;
		Gates[gate][gateObjectOn] = 1;

		AddGateToFile(gate, modelid, faction, GetInterior(playerid), GetWorld(playerid), Gates[gate][gatePosX], Gates[gate][gatePosY], Gates[gate][gatePosZ]);
		SendAdmMessageToPlayer(playerid, "Portão [ID: %i] criado.", gate);
		return 1;
	}

	else if (CompareStrings(option, "remover"))
	{
		if (sscanf(secoption, "i", gateid)) return SendHelpMessage(playerid, "/agate remover [Portão ID]");
		if (!Gates[gateid][gateObjectOn])	return SendErrorMessage(playerid, "ID de portão inválido.");

		new query[128];

		mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `gates` WHERE `id`= %i", Gates[gateid][gateID]);
		mysql_tquery(dbHandle, query, "OnGateRemove", "i", gateid);

		if (IsValidDynamicObject(Gates[gateid][gateObject])) DestroyDynamicObject(Gates[gateid][gateObject]);

		Gates[gateid][gatePosX] = 
		Gates[gateid][gatePosY] = 
		Gates[gateid][gatePosZ] = 
		Gates[gateid][gatePosRX] =
		Gates[gateid][gatePosRY] = 
		Gates[gateid][gatePosRZ] = 
		Gates[gateid][gateSpeed] = 
		Gates[gateid][gateRadius] =
		Gates[gateid][gateMoveX] = 
		Gates[gateid][gateMoveY] = 
		Gates[gateid][gateMoveZ] = 
		Gates[gateid][gateMoveRX] = 
		Gates[gateid][gateMoveRY] =
		Gates[gateid][gateMoveRZ] = 0.0;
		Gates[gateid][gateObjectOn] = 
		Gates[gateid][gateInt] = 
		Gates[gateid][gateFaction] = 
		Gates[gateid][gateOpened] = 
		Gates[gateid][gateVW] = 
		Gates[gateid][gateID] = 0;

		SendAdmMessageToPlayer(playerid, "Portão removido [ID: %i].", gateid);
		return 1;
	}

	else if (CompareStrings(option, "editar"))
	{
		new parameters[16], arg[112];

		if (sscanf(secoption, "s[16]S()[5]", parameters, arg))
		{
			SendHelpMessage(playerid, "/agate editar [pos/mover/velocidade/raio/som]");
			return 1;
		}

		if (CompareStrings(parameters, "pos"))
		{
			if (sscanf(arg, "i", gateid)) 															return SendHelpMessage(playerid, "/agate editar pos [Portão ID]");
			if (gateid < 0 || gateid >= MAX_GATES || !Gates[gateid][gateObjectOn]) 					return SendErrorMessage(playerid, "ID de portão inválido");
			if (GetPVarInt(playerid, #2_edit_gate) != 0 || GetPVarInt(playerid, #edit_gate) != 0) 	return SendErrorMessage(playerid, "Você já está editando este objeto.");

			SetPVarInt(playerid, #edit_gate, gateid+1);
			EditDynamicObject(playerid, Gates[gateid][gateObject]);
			return 1;

		}

		else if (CompareStrings(parameters, "mover"))
		{
			if (sscanf(arg, "i", gateid)) 															return SendHelpMessage(playerid, "/agate editar mover [Portão ID]");
			if (gateid < 0 || gateid >= MAX_GATES || !Gates[gateid][gateObjectOn])					return SendErrorMessage(playerid, "ID de portão inválido");
			if (GetPVarInt(playerid, #2_edit_gate) != 0 || GetPVarInt(playerid, #edit_gate) != 0) 	return SendErrorMessage(playerid, "Você já está editando este objeto. ");

			SetPVarInt(playerid, #2_edit_gate, gateid+1);
			EditDynamicObject(playerid, Gates[gateid][gateObject]);
			return 1;
		}

		else if (CompareStrings(parameters, "velocidade"))
		{
			if (sscanf(arg, "if", gateid, o_rad)) 									return SendHelpMessage(playerid, "/agate editar velocidade [Portão ID] [velocidade (0.1 - 255.0)]");
			if (gateid < 0 || gateid >= MAX_GATES || !Gates[gateid][gateObjectOn]) 	return SendErrorMessage(playerid, "ID de portão inválido");
			if (o_rad < 0.1 || o_rad > 255.0) 										return SendErrorMessage(playerid, "O valor da velocidade deve ser de 0.1 à 255.0.");

			Gates[gateid][gateSpeed] = o_rad;
			SQL_SetFloat("gates", "speed", Gates[gateid][gateSpeed], Gates[gateid][gateID]);
			SendAdmMessageToPlayer(playerid, "Você alterou a velocidade do portão [ID: %i] para %.2f", gateid, o_rad);
			return 1;
		}

		else if (CompareStrings(parameters, "raio"))
		{
			if (sscanf(arg, "if", gateid, o_rad)) 									return SendHelpMessage(playerid, "/agate editar raio [Portão ID] [raio de distancia (0.1 - 30.0)] ");
			if (gateid < 0 || gateid >= MAX_GATES || !Gates[gateid][gateObjectOn])	return SendErrorMessage(playerid, "ID de portão inválido");
			if (o_rad < 0.1 || o_rad > 30.0)										return SendErrorMessage (playerid, "O raio de distancia deve estar de 0.1 à 30.0.");

			Gates[gateid][gateRadius] = o_rad;
			SQL_SetFloat("gates", "radius", Gates[gateid][gateRadius], Gates[gateid][gateID]);
			SendAdmMessageToPlayer(playerid, "Você alterou o raio de interação do portão [ID: %i] para %.2f", gateid, o_rad);
			return 1;
		}

		else if (CompareStrings(parameters, "som"))
		{
			if (sscanf(arg, "ii", gateid, toption))									return SendHelpMessage(playerid, "/agate editar som [Portão ID][Som ID | 0 - remover som] ");
			if (gateid < 0 || gateid >= MAX_GATES || !Gates[gateid][gateObjectOn]) 	return SendErrorMessage(playerid, "ID de portão inválido");
			if (toption < 0)														return SendErrorMessage(playerid, "Som não encontrado.");

			Gates[gateid][gateSound] = toption;
			SQL_SetInteger("gates", "sound", Gates[gateid][gateSound], Gates[gateid][gateID]);
			SendAdmMessageToPlayer(playerid, "Você alterou o som do portão [ID: %i] para %d.", gateid, toption);
			return 1;
		}

		else 
		{
			SendHelpMessage(playerid, "/agate editar [pos/mover/velocidade/raio/som]");
		}
		return 1;
	}

	else if (CompareStrings(option, "ir"))
	{
		if (sscanf(secoption, "i", gateid)) 									return SendHelpMessage(playerid, "/agate ir [Portão ID]");
		if (gateid < 0 || gateid >= MAX_GATES || !Gates[gateid][gateObjectOn])	return SendErrorMessage(playerid, "ID de portão inválido");

		SetPlayerPosEx(playerid, Gates[gateid][gatePosX], Gates[gateid][gatePosY], Gates[gateid][gatePosZ]);
		SetWorld(playerid, Gates[gateid][gateVW]);
		SetInterior(playerid, Gates[gateid][gateInt]);
		return 1;	
	}

	else 
	{
		SendHelpMessage(playerid, "/agate [opção]");
		SendSyntaxMessage(playerid, "criar/remover/editar/ir");
	}
	return 1;
}

CMD:astreet(playerid, params[])
{
	if (!CheckAdmin(playerid, HIGH_ADMIN_LEVEL))	return SendNotAdmin(playerid);

	new option[5], secoption[5];
	if (sscanf(params, "s[7]S()[5]", option, secoption)) return SendHelpMessage(playerid, "/astreet [add/editar/remover/ir]");

	if (CompareStrings(option, "add")) {
		if (!GetPlayerVirtualWorld(playerid) && !GetPlayerInterior(playerid)) {
			new Float:x, Float:y, Float:z;
			GetPlayerPos(playerid, x, y, z);

			x += 1.0;

			SetPVarInt(playerid, #edit_street, 1);

			new Street_Sign = CreateDynamicObject(19981, x, y, z, 0.0, 0.0, 0.0, -1, -1, -1, 300.00, 300.00);
			EditDynamicObject(playerid, Street_Sign);
		}
	}
	else if (CompareStrings(option, "editar")) {
		new id;
		if (sscanf(secoption,"i", id)) 	return SendHelpMessage(playerid, "/astreet editar [id]");
		if (!street_data[id][street_id])	return SendErrorMessage(playerid, "ID inválido. (( /perto ))");
		if (GetPlayerDistanceFromPoint(playerid, street_data[id][sSign][0], street_data[id][sSign][1], street_data[id][sSign][2]) > 5.0)
			return SendErrorMessage(playerid, "Você está muito longe da placa!");

		SetPVarInt(playerid, #edit_street, 2);
		SetPVarInt(playerid, #id_street, id);

		EditDynamicObject(playerid, street_data[id][street_object][0]);
	}
	else if (CompareStrings(option, "remover")) {
		new id;
		if (sscanf(secoption,"i", id))	return SendHelpMessage(playerid, "/astreet remover [id]");
		if (!street_data[id][street_id])	return SendErrorMessage(playerid, "ID inválido. (( /perto ))");

		DeleteStreet(playerid, id);
	}
	else if (CompareStrings(option, "ir")) {
		new id;
		if (sscanf(secoption,"i", id))	return SendHelpMessage(playerid, "/astreet ir [id]");
		if (!street_data[id][street_id])	return SendErrorMessage(playerid, "ID inválido. (( /perto ))");

		SetPlayerPosEx(playerid, street_data[id][sSign][0],street_data[id][sSign][1],street_data[id][sSign][2]);
	}
	return 1;
}

CMD:stname(playerid, params[])
{
	if (!CheckAdmin(playerid, HIGH_ADMIN_LEVEL))	return SendNotAdmin(playerid);
	
	new id;
	if (sscanf(params, "i", id)) 		return SendHelpMessage(playerid, "/stname [id]");
	if (!street_data[id][street_id])	return SendErrorMessage(playerid, "ID inválido. (( /perto ))");
	if (GetPlayerDistanceFromPoint(playerid, street_data[id][sSign][0], street_data[id][sSign][1], street_data[id][sSign][2]) > 5.0)
		return SendErrorMessage(playerid, "Você está muito longe da placa!");

	SetPVarInt(playerid, #edit_street, 3);
	EditDynamicObject(playerid, street_data[id][street_object][1]); //Nome
	return 1;
}

CMD:stdir(playerid, params[])
{
	if (!CheckAdmin(playerid, HIGH_ADMIN_LEVEL))	return SendNotAdmin(playerid);

	new id;
	if (sscanf(params, "i", id)) 		return SendHelpMessage(playerid, "/stdir [id]");
	if (!street_data[id][street_id])	return SendErrorMessage(playerid, "ID inválido. (( /perto ))");
	if (GetPlayerDistanceFromPoint(playerid, street_data[id][sSign][0], street_data[id][sSign][1], street_data[id][sSign][2]) > 5.0)
		return SendErrorMessage(playerid, "Você está muito longe da placa!");

	SetPVarInt(playerid, #edit_street, 3);
	EditDynamicObject(playerid, street_data[id][street_object][2]); //Direção
	return 1; 
}

CMD:getareas(playerid, params[])
{
	if (!CheckAdmin(playerid, HIGH_ADMIN_LEVEL))	return SendNotAdmin(playerid);
	
	new areas[10], j = GetPlayerNumberDynamicAreas(playerid);
	GetPlayerDynamicAreas(playerid, areas, sizeof(areas));

	for(new s; s < 10; s ++) {
		for(new i; i < sizeof(street_names); i++) {
			if (IsValidDynamicArea(areas[s]) && areas[s] == street_zones[i]) {
				SM1(playerid, "[%i] %i. %s", s, i, street_names[i]);
			}
		}
	}

	SM2(playerid, "-------TOTAL: %i-------", j);
	return 1;
}

ALTX:abillboard("/abb");
CMD:abillboard(playerid, params[])
{
	if (!CheckAdmin(playerid, HIGH_ADMIN_LEVEL)) return SendNotAdmin(playerid);
	
	new option[16], secoption[128];

	if (sscanf(params, "s[16]S()[7]", option, secoption))
	{
		SendHelpMessage(playerid, "/abillboard [criar/remover/editar/ir]");
		return 1;
	}

	if (CompareStrings(option, "criar"))
	{
        new bb = GetUnusedBillBoard();
	    if (bb == -1) return SendErrorMessage(playerid, "O servidor atingiu o limte máximo de outdoors que podem ser criados!");
		
		if (strval(secoption) <= 0) return SendHelpMessage(playerid, "/abb criar [preço]");

		SetPVarInt(playerid, #BB_COST, strval(secoption));
        ShowModelSelectionMenu(playerid, "Billboards", MODEL_SELECTION_BB, billboard_obj, sizeof(billboard_obj));
	}
	if (CompareStrings(option, "remover"))
	{
		new id;
		if (sscanf(secoption, "I(-1)", id)) return SendHelpMessage(playerid, "/abb remover {c7c7c7}[ID outdoor]");
		
		if (id == -1) {
			if ((id = GetClosestBillBoard(playerid, 15.0)) == -1) return SendErrorMessage(playerid, "Não há outdoor próximo a você!");
		}
		
		if (id >= MAX_BILLBOARDS || id < 0)	return SendErrorMessage(playerid, "ID inválido.");
		if (!BillboardInfo[id][bbActive])	return SendErrorMessage(playerid, "ID inválido.");

		SendAdmMessageToPlayer(playerid, "Você removeu o outdoor #%i!", id+1);
		DeleteBillBoard(id);
	}
	if (CompareStrings(option, "editar"))
	{
		new id;
		if (sscanf(secoption, "I(-1)", id)) return SendHelpMessage(playerid, "/abb editar {c7c7c7}[ID outdoor]");
		
		if (id == -1) {
			if ((id = GetClosestBillBoard(playerid, 15.0)) == -1) return SendErrorMessage(playerid, "Não há outdoor próximo a você!");
		}
		
		if (id >= MAX_BILLBOARDS || id < 0)	return SendErrorMessage(playerid, "ID inválido.");
		if (!BillboardInfo[id][bbActive])	return SendErrorMessage(playerid, "ID inválido.");

		SetPVarInt(playerid, "BB:Edit", id+1);
		EditDynamicObject(playerid, BillboardInfo[id][bbObject]);
	}
	if (CompareStrings(option, "ir"))
	{
		new id;
		if (sscanf(secoption, "i", id)) return SendHelpMessage(playerid, "/abb ir {c7c7c7}[ID outdoor]");
		
		if (id >= MAX_BILLBOARDS || id < 0)	return SendErrorMessage(playerid, "ID inválido.");
		if (!BillboardInfo[id][bbActive])	return SendErrorMessage(playerid, "ID inválido.");

		SetPlayerPosEx(playerid, BillboardInfo[id][bbPosX], BillboardInfo[id][bbPosY], BillboardInfo[id][bbPosZ]);
		SetInterior(playerid, BillboardInfo[id][bbInt]);
		SetWorld(playerid, BillboardInfo[id][bbVW]);
	} 
	else SendHelpMessage(playerid, "/abillboard [criar/remover/editar/ir]");

	return 1;
}

