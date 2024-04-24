#define MAX_FISH_ZONES 4

new gz_fish[MAX_FISH_ZONES];

new Float: fish_pos[MAX_FISH_ZONES][5] =
{
	{1429.9701,-237.4749,1.3174,182.8500},
	{1429.9701,-237.4749,1.3174,182.8500},
	{1429.9701,-237.4749,1.3174,182.8500},
	{1429.9701,-237.4749,1.3174,182.8500}
};

new fish_names[10][13] =
{
    {"Salmão"},
    {"Carpa"},
    {"Sono"},
    {"Platija"},
    {"Lúcio"},
    {"Peixe-badejo"},
    {"Sardinha"},
    {"Cachorro"},
    {"Atum"},
    {"Nemo"}
};


Fish_Init()
{
	for(new i; i < MAX_FISH_ZONES; i++)
	{
    	gz_fish[i] = GangZoneCreate(fish_pos[i][0], fish_pos[i][1], fish_pos[i][2], fish_pos[i][3]);
    	GangZoneHideForAll(gz_fish[i]);
	}
	return 1;
}

CMD:meuspeixes(playerid, params[])
{
    SendSyntaxMessage(playerid, "O peso dos peixes que você pegou é de %i lb.", PlayerInfo[playerid][pFishCount]);
    if (PlayerInfo[playerid][pFishCount] >= 300)
    {
        return SendSyntaxMessage(playerid, "Use /venderpeixes para entregar os peixes. ");
    }
    return 1;
}

CMD:pararpescaria(playerid, params[])
{
    if (pTemp[playerid][pFishTime] == 0) return SendErrorMessage(playerid, "Você não lançou sua vara de pesca.");

    pTemp[playerid][pFishTime] = 0;
    StopPlayerAnims(playerid);
    return 1;
}


CMD:venderpeixes(playerid, params[])
{
    if (!PlayerToPoint(4, playerid, 2478.0762, -2707.8352, 3.0000))
    {
        SendFishMessage(playerid, "Você não está no local de venda de peixes.");
        InfoMSG(playerid, "Rota traçada ~r~(/removercp)", 3);
        Waypoint_Set(playerid, 2478.0762, -2707.8352, 3.0000, 8.0);
        return 1;
    }

    if (PlayerInfo[playerid][pFishTime] > gettime()) return SendErrorMessage(playerid, "Você pode entregar os peixes em %s.", ConvertSecond(PlayerInfo[playerid][pFishTime] - gettime()));
    //if (PlayerInfo[playerid][pFishCount] < 300) return SendErrorMessage(playerid, "Você não tem peixes suficientes. A quantidade mínima é de 300 lb.");

    new dinheiro = floatround(0.7 * float(PlayerInfo[playerid][pFishCount]));

    GiveCash(playerid, dinheiro);
    PlayerInfo[playerid][pFishCount] = 0;
    PlayerInfo[playerid][pFishTime] = gettime() + 2400;
    SendSyntaxMessage(playerid, "Você entregou %i kg de peixe por $%i.", PlayerInfo[playerid][pFishCount], dinheiro);
    SendHelpMessage(playerid, "No momento não estamos precisando de peixes. Você pode entregar mais apenas após 40 minutos. ");
    return 1;
}

CMD:zonapesca(playerid, params[])
{
    if (GetPVarInt(playerid, #show_fish) == 1)
    {
        DeletePVar(playerid, #show_fish);
        for(new i; i < MAX_FISH_ZONES; i++) GangZoneHideForPlayer(playerid, gz_fish[i]);

        SendSyntaxMessage(playerid, "As zonas de pesca foram desativadas.");
        return 1;
    }

    SetPVarInt(playerid, #show_fish, 1);
    for(new i; i < MAX_FISH_ZONES; i++) GangZoneShowForPlayer(playerid, gz_fish[i], 0xFF0000AA);

    SendSyntaxMessage(playerid, "As zonas de pesca foram marcadas no seu mapa (ESC > Mapas).");
    return 1;
}


CMD:pescar(playerid, params[])
{
    if (PlayerInfo[playerid][pFishCount] >= 1000) 			return SendErrorMessage(playerid, "Você não pode mais pescar.");
    if (pTemp[playerid][pFishTime] != 0)						return SendErrorMessage(playerid, "Você já lançou sua linha.");
    if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT)		return SendErrorMessage(playerid, "Você deve estar fora do veículo.");
        
    new id = -1;

    for(new i; i != MAX_PLAYER_ATTACH; i++)
    {
        if (!AttachInfo[playerid][i][aID] || AttachInfo[playerid][i][aModel] != 18632 || !AttachInfo[playerid][i][aUsed]) continue;

        id = i;
        break;
    }

    if (id == -1) return SendErrorMessage(playerid, "Você precisa de uma vara de pesca.");

    id = -1;

    for(new i; i != 4; i++)
    {
        if (!IsPlayerInCube(playerid, fish_pos[i][0], fish_pos[i][1], fish_pos[i][2], fish_pos[i][3])) continue;

        id = i;
        break;
    }

    if (id == -1) return SendErrorMessage(playerid, "Você não está em um local adequado para pescar (/zonapesca).");

    new veh = 0;

    if (id && ((veh = GetNearVehicle(playerid, 5.0)) == 0 || !IsABoat(veh))) return SendErrorMessage(playerid, "Você precisa estar perto de um barco.");
    if (!OnPlayAnim(playerid, "SWORD", "SWORD_BLOCK", 4.1, 0, 0, 0, 1, 0)) return SendErrorMessage(playerid, "Você não pode pescar porque não tem espaço suficiente.");

    //pTemp[playerid][pFishTime] = 10 + random(10);
    new totalpeste[85];
    new descarca[95];
    format(totalpeste, sizeof(totalpeste), "Total de peixes ~b~~h~%d ~w~kg", PlayerInfo[playerid][pFishCount]);
    TextDrawSetString(fishTD[5], totalpeste);
    if(PlayerInfo[playerid][pFishCount] >= 300) {
        format(descarca, sizeof(descarca), "~r~Use /venderpeixes");
        TextDrawSetString(fishTD[6], descarca);
        TextDrawShowForPlayer(playerid, fishTD[6]);
    }
    TextDrawShowForPlayer(playerid, fishTD[0]);
    TextDrawShowForPlayer(playerid, fishTD[1]);
    TextDrawShowForPlayer(playerid, fishTD[4]);
    TextDrawShowForPlayer(playerid, fishTD[5]);
    //TextDrawShowForPlayer(playerid, fishTD[6]);
    TextDrawShowForPlayer(playerid, fishTD[7]);
    TextDrawShowForPlayer(playerid, fishTD[8]);
    TextDrawShowForPlayer(playerid, fishTD[9]);
    SelectTextDraw(playerid, 0xFFFFFF00);
    return 1;
}

