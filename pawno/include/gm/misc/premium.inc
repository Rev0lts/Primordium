new bronze_str[] = {"{9ACD32} Premium: {FFFFFF} BRONZE\n\n {9ACD32} Vantagens:\n {FFFFFF} Título bronze no fórum;\nCor e grupo no fórum;\nCapacidade da LAN no fórum é 150;\nMudança de nome no jogo 2 vezes;\n /blockpm (bloquear mensagens PM de um jogador específico);\nMáximo de saúde na entrada;\nDesativar sistema AntiAfk;\nLimite de contatos na agenda telefônica - 25;\nMáximo de casas para compra - 2;\nPossibilidade de trocar a placa do seu veículo - 3 vezes "},
    bronze_str_2[] = {"Veículos disponíveis para compra: Bf Injection, Bandito;\n /setstyle (Mudança de estilo do HUD (a), disponível em escolha 4);\n /setstyle (Mudança de estilo de luta, disponível em escolha 4);\nLimite de acessórios disponíveis na casa - 150 objetos;\n\n {9ACD32} Custo: {FFFFFF} 150 moedas."};

new silver_str[] = {"{9ACD32} Premium: {FFFFFF} SILVER\n\n {9ACD32} Vantagens:\n {FFFFFF} Título silver no fórum;\nCor e grupo no fórum;\nCapacidade da LAN no fórum é 200;\nMudança de nome no jogo 3 vezes;\n /blockpm (bloquear mensagens PM de um jogador específico);\nMáximo de saúde na entrada;\nDesativar sistema AntiAfk;\nLimite de contatos na agenda telefônica - 35"},
    silver_str_2[] = {"Sair do seu emprego imediatamente, e não após 2 horas;\nSalvar a máscara após sair do jogo;\nMáximo de casas para compra - 4;\nMáximo de negócios para compra - 2;\nMáximo de veículos para compra - 5;\nPossibilidade de instalar nitro no carro (2x, 5x, 10x);\nPossibilidade de trocar a placa do seu veículo - 5 vezes;\nVeículos disponíveis para compra: Bf Injection, Bandito, Sandking, Bullet, Cheetah"},
    silver_str_3[] = {"Tempo reduzido de anúncio /ad /cad (60 segundos para contas regulares, 30 segundos para silver);\n /setstyle (Mudança de estilo do HUD (a), disponível em escolha 4);\n /setstyle (Mudança de estilo de combate, disponível em escolha 4);\nLimite de acessórios disponíveis na casa - 250 objetos;\n\n {9ACD32} Custo: {FFFFFF} 250 moedas."};

new gold_str[] = {"{9ACD32} Premium: {FFFFFF} GOLD\n\n {9ACD32} Vantagens:\n {FFFFFF} Título gold no fórum;\nCor e grupo no fórum;\nCapacidade da LAN no fórum é 200;\nMudança de nome no jogo 5 vezes;\n /blockpm (bloquear mensagens PM de um jogador específico);\nMáximo de saúde na entrada;\nDesativar sistema AntiAfk"},
    gold_str_2[] = {"Limite de contatos na agenda telefônica - 40;\nSair do emprego imediatamente, não após 2 horas;\nSalvar a máscara após sair do jogo;\nMáximo de casas para compra - 5;\nMáximo de negócios para compra - 3;\nMáximo de veículos para compra - 7;\nPossibilidade de instalar nitro (2x, 5x, 10x) no carro;\nPossibilidade de trocar a placa do seu veículo - 7 vezes"},
    gold_str_3[] = {"Veículos disponíveis para compra: Infernus, Bullet, Cheetah, NRG-500, FBI Rancher, Sandking, Bf Injection, Bandito;\nTempo reduzido de anúncio /ad /cad (Para contas regulares, 60 segundos, para gold 30 segundos);\n /setstyle (Mudança de estilo do HUD (a), disponível em escolha 4);\n /setstyle (Mudança de estilo de combate, disponível em escolha 4);\nLimite de hardware disponível na casa - 400 objetos;\n\n {9ACD32} Custo: {FFFFFF} 450 moedas."};
		
