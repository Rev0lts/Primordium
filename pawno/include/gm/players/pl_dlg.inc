Dialog:SelectSpawn(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

    SendSyntaxMessage(playerid, "Agora você irá aparecer no local habitual.");
	PlayerInfo[playerid][pSpawnType] = 0;
	PlayerInfo[playerid][pSpawnPoint] = listitem+1;

	SQL_SetInteger("users", "spawntype", PlayerInfo[playerid][pSpawnType], PlayerInfo[playerid][pID]);
	SQL_SetInteger("users", "spawnpoint", PlayerInfo[playerid][pSpawnPoint], PlayerInfo[playerid][pID]);
	return 1;
}

Dialog:ChangeWalk(playerid, response, listitem, inputtext[])
{
	if (!response) return ShowSettings(playerid);
	    
	new str[5];
	format(str, sizeof(str), "1 %i", listitem);
	cmd::definirestilo(playerid, str);
	ShowSettings(playerid);
	return 1;
}

Dialog:ChangeTalk(playerid, response, listitem, inputtext[])
{
	if (!response) return ShowSettings(playerid);

	new str[5];
	format(str, sizeof(str), "2 %i", listitem);
	cmd::definirestilo(playerid, str);
	ShowSettings(playerid);
	return 1;
}

Dialog:ChangeFightstyle(playerid, response, listitem, inputtext[])
{
	if (!response) return ShowSettings(playerid);

	new str[5];
	format(str, sizeof(str), "3 %i", listitem+1);
	cmd::definirestilo(playerid, str);
	ShowSettings(playerid);
	return 1;
}

Dialog:ChangeHud(playerid, response, listitem, inputtext[])
{
	if (!response) return ShowSettings(playerid);

	new str[5];
	format(str, sizeof(str), "4 %i", listitem+1);
	cmd::definirestilo(playerid, str);
	ShowSettings(playerid);

	for(new i=0; i<24; i++)	PlayerTextDrawDestroy(playerid, speedTD[i][playerid]);
	Speedometr_TDcreate(playerid);
	return 1;
}

