Apart_OnPlayerEnterDynamicCP(playerid, checkpointid)
{
    if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
    {
        for(new i = 0; i != MAX_APART; i++)
        {
            if (ApartInfo[i][aApartOn] == 0 || checkpointid != ApartInfo[i][aCheckPoint]) continue;

            // Envie uma mensagem de sintaxe para o jogador, "Você está na entrada do complexo de apartamentos. Endereço: %s, %i.", GetApartmentZone(i), i+1.
            // Envie uma mensagem de ajuda para o jogador, "/enter, /(ds)hout, /ddo."

            new string[128];
            format(string, sizeof(string),"~h~~p~Apartamentos~n~~y~%s", GetApartmentZone(i, 2)); 
            CreateLableText(playerid, string, "~h~Pressione ~y~~h~'F' ~w~ para entrar ou~n~digite ~y~~h~/enter~h~~w~ para entrar");
            return 1;
        }
    }
    return 1;
}

Apart_OnGameModeInit()
{
    mysql_tquery(dbHandle, "SELECT * FROM `apartment`", "LoadDynamicApartment");
    return 1;
}