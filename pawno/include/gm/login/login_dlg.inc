Dialog:AccountRegister(playerid, response, listitem, inputtext[])
{
	if(!response)	KickEx(playerid);
	ClearChatbox(playerid, 50);

	if(isnull(inputtext) || strlen(inputtext) > 24 || strlen(inputtext) < 5) {
		Dialog_Show(playerid, AccountRegister, DIALOG_STYLE_INPUT, "Registro de Conta", "Digite o apelido desejado (24 caracteres):\n{C7C7C7}Exemplo: {FFFFFF}danny", ">>>", "");
		return SendErrorMessage(playerid, "O comprimento do nome da conta deve ser de 5 a 24 caracteres!");
	}
	
	if(strfind(inputtext, "/") != -1 || strfind(inputtext, "\\") != -1 ||
		strfind(inputtext, ":") != -1 || strfind(inputtext, "*") != -1 ||
		strfind(inputtext, "?") != -1 || strfind(inputtext, "\"") != -1 ||
		strfind(inputtext, "<") != -1 || strfind(inputtext, ">") != -1 ||
		strfind(inputtext, "|") != -1 || isnull(inputtext) || strfind(inputtext, " ") != -1)	{
		
		Dialog_Show(playerid, AccountRegister, DIALOG_STYLE_INPUT, "Registro de Conta", "Digite o apelido desejado (24 caracteres):\n{C7C7C7}Exemplo: {FFFFFF}danny", ">>>", "");
		return SendErrorMessage(playerid, "Caracteres inválidos no nome da conta!");
	}

	SetPVarString(playerid, #AccountName, inputtext);
	
	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `accounts` WHERE `login` = '%e'", inputtext);
	mysql_tquery(dbHandle, query, "CheckAccName", "i", playerid);
	return 1;
}


this::CheckAccName(playerid)
{
	if(cache_num_rows()) {
		SendErrorMessage(playerid, "Este apelido já está em uso!");
		Dialog_Show(playerid, AccountRegister, DIALOG_STYLE_INPUT, "Registro de Conta", "Digite o apelido desejado (24 caracteres):\n{C7C7C7}Exemplo: {FFFFFF}danny", ">>>", "");
		return 1;
	}

	ClearChatbox(playerid, 50);
	SendServerMessage(playerid, "Agora você precisa inserir um e-mail válido.");
	Dialog_Show(playerid, EmailRegister, DIALOG_STYLE_INPUT, "Registro de Conta", "Digite um e-mail válido:", ">>>", "");
	return 1;
}

Dialog:EmailRegister(playerid, response, listitem, inputtext[])
{
	if(!response)	KickEx(playerid);
	ClearChatbox(playerid, 50);
	
	SetPVarString(playerid, #EmailName, inputtext);

	SendServerMessage(playerid, "Escolha uma senha.");

	Dialog_Show(playerid, PassRegister, DIALOG_STYLE_PASSWORD, "Registro de Conta", "Crie e digite uma senha forte:", ">>>", "");
	return 1;
}
Dialog:PassRegister(playerid, response, listitem, inputtext[])
{
	if(!response)	KickEx(playerid);

    if(strlen(inputtext) < 6 || strlen(inputtext) > 32)
    {
        Dialog_Show(playerid, PassRegister, DIALOG_STYLE_PASSWORD, "{FFFFFF} Nova senha", "{FFFFFF} Digite a senha:", ">>>", "");
		return SendErrorMessage(playerid, "A senha deve ter pelo menos 6 e no máximo 32 caracteres.");
	}

	for(new i = 0; i < strlen(inputtext); i++)
	{
		switch(inputtext[i])
		{
			case '!', '@', '#', '$','%','^','&','*','(',')','_','+','=','|','[',']','{','}','-','.','`','~','<','>','?',',','/':
			{
			    Dialog_Show(playerid, PassRegister, DIALOG_STYLE_PASSWORD, "Nova senha", "Digite a senha:", ">>>", "");
				return SendErrorMessage(playerid, "Você usou um símbolo proibido.");
			}
			default: continue;
		}
	}

	new query[512], pass[129], ip_reg[8], name[24], email[24];

	GetPlayerIp(playerid, ip_reg, sizeof(ip_reg));
	GetPVarString(playerid, #AccountName, name, 24);
	GetPVarString(playerid, #EmailName, email, 24);
	
	WP_Hash(pass, sizeof(pass), inputtext);
	mysql_format(dbHandle, query, sizeof(query), "INSERT INTO `accounts` (`login`,`password`,`email`,`reg_ip`,`reg_date`) VALUES ('%s','%e','%e','%s','%e')", name, pass, email, ip_reg, get_date("%dd/%mm/%yyyy, %hh:%ii", gettime()));
	mysql_tquery(dbHandle, query);
	
	ClearChatbox(playerid, 50);
	SendServerMessage(playerid, "Parabéns, você se registrou com sucesso! Aguarde um pouco...");

	SetPlayerName(playerid, name);

	SetTimerEx("CheckAccount", 1500, false, "i", playerid);
	return 1;
}


Dialog:CreateCharacter(playerid, response, listitem, inputtext[])
{
	if(!response)	return ShowCharacterMenu(playerid);

	if(strfind(inputtext, "/") != -1 || strfind(inputtext, "\\") != -1 ||
		strfind(inputtext, ":") != -1 || strfind(inputtext, "*") != -1 ||
		strfind(inputtext, "?") != -1 || strfind(inputtext, "\"") != -1 ||
		strfind(inputtext, "<") != -1 || strfind(inputtext, ">") != -1 ||
		strfind(inputtext, "|") != -1 || isnull(inputtext) || strfind(inputtext, " ") != -1)	{
		Dialog_Show(playerid, CreateCharacter, DIALOG_STYLE_INPUT, "Criar personagem", "Digite o nome do personagem:", ">>>", "Voltar");
		return SendErrorMessage(playerid, "Caracteres inválidos no nome do personagem! ");
	}
	
	if(!RPnamecheck(inputtext)) {
		Dialog_Show(playerid, CreateCharacter, DIALOG_STYLE_INPUT, "Criar personagem", "Digite o nome do personagem:", ">>>", "Voltar");
		return SendErrorMessage(playerid, "Caracteres inválidos no nome do personagem! ");
	}

	new underline = 0, maxname = strlen(inputtext);

    for(new i = 0; i < maxname; i++)
    {
       if(inputtext[i] == '_') underline++;
    }
    if(underline != 1) return 0;
    inputtext[0] = toupper(inputtext[0]);
    for(new x = 1; x < maxname; x++)
    {
        if(inputtext[x] == '_') inputtext[x + 1] = toupper(inputtext[x + 1]);
        else if(inputtext[x] != '_' && inputtext[x - 1] != '_') inputtext[x] = tolower(inputtext[x]);
    }

	SetPVarString(playerid, #CharacterName, inputtext);

	const MAX_SKINS = 312;
	new subString[16];
	static string[MAX_SKINS * sizeof(subString)];

	if (string[0] == EOS) {
		for (new i; i < MAX_SKINS; i++) {
			format(subString, sizeof(subString), "%i\tID: %i\n", i, i);
			strcat(string, subString);
		}
	}

	Dialog_Show(playerid, CreationCharSkinSel, DIALOG_STYLE_PREVIEW_MODEL, "Escolha a skin do seu personagem", string, "Selecionar", "Cancelar");
	return 1;
}


Dialog:CreationCharSkinSel(playerid, response, listitem, inputtext[])
{
	if (!response) ShowCharacterMenu(playerid);

	new name[MAX_PLAYER_NAME], query[78 - 2 - 2 - 2 + 4 + MAX_PLAYER_NAME + 3];
	GetPVarString(playerid, #CharacterName, name, 24);

	mysql_format(dbHandle, query, sizeof(query), "INSERT INTO `users` (`accountid`,`name`,`skin`,`status`) VALUES (%i,'%e',%i,1)", PlayerInfo[playerid][pAccID], name, listitem);
	mysql_tquery(dbHandle, query);

	ShowCharacterMenu(playerid);
}

Dialog:FirstSpawn(playerid, response, listitem, inputtext[])
{
	if (!response)
	{
    	Dialog_Show(playerid, FirstSpawn, DIALOG_STYLE_LIST, "{8D8DFF}Escolha um local de spawn permanente", "Palomino Creek\nMontgomery\nDillimore\nFort Carson", "Selecionar", "");
		return 1;
	}
	
	PlayerInfo[playerid][pSpawnPoint] = listitem+1;
	SetSpawnInfoEx(playerid, first_spawn[PlayerInfo[playerid][pSpawnPoint]-1][0], first_spawn[PlayerInfo[playerid][pSpawnPoint]-1][1], first_spawn[PlayerInfo[playerid][pSpawnPoint]-1][2]);
	pTemp[playerid][pLogged] = 2;
	
	SQL_SetInteger("users", "spawnpoint", PlayerInfo[playerid][pSpawnPoint], PlayerInfo[playerid][pID]);
	
 	SpawnPlayer(playerid);
	return 1;
}

Dialog:SelectChar(playerid, response, listitem, inputtext[])
{
	if (!response) 						return KickEx(playerid);
	if (listitem < 0 || listitem > 2)	return KickEx(playerid);

	if (login_char_id[playerid][listitem] == 0)
	{	    
		#if REGISTRATION == 0
			ShowCharacterMenu(playerid);
			SendErrorMessage(playerid,"Este slot está vazio.");
		#else
			Dialog_Show(playerid, CreateCharacter, DIALOG_STYLE_INPUT, "Criação de Personagem", "Digite o nome do personagem:\n {C7C7C7}Exemplo: {FFFFFF}Joe_Johnson", ">>>", "Voltar");
	    #endif

		return 1;
	}

	SetPVarInt(playerid, #select_char, listitem);

	new mes[128];
	format(mes, sizeof(mes), "{FFFFFF}Tem certeza de que quer jogar com este personagem - {A9C4E4}%s?", login_char_name[playerid][listitem]);

	ConfirmDialog(playerid, "Confirmação", mes, "SelectChar2");
	return 1;
}


this::SelectChar2(playerid, response)
{
	if (!response)	return ShowCharacterMenu(playerid);

	new listitem = GetPVarInt(playerid, #select_char);
	if (listitem < 0 || listitem > 2)
		return printf("Catch"), KickEx(playerid);

	new query[256];
	format(query, sizeof(query), "SELECT users.*, accounts.admin, accounts.admin_code, accounts.donate, accounts.helper, accounts.name_changes, accounts.number_changes FROM users, accounts WHERE users.id = %i AND users.accountid = accounts.id", login_char_id[playerid][listitem]);
    mysql_tquery(dbHandle, query, "LoadCharacter", "i", playerid);
	return 1;
}

Dialog:Login(playerid, response, listitem, inputtext[])
{
    if (!response)	
    {
        SendSyntaxMessage(playerid, "Você saiu do servidor.");
        KickEx(playerid);
        return 1;
    }

    if (isnull(inputtext))
    {   
        static const str_size[] = "{c7c3c3}SERVIDOR:\t{FFFFFF}Você tem 5 minutos para logar\n\
                                   {c7c3c3}DICA:\t\t{FFFFFF}Você pode recuperar sua senha em\n\
                                   \t\t{c7c3c3}"WeburlName"\n\
                                   \t\t{FFFFFF}acessando a área do UCP\n\n\
                                   Insira sua senha:"; // Janela de autenticação
        new mes[sizeof(str_size)+103];
        
        if (pTemp[playerid][pLoggedTry] != 0)
            format(mes, sizeof(mes), "{FF6347}[ ! ] {FFFFFF}Você inseriu uma senha incorreta!\n{FF6347}[ ! ] {FFFFFF}Tentativas restantes: {FF6347}%d\n\n%s", 3-pTemp[playerid][pLoggedTry], str_size);  
        else 
            format(mes, sizeof(mes), str_size);
        
        Dialog_Show(playerid, Login, DIALOG_STYLE_PASSWORD, "Bem-vindo ao "ProjectName"", mes, "Login", "Cancelar");
        return 1;
    }

    new query[128 + 129], buf[129];

    WP_Hash(buf, sizeof (buf), inputtext);
    mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `accounts` WHERE `id` = %i AND `password`='%e'", PlayerInfo[playerid][pAccID], buf);
    mysql_tquery(dbHandle, query, "LoginPlayer", "i", playerid);
    return 1;
}
