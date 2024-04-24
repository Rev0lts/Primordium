//���������

#define MODE_SKATE (0)        // 0 - medium speed | 1 - fast speed
#define TYPE_SKATE (0)        // 0 - the skate is placed on the right arm | 1 - the skate is placed in the back
#define INDEX_SKATE (0)       //is the slot that used SetPlayerAttachedObject

enum skate
{
        bool:sActive,
        sSkate,
};
new InfoSkate[MAX_PLAYERS][skate];

CMD:skate(playerid, params[])
{
    new mes[128];

    if (!IsPlayerHasItem(playerid, INV_SKATE)) return SendErrorMessage(playerid, "Voc� n�o possui um skate.");

    if (InfoSkate[playerid][sActive] == true)
    {
        InfoSkate[playerid][sActive] = false;
        DestroyObject(InfoSkate[playerid][sSkate]);
        PlayerPlaySound(playerid, 21000, 0, 0, 0);
        format(mes, sizeof(mes), "removeu o skate das m�os.");
        cmd::me(playerid, mes);
    }
    else if (InfoSkate[playerid][sActive] == false)
    {
        InfoSkate[playerid][sActive] = true;
        DestroyObject(InfoSkate[playerid][sSkate]);
        //PlayerPlaySound(playerid, 21000, 0, 0, 0);
        SendSyntaxMessage(playerid, "Para andar de skate, clique com o 'RMB'. Para remover o skate - use-o atrav�s do seu invent�rio.");
    }

    return 1;
}


Skate_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if (InfoSkate[playerid][sActive] && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
	{
        static bool:act;

        SetPlayerArmedWeapon(playerid,0);
        
        if (newkeys & KEY_HANDBRAKE)
		{
            #if MODE_SKATE == 0
                ApplyAnimation(playerid, "SKATE","skate_run",4.1,1,1,1,1,1,1);
            #else
            	ApplyAnimation(playerid, "SKATE","skate_sprint",4.1,1,1,1,1,1,1);
            #endif
        	
			if (!act)
			{
                act = true;
                DestroyObject(InfoSkate[playerid][sSkate]);
                InfoSkate[playerid][sSkate] = CreateObject(19878,0,0,0,0,0,0);
                AttachObjectToPlayer(InfoSkate[playerid][sSkate],playerid, -0.2,0,-0.9,0,0,90);
            }
		}
        if (oldkeys & KEY_HANDBRAKE)
		{
        	ApplyAnimation(playerid, "CARRY","crry_prtial",4.0,0,0,0,0,0);
        	
			if (act)
			{
                act = false;
                DestroyObject(InfoSkate[playerid][sSkate]);
            }
        }
	}
	return 1;
}