Donate_OnGameModeInit()
{
	new query[128];
	format(query, sizeof(query), "DELETE FROM `donate` WHERE `status`='0' AND `dateDelete` > %i", gettime());
	mysql_tquery(dbHandle, query);

	bronze_str[sizeof(bronze_str)-1] = '\n';
	bronze_str_2[sizeof(bronze_str_2)-1] = '\0';
	
    silver_str[sizeof(silver_str)-1] = '\n';
	silver_str_2[sizeof(silver_str_2)-1] = '\n';
	silver_str_3[sizeof(silver_str_3)-1] = '\0';

    gold_str[sizeof(gold_str)-1] = '\n';
	gold_str_2[sizeof(gold_str_2)-1] = '\n';
	gold_str_3[sizeof(gold_str_3)-1] = '\0';
}


CMD:donate(playerid, params[])
{
    Dialog_Show(playerid, DonateMenu, DIALOG_STYLE_LIST, "Privilégios", "1. Comprar\n\
	2. Informações\n\
	{6EA3FF}"WeburlName"", "Selecionar", "Cancelar");
	return 1;
}

Dialog:DonateMenu(playerid, response, listitem, inputtext[])
{
    if (!response)
        return 1;

    switch (listitem)
    {
        case 0:
        {
            new title[32];
            format(title, sizeof(title), "Sua conta: {9ACD32}%i moedas.", PlayerInfo[playerid][pDonate]);

            Dialog_Show(playerid, DonateBuy, DIALOG_STYLE_TABLIST, title, "Premium \"BRONZE\"\t{9ACD32}[250 moedas]\n\
            Premium \"SILVER\"\t{9ACD32}[350 moedas]\n\
            Premium \"GOLD\"\t{9ACD32}[450 moedas]\n\
            Mudar apelido\t{9ACD32}[80 moedas]\n\
            Mudar número de telefone\t{9ACD32}[100 moedas]\n\
            Mudar placa do veículo\t{9ACD32}[70 moedas]", "Selecionar", "Cancelar");
        }
        case 1:
        {
            new mes[256];
            format(mes, sizeof(mes), "{ffffff}Na sua conta, você tem {9ACD32}%i {FFFFFF}moedas de doação.\n\
            Possibilidade de mudar o apelido: %i vez (/alterarnome).\n\
            Possibilidade de mudar o número do veículo: %i vez (/alterarplaca).", PlayerInfo[playerid][pDonate], PlayerInfo[playerid][pNameChanges], PlayerInfo[playerid][pNumberChanges]);

            new premium_info[4][8] =
                {"Normal", "BRONZE", "SILVER", "GOLD"};

            if (PlayerInfo[playerid][pPremium])
            {
                new Float:days = float(PlayerInfo[playerid][pPremiumTime] - gettime()) / 86400.0;
                format(mes, sizeof(mes), "%s\nVocê possui uma conta {9ACD32}%s{FFFFFF}. {9ACD32}%i{FFFFFF} dias restantes.", mes, premium_info[PlayerInfo[playerid][pPremium]], floatround(days));
            }
            Dialog_Show(playerid, DonateCancel, DIALOG_STYLE_MSGBOX, "Informações", mes, "Retornar", "");
        }
        case 2:
            return cmd::donate(playerid, "");
    }
    return 1;
}


