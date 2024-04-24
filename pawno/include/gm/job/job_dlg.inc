Dialog:BuyDrugs(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new drug_list_name[][] = {"Sementes de Maconha", "LSD", "Heroína", "Cocaína", "Efedrina"}, drug_list_price[] = {50, 150, 100, 200, 50};
	
	static const str_size[] = "{FFFFFF}Você deseja comprar %s por $%i o grama.\nQuantidade disponível para compra: 1 - 1000:";
	new mes[sizeof(str_size) - 4 + 7 + 3 + 1];

	format(mes, sizeof(mes), str_size, drug_list_name[listitem], drug_list_price[listitem]);
    Dialog_Show(playerid, BuyDrugsAccept, DIALOG_STYLE_INPUT, "{FFFFFF}Comprar Drogas", mes, "Selecionar", "Cancelar");
    SetPVarInt(playerid, "select_drug", listitem);
	return 1;
}

Dialog:BuyDrugsAccept(playerid, response, listitem, inputtext[])
{
	if (!response)	return cmd::comprardrogas(playerid, "");

	new amount = strval(inputtext);

	listitem = GetPVarInt(playerid, "select_drug");

	if (amount < 1 || amount > 1000)	return SendErrorMessage(playerid, "Quantidade de drogas incorreta: 1 - 1000.");
	
    new faction = PlayerInfo[playerid][pFaction];

	if (!PlayerInfo[playerid][pFaction])															return SendErrorMessage(playerid, "Você não é membro de uma facção oficial.");
	if (PlayerInfo[playerid][pSideJob] != JOB_DRUGDEALER)											return SendErrorMessage(playerid, "Você não tem acesso à compra de drogas.");
	if (Factions[faction][fType] != FACTION_GANG && Factions[faction][fType] != FACTION_MAFIA)		return 1;
    if (!PlayerToPoint(2.0, playerid, BLACKMARKET_X, BLACKMARKET_Y, BLACKMARKET_Z))
	{
	    Waypoint_Set(playerid, BLACKMARKET_X, BLACKMARKET_Y, BLACKMARKET_Z);
	    return SendErrorMessage(playerid, "Você deve estar no cais.");
	}

	new drug_list[] = {INV_SEED_GANJA, INV_LSD, INV_HEROIN, INV_COCAINE, INV_EPHEDRINE}, drug_price[] = {50, 150, 100, 200, 50};

	if (GetCash(playerid) < drug_price[listitem] * amount)			return SendNotCash(playerid);
	if (GiveInvItem(playerid, drug_list[listitem], amount) == -1)	return SendErrorMessage(playerid, "Seu inventário está cheio.");

	SendSyntaxMessage(playerid, "Você comprou %i gramas de %s por $%i.", amount, inventory_items[drug_list[listitem]][itemName], drug_price[listitem] * amount);
	GiveCash(playerid, -drug_price[listitem] * amount);
	return 1;
}

