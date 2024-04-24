CMD:anticheat_settings(playerid, params[])
{
    if (!CheckAdmin(playerid, DEVELOPER_LEVEL)) return SendNotAdmin(playerid);
    
    pAntiCheatSettingsPage{playerid} = 1; // Define a vari�vel que armazena o n�mero da p�gina em que o jogador est� com o valor 1 (ou seja, agora o jogador est� na p�gina 1)
    return ShowPlayer_AntiCheatSettings(playerid); // Mostrar ao jogador a caixa de di�logo principal das configura��es anti-cheat
}

// Fun��o para mostrar o menu principal de configura��es anti-cheat
stock ShowPlayer_AntiCheatSettings(playerid)
{
    static
        dialog_string[42 + 19 - 8 + (AC_MAX_CODE_LENGTH + AC_MAX_CODE_NAME_LENGTH + AC_MAX_TRIGGER_TYPE_NAME_LENGTH + 10)*AC_MAX_CODES_ON_PAGE] = EOS;

    new
        triggeredCount = 0,
        page = pAntiCheatSettingsPage{playerid},
        next = 0,
        index = 0;

    dialog_string = "Nome\tPuni��o\tN�mero de positivos\n";

    for(new i = 0; i < AC_MAX_CODES; i++)
    {
        if (i >= (page * AC_MAX_CODES_ON_PAGE) && i < (page * AC_MAX_CODES_ON_PAGE) + AC_MAX_CODES_ON_PAGE)
            next++;

        if (i >= (page - 1) * AC_MAX_CODES_ON_PAGE && i < ((page - 1) * AC_MAX_CODES_ON_PAGE) + AC_MAX_CODES_ON_PAGE)
        {
            triggeredCount = AC_CODE_TRIGGERED_COUNT[i];

            format(dialog_string, sizeof(dialog_string), "%s[%s] %s\t%s\t%d\n", 
                dialog_string,
                AC_CODE[i], 
                AC_CODE_NAME[i],
                AC_TRIGGER_TYPE_NAME[AC_CODE_TRIGGER_TYPE[i]],
                triggeredCount);

            pAntiCheatSettingsMenuListData[playerid][index++] = i;
        }
    }

    if (next) 
        strcat(dialog_string, ""AC_DIALOG_NEXT_PAGE_TEXT"\n");

    if (page > 1) 
        strcat(dialog_string, AC_DIALOG_PREVIOUS_PAGE_TEXT);

    return Dialog_Show(playerid, ANTICHEAT_SETTINGS, DIALOG_STYLE_TABLIST_HEADERS, "Configura��es anti-cheat", dialog_string, "Selecionar", "Cancelar");
}

// Fun��o para mostrar o menu de edi��o do tipo de acionamento de um determinado c�digo no anti-cheat
stock ShowPlayer_AntiCheatEditCode(playerid, code)
{
    new
        dialog_header[22 - 4 + AC_MAX_CODE_LENGTH + AC_MAX_CODE_NAME_LENGTH],
        dialog_string[AC_MAX_TRIGGER_TYPE_NAME_LENGTH*AC_MAX_TRIGGER_TYPES];

    format(dialog_header, sizeof(dialog_header), "C�digo: %s | Nome: %s", AC_CODE[code], AC_CODE_NAME[code]); // Cabe�alho

    for(new i = 0; i < AC_MAX_TRIGGER_TYPES; i++)
    {
        strcat(dialog_string, AC_TRIGGER_TYPE_NAME[i]);

        if (i + 1 != AC_MAX_TRIGGER_TYPES)
            strcat(dialog_string, "\n");
    }

    return Dialog_Show(playerid, ANTICHEAT_EDIT_CODE, DIALOG_STYLE_LIST, dialog_header, dialog_string, "Selecionar", "Retornar");
}