ALTX:setstation("/ss");
CMD:setstation(playerid, params[])
{
    new vehicle = GetPlayerVehicleID(playerid), id;

    if (IsDriver(playerid))
    {
        if (VehicleInfo[vehicle][carRadio] == 0) return SendErrorMessage(playerid, "N�o h� r�dio neste ve�culo.");

        ShowBoomBoxStations(playerid);
        return 1;
    }

    if ((id = GetNearInsideHouse(playerid)) != -1 && HouseInfo[id][hOwnerID] == PlayerInfo[playerid][pID])
    {
        if (HouseInfo[id][hRadio] == 0) return SendErrorMessage(playerid, "N�o h� r�dio em sua casa.");

        ShowBoomBoxStations(playerid);
        return 1;
    }
    else if ((id = GetNearInsideBiz(playerid)) != -1 && BizInfo[id][bizOwnerID] == PlayerInfo[playerid][pID])
    {
        if (BizInfo[id][bizRadio] == 0) return SendErrorMessage(playerid, "N�o h� r�dio em seu neg�cio.");

        ShowBoomBoxStations(playerid);
        return 1;
    }
    return 1;
}