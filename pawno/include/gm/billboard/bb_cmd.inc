ALTX:rentbillboard("/alugaroutdoor");
CMD:rentbillboard(playerid, params[]) 
{
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed]) return 1;
	if (!PlayerInfo[playerid][pNumber]) return SendErrorMessage(playerid, "Para enviar um anúncio, você precisa de um telefone.");

    new bizid = -1;
    if ((bizid = Advertisement_Nearest(playerid)) == -1) return SendErrorMessage(playerid, "Você não está com uma agência de publicidade.");
	if (BizInfo[bizid][bizLocked] == 1)					 return GameTextForPlayer(playerid, "~r~FECHADO!", 2500, 1);

	new str[128], mes[(13+MAX_ZONE_NAME+60)*MAX_BILLBOARDS+27] = "Localização\tNome\tStatus\n";

	new count;
	for(new i; i < MAX_BILLBOARDS; i++) if (BillboardInfo[i][bbRentBy] == PlayerInfo[playerid][pID]) count++;
	if (count >= 3) return SendErrorMessage(playerid, "Você já alugou o número máximo de outdoors! /meusooutdoor - para gerenciamento.");

	new location[MAX_ZONE_NAME];
	count = 0;

	for(new i; i < MAX_BILLBOARDS; i++) {
		if (!BillboardInfo[i][bbActive]) continue;
		GetZone(BillboardInfo[i][bbPosX],BillboardInfo[i][bbPosY], location, MAX_ZONE_NAME);

		if (!BillboardInfo[i][bbRentDate]) 	format(str, sizeof(str), "%s\tOutdoor #%i\t{16b819}Livre{FFFFFF}", location, i+1);
		else								format(str, sizeof(str), "%s\tOutdoor #%i\t{FF6347}Alugado por %s até %s{FFFFFF}", location, i+1, GetNameFromBD(BillboardInfo[i][bbRentBy]), get_date("%dd.%mm.%yy", BillboardInfo[i][bbRentDate]));

		format(mes, sizeof(mes), "%s\n%s", mes, str);
		count++;
	}

	if (!count) return SendErrorMessage(playerid, "Não há outdoors no servidor!");

	Dialog_Show(playerid, BillboardList, DIALOG_STYLE_TABLIST_HEADERS, "Aluguel de outdoor", mes, "Alugar", "Cancelar");
	return 1;
}


ALTX:mybillboards("/mybb","/meusooutdoor","/meuatd");
CMD:mybillboards(playerid, params[])
{
    if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed]) return 1;
    if (!PlayerInfo[playerid][pNumber]) return SendErrorMessage(playerid, "Você precisa de um número de telefone para enviar um anúncio.");

    new bizid = -1;
    if ((bizid = Advertisement_Nearest(playerid)) == -1) return SendErrorMessage(playerid, "Você não está com uma agência de publicidade.");
    if (BizInfo[bizid][bizLocked] == 1) return GameTextForPlayer(playerid, "~r~FECHADO!", 2500, 1);

    new count, str[90], mes[(60 + MAX_ZONE_NAME + 13) * 3 + 27] = "Localização\tNome\tStatus\n", location[MAX_ZONE_NAME];

    for (new i; i < MAX_BILLBOARDS; i++)
    {
        if (BillboardInfo[i][bbRentBy] == PlayerInfo[playerid][pID])
        {
            GetZone(BillboardInfo[i][bbPosX], BillboardInfo[i][bbPosY], location, MAX_ZONE_NAME);
            format(str, sizeof(str), "%s\tOutdoor #%i\tAlugado até %s{FFFFFF}", location, i + 1, get_date("%dd.%mm.%yy", BillboardInfo[i][bbRentDate]));

            format(mes, sizeof(mes), "%s\n%s", mes, str);
            count++;

            if (count == 3) break;
        }
    }

    if (!count) return SendErrorMessage(playerid, "Você não tem outdoors alugados!");

    Dialog_Show(playerid, MyBillboards, DIALOG_STYLE_TABLIST_HEADERS, "Seus outdoors", mes, "Remover", "Cancelar");
    return 1;
}
