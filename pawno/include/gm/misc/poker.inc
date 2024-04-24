//�����

#define LEAR_HEARTS 1
#define LEAR_DIAMONDS 2
#define LEAR_CLUBS 3
#define LEAR_SPADES 4

#define MAX_TYPE_CARDS 14
#define MAX_CARDS_FOR_LEAR 4
#define MAX_PLAYER_CURRENT_CARDS 6

new exicards[0][13] = {"2","3","4","5","6","7","8","9","10","J","Q","K","A"};

new PackCards[MAX_PLAYERS][MAX_TYPE_CARDS];
new PackCardsLears[MAX_PLAYERS][14][MAX_CARDS_FOR_LEAR];

new PlayerSumm[MAX_PLAYERS];
new PlayerSummLear[MAX_PLAYERS][14];
new PlayerCurrentCards[MAX_PLAYERS][MAX_PLAYER_CURRENT_CARDS][2];

ClearPackCards(playerid)
{
	for(new i = 0; i < MAX_PLAYER_CURRENT_CARDS; i++)
	{
		PlayerCurrentCards[playerid][i][0] = 0;
		PlayerCurrentCards[playerid][i][1] = 0;
	}

	DeletePVar(playerid, "SummCards");
	InsertPackCards(playerid);
	return 1;
}
InsertPackCards(playerid)
{
	for(new i = 0; i < MAX_TYPE_CARDS; i++)
	{
		PackCards[playerid][i] = i;

		for(new c = 0; c < MAX_CARDS_FOR_LEAR; c++)
		{
			PackCardsLears[playerid][i][c] = c+1;
		}
	}
	return 1;
}
GetPlayerSummCard(playerid)
{
	PlayerSumm[playerid] = 0;

	for(new i = 0; i < MAX_TYPE_CARDS; i++)
	{
		if (PackCards[playerid][i] == -1) continue;

		PlayerSumm[playerid]++;
		PlayerSummLear[playerid][i] = 0;

		for(new c = 0; c < MAX_CARDS_FOR_LEAR; c++)
		{
			if (PackCardsLears[playerid][i][c] == -1) continue;

			PlayerSummLear[playerid][i]++;
		}
	}
}
GivePlayerCard(playerid, &newcard, &newcardlear)
{
	new SummEx[2];
	new returnresult[2];

	returnresult[0] = random(PlayerSumm[playerid]);
	returnresult[1] = random(PlayerSummLear[playerid][returnresult[0]]);

	if (returnresult[0] == 0)	returnresult[0] += 1;
	if (returnresult[1] == 0) 	returnresult[1] += 1;

	for(new i = 0; i < MAX_TYPE_CARDS; i++)
	{
		if (PackCards[playerid][i] == -1) continue; 

		SummEx[0]++;

		if (SummEx[0] == returnresult[0])
		{
			for(new c = 0; c < MAX_CARDS_FOR_LEAR; c++)
			{
				if (PackCardsLears[playerid][i][c] == -1) continue;

				SummEx[1]++;

				if (SummEx[1] == returnresult[1])
				{
					PackCardsLears[playerid][i][c] = -1;

					if (PlayerSummLear[playerid][i] == 1) PackCards[playerid][i] = -1;

					newcard = i; newcardlear = c+1;
					return 1;
				}
			}
		}
	}
	return 0;
}
ShowPlayerCards(playerid)
{
    new cardname[156], cardslist[186];

    for (new i = 0; i < GetPVarInt(playerid, "SummCards"); i++)
    {
        format(cardname, sizeof(cardname), "%s%s ", exicards[PlayerCurrentCards[playerid][i][0]][0], GetTypeCard(PlayerCurrentCards[playerid][i][1]));
        strcat(cardslist, cardname, sizeof(cardslist));
    }

    SendClientMessageEx(playerid, COLOR_YELLOW, "Cartas na m�o: %s. Use /showdown para mostrar as cartas.", cardslist);
    return 1;
}

ShowPlayerCardsToAll(playerid)
{
    new cardname[156], cardslist[186];

    for (new i = 0; i < GetPVarInt(playerid, "SummCards"); i++)
    {
        format(cardname, sizeof(cardname), "%s%s ", exicards[PlayerCurrentCards[playerid][i][0]][0], GetTypeCard(PlayerCurrentCards[playerid][i][1]));
        strcat(cardslist, cardname, sizeof(cardslist));
    }

    SendNearbyMessage(Range_Normal, playerid, COLOR_WHITE, "* %s abriu as cartas: %s", GetNameWithMask(playerid), cardslist);
    ClearPackCards(playerid);
    return 1;
}

GetTypeCard(learid)
{
    new leartext[15];

    if (learid == LEAR_HEARTS) { leartext = "[Cora��es]"; }
    if (learid == LEAR_DIAMONDS) { leartext = "[Diamantes]"; }
    if (learid == LEAR_CLUBS) { leartext = "[Paus]"; }
    if (learid == LEAR_SPADES) { leartext = "[Espadas]"; }

    return leartext;
}

CMD:mostrarcartas(playerid, params[])
{
    if (GetPVarInt(playerid, "SummCards") < 1) return SendErrorMessage(playerid, "Voc� n�o possui cartas.");
    ShowPlayerCardsToAll(playerid);
    ClearPackCards(playerid);
    return 1;
}

CMD:darcartas(playerid, params[])
{
    new id;
    if (sscanf(params, "u", id)) return SendHelpMessage(playerid, "/darcartas [ID/Parte do Nome]");
    if (!IsPlayerHasItem(playerid, INV_CARDS)) return SendErrorMessage(playerid, "Voc� n�o possui cartas de jogo.");
    if (!IsPlayerLogged(id)) return SendNotConnected(playerid);
    if (!PlayerNearPlayer(5.0, playerid, id)) return SendNotNearPlayer(playerid);
    if (GetPVarInt(id, "SummCards") > 6) return SendErrorMessage(playerid, "Este jogador j� possui cartas suficientes.");

    new newcarid, newcaridlear;

    GetPlayerSummCard(playerid);

    if (PlayerSumm[playerid] < 1) return SendErrorMessage(playerid, "N�o h� mais cartas no baralho.");

    GivePlayerCard(playerid, newcarid, newcaridlear);
    PlayerCurrentCards[id][GetPVarInt(id, "SummCards")][0] = newcarid;
    PlayerCurrentCards[id][GetPVarInt(id, "SummCards")][1] = newcaridlear;
    SetPVarInt(id, "SummCards", GetPVarInt(id, "SummCards") + 1);

    SendSyntaxMessage(id, "Voc� recebeu a carta: %s%s", exicards[newcarid][0], GetTypeCard(newcaridlear));
    ShowPlayerCards(id);

    if (id == playerid) cmd::server_ame(playerid, "tirou uma carta virada para si mesmo.");
    else
    {
        new mes[128];
        format(mes, sizeof(mes), "passa a carta virada %s.", GetNameWithMask(id));
        SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);
    }
    return 1;
}