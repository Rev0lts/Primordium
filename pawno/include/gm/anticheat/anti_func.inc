// Fun��o para fazer o upload das configura��es anti-cheat
stock UploadAntiCheatSettings()  
{  
    mysql_tquery(dbHandle, "SELECT * FROM `anticheat_settings`", "UploadAntiCheat");
}

this::UploadAntiCheat()
{
    new rows = cache_num_rows(), tick = GetTickCount();

    if (!rows) {  
        print("[MySQL]: As configura��es anti-cheat n�o foram encontradas no banco de dados. A carga do mod foi interrompida - configure o anti-cheat. ");
        return GameModeExit();  
    }  

    for(new i = 0; i < AC_MAX_CODES; i++)
    {
        AC_CODE_TRIGGER_TYPE[i] = cache_get_field_content_int(i, "ac_code_trigger_type");

        if (AC_CODE_TRIGGER_TYPE[i] == AC_CODE_TRIGGER_TYPE_DISABLED) {
            EnableAntiCheat(i, 0);
        }
    }

    new mes[128];
    format(mes, sizeof(mes), "[AntiCheat] Configura��es anti-cheat carregadas com sucesso (carregadas: %i). Tempo: %i milissegundos.", rows, GetTickCount() - tick);  
    print(mes);

    return 1;  
}

stock Anticheat_OnPlayerConnect(playerid)
{
    /*
        Em vez disso, inicializamos o valor -1 para cada elemento do array para registrar o �ltimo tempo de ativa��o de um c�digo anti-cheat espec�fico.
    */
    for(new i = 0; i < AC_MAX_CODES; i++)
        pAntiCheatLastCodeTriggerTime[playerid][i] = -1;

    pAntiCheatSettingsPage{playerid} = 0; // Inicializamos o valor 0 para a vari�vel que controla a p�gina atual das configura��es anti-cheat
    pAntiCheatSettingsEditCodeId[playerid] = -1; // Inicializamos o valor -1 para o ID do c�digo anti-cheat que est� sendo editado
    return 1;
} 

this::OnCheatDetected(playerid, ip_address[], type, code)
{
    if (type == AC_GLOBAL_TRIGGER_TYPE_PLAYER)
    {
        switch(code)
        {
            case 5, 6, 11, 22: 
            {
                return 1;
            }
            case 32: // Roubo de carro
            {
                new 
                    Float:x, 
                    Float:y, 
                    Float:z;

                AntiCheatGetPos(playerid, x, y, z);
                return SetPlayerPos(playerid, x, y, z);
            }
            case 40: // Modo de jogo n�o autorizado
            {
                SendClientMessage(playerid, -1, MAX_CONNECTS_MSG);
                return AntiCheatKickWithDesync(playerid, code);
            }
            case 41: // Vers�o inv�lida do cliente
            {
                SendClientMessage(playerid, -1, UNKNOWN_CLIENT_MSG);
                return AntiCheatKickWithDesync(playerid, code);
            }
            default:
            {
                if (gettime() - pAntiCheatLastCodeTriggerTime[playerid][code] < AC_TRIGGER_ANTIFLOOD_TIME)
                    return 1;

                pAntiCheatLastCodeTriggerTime[playerid][code] = gettime();
                AC_CODE_TRIGGERED_COUNT[code]++;

                new
                    string[88 - 10 + MAX_PLAYER_NAME + 5 + AC_MAX_CODE_NAME_LENGTH + AC_MAX_CODE_LENGTH],
                    trigger_type = AC_CODE_TRIGGER_TYPE[code];

                switch(trigger_type)
                {
                    case AC_CODE_TRIGGER_TYPE_DISABLED: return 1;
                    case AC_CODE_TRIGGER_TYPE_WARNING:  SendAdmMessage("%s[%d] suspeito de usar programas de trapa�a: %s [c�digo: %03d].", GetNameEx(playerid), playerid, AC_CODE_NAME[code], code);
                    case AC_CODE_TRIGGER_TYPE_KICK: {
                        SendAdmMessage("%s[%d] foi expulso sob suspeita de usar programas de trapa�a: %s [c�digo: %03d].", GetNameEx(playerid), playerid, AC_CODE_NAME[code], code);

                        format(string, sizeof(string), "Voc� foi expulso sob suspeita de usar programas de trapa�a: %s [c�digo: %03d].", AC_CODE_NAME[code], code);
                        SendClientMessage(playerid, -1, string);

                        AntiCheatKickWithDesync(playerid, code);
                    }
                } 
            }
        }
    }
    else // AC_GLOBAL_TRIGGER_TYPE_IP
    {
        AC_CODE_TRIGGERED_COUNT[code]++;

        SendAdmMessage("<AC-BAN-IP> Endere�o IP %s foi bloqueado: %s [c�digo: %03d].", ip_address, AC_CODE_NAME[code], code);

        BlockIpAddress(ip_address, 0);
    }
    return 1;
}