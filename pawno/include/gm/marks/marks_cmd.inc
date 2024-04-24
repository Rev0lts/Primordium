CMD:mark(playerid, params[]) // Comando para marcar
{
    if (!CheckAdmin(playerid, ADMIN_LEVEL_2))   return SendNotAdmin(playerid);
    
    new option[16], string[145];
    if (sscanf(params, "s[16]S()[144]", option, string))
    {
        SendHelpMessage(playerid, "/mark [opções]");
        SendSyntaxMessage(playerid, "listar / criar / destruir / editar");
        return 1;
    }

    if (CompareStrings(option, "listar"))
    {
        new list[512] = "{FFFFFF}Slot\t{FFFFFF}ID das marcas\n";
        for(new i = 0; i < MAX_PLAYER_MARKS; i++)
        {
            new markid = PlayerInfo[playerid][pMark][i];
            if (PlayerInfo[playerid][pMark][i] != -1)
            {
                format(list, sizeof(list), "%s%i\t%i\n", list, i + 1, markid);
            }
            else
            {
                format(list, sizeof(list), "%s%i\t{FF0000}Nenhuma\n", list, i + 1);
            }
        }
        Dialog_Show(playerid, None, DIALOG_STYLE_TABLIST_HEADERS, "{FFFFFF}Lista de marcas", list, "Fechar", "");
        return 1;
    }
    else if (CompareStrings(option, "criar"))
    {
        new m_mes[129];
        if (sscanf(string, "s[128]", m_mes))    return SendHelpMessage(playerid, "/mark criar [texto]");
        if (strlen(m_mes) > 128)    return SendErrorMessage(playerid, "Texto da etiqueta até 128 caracteres.");

        new markid = Mark_GetPlayerFreeSlot(playerid);

        if (markid == -1)   return SendErrorMessage(playerid, "Você não pode criar mais de %i marcas.", MAX_PLAYER_MARKS);
        if (Mark_GetFreeID() == -1) return SendErrorMessage(playerid, "Limite de etiquetas excedido. Tente criar uma nova etiqueta após um tempo.");
        if (Mark_Nearest(playerid) != -1)   return SendErrorMessage(playerid, "Já existe uma marca próxima a você.");

        new mark = Mark_GetFreeID();

        PlayerInfo[playerid][pMark][markid] = mark;
        CreateDynamicMark(playerid, mark, m_mes);
        SendSyntaxMessage(playerid, "Marca [ID: %i | SLOT: %i] foi criada com sucesso.", mark, markid + 1);
        return 1;
    }
    else if (CompareStrings(option, "destruir"))
    {
        new markid = Mark_Nearest(playerid);
        
        if (markid == -1)   return SendErrorMessage(playerid, "Não há marcas próximas a você.");
        if (!CompareStrings(E_MARKS[markid][mCreator], GetNameEx(playerid)))    return SendErrorMessage(playerid, "Esta etiqueta não pertence a você.");

        DestroyDynamicMark(markid);
        SendSyntaxMessage(playerid, "Você removeu sua marca [ID: %i].", markid);
        return 1;
    }
    else if (CompareStrings(option, "editar"))
    {
        new m_mes[129];
        if (sscanf(string, "s[128]", m_mes))    return SendHelpMessage(playerid, "/mark editar [novo texto]");
        if (strlen(m_mes) > 128)    return SendErrorMessage(playerid, "Texto da etiqueta até 128 caracteres.");

        new markid = Mark_Nearest(playerid);

        if (markid == -1)   return SendErrorMessage(playerid, "Não há marcas próximas a você.");
        if (!CompareStrings(E_MARKS[markid][mCreator], GetNameEx(playerid)))    return SendErrorMessage(playerid,"Esta etiqueta não pertence a você.");

        format(E_MARKS[markid][mText], 128, "%s\n(( %s ))", m_mes, GetNameEx(playerid));
        SQL_SetString("marcas", "texto", E_MARKS[markid][mText], markid);
        UpdateDynamic3DTextLabelText(E_MARKS[markid][mLabel], 0xFFFFFFFF, E_MARKS[markid][mText]);
        SendSyntaxMessage(playerid, "Você definiu um novo texto para sua marca [ID: %i].", markid);
    }
    else 
    {
        SendHelpMessage(playerid, "/mark [opções]");
        SendSyntaxMessage(playerid, "listar criar destruir editar");
    }
    return 1;
}