Dialog:ChangeSpawn(playerid, response, listitem, inputtext[])
{
	if (!response) return ShowSettings(playerid);

	new str[2];
	format(str, sizeof(str), "%i", listitem);
	cmd::setspawn(playerid, str);

	if (listitem > 1)
		ShowSettings(playerid);

	return 1;
}
Dialog:MainSettings(playerid, response, listitem, inputtext[])
{
    if (!response)                   return cmd::mm(playerid, "");
    if (!IsPlayerLogged(playerid))   return 1;
        
    switch(listitem)
    {
        case 0: Dialog_Show(playerid, ChangeWalk, DIALOG_STYLE_LIST, "{FFFFFF}Estilo de caminhada (/walk)", "Desativar\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17", "Selecionar", "Retornar");
        case 1: Dialog_Show(playerid, ChangeTalk, DIALOG_STYLE_LIST, "{FFFFFF}Estilo de conversação", "Desativar\n1\n2\n3\n4\n5\n6\n7\n8\n9", "Selecionar", "Retornar");
        case 2: Dialog_Show(playerid, ChangeFightstyle, DIALOG_STYLE_LIST, "{FFFFFF}Estilo de combate", "Usual\nBoxe\nKungfu\nCabeçadas\nChutes", "Selecionar", "Retornar");
        case 3: Dialog_Show(playerid, ChangeHud, DIALOG_STYLE_LIST, "{FFFFFF}Estilo do velocímetro", "Fonte 1, acima\nFonte 1, abaixo\nFonte 2, acima\nFonte 2, abaixo", "Selecionar", "Retornar");
        case 4: Dialog_Show(playerid, ChangeSpawn, DIALOG_STYLE_LIST, "{FFFFFF}Local de surgimento", "Surgimento padrão\nCasa\nFacção\nÚltima saída", "Selecionar", "Retornar");
        case 5: cmd::tog(playerid, "ooc");
        case 6: cmd::tog(playerid, "nome");
        case 7: cmd::tog(playerid, "ads");
        case 8: cmd::tog(playerid, "infoentrada");
        case 9: cmd::tog(playerid, "gov");
        case 10: cmd::tog(playerid, "pm");
        case 11: cmd::tog(playerid, "adm");
        case 12: cmd::tog(playerid, "noticias");
        case 13: cmd::tog(playerid, "afk");
        case 14: cmd::tog(playerid, "hud");
        case 15: cmd::tog(playerid, "f");
        case 16: cmd::tog(playerid, "entrar");
        case 17:
        {
            if (!CompareStrings(PlayerInfo[playerid][pGoogle], "!"))
            {
                format(PlayerInfo[playerid][pGoogle], 17, "!");

                new query[128];
        
                mysql_format(dbHandle, query, sizeof(query), "UPDATE `accounts` SET `google_auth`='!' WHERE `id` = %i", PlayerInfo[playerid][pAccID]);
                mysql_tquery(dbHandle, query);
                SendSyntaxMessage(playerid, "A verificação em duas etapas do Google Authenticator está desativada. ");
            }
            else
            {
                new __letters[32][] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y","Z","2", "3", "4", "5", "6", "7"};

                pTemp[playerid][pGoogle] = EOS;

                for(new i; i < 17; i++)
                {
                    strcat(pTemp[playerid][pGoogle], __letters[random(sizeof(__letters))], 17);
                }
                    
                static const str_size[] = "{FFFFFF}Sua chave secreta: {FF6347}%s. {FFFFFF}Insira-a no aplicativo Google Authenticator e salve.\n{FF6347} Atenção! Não compartilhe a chave secreta com ninguém em hipótese alguma.\n\n {ffffff} Para confirmação, insira o código do Google Authenticator na caixa abaixo:";
                new mes[sizeof(str_size) - 2 + 1 + 17];

                format(mes, sizeof(mes), str_size, pTemp[playerid][pGoogle]);
                Dialog_Show(playerid, AcceptGoogle, DIALOG_STYLE_INPUT, "Confirmação", mes, "Confirmar", "Cancelar");
            }
        }
        case 18: cmd::alterarsenha(playerid, "");
    }
    
    if (listitem >= 5 && listitem <= 16)  ShowSettings(playerid);
        
    return 1;
}


CMD:alterarsenha(playerid, params[])
{
	Dialog_Show(playerid, ChangePass, DIALOG_STYLE_PASSWORD, "Inserindo informações classificadas", "Digite a senha atual para alterar:", ">>>", "Cancelar");
	return 1;
}

ALTX:mm("/menu");
CMD:mm(playerid, params[])
{
    Dialog_Show(playerid, MainMenu, DIALOG_STYLE_LIST, "{FFFFFF}Menu", "1. Inventário\n\
    2. Estatísticas do personagem\n\
    3. Registrar uma queixa contra um jogador {ffc46a} (/report) {FFFFFF}\n\
    4. Fazer uma pergunta à administração {ffc46a} (/askq) {FFFFFF}\n\
    5. Ajuda com comandos do servidor\n\
    6. Configurações\n\
    7. Doar", "Selecionar", "Cancelar");
    return 1;
}


CMD:settings(playerid, params[])
{
	ShowSettings(playerid);
	return 1;
}

Dialog:MainMenu(playerid, response, listitem, inputtext[])
{
    if (!response) return 1;
        
    switch(listitem)
    {
        case 0: cmd::inventory(playerid, "");
        case 1: cmd::stats(playerid, "");
        case 2: Dialog_Show(playerid, MakeReport, DIALOG_STYLE_INPUT, "{FFFFFF}Denúncia"," {FFFFFF} Insira o ID do jogador e o conteúdo da denúncia na janela abaixo: ","Enviar", "Cancelar");
        case 3: Dialog_Show(playerid, MakeHelp, DIALOG_STYLE_INPUT, "{FFFFFF}Pergunta "," {FFFFFF} Insira o texto da sua pergunta na caixa abaixo: "," Perguntar ", "Cancelar");
        case 4: cmd::ajuda(playerid, "");
        case 5: ShowSettings(playerid);
        case 6: cmd::donate(playerid, "");
        //case 7: return cmd::mm(playerid, "");
    }
    return 1;
}