Dialog:BuyAmmoLaw(playerid, response, listitem, inputtext[])
{
    if (!response) return cmd::comprararma(playerid, "");

    new faction = PlayerInfo[playerid][pFaction], ammo = 1;

    new gun_list[][20] = {"M1911","M1911","Desert Eagle","Desert Eagle","Shotgun","Shotgun","Shotgun","Tec-9 / MP5 / Uzi", "Tec-9 / MP5 / Uzi","Country Rifle"},
        ammo_list[] = {82, 83, 84, 85, 86, 87, 88, 90, 91, 96},
        ammo_pr[] = {35, 80, 60, 130, 300, 200, 100, 200, 450, 400};

    if (!PlayerInfo[playerid][pFaction]) return SendErrorMessage(playerid, "Você não é membro de uma facção oficial.");
    if (PlayerInfo[playerid][pSideJob] != JOB_GUNDEALER) return SendErrorMessage(playerid, "Você não tem acesso à compra de munições.");
    if (Factions[faction][fType] != FACTION_GANG && Factions[faction][fType] != FACTION_MAFIA) return 1;
    if (!PlayerToPoint(2.0, playerid, BLACKMARKET_X, BLACKMARKET_Y, BLACKMARKET_Z))
    {
        Waypoint_Set(playerid, BLACKMARKET_X, BLACKMARKET_Y, BLACKMARKET_Z);
        return SendErrorMessage(playerid, "Você precisa estar nos cais.");
    }

    if (!CheckInv(playerid)) return SendErrorMessage(playerid, "Não há espaço suficiente no seu inventário.");
    if (GetCash(playerid) < ammo_pr[listitem]) return SendNotCash(playerid);
    if (Factions[faction][fPoint] - ammo < 0) return SendErrorMessage(playerid, "Sua facção não tem pontos suficientes para comprar.");

    GiveCash(playerid, -ammo_pr[listitem]);
    GiveInvItem(playerid, ammo_list[listitem], CheckAmmo(ammo_list[listitem])); 

    Factions[faction][fPoint] -= 1;
    SQL_SetInteger("factions", "point", Factions[faction][fPoint], Factions[faction][fID]);

    SendBlackMarketMessage(playerid, "Você comprou um carregador %s [%i rodada(s)] por %s por $%i e 1 ponto de experiência.", inventory_items[ammo_list[listitem]][itemName], CheckAmmo(ammo_list[listitem]), gun_list[listitem], ammo_pr[listitem]);
    return 1;
}

Dialog:BuyGunLaw(playerid, response, listitem, inputtext[])
{
	if (!response)	return cmd::comprararma(playerid, "");

    new faction = PlayerInfo[playerid][pFaction];

	if (!PlayerInfo[playerid][pFaction])														return SendErrorMessage(playerid, "Você não é membro de uma facção oficial.");
	if (PlayerInfo[playerid][pSideJob] != JOB_GUNDEALER)										return SendErrorMessage(playerid, "Você não tem acesso à compra de armas.");
	if (Factions[faction][fType] != FACTION_GANG && Factions[faction][fType] != FACTION_MAFIA)	return 1;

	if (Factions[faction][fType] == FACTION_MAFIA)
	{
		new gun_list[] = {24, 22, 23, 25, 26, 27, 28, 32, 29, 30, 31, 33, 16, 18, INV_OPTICS, INV_SILENCER, INV_TOOLS}, 
			gun_point[] = {3, 3, 3, 4, 4, 4, 5, 5, 5, 7, 7, 10, 10, 4, 2, 2, 1}, 
			gun_price[] = {1800, 1000, 1500, 2400, 2100, 4000, 2000, 2500, 2800, 3500, 4300, 3800, 1000, 300, 1700, 500, 300}, 
			gun_ammo[] = {50, 50, 50, 30, 30, 30, 120, 120, 120, 150, 150, 15, 1, 1, 1, 1, 1};

        if (Factions[faction][fPoint]-gun_point[listitem] < 0)																	return SendErrorMessage(playerid, "Sua facção não tem pontos suficientes para comprar.");
        // Se você já tem uma arma nesse slot, retorna uma mensagem de erro.
		    
		new mes[256];
		if (ValidWeaponForInventory(gun_list[listitem]))
		{
			format(mes, sizeof(mes), "{FFFFFF}Você tem certeza de que quer comprar %s e %i munições por %i de experiência e $%i?\nSua facção terá %i de experiência.",
			inventory_items[gun_list[listitem]][itemName], gun_ammo[listitem], gun_point[listitem], gun_price[listitem], Factions[faction][fPoint]-gun_point[listitem]);
		}
		else
		{
			format(mes, sizeof(mes), "{FFFFFF}Você tem certeza de que quer comprar %s por %i de experiência e $%i?\nSua facção terá %i de experiência.",
			inventory_items[gun_list[listitem]][itemName], gun_point[listitem], gun_price[listitem], Factions[faction][fPoint]-gun_point[listitem]);
		}
    	Dialog_Show(playerid, BuyGunAccept, DIALOG_STYLE_MSGBOX, "Compra de armas", mes, "Comprar", "Retornar");
	}

	else if (Factions[faction][fType] == FACTION_GANG)
	{
		new gun_list[] = {24, 22, 25, 26, 28, 32, 18, INV_TOOLS}, gun_point[] = {3, 3, 4, 4, 5, 5, 4, 1}, gun_price[] = {1800, 1000, 2400, 2100, 2000, 2500, 300, 300}, gun_ammo[] = {50, 50, 30, 30, 120, 120, 1, 1};

        if (Factions[faction][fPoint]-gun_point[listitem] < 0)																	return SendErrorMessage(playerid, "Sua facção não tem pontos suficientes para comprar.");
        // Se você já tem uma arma nesse slot, retorna uma mensagem de erro.
		    
		new mes[256];

		if (ValidWeaponForInventory(gun_list[listitem]))
		{
			format(mes, sizeof(mes), "{FFFFFF}Você tem certeza de que quer comprar %s e %i munições por %i de experiência e $%i?\nSua facção terá %i de experiência.",
			inventory_items[gun_list[listitem]][itemName], gun_ammo[listitem], gun_point[listitem], gun_price[listitem], Factions[faction][fPoint]-gun_point[listitem]);
		}
		else
		{
			format(mes, sizeof(mes), "{FFFFFF}Você tem certeza de que quer comprar %s por %i de experiência e $%i?\nSua facção terá %i de experiência.",
			inventory_items[gun_list[listitem]][itemName], gun_point[listitem], gun_price[listitem], Factions[faction][fPoint]-gun_point[listitem]);
		}
    	Dialog_Show(playerid, BuyGunAccept, DIALOG_STYLE_MSGBOX, "Compra de armas", mes, "Comprar", "Retornar");
	}
	SetPVarInt(playerid, #select_law, listitem);
	return 1;
}


Dialog:BuyGunAccept(playerid, response, listitem, inputtext[])
{
	if (!response)	return cmd::comprararma(playerid, "");

	listitem = GetPVarInt(playerid, #select_law);

    new faction = PlayerInfo[playerid][pFaction];

	if (!PlayerInfo[playerid][pFaction])															    return SendErrorMessage(playerid, "Você não é membro de uma facção oficial.");
	if (PlayerInfo[playerid][pSideJob] != JOB_GUNDEALER)											    return SendErrorMessage(playerid, "Você não tem acesso à compra de armas.");
	if (Factions[faction][fType] != FACTION_GANG && Factions[faction][fType] != FACTION_MAFIA)	    return 1;
    if (!PlayerToPoint(2.0, playerid, BLACKMARKET_X, BLACKMARKET_Y, BLACKMARKET_Z))
	{
	    Waypoint_Set(playerid, BLACKMARKET_X, BLACKMARKET_Y, BLACKMARKET_Z);
	    return SendErrorMessage(playerid, "Você deve estar nos cais.");
	}

	if (Factions[faction][fType] == FACTION_MAFIA)
	{
		new gun_list[] = {24, 22, 23, 25, 26, 27, 28, 32, 29, 30, 31, 33, 16, 18, INV_OPTICS, INV_SILENCER, INV_TOOLS}, 
		    gun_point[] = {3, 3, 3, 4, 4, 4, 5, 5, 5, 7, 7, 10, 10, 4, 2, 2, 1},
	    	gun_price[] = {1800, 1000, 1500, 2400, 2100, 4000, 2000, 2500, 2800, 3500, 4300, 3800, 1000, 300, 1700, 500, 300}, 
	    	gun_ammo[] = {50, 50, 50, 30, 30, 30, 120, 120, 120, 150, 150, 15, 1, 1, 1, 1, 1};

		if (Factions[faction][fPoint]-gun_point[listitem] < 0)	return SendErrorMessage(playerid, "Sua facção não tem pontos suficientes para comprar.");
		if (GetCash(playerid) < gun_price[listitem])				return SendNotCash(playerid);
        // Se você já tem uma arma nesse slot, retorna uma mensagem de erro.
		    
		if (ValidWeaponForInventory(gun_list[listitem]))
		{
		    GivePlayerWeaponEx(playerid, gun_list[listitem], gun_ammo[listitem]);
		    SendSyntaxMessage(playerid, "Você comprou %s e %i munições.", inventory_items[gun_list[listitem]][itemName], gun_ammo[listitem]);
		}
		else
		{
        	if (GiveInvItem(playerid, gun_list[listitem], gun_ammo[listitem]) == -1)	return SendErrorMessage(playerid, "Inventário cheio.");
				
			SendSyntaxMessage(playerid, "Você comprou %s.", inventory_items[gun_list[listitem]][itemName]);
		}
		
        Factions[faction][fPoint] -= gun_point[listitem];
        SQL_SetInteger("factions", "point", Factions[faction][fPoint], Factions[faction][fID]);
		GiveCash(playerid, -gun_price[listitem]);
	}

	else if (Factions[faction][fType] == FACTION_GANG)
	{
  		new gun_list[] = {24, 22, 25, 26, 28, 32, 18, INV_TOOLS}, 
  		    gun_point[] = {3, 3, 4, 4, 5, 5, 4, 1}, 
  		    gun_price[] = {1800, 1000, 2400, 2100, 2000, 2500, 300, 300}, 
  		    gun_ammo[] = {50, 50, 30, 30, 120, 120, 1, 1};

		if (Factions[faction][fPoint]-gun_point[listitem] < 0)	return SendErrorMessage(playerid, "Sua facção não tem pontos suficientes para comprar.");
		if (GetCash(playerid) < gun_price[listitem])				return SendNotCash(playerid);
        // Se você já tem uma arma nesse slot, retorna uma mensagem de erro.

		if (ValidWeaponForInventory(gun_list[listitem]))
		{
		    GivePlayerWeaponEx(playerid, gun_list[listitem], gun_ammo[listitem]);
		    SendSyntaxMessage(playerid, "Você comprou %s e %i munições.", inventory_items[gun_list[listitem]][itemName], gun_ammo[listitem]);
		}
		else
		{
        	if (GiveInvItem(playerid, gun_list[listitem], gun_ammo[listitem]) == -1)	return SendErrorMessage(playerid, "Inventário cheio.");

			SendSyntaxMessage(playerid, "Você comprou %s.", inventory_items[gun_list[listitem]][itemName]);
		}

        Factions[faction][fPoint] -= gun_point[listitem];
		SQL_SetInteger("factions", "point", Factions[faction][fPoint], Factions[faction][fID]);
		GiveCash(playerid, -gun_price[listitem]);
	}
	return 1;
}

Dialog:BuyGunMain(playerid, response, listitem, inputtext[])
{
	if (!response)	return 1;

    new faction = PlayerInfo[playerid][pFaction];

	if (!PlayerInfo[playerid][pFaction])	return SendErrorMessage(playerid, "Você não é membro de uma facção oficial.");
	if (PlayerInfo[playerid][pSideJob] != JOB_GUNDEALER)	return SendErrorMessage(playerid, "Você não tem acesso à compra de armas.");
	if (Factions[faction][fType] != FACTION_GANG && Factions[faction][fType] != FACTION_MAFIA)	return 1;

    new title[56];

	format(title, sizeof(title), "{FFFFFF}Na conta de sua facção - %i exp.", Factions[faction][fPoint]);

	if (listitem == 0)
	{
	    if (Factions[faction][fType] == FACTION_MAFIA)
		{
			new mes[1024] = "Desert Eagle\t-3 exp\t$1.800\n\
							COLT45\t-3 exp\t$1.000\n\
							COLT45 (silenciador)\t-3 exp\t$1.500\n\
							Espingarda\t-4 exp\t$2.400\n\
							Espingarda Serrada\t-4 exp\t$2.100\n\
							Espingarda de Combate\t-4 exp\t$4.000\n\
							UZI\t-5 exp\t$2.000\n\
							Tec-9\t-5 exp\t$2.500\n\
							MP5\t-5 exp\t$2.800\n\
							M4\t-7 exp\t$4.300\n\
							Rifle\t-10 exp\t$3.800\n\
							Granada\t-10 exp\t$1000\n\
							Coctel Molotov\t-4 exp\t$300\n\
							Mira Óptica\t-2 exp\t$1700\n\
							Supressor\t-2 exp\t$500\n\
							Gazuas\t-1 exp\t$300";
			
			Dialog_Show(playerid, BuyGunLaw, DIALOG_STYLE_TABLIST, title, mes, "Comprar", "Cancelar");
		}
		else
		{
		    Dialog_Show(playerid, BuyGunLaw, DIALOG_STYLE_TABLIST, title, "Desert Eagle\t-3 exp\t$1.800\n\
																			COLT45\t-3 exp\t$1.000\n\
																			Espingarda\t-4 exp\t$2.400\n\
																			Espingarda Serrada\t-4 exp\t$2.100\n\
																			UZI\t-5 exp\t$2.000\n\
																			Tec-9\t-5 exp\t$2.500\n\
																			Coctel Molotov\t-4 exp\t$300\n\
																			Gazuas\t-1 exp\t$300", "Comprar", "Cancelar");
		}
	}

	else if (listitem == 1)
	{
	    Dialog_Show(playerid, BuyAmmoLaw, DIALOG_STYLE_TABLIST, title, "100 unidades.\t-1 exp\t$400\n200 unidades.\t-2 exp\t$800\n300 unidades.\t-3 exp\t$1.200\n400 unidades.\t-4 exp\t$1.600\n500 unidades.\t-5 exp\t$2.000\n600 unidades.\t-6 exp\t$2.400", "Comprar", "Cancelar");
	}
	return 1;
}

Dialog:TakeJobMenu(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	switch(listitem) {
		case 0: {
		    new i = Job_Nearest(playerid);
		    if (i == -1) return 1;

		    if (JobsInfo[i][jobType] == JOB_PIZZA || JobsInfo[i][jobType] == JOB_GUNDEALER || JobsInfo[i][jobType] == JOB_DRUGDEALER)
		    {
		        if (PlayerInfo[playerid][pSideJob] != JOBLESS) 
		            return SendSyntaxMessage(playerid, "Você já tem um segundo emprego. Digite /quitsidejob para sair.");
		        
		        PlayerInfo[playerid][pSideJob] = JobsInfo[i][jobType];
		        SQL_SetInteger("users", "sidejob", JobsInfo[i][jobType], PlayerInfo[playerid][pID]);
		        
		        if (JobsInfo[i][jobType] == JOB_PIZZA)        
		            SendSyntaxMessage(playerid, "Você conseguiu um trabalho como entregador de pizza. Para entregar pizzas - /entregarpizza");
		        if (JobsInfo[i][jobType] == JOB_GUNDEALER)    
		            SendSyntaxMessage(playerid, "Você conseguiu um trabalho como vendedor de armas. Para comprar armas - /buygun");
		        if (JobsInfo[i][jobType] == JOB_DRUGDEALER)   
		            SendSyntaxMessage(playerid, "Você conseguiu um trabalho como traficante de drogas. Para comprar drogas - /buydrug");
		        return 1;
		    }
		    
		    if (PlayerInfo[playerid][pJob] != JOBLESS)                                   
		        return SendErrorMessage(playerid, "Você já tem um emprego. Digite /quitjob para sair.");
		    if (JobsInfo[i][jobType] == JOB_TAXI && !PlayerInfo[playerid][pTaxiLic])     
		        return SendErrorMessage(playerid, "Você não tem uma licença de motorista de táxi. Para obtê-la, entre em contato com uma autoescola.");

		    PlayerInfo[playerid][pJob] = JobsInfo[i][jobType];
		    PlayerInfo[playerid][pCareer] = 0;

		    SQL_SetInteger("users", "job", JobsInfo[i][jobType], PlayerInfo[playerid][pID]);

		    SendSyntaxMessage(playerid, "Você conseguiu um trabalho de '%s'. Saiba mais sobre o emprego - /jobhelp", jobs_names[JobsInfo[i][jobType]]);
		    return 1;
		}

		case 1: {
		    new i = Job_Nearest(playerid);
		    if (i == -1) return 1;

		    new mes[512];
		    format(mes, sizeof(mes), "/vtrabalho - pegar um veículo de trabalho\n/devolvervtrabalho(/dvt) - devolver o veículo de trabalho\n");

		    if (JobsInfo[i][jobType] == JOB_PIZZA) {
		        format(mes, sizeof(mes), "%s/entregarpizza - entregar pizza\n\
		                                        /pegarpizza - pegar pizza\n\
		                                        /sairempregosecundario - sair", mes);
		    } else if (JobsInfo[i][jobType] == JOB_CARJACKER) {
		        format(mes, sizeof(mes), "%s/entregarcarro - desmanchar partes valiosas do transporte de carvão\n\
		                                    /largacarro - se livrar do transporte\n\
		                                    /abandonarmissao - reiniciar missão\n\n\
		                                    /vbl(ock) (n)oisy - hackear transporte de forma barulhenta\n\
		                                    /vbl(ock) (q)uiet - hackear transporte de forma silenciosa", mes);
		    } else if (JobsInfo[i][jobType] == JOB_TAXI) {
		        format(mes, sizeof(mes), "%s/taxi duty - começar/parar o trabalho\n\
		                                        /taxi taxas - mudar tarifa\n\
		                                        /taxi aceitar - aceitar o desafio", mes);
		    } else if (JobsInfo[i][jobType] == JOB_MECHANIC) {
		        format(mes, sizeof(mes), "%s/reabastecer - reabastecer transporte\n\
		                                        /reparar - consertar transporte\n\
		                                        /pintarveiculo - pintar veículos\n\n\
		                                        /rebocarv - rebocar transporte\n\
		                                        /comprarpecas - comprar componentes\n\
		                                        /minhaspecas - descobrir o número de componentes no transporte", mes);
		    } else if (JobsInfo[i][jobType] == JOB_TRUCKER) {
		        format(mes, sizeof(mes), "%s/tpda - para ver o menu de ordens\n\
		                                        /industrias - informações sobre a empresa\n\
		                                        /carga - trabalhar com mercadorias\n\
		                                        /trailer - controle de reboque\n", mes);
		        switch(PlayerInfo[playerid][pCareer]) {
		            case 0..11: strcat(mes, "Veículo disponível: Picador, Sadler, Bobcat, Walton, Yosemite.");
		            case 12..23: strcat(mes, "Veículo disponível: Picador, Sadler, Bobcat, Walton, Yosemite,\n Pony, Berkley's RC Van, Burrito.");
		            case 24..31: strcat(mes, "Veículo disponível: Picador, Sadler, Bobcat, Walton, Yosemite,\n Pony, Berkley's RC Van, Burrito, Rumpo, Boxville.");
		            case 32..47:
		            {
		                strcat(mes, "Veículo disponível: Picador, Sadler, Bobcat, Walton, Yosemite,\n Pony, Berkley's RC Van, Burrito, Rumpo, Boxville.\n");
		                strcat(mes, "Benson, Mule, DFT-30, Tanker, Securicar.");
		            }
		            case 48..58:
		            {
		                strcat(mes, "Veículo disponível: Picador, Sadler, Bobcat, Walton, Yosemite,\n Pony, Berkley's RC Van, Burrito, Rumpo, Boxville.\n");
		                strcat(mes, "Benson, Mule, DFT-30, Tanker, Securicar, Yankee, Flatbed.");
		            }
		            default:
		            {
		                strcat(mes, "Veículo disponível: Picador, Sadler, Bobcat, Walton, Yosemite,\n Pony, Berkley's RC Van, Burrito, Rumpo, Boxville.\n");
		                strcat(mes, "Benson, Mule, DFT-30, Tanker, Securicar, Yankee, Flatbed, Reboques.");
		            }
		        }
		    } else if (JobsInfo[i][jobType] == JOB_TRASHMASTER) {
		        format(mes, sizeof(mes), "%s\nVocê precisa coletar lixo dos contêineres e levá-los para o aterro sanitário!", mes);
		    } else return cmd::ajudatrabalho(playerid, "");

		    Dialog_Show(playerid, None, DIALOG_STYLE_MSGBOX, "{FFFFFF}Comandos de trabalho", mes, "Voltar", "Fechar");
		    return 1;
		}

		case 2: cmd::pegartrabalho(playerid, "");
		case 3: cmd::abandonartrabalho(playerid, "");
	}
	return 1;
}