Dialog:DonateBuy(playerid, response, listitem, inputtext[])
{
    if (!response)
        return cmd::donate(playerid, "");

    switch (listitem)
    {
        case 0:
            Dialog_Show(playerid, BuyPremium, DIALOG_STYLE_MSGBOX, "Premium", bronze_str, "Comprar", "Retornar");
        case 1:
            Dialog_Show(playerid, BuyPremium, DIALOG_STYLE_MSGBOX, "Premium", silver_str, "Comprar", "Retornar");
        case 2:
            Dialog_Show(playerid, BuyPremium, DIALOG_STYLE_MSGBOX, "Premium", gold_str, "Comprar", "Retornar");
        case 3:
            Dialog_Show(playerid, BuyName, DIALOG_STYLE_MSGBOX, "Mudança de Nome", "{FFFFFF}Você deseja comprar a capacidade de mudar seu nome? Preço: 80 rublos.", "Comprar", "Retornar");
        case 4:
            Dialog_Show(playerid, BuyNumber, DIALOG_STYLE_INPUT, "Mudança de Número de Telefone", "{FFFFFF}Digite seu novo número, no mínimo 4 caracteres. Preço: 100 rublos.", "Comprar", "Retornar");
        case 5:
            Dialog_Show(playerid, BuyNumberplate, DIALOG_STYLE_MSGBOX, "Mudança de Placa de Veículo", "{FFFFFF}Você deseja comprar a capacidade de mudar a placa do veículo? Preço: 70 rublos.", "Comprar", "Retornar");
    }
    SetPVarInt(playerid, #premium_buy, listitem + 1);
    return 1;
}



Dialog:BuyName(playerid, response, listitem, inputtext[])
{
    if (!response) return cmd::donate(playerid, "");

    #define PRICE_CHANGE_NAME (80)

    if (PlayerInfo[playerid][pDonate] < PRICE_CHANGE_NAME) return SendErrorMessage(playerid, "Não há rublos de doação suficientes em sua conta.");

    SendSyntaxMessage(playerid, "Você adquiriu a oportunidade de mudar seu apelido por %i rublos.", PRICE_CHANGE_NAME);

    PlayerInfo[playerid][pDonate] -= PRICE_CHANGE_NAME;
    PlayerInfo[playerid][pNameChanges]++;

    new query[128];
    mysql_format(dbHandle, query, sizeof(query), "UPDATE `accounts` SET `donate`=%i,`name_changes`=%i WHERE `id` = %i", PlayerInfo[playerid][pDonate], PlayerInfo[playerid][pNameChanges], PlayerInfo[playerid][pAccID]);
    mysql_tquery(dbHandle, query);

    format(query, sizeof(query), "%s [MYSQL: %i | IP: %s] comprou uma mudança de apelido",
        GetName(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid));
    insert_log(query, DONATE_LOG, 1, PlayerInfo[playerid][pAccID]);
    return 1;
}

Dialog:BuyNumberplate(playerid, response, listitem, inputtext[])
{
    if (!response) return cmd::donate(playerid, "");

    #define PRICE_CHANGE_NUMBERPLATE (70)

    if (PlayerInfo[playerid][pDonate] < PRICE_CHANGE_NUMBERPLATE) return SendErrorMessage(playerid, "Não há rublos de doação suficientes em sua conta.");

    SendSyntaxMessage(playerid, "Você adquiriu a oportunidade de mudar a placa do veículo por %i rublos.", PRICE_CHANGE_NUMBERPLATE);

    PlayerInfo[playerid][pDonate] -= PRICE_CHANGE_NUMBERPLATE;
    PlayerInfo[playerid][pNumberChanges]++;

    new query[128];
    mysql_format(dbHandle, query, sizeof(query), "UPDATE `accounts` SET `donate`=%i,`number_changes`=%i WHERE `id` = %i", PlayerInfo[playerid][pDonate], PlayerInfo[playerid][pNumberChanges], PlayerInfo[playerid][pAccID]);
    mysql_tquery(dbHandle, query);

    format(query, sizeof(query), "%s [MYSQL: %i | IP: %s] comprou uma mudança de placa",
        GetName(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid));
    insert_log(query, DONATE_LOG, 3, PlayerInfo[playerid][pAccID]);
    return 1;
}


Dialog:BuyNumber(playerid, response, listitem, inputtext[])
{
    if (!response) return cmd::donate(playerid, "");

    if (strlen(inputtext) < 4 || strval(inputtext) < 1000)
    {
        return Dialog_Show(playerid, BuyNumber, DIALOG_STYLE_MSGBOX, "{FFFFFF}Mudar número de telefone", "{FFFFFF}Digite seu novo número, no mínimo 4 caracteres. Preço: 100 rublos.", "Comprar", "Voltar");
    }

    new query[128];
    mysql_format(dbHandle, query, sizeof(query), "SELECT `number` FROM `users` WHERE `number` = %i", strval(inputtext));
    mysql_tquery(dbHandle, query, "OnPlayerBuyDonateNumber", "ii", playerid, strval(inputtext));
    return 1;
}

forward OnPlayerBuyDonateNumber(playerid, number);
public OnPlayerBuyDonateNumber(playerid, number)
{
    if (cache_num_rows())
    {
        Dialog_Show(playerid, BuyNumber, DIALOG_STYLE_MSGBOX, "Mudar número de telefone", "Digite seu novo número, no mínimo 4 caracteres. Preço: 100 rublos.", "Comprar", "Voltar");
        return SendErrorMessage(playerid, "Este número já está sendo usado por alguém.");
    }

    #define PRICE_CHANGE_NUMBER (100)

    if (PlayerInfo[playerid][pDonate] < PRICE_CHANGE_NUMBER) return SendErrorMessage(playerid, "Não há rublos de doação suficientes em sua conta.");
    if (!CheckNumberAvailable(number)) return SendErrorMessage(playerid, "Este número de telefone já está em uso!");

    SendSyntaxMessage(playerid, "Você comprou um número [%i] por %i rublos.", number, PRICE_CHANGE_NUMBER);
    PlayerInfo[playerid][pDonate] -= PRICE_CHANGE_NUMBER;
    PlayerInfo[playerid][pNumber] = number;

    new query[128];

    mysql_format(dbHandle, query, sizeof(query), "UPDATE `accounts` SET `donate`=%i WHERE `id` = %i", PlayerInfo[playerid][pDonate], PlayerInfo[playerid][pAccID]);
    mysql_tquery(dbHandle, query);
    mysql_format(dbHandle, query, sizeof(query), "UPDATE `users` SET `number`=%i WHERE `id` = %i", number, PlayerInfo[playerid][pID]);
    mysql_tquery(dbHandle, query);

    format(query, sizeof(query), "%s [MYSQL: %i | IP: %s] comprou um número de telefone - %i", GetName(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), number);
    insert_log(query, DONATE_LOG, 4, PlayerInfo[playerid][pAccID]);
    return 1;
}


Dialog:BuyPremium(playerid, response, listitem, inputtext[])
{
	if (!response) 						return cmd::donate(playerid, "");
	if (PlayerInfo[playerid][pPremium])	return SendErrorMessage(playerid, "Para comprar um prêmio, você precisa esperar até que o atual expire.");
	        
	listitem = GetPVarInt(playerid, #premium_buy)-1;
	
	if (listitem < 0 || listitem > 2) return 1;
	    
	new price_premium[3] = {150, 250, 350}, premium_name_changes[3] = {2, 3, 5}, premium_number_changes[3] = {3, 5, 7};
	new premium_info[3][8] = {"BRONZE", "SILVER", "GOLD"};
			
	if (PlayerInfo[playerid][pDonate] < price_premium[listitem]) return SendErrorMessage(playerid, "Você não possui rublos de doação suficientes em sua conta.");

	SendSyntaxMessage(playerid, "Você comprou um prêmio %s por 30 dias adiantado por %i rublos.", premium_info[listitem], price_premium[listitem]);

	PlayerInfo[playerid][pDonate] -= price_premium[listitem];
	PlayerInfo[playerid][pNameChanges] += premium_name_changes[listitem];
	PlayerInfo[playerid][pNumberChanges] += premium_number_changes[listitem];
	PlayerInfo[playerid][pPremium] = listitem + 1;
    PlayerInfo[playerid][pPremiumTime] = gettime() + 30 * 86400;
	
	new query[128];
	
	mysql_format(dbHandle, query, sizeof(query), "UPDATE `accounts` SET `donate`=%i,`name_changes`=%i,`number_changes`=%i WHERE `id` = %i", PlayerInfo[playerid][pDonate], PlayerInfo[playerid][pNameChanges], PlayerInfo[playerid][pNumberChanges], PlayerInfo[playerid][pAccID]);
	mysql_tquery(dbHandle, query);
	mysql_format(dbHandle, query, sizeof(query), "UPDATE `users` SET `vip`=%i,`vip_time`=%i,`spawnhealth`=100.0 WHERE `id` = %i", PlayerInfo[playerid][pPremium], PlayerInfo[playerid][pPremiumTime], PlayerInfo[playerid][pID]);
	mysql_tquery(dbHandle, query);
    
	format(query, sizeof(query), "%s [MYSQL: %i | IP: %s] comprou um prêmio %s", GetName(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), premium_info[listitem]);
 	insert_log(query, DONATE_LOG, 2, PlayerInfo[playerid][pAccID]);
	return 1;
}


Dialog:DonateCancel(playerid, response, listitem, inputtext[])
{
	cmd::donate(playerid, "");
	return 1;
}

CMD:alterarplaca(playerid, params[])
{
	if (PlayerInfo[playerid][pNumberChanges] < 1) return SendErrorMessage(playerid, "Para alterar a placa do veículo, você deve adquirir este privilégio. (/donate)");

	new vehicleid = GetPlayerVehicleID(playerid);

	if (!IsValidVehicle(PlayerInfo[playerid][pCarKey])) return SendErrorMessage(playerid, "Seu veículo não está spawnado.");
	if (vehicleid == 0) return SendNotInVehicle(playerid);
	if (VehicleInfo[vehicleid][carOwnerID] != PlayerInfo[playerid][pID]) return SendErrorMessage(playerid, "Você não é o proprietário deste veículo.");

	Dialog_Show(playerid, ChangeNumberplate, DIALOG_STYLE_INPUT, "{FFFFFF}Informe uma nova placa", "{FFFFFF}Informe uma nova placa para o seu veículo:", "Alterar", "Cancelar");
	return 1;
}

Dialog:ChangeNumberplate(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;
	if (strlen(inputtext) < 3 || strlen(inputtext) > 11) return SendErrorMessage(playerid, "O número de caracteres na placa do veículo deve ter entre 3 e 11.");

	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "SELECT `plate` FROM `cars` WHERE `plate` = '%e'", inputtext);
	mysql_tquery(dbHandle, query, "DonateSetPlate", "is", playerid, inputtext);
	return 1;
}

this::DonateSetPlate(playerid, plate[])
{
	if (cache_num_rows()) return SendErrorMessage(playerid, "A placa já está em uso por outro veículo.");
	if (PlayerInfo[playerid][pNumberChanges] < 1) return SendErrorMessage(playerid, "Para alterar a placa do veículo, você deve adquirir este privilégio. (/donate)");

	new vehicleid = GetPlayerVehicleID(playerid);

	if (!IsValidVehicle(PlayerInfo[playerid][pCarKey])) return SendErrorMessage(playerid, "Seu veículo não está spawnado.");
	if (vehicleid == 0) return SendNotInVehicle(playerid);
	if (VehicleInfo[vehicleid][carOwnerID] != PlayerInfo[playerid][pID]) return SendErrorMessage(playerid, "Você não é o proprietário deste veículo.");

	PlayerInfo[playerid][pNumberChanges]--;
	DeleteVehicleTickets(vehicleid);
	format(VehicleInfo[vehicleid][carPlate], 24, "%s", plate);
	SetVehicleNumberPlate(vehicleid, VehicleInfo[vehicleid][carPlate]);
	RespawnVehicle(vehicleid, 1);
	Save_Car(vehicleid);
	SendSyntaxMessage(playerid, "Você alterou a placa do seu veículo %s. Novas placas: %s", VehicleNames[GetVehicleModel(vehicleid)-400], plate);
	SendSyntaxMessage(playerid, "Multas de veículos foram pagas.");

	new query[128];

	mysql_format(dbHandle, query, sizeof(query), "UPDATE `accounts` SET `number_changes`=%i WHERE `id` = %i", PlayerInfo[playerid][pNumberChanges], PlayerInfo[playerid][pAccID]);
	mysql_tquery(dbHandle, query);
	return 1;
}

CMD:alterarnome(playerid, params[])
{
	if (PlayerInfo[playerid][pNameChanges] < 1) return SendErrorMessage(playerid, "Para solicitar uma mudança de nome, você deve adquirir este privilégio. (/donate)");

	if (strlen(pTemp[playerid][pChangeNick]))
	{
	    pTemp[playerid][pChangeNick] = EOS;
	    return SendErrorMessage(playerid, "Você cancelou sua solicitação de mudança de nome.");
	}

	Dialog_Show(playerid, ChangeNick, DIALOG_STYLE_INPUT, "{FFFFFF}Informe um novo nome", "{FFFFFF}Informe um novo apelido (Nome_Sobrenome):", ">>>", "Cancelar");
	return 1;
}

Dialog:ChangeNick(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;
    if (!IsValidPlayerName(inputtext)) return SendErrorMessage(playerid, "Formato de apelido inválido.");
	if (strlen(inputtext) < 4 || strlen(inputtext) > 24) return SendErrorMessage(playerid, "O apelido deve ter entre 4 e 24 caracteres.");

	format(pTemp[playerid][pChangeNick], MAX_PLAYER_NAME, inputtext);
	SendSyntaxMessage(playerid, "Você solicitou uma mudança de apelido - %s", inputtext);
	SendAdmMessage("%s (ID: %i) solicitou uma mudança de apelido - %s. (/acceptname - para confirmar)", GetNameEx(playerid), playerid, inputtext);
	return 1;
}