Dialog:MakeReport(playerid, response, listitem, inputtext[])
{
    if (!response)          return cmd::mm(playerid, "");
    if (!strlen(inputtext)) return Dialog_Show(playerid, MakeReport, DIALOG_STYLE_INPUT, "{FFFFFF}Denúncia", "{FFFFFF}Insira seu ID de jogador e o conteúdo da denúncia na caixa abaixo:\n{C3C3C3}O formato: [ID do Jogador] [Denúncia]", "Enviar", "Cancelar");
        
    cmd::denuncia(playerid, inputtext);
    return 1;
}


Dialog:CmdServer2(playerid, response, listitem, inputtext[])
{
	if (response) cmd::ajuda(playerid, "");
	    
	return 1;
}

Dialog:CmdServer(playerid, response, listitem, inputtext[])
{
	if (!response)
	    return 1;
	    
	switch(listitem)
	{
	    case 0: cmd::ajudatecnica(playerid, "");
	    case 1: cmd::ajudaprincipal(playerid, "");
	    case 2: cmd::ajudachat(playerid, "");
	    case 3: cmd::ajudafaccao(playerid, "");
	    case 4: cmd::ajudacasa(playerid, "");
	    case 5: cmd::ajudaempresa(playerid, "");
		case 6: cmd::ajudaveiculo(playerid, "");
	    case 7: cmd::ajudabanco(playerid, "");
	    case 8: cmd::ajudacelular(playerid, "");
		case 9: cmd::ajudaarmas(playerid, "");
		case 10: cmd::ajudadroga(playerid, "");
	    case 11: cmd::ajudatrabalho(playerid, "");
		case 12: cmd::ajudapesca(playerid, "");
		case 13: cmd::ajudaradio(playerid, "");
		case 14: cmd::ajudadesmanche(playerid, "");
	}
	return 1;
}

Dialog:MakeHelp(playerid, response, listitem, inputtext[])
{
    if (!response)          return cmd::mm(playerid, "");
    if (!strlen(inputtext)) return Dialog_Show(playerid, MakeHelp, DIALOG_STYLE_INPUT, "{FFFFFF}Pergunta", "{FFFFFF}Insira o texto da sua pergunta na caixa abaixo: "," Perguntar", "Cancelar");
        
    cmd::pergunta(playerid, inputtext);
    return 1;
}


Dialog:ChangePass(playerid, response, listitem, inputtext[])
{
	if (!response) 																return 1;
	if (isnull(inputtext) || strlen(inputtext) < 6 || strlen(inputtext) > 64)	return Dialog_Show(playerid, ChangePass, DIALOG_STYLE_PASSWORD, "{FFFFFF}Entering classified information", "{FFFFFF}Enter your current password to change:", ">>>", "Cancel");

	new query[128 + 129], buf[129];

	WP_Hash(buf, sizeof (buf), inputtext);
	mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `accounts` WHERE `id`=%i AND `password`='%e'", PlayerInfo[playerid][pAccID], buf);
	mysql_tquery(dbHandle, query, "OnPlayerChangePass", "i", playerid);
	return 1;
}

