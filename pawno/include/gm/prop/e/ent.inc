Enterance_OnGameModeInit()
{
	return mysql_tquery(dbHandle, "SELECT * FROM `entrance`", "LoadDynamicEntrance");
}

Ent_OnPlayerPickUpDynamicPickup(playerid, pickupid)
{
    for(new i; i != MAX_ENTRANCE; i++)
    {
        if (!EntranceInfo[i][entranceOn] || EntranceInfo[i][eEnterPickup] != pickupid) continue;
        if (strlen(EntranceInfo[i][eText]) > 1) {
            new title[128];
            format(title, sizeof(title), "~h~~p~%s", EntranceInfo[i][eText]);
            CreateLableText(playerid, title, "~w~Para entrar~n~~w~Digite ~y~/entrar~w~ ou pressione ~y~~h~'F'");
        }

        return 1;
    }

    if (PlayerInfo[playerid][pFaction] != 0 && pickupid == Factions[PlayerInfo[playerid][pFaction]][fPickUp])
    {
        new mes[32];
        format(mes, sizeof(mes), "_~g~%s", Factions[PlayerInfo[playerid][pFaction]][fName]);
        InfoMSG2(playerid, mes, "~y~/duty ~w~- para abrir o menu", 5000);
    }

    for(new i; i != MAX_JOBS; i++) {
        if (JobsInfo[i][jobOn] != 1) continue;
        if (JobsInfo[i][jobPickUp] == pickupid) {
            new mes[32];
            format(mes, sizeof(mes), "_~r~%s", GetJobName(JobsInfo[i][jobType]));
            InfoMSG2(playerid, mes, "~w~Pressione ~y~ENTER~w~ para abrir o menu", 5000);
            break;
        }
    }
    
    return 1;
}
