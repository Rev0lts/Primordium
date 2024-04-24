#define MAX_ADVERT_SLOT 	(10)
#define MAX_LAST_ADVERT     (10)

enum e_advert_data
{
    advert_id,
    advert_text[256],
    publish_time,
	advert_placeby[MAX_PLAYER_NAME],
    advert_contact,
    advert_type,
	advert_placeby_id,
    bool:advert_exists
}
new advert_data[MAX_ADVERT_SLOT][e_advert_data];

enum LAST_ADVERT_ENUM
{
	advertText[256],
	adverttime,
	advertnumber
}
new LastAdvertInfo[MAX_LAST_ADVERT][LAST_ADVERT_ENUM];

new lastadvert = 0;

stock Advert_OnGameModeInit()
{
	SetTimer("PublishAds", 1000, true);
	return 1;
}


ALTX:lastad("/ultimosanuncios");
CMD:lastad(playerid, params[])
{
    new mes[256 * MAX_LAST_ADVERT + 9] = "{FFFFFF}";
    
    for(new i; i != MAX_LAST_ADVERT; i++)
    {
        if (strlen(LastAdvertInfo[i][advertText]) == 0) continue;

        if (gettime() - LastAdvertInfo[i][adverttime] > 1800)
        {
            LastAdvertInfo[i][advertText] = EOS;
            LastAdvertInfo[i][adverttime] = 0;
            LastAdvertInfo[i][advertnumber] = 0;
        }
        else
        {
            if (strlen(LastAdvertInfo[i][advertText]) > MAXLEN)
            {
                new pos = MAXLEN;

                if (pos < MAXLEN-1) pos = MAXLEN;

                format(mes, sizeof(mes), "%s%i\t%.*s ...\n", mes, i+1, pos, LastAdvertInfo[i][advertText]);
                format(mes, sizeof(mes), "%s\t... %s", mes, LastAdvertInfo[i][advertText][pos]);
            }
            else
            {
                format(mes, sizeof(mes), "%s%i\t%s", mes, i+1, LastAdvertInfo[i][advertText]);
            }
            
            new time[10];

            if (gettime() - LastAdvertInfo[i][adverttime] <= 59)
                format(time, sizeof(time), "%d segundos", gettime() - LastAdvertInfo[i][adverttime]);
            else 
                format(time, sizeof(time), "%d minutos", floatround((gettime() - LastAdvertInfo[i][adverttime])/60, floatround_floor));
            
            format(mes, sizeof(mes), "%s\n\t{a9c4e4}Anúncio %i\t(Adicionado %s atrás){FFFFFF}\n\n", mes, LastAdvertInfo[i][advertnumber], time);
        }
    }
    
    if (strlen(mes) < 9) return Dialog_Show(playerid, None, DIALOG_STYLE_MSGBOX, "{bccd32}Últimos anúncios", "A lista de anúncios recentes está vazia.", "Fechar", "");
        
    Dialog_Show(playerid, None, DIALOG_STYLE_MSGBOX, "{bccd32}Últimos anúncios", mes, "Fechar", "");
    return 1;
}

ALTX:advert("/ad","/anunciar","/an");
CMD:advert(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed]) return 1;
	if (!PlayerInfo[playerid][pNumber])				return SendErrorMessage(playerid, "Você precisa de um telefone.");
    if (isnull(params))								return SendHelpMessage(playerid, "/an [mensagem]");
    if (PlayerInfo[playerid][pSettings] & togAdv)	return SendErrorMessage(playerid, "Ative os anúncios primeiro.");

    new anunt[10];
	new bizid = -1;

    if ((bizid = Advertisement_Nearest(playerid)) == -1)				return SendErrorMessage(playerid, "Você não está em uma agência.");
	if (BizInfo[bizid][bizLocked] == 1)									return GameTextForPlayer(playerid, "~r~FECHADO!", 2500, 1);
	if (BizInfo[bizid][bizOwnerID] != 0 && !BizInfo[bizid][bizProduct])	return SendErrorMessage(playerid, "O negócio não tem produtos suficientes.");
	if (GetCash(playerid) < BizInfo[bizid][bizEntrance])				return SendErrorMessage(playerid, "Você não tem dinheiro suficiente. O preço é de $%i.", BizInfo[bizid][bizEntrance]);
	if (PlayerInQueue(playerid) > GetAdLimit(playerid)) 				return SendErrorMessage(playerid, "Você atingiu o limite máximo de anúncios - %d anúncios.", PlayerInQueue(playerid));
	if (GetNextAdSlot() == -1)											return SendErrorMessage(playerid, "A lista está cheia, tente mais tarde!");
	if(sscanf(params, "s[10]", anunt)) return SendErrorMessage(playerid, "Seu anúncio é muito curto.");
    

    if (BizInfo[bizid][bizOwnerID] != 0)
    {
        BizInfo[bizid][bizProduct]--;
        GiveBizCash(bizid, BizInfo[bizid][bizEntrance]);
    }

	GiveCash(playerid, -BizInfo[bizid][bizEntrance]);

	publishAdvertisement(playerid, params, true);

    SendSpecialMessage(playerid, "Seu anúncio foi registrado com sucesso. Será postado nos próximos minutos.");
	SendHelpMessage(playerid, "Use /ads para ver todos os anúncios.");

    SendAdmMessage("%s[ID:%i] usou /an (id:%i): %.50s", GetName(playerid), playerid, GetNextAdSlot()-1, params);

	return 1;
}


CMD:cad(playerid, params[])
{
	if (!PlayerInfo[playerid][pNumber])				return SendErrorMessage(playerid, "Você precisa de um número de telefone para enviar um anúncio.");
	if (isnull(params))								return SendHelpMessage(playerid, "/cad [mensagem]");
    if (PlayerInfo[playerid][pSettings] & togAdv)	return SendErrorMessage(playerid, "Por favor, ative a veiculação de anúncios primeiro.");

    new bizid = -1;

    if ((bizid = Advertisement_Nearest(playerid)) == -1)				return SendErrorMessage(playerid, "Você não está em uma agência de publicidade.");
	if (BizInfo[bizid][bizLocked] == 1)									return GameTextForPlayer(playerid, "~r~FECHADO!", 2500, 1); //adicionar russo
	if (BizInfo[bizid][bizOwnerID] != 0 && !BizInfo[bizid][bizProduct])	return SendErrorMessage(playerid, "Não há produtos suficientes no negócio.");
	if (GetCash(playerid) < BizInfo[bizid][bizEntrance])				return SendErrorMessage(playerid, "Você não tem dinheiro suficiente. Preço do anúncio $%i.", BizInfo[bizid][bizEntrance]);
	if (PlayerInQueue(playerid) > GetAdLimit(playerid)) 				return SendErrorMessage(playerid, "Você atingiu o limite de anúncios, você já enviou - %d anúncios.", PlayerInQueue(playerid));
	if (GetNextAdSlot() == -1)											return SendErrorMessage(playerid, "Todos os espaços estão ocupados, por favor tente novamente mais tarde! ");
    
    if (BizInfo[bizid][bizOwnerID] != 0)
    {
        BizInfo[bizid][bizProduct]--;
        GiveBizCash(bizid, BizInfo[bizid][bizEntrance]);
    }
	GiveCash(playerid, -BizInfo[bizid][bizEntrance]);

	publishAdvertisement(playerid, params, false);

    SendSpecialMessage(playerid, "Você enviou seu anúncio com sucesso, ele agora está na fila.");
	SendHelpMessage(playerid, "Use /ads para acompanhar seu anúncio.");

    SendAdmMessage("%s[ID:%i] usa /ad(id:%i): %.50s", GetName(playerid), playerid, GetNextAdSlot()-1, params);

	return 1;
}


CMD:ads(playerid, params[])
{
	return ListAds(playerid);
}

Advertisement_Nearest(playerid)
{
 	if (!IsPlayerLogged(playerid))	return -1;

	for(new i = 0; i < MAX_BIZ; i++)
	{
	    if (BizInfo[i][bizID] == 0 || BizInfo[i][bizType] != BIZ_ADVERT)	continue;
		if (PlayerToPoint(7.0,playerid,BizInfo[i][bizPosX], BizInfo[i][bizPosY], BizInfo[i][bizPosZ]) && GetWorld(playerid) == BizInfo[i][bizPosVW] && GetInterior(playerid) == BizInfo[i][bizPosInt])	return i;
	}
	
	return -1;
}