Dialog:ChangePass2(playerid, response, listitem, inputtext[])
{
    if (!response) return 1;

    if (strlen(inputtext) < 6 || strlen(inputtext) > 32)
    {
        Dialog_Show(playerid, ChangePass2, DIALOG_STYLE_PASSWORD, "{FFFFFF}Nova senha", "{FFFFFF} Digite a nova senha:", "Mudar", "Cancelar");
        return SendErrorMessage(playerid, "A senha deve ter pelo menos 6 e no máximo 32 caracteres. ");
    }

    for(new i = 0; i < strlen(inputtext); i++)
    {
        switch(inputtext[i])
        {
            case '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '|', '[', ']', '{', '}', '-', '.', '`', '~', '<', '>', '?', ',', '/':
            {
                Dialog_Show(playerid, ChangePass2, DIALOG_STYLE_PASSWORD, "Nova senha", "Digite a nova senha:", "Mudar", "Cancelar");
                return SendErrorMessage(playerid, "Você usou um símbolo proibido.");
            }
            default: continue;
        }
    }

    new query[128 + 129], buf[129];

    WP_Hash(buf, sizeof (buf), inputtext);
    mysql_format(dbHandle, query, sizeof(query), "UPDATE `accounts` SET `password`='%e' WHERE `id` = %i", buf, PlayerInfo[playerid][pAccID]);
    mysql_tquery(dbHandle, query);

    SendSyntaxMessage(playerid, "Sua senha foi alterada.");
    return 1;
}


Dialog:ConfirmWord(playerid, response, listitem, inputtext[])
{
    if (!response || isnull(inputtext))
    {
        Dialog_Show(playerid, ConfirmWord, DIALOG_STYLE_PASSWORD, "{c7c3c3}Google Authenticator", "{FFFFFF}Você está se conectando de um endereço IP não verificado.\nDigite o código do aplicativo Google Authenticator para confirmar que você é o dono da conta.", ">>>", "");
        return 1;
    }

    if (strval(inputtext) != GoogleAuthenticatorCode(PlayerInfo[playerid][pGoogle], gettime()))
    {
        if (pTemp[playerid][pLoggedTry]++ >= 2)
        {
            SendErrorMessage(playerid, "Você digitou o código incorretamente 3 vezes. ");
            KickEx(playerid);
            return 1;
        }

        SendErrorMessage(playerid, "Você digitou o código errado.");
        Dialog_Show(playerid, ConfirmWord, DIALOG_STYLE_PASSWORD, "{c7c3c3}Google Authenticator", "{FFFFFF}Você está se conectando de um endereço IP não verificado.\nDigite o código do aplicativo Google Authenticator para assegurar que você é o dono da conta.", ">>>", "");
        return 1;
    }
    static const str_size[] = "{c7c3c3}SERVIDOR:\t{FFFFFF}Você tem 5 minutos para entrar\n\
                               {c7c3c3}DICA:\t\t{FFFFFF}Você pode recuperar sua senha\n\
                               \t\tno site: {c7c3c3}"WeburlName"\n\
                               \t\t{FFFFFF}e entrar no painel da conta\n\n\
                               Digite sua senha:"; // Mensagem de ajuda
    new mes[sizeof(str_size)];

    format(mes, sizeof(mes), str_size);
    Dialog_Show(playerid, Login, DIALOG_STYLE_PASSWORD, "Bem-vindo ao "ProjectName"", mes, "Entrar", "Cancelar");
    pTemp[playerid][pLoggedTime] = 0;
    pTemp[playerid][pLoggedTry] = 0;
    return 1;
}


Dialog:ChangeFight(playerid, response, listitem, inputtext[])
{
    if (!response) return 1;

    SendAdmMessage("%s alterou o estilo de luta de %s para %s.", GetAccName(playerid), GetNameEx(GetPVarInt(playerid, "ChangeFightTo")), fight_name[listitem]);
    SendErrorMessage(GetPVarInt(playerid, "ChangeFightTo"), "Seu estilo de luta foi alterado para %s pelo administrador %s.", fight_name[listitem], GetAccName(playerid));
    SetPlayerFightingStyle(GetPVarInt(playerid, "ChangeFightTo"), fight_id[listitem]);
    PlayerInfo[GetPVarInt(playerid, "ChangeFightTo")][pFightStyle] = fight_id[listitem];
    DeletePVar(playerid, "ChangeFightTo");
    return 1;
}

Dialog:AcceptGoogle(playerid, response, listitem, inputtext[])
{
    if (!IsPlayerLogged(playerid))    return 1;
    if (!response)                    return ShowSettings(playerid);
        
    if (strval(inputtext) != GoogleAuthenticatorCode(pTemp[playerid][pGoogle], gettime()))
    {
        SendErrorMessage(playerid, "Código inválido, por favor tente novamente.");
        return ShowSettings(playerid);
    }

    format(PlayerInfo[playerid][pGoogle], 17, pTemp[playerid][pGoogle]);

    new query[128];

    mysql_format(dbHandle, query, sizeof(query), "UPDATE `accounts` SET `google_auth`='%s' WHERE `id` = %i", PlayerInfo[playerid][pGoogle], PlayerInfo[playerid][pAccID]);
    mysql_tquery(dbHandle, query);
    SendSyntaxMessage(playerid, "Verificação em Duas Etapas com Google Authenticator conectada com sucesso.");
    return 1;
}


Dialog:SendPM(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new userid = GetPVarInt(playerid, #pm_to), text[128];

	GetPVarString(playerid, #pm_text, text, 128);
	SendPMToPlayer(playerid, userid, text);
	return 1;
}

CMD:comprumentar(playerid, params[])
{
    new userid, type;

    if (sscanf(params,"ud", userid, type))
    {
        SendHelpMessage(playerid, "/cumprimentar [ID/Parte do Nome] [tipo]");
        SendSyntaxMessage(playerid, "[1] Beijo [2-9] Aperto de mão");
        return 1;
    }

    if (!IsPlayerLogged(userid))                     return SendNotConnected(playerid);
    if (userid == playerid)                          return 1;
    if (type > 9 || type < 1)                        return SendErrorMessage(playerid, "Tipo de 1 a 9.");
    if (!PlayerNearPlayer(2.0, playerid, userid))    return SendNotNearPlayer(playerid);

    pTemp[userid][pGreetPlayer] = playerid;
    pTemp[userid][pGreetType] = type;
    SetPVarInt(userid, #accept_time, gettime() + 30);

    SendSyntaxMessage(playerid, "Você enviou um pedido de cumprimento para %s.", GetNameWithMask(userid));
    SendSyntaxMessage(userid, "%s quer te cumprimentar (%s).", GetNameWithMask(playerid), (type == 1)?("com um beijo"):("com um aperto de mão"));

    InfoMSG(playerid, "~h~~g~Y~w~ - aceitar\n~h~~r~N~w~ - recusar", 3);
    return 1;
}


forward OnPlayerChangePass(playerid);
public OnPlayerChangePass(playerid)
{ 
    if (!cache_num_rows())  return Dialog_Show(playerid, ChangePass, DIALOG_STYLE_PASSWORD, "{FFFFFF}Inserindo informações secretas", "{FFFFFF} Digite a senha atual para alterar:", ">>>", "Cancelar");

    Dialog_Show(playerid, ChangePass2, DIALOG_STYLE_PASSWORD, "Nova senha", "Digite a nova senha:", "Mudar", "Cancelar");
    return 1;
}

Dialog:BuyClothes(playerid, response, listitem, inputtext[])
{
    new modelid = GetPVarInt(playerid, "clothes_model");

    if (modelid < 1 || modelid > 311)    return 1;
    if (GetCash(playerid) < 100)         return SendErrorMessage(playerid, "Você não tem dinheiro suficiente. Você precisa de $ 100.");
    if (response)                        PlayerInfo[playerid][pTog][listitem] = PlayerInfo[playerid][pSkin];
    SendSyntaxMessage(playerid, "Você comprou roupas por $ 100. ");
    SetPlayerSkinEx(playerid, modelid);
    GiveCash(playerid, -100);
    return 1;
}

Dialog:EditClothes(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

    if (PlayerInfo[playerid][pTog][listitem] == 0)
    {
        PlayerInfo[playerid][pTog][listitem] = PlayerInfo[playerid][pSkin];
        cmd::trocarroupa(playerid, "1");
        PlayerInfo[playerid][pSkin] = 0;
        return 1;
    }
    new title[32];

    format(title, 32,"{FFFFFF}clothing %i.", PlayerInfo[playerid][pTog][listitem]);
    Dialog_Show(playerid, EditClothes2, DIALOG_STYLE_LIST, title, "- Usar\n- Descartar", "Selecionar", "Cancelar");
	SetPVarInt(playerid, "select_tog", listitem);
	return 1;
}

Dialog:EditClothes2(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new slot = GetPVarInt(playerid, "select_tog");

    if (PlayerInfo[playerid][pTog][slot] == 0) return 1;

	if (!listitem)
	{
	    new skin = PlayerInfo[playerid][pSkin];
    	SetPlayerSkinEx(playerid,PlayerInfo[playerid][pTog][slot]);
    	PlayerInfo[playerid][pTog][slot] = skin;
    	return 1;
	}
	
    PlayerInfo[playerid][pTog][slot] = 0;
    return 1;
}