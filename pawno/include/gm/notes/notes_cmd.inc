CMD:nota(playerid, params[])
{
    new option[7], secoption[128];
    if (sscanf(params, "s[7]S(NULL)[128]", option, secoption)) return SendHelpMessage(playerid, "/nota [mostrar / criar / remover / editar / dar]");

    if (CompareStrings(option, "mostrar"))
    {
        new count = 0;

        for(new i = 0; i < MAX_PLAYER_NOTES; i++)
        {
            if (PlayerNotes[playerid][i][noteOn] != 1) continue;

            SendClientMessageEx(playerid, COLOR_HELPS, "[Bloco de Notas] Nota [ID: %d]: %s", i+1, PlayerNotes[playerid][i][noteText]);

            count++;
        }

        if (count == 0) return SendErrorMessage(playerid, "Você não tem notas.");

        cmd::server_ame(playerid, "tirou um bloco de notas do bolso e olhou para ele.");
        return 1;
    }
    else if (CompareStrings(option, "editar"))
    {
        new slot, text[128];

        if (sscanf(params, "{s[7]}ds[128]", slot, text)) return SendHelpMessage(playerid, "/nota editar [slot] [novo texto]");
        if (!IsPlayerHasItem(playerid, INV_PEN)) return SendErrorMessage(playerid, "Você não tem uma caneta.");
        if (slot < 1 || slot > MAX_PLAYER_NOTES) return SendErrorMessage(playerid, "Slot de 1 a 10.");
        if (strlen(text) < 1 || strlen(text) > 127) return SendErrorMessage(playerid, "O comprimento do texto deve ser de 1 a 127.");

        slot = slot - 1;

        if (PlayerNotes[playerid][slot][noteOn] != 1) return SendSyntaxMessage(playerid, "Não há nota neste slot.");

        format(PlayerNotes[playerid][slot][noteText], 128, "%s", text);
        SQL_SetString("player_notes", "text", text, PlayerNotes[playerid][slot][noteID]);
        cmd::server_ame(playerid, "tirou um bloco de notas do bolso e escreveu algo nele.");
        SendSyntaxMessage(playerid, "Você alterou com sucesso a nota [ID: %i] para: %s", slot+1, text);
        return 1;
    }
    else if (CompareStrings(option, "criar"))
    {
        new text[128];

        if (sscanf(params, "{s[7]}s[128]", text)) return SendHelpMessage(playerid, "/nota criar [texto]");
        if (!IsPlayerHasItem(playerid, INV_PEN)) return SendErrorMessage(playerid, "Você não tem uma caneta.");
        if (strlen(text) < 1 || strlen(text) > 127) return SendErrorMessage(playerid, "Comprimento do texto deve ser de 1 a 127.");

        new noteid = -1;

        for(new i = 0; i < MAX_PLAYER_NOTES; i++)
        {
            if (PlayerNotes[playerid][i][noteOn] == 1) continue;

            noteid = i;
            break;
        }

        if (noteid == -1) return SendErrorMessage(playerid, "Não há espaço para escrever no bloco de notas.");

        format(PlayerNotes[playerid][noteid][noteText], 128, "%s", text);
        PlayerNotes[playerid][noteid][noteOn] = 1;

        new query[128 + 128];
        mysql_format(dbHandle, query, sizeof(query), "INSERT INTO `player_notes` (userid, text) VALUES (%i, '%e')", PlayerInfo[playerid][pID], text);
        mysql_tquery(dbHandle, query, "OnNoteInsert", "ii", playerid, noteid);

        cmd::server_ame(playerid, "tirou um bloco de notas do bolso e escreveu algo nele.");
        SendSyntaxMessage(playerid, "A nota [ID: %i] foi criada com sucesso.", noteid+1);

        return 1;
    }
    else if (CompareStrings(option, "remover"))
    {
        new slot;

        if (sscanf(params, "{s[7]}d", slot)) return SendHelpMessage(playerid, "/nota remover [slot]");
        if (slot < 1 || slot > MAX_PLAYER_NOTES) return SendErrorMessage(playerid, "Slot incorreto. 1 - %i.", MAX_PLAYER_NOTES);

        slot = slot - 1;

        if (PlayerNotes[playerid][slot][noteOn] != 1) return SendSyntaxMessage(playerid, "Não há nota neste slot.");
        PlayerNotes[playerid][slot][noteOn] = 0;
        PlayerNotes[playerid][slot][noteID] = 0;
        format(PlayerNotes[playerid][slot][noteText], 128, "");

        new query[128];
        mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `player_notes` WHERE `id` = %i", PlayerNotes[playerid][slot][noteID]);
        mysql_tquery(dbHandle, query);

        cmd::server_ame(playerid, "tirou um bloco de notas do bolso, rasgou uma página e jogou no chão.");
        SendSyntaxMessage(playerid, "A nota [ID: %i] foi removida com sucesso.", slot+1);
        return 1;
    }
    else if (CompareStrings(option, "dar"))
    {
        new targetid, slot;

        if (sscanf(params, "{s[7]}du", slot, targetid)) return SendHelpMessage(playerid, "/nota dar [slot] [ID/Parte do Nome]");
        if (slot < 1 || slot > MAX_PLAYER_NOTES) return SendErrorMessage(playerid, "Slot de 1 a 10.", MAX_PLAYER_NOTES);

        slot = slot - 1;

        if (PlayerNotes[playerid][slot][noteOn] != 1) return SendSyntaxMessage(playerid, "Não há notas neste slot.");
        if (targetid == playerid) return SendErrorMessage(playerid, "Você não pode dar uma nota para si mesmo.");
        if (!IsPlayerLogged(targetid)) return SendNotConnected(playerid);
        if (!PlayerNearPlayer(3.0, playerid, targetid)) return SendNotNearPlayer(playerid);

        new noteid = -1;

        for(new i = 0; i < MAX_PLAYER_NOTES; i++)
        {
            if (PlayerNotes[targetid][i][noteOn] == 1) continue;

            noteid = i;
            break;
        }

        if (noteid == -1) return SendErrorMessage(playerid, "Não há espaço no bloco de notas do jogador.");

        PlayerNotes[targetid][noteid][noteID] = PlayerNotes[playerid][slot][noteID];
        format(PlayerNotes[targetid][noteid][noteText], 128, "%s", PlayerNotes[playerid][slot][noteText]);
        PlayerNotes[targetid][noteid][noteOn] = 1;
        PlayerNotes[playerid][slot][noteID] = 0;
        PlayerNotes[playerid][slot][noteOn] = 0;

        SQL_SetInteger("player_notes", "userid", PlayerInfo[targetid][pID], PlayerNotes[playerid][slot][noteID]);
        SendSyntaxMessage(playerid, "Você passou a nota [ID: %i] para o jogador %s [ID: %i].", slot+1, GetNameWithMask(targetid), targetid);
        SendSyntaxMessage(targetid, "Você recebeu uma nota [ID: %i] do jogador %s [ID: %i].", noteid+1, GetNameWithMask(playerid), playerid);

        new mes[128];
        format(mes, sizeof(mes), "tirou um bloco de notas do bolso, rasgou uma página e entregou para %s.", GetNameWithMask(targetid));
        SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);
        return 1;
    }
    SendHelpMessage(playerid, "/nota [mostrar / criar / remover / editar / dar]");
    return 1;
}