stock GetAdLimit(playerid)
{
	switch (PlayerInfo[playerid][pPremium])
	{
		case 1://DONATOR_BRONZE:
		    return 1;
		case 2://DONATOR_SILVER:
		    return 2;
		case 3://DONATOR_GOLD:
		    return 3;
		default://REGULAR_PLAYER:
	        return 1;
	}
	return 1;
}

stock ListAds(playerid)
{
    static
        gListString[22 + MAX_ADVERT_SLOT * 69]
    ;

    new count = 0;

    gListString = "#\tPublicidade\tEspera\n";
    for(new i = 0; i < MAX_ADVERT_SLOT; i++)
    {
        if (advert_data[i][advert_exists])
        {
            format(gListString, sizeof(gListString), "%s\n%d\t%.50s ...\t~%i segundos", gListString, i+1, advert_data[i][advert_text], advert_data[i][publish_time]);
            count ++;
        }
    }
    if (count == 0) 
        Dialog_Show(playerid, None, DIALOG_STYLE_MSGBOX, "Publicidade", "A fila está vazia.\nSeja o primeiro a anunciar! (( /ad [texto do anúncio] ))", "<<", "");
    else 
        Dialog_Show(playerid, None, DIALOG_STYLE_TABLIST_HEADERS, "Publicidade", gListString, "<<", "");

    return 0;
}


stock PlayerInQueue(playerid)
{
   	new count = 0;
	new name[MAX_PLAYER_NAME];
 	GetPlayerName(playerid, name, sizeof(name));
 	for(new i = 0; i < MAX_ADVERT_SLOT; i ++)
 	{
 	    if (strcmp(advert_data[i][advert_placeby], name, false) == 0 && advert_data[i][advert_exists]) count ++;
	}
	return count;
}

stock GetNextAdSlot()
{
	for(new i = 0; i < MAX_ADVERT_SLOT; i++)
	{
		if (!advert_data[i][advert_exists])
			return i;
	}
	return -1;
}

this::publishAdvertisement(playerid, text[], bool:personal)
{
	new new_ad = GetNextAdSlot();
	advert_data[ new_ad ][advert_id] = new_ad;
	advert_data[ new_ad ][advert_contact] = PlayerInfo[playerid][pNumber];
	format(advert_data[ new_ad ][advert_text], 256, text);
	format(advert_data[ new_ad ][advert_placeby], MAX_PLAYER_NAME, GetName(playerid));
	advert_data[ new_ad ][advert_exists] = true;
	advert_data[ new_ad ][advert_type] = 1;
	advert_data[ new_ad ][advert_placeby_id] = playerid;

	if (new_ad != 0)
	{
		advert_data[ new_ad ][publish_time] = advert_data[ new_ad - 1 ][publish_time] + 60 + random(10); 
	}
	else 
	{
		advert_data[ new_ad ][publish_time] = 60 + random(10);
	}

	if (!personal)
	{
		advert_data[ new_ad ][advert_type] = 2;
	}

	return 1;
}

stock ClearAd(ad_id)
{
	advert_data[ ad_id ][advert_id] = -1;
	advert_data[ ad_id ][publish_time] = -1;
	advert_data[ ad_id ][advert_contact] = 0;
	format(advert_data[ ad_id ][advert_text], 256, "None");
	format(advert_data[ ad_id ][advert_placeby], 32, "None");
	advert_data[ ad_id ][advert_exists] = false;
	advert_data[ ad_id ][advert_type] = 0;
	advert_data[ ad_id ][advert_placeby_id] = INVALID_PLAYER_ID;

	if (ad_id == MAX_ADVERT_SLOT) 				return 1;
	if (!advert_data[ ad_id + 1][advert_exists]) return 1;

	for(new i = ad_id+1; i < MAX_ADVERT_SLOT; i++)
	{
		if (advert_data[i][advert_exists])
		{
			advert_data[ i-1 ][advert_id] = advert_data[ i ][advert_id];
			advert_data[ i-1 ][publish_time] = advert_data[ i ][publish_time];
			advert_data[ i-1 ][advert_contact] = advert_data[ i ][advert_contact];
			format(advert_data[ i-1 ][advert_text], 256, "%s", advert_data[ i ][advert_text]);
			format(advert_data[ i-1 ][advert_placeby], 32, "%s", advert_data[ i ][advert_placeby]);
			advert_data[ i-1 ][advert_type] = advert_data[ i ][advert_type];
			advert_data[ i-1 ][advert_exists] = true;
			advert_data[ i-1 ][advert_placeby_id] = advert_data[ i ][advert_placeby_id];

			advert_data[ i ][advert_id] = -1;
			advert_data[ i ][publish_time] = -1;
			advert_data[ i ][advert_contact] = 0;
			format(advert_data[ i ][advert_text], 256, "None");
			format(advert_data[ i ][advert_placeby], 32, "None");
			advert_data[ i ][advert_exists] = false;
			advert_data[ i ][advert_type] = 0;
			advert_data[ i ][advert_placeby_id] = INVALID_PLAYER_ID;
			
		}
		else break;
	}

	return 1;
}

this::PublishAds()
{
	for(new ad_id = 0; ad_id < MAX_ADVERT_SLOT; ad_id ++)
	{
	    if (advert_data[ ad_id ][advert_exists] && advert_data[ ad_id ][publish_time] != -1)
	    {
	        advert_data[ ad_id ][publish_time] --;
	        if (advert_data[ ad_id ][publish_time] == 0)
	        {
	            foreach(new i : Player) 
	            {
				    if ( advert_data[ ad_id ][advert_type] == 1)
				    {
						if (IsPlayerConnected(i))
						{
	                        if (strlen(advert_data[ ad_id ][advert_text]) > MAXLEN)
							{
								new pos = MAXLEN;

								if (pos < MAXLEN-1) pos = MAXLEN;

								SendClientMessageEx(i, COLOR_GREENFORADVERT, "[Anúncio] %.*s ...", pos, advert_data[ ad_id ][advert_text]);
								SendClientMessageEx(i, COLOR_GREENFORADVERT, "[Anúncio] ... %s Cel. %i.", advert_data[ ad_id ][advert_text][pos], advert_data[ ad_id ][advert_contact], (strlen(advert_data[ ad_id ][advert_text]) < MAXLEN-10)?(" (( /ultimosanuncios ))"):(""));
							}
							else
							{
								SendClientMessageEx(i, COLOR_GREENFORADVERT, "[Anúncio] %s Cel. %i.", advert_data[ ad_id ][advert_text], advert_data[ ad_id ][advert_contact], (strlen(advert_data[ ad_id ][advert_text]) < MAXLEN-10)?(" (( /ultimosanuncios ))"):(""));
							}
						}
				    }
					else if ( advert_data[ ad_id ][advert_type] == 2)
					{
						if (strlen(advert_data[ ad_id ][advert_text]) > MAXLEN)
						{
							new pos = MAXLEN;

							if (pos < MAXLEN-1) pos = MAXLEN;

							SendClientMessageEx(i, COLOR_GREENFORADVERT, "[Anúncio empresarial] %.*s ...", pos, advert_data[ ad_id ][advert_text]);
							SendClientMessageEx(i, COLOR_GREENFORADVERT, "[Anúncio empresarial] ... %s Cel. %i.", advert_data[ ad_id ][advert_text][pos], advert_data[ ad_id ][advert_contact], (strlen(advert_data[ ad_id ][advert_text]) < MAXLEN-10)?(" (( /ultimosanuncios ))"):(""));
						}
						else
						{
							SendClientMessageEx(i, COLOR_GREENFORADVERT, "[Anúncio empresarial] %s Cel. %i.", advert_data[ ad_id ][advert_text], advert_data[ ad_id ][advert_contact], (strlen(advert_data[ ad_id ][advert_text]) < MAXLEN-10)?(" (( /ultimosanuncios ))"):(""));
						}
					}
				}	

				format(LastAdvertInfo[lastadvert][advertText], 256, "%s", advert_data[ ad_id ][advert_text]);
				LastAdvertInfo[lastadvert][adverttime] = gettime();
				LastAdvertInfo[lastadvert][advertnumber] = advert_data[ ad_id ][advert_contact];
				if (lastadvert == MAX_LAST_ADVERT) lastadvert = 0;
				else lastadvert++;

				ClearAd(ad_id);
	        }
	    }
	}
}