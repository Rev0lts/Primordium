ALTX:gr("/graffity","/grafite"); //grafite
CMD:gr(playerid, params[])
{
    new option[16], parameters[145];
    if (sscanf(params, "s[16]S()[144]", option, parameters))
    {
        SendHelpMessage(playerid, "(/gr)afite [criar / parar]");
        return 1;
    }
    if (CompareStrings(option, "criar"))
    {
        new g_text[33];
        if (sscanf(parameters, "s[32]", g_text))                                         return SendHelpMessage(playerid, "/gr criar [texto]");
        if (strlen(g_text) > MAX_GRAFFITY_TEXT || strlen(g_text) < MIN_GRAFFITY_TEXT)    return SendErrorMessage(playerid, "Mín: %i caracteres, Máx: %i caracteres.", MIN_GRAFFITY_TEXT, MAX_GRAFFITY_TEXT);
        if (Graffity_GetFreeID() == -1)                                                  return SendErrorMessage(playerid, "Limite de grafite excedido. Por favor, tente criar um grafite mais tarde.");
        if (AntiCheatGetWeapon(playerid) != 41)                                          return SendErrorMessage(playerid, "Você deve ter uma lata nas mãos.");
        if (Graffity_GetAccepts(playerid) != -1) 
        {
            SendErrorMessage(playerid, "Você já criou um grafite e ele está sob consideração.");
            SendSyntaxMessage(playerid, "Espere até que o grafite seja rejeitado ou aprovado antes de pintar um novo.");
            return 1;
        }
        if (Graffity_Nearest(playerid) != -1)        return SendErrorMessage(playerid, "Já existe um grafite próximo a você.");
        if (Graffity_GetPlayerEdit(playerid) != -1)  return SendErrorMessage(playerid, "Você já está editando um grafite.");
        if (IsPlayerInAnyVehicle(playerid))          return SendErrorMessage(playerid, "Você não deve estar em um veículo.");
        if (PlayerInfo[playerid][pInjured])          return SendErrorMessage(playerid, "Você não pode pintar grafite enquanto estiver ferido/morrendo.");
        
        Graffity_CreateTemporary(playerid, g_text);
        return 1;
    }
    else if (CompareStrings(option, "parar"))
    {
        if (Graffity_GetPlayerEdit(playerid) == -1)      return SendErrorMessage(playerid, "Você não está editando um grafite.");
        if (!GetPVarInt(playerid, "Graffity:Spraying"))   return SendErrorMessage(playerid, "Você não está pintando um grafite.");
        
        SendSyntaxMessage(playerid, "A criação do grafite foi cancelada.");
        Graffity_DestroyTemporary(Graffity_GetPlayerEdit(playerid), 1);
        Graffity_ResetPlayer(playerid);
        UnFreezePlayer(playerid);
        cmd::stopanim(playerid, "");
        return 1;
    }
    else
    {
        SendHelpMessage(playerid, "(/gr)afite [criar / parar]");
    }
    return 1;
}
