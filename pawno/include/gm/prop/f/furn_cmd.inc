ALTX:furniture("/mobilia");
CMD:furniture(playerid, params[])
{
    if (GetPVarInt(playerid, "buy_furniture")) return SendErrorMessage(playerid, "A ação já está em progresso.");
        
    new type = GetNearBuilding(playerid);
    if (type == -1) return SendErrorMessage(playerid, "Você deve estar dentro ou próximo à sua propriedade."); // erro

    switch(type)
    {
        case 0: HouseFurn(playerid);
        case 1: BizFurn(playerid);
        case 2: GarageFurn(playerid);
    }

    Check3DMenu(playerid);
    return 1;
}
