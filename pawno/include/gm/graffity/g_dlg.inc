Dialog:Graffity_CE(playerid, response, listitem, inputtext[])
{
    if (!response) return Dialog_Show(playerid, Graffity_CE_Cancel, DIALOG_STYLE_MSGBOX, "{FFFFFF}Cancelar a criação de grafite", "{FFFFFF}Tem certeza de que deseja cancelar a criação do grafite?", "Sim", "Não");

    new grafid = Graffity_GetPlayerEdit(playerid);
    switch(listitem)
    {
        case 0:
        {
            SetPVarInt(playerid, "Graffity:Showing", 1);
            SetPVarInt(playerid, "Graffity:Show", 8);
            SendSyntaxMessage(playerid, "A janela de edição aparecerá novamente após 8 segundos.");
            SendSyntaxMessage(playerid, "Permaneça perto do grafite.");
        }
         case 1:
        {
            Dialog_Show(playerid, Graffity_CE_Done, DIALOG_STYLE_MSGBOX, "{FFFFFF}Criação de grafite", "{FFFFFF}Você tem certeza que quer pintar o grafite com esses parâmetros?", "Sim", "Não"); // yes no
        }
        case 2:
        {
            EditDynamicObject(playerid, E_GRAFFITY[grafid][gObject]);
            SetPVarInt(playerid, "Graffity:EditPos", 1);
            SendSyntaxMessage(playerid, "Você pode usar {FF6347}ESPAÇO {FFFFFF}para mover a câmera ao redor. ");
        }
        case 3:
        {
            new graf_string[256];
            format(graf_string, sizeof(graf_string), "{FFFFFF}Texto atual: %s\n\n* O texto deve conter pelo menos %i e no máximo %i caracteres.\n\nInsira o novo texto na caixa abaixo:", E_GRAFFITY[grafid][gText], MIN_GRAFFITY_TEXT, MAX_GRAFFITY_TEXT);
            Dialog_Show(playerid, Graffity_CE_Text, DIALOG_STYLE_INPUT, "{FFFFFF}Grafite - alterar texto", graf_string, ">>>", "Voltar");
        }
        case 4:
        {
            Graffity_ShowPlayerEditDialog(playerid);
        }
        case 5:
        {
            new graf_string[196];
            graf_string[0] = EOS;

            for(new i = 0; i < MAX_GRAFFITY_COLORS; i++)
            {
                format(graf_string, sizeof(graf_string), "%s%s%s\n", graf_string, GRAFFITY_COLOR[i][color_dlg], GRAFFITY_COLOR[i][color_name]);
            }
            Dialog_Show(playerid, Graffity_CE_Color, DIALOG_STYLE_LIST, "{FFFFFF}Grafite - escolha de cor", graf_string, "Selecionar", "Voltar");
        }
        case 6:
        {
            new graf_string[256];
            graf_string[0] = EOS;

            for(new i = 0; i < MAX_GRAFFITY_BACKS; i++)
            {
                format(graf_string, sizeof(graf_string), "%s%s%s\n", graf_string, GRAFFITY_BACK[i][color_dlg], GRAFFITY_BACK[i][color_name]);
            }
            Dialog_Show(playerid, Graffity_CE_BackColor, DIALOG_STYLE_LIST, "{FFFFFF}Grafite - seleção de fundo", graf_string, "Selecionar", "Voltar");
        }
        case 7:
        {
            new graf_string[256];
            graf_string[0] = EOS;

            for(new i = 0; i < MAX_GRAFFITY_FONTS; i++)
            {
                format(graf_string, sizeof(graf_string), "%s%s\n", graf_string, GRAFFITY_FONT[i][font_name]);
            }
            Dialog_Show(playerid, Graffity_CE_Font, DIALOG_STYLE_LIST, "{FFFFFF}Grafite - seleção de fonte", graf_string, "Selecionar", "Voltar");
        }
        case 8:
        {
            new graf_string[196];
            format(graf_string, sizeof(graf_string), "{FFFFFF}Tamanho atual: %i\n\n* Tamanho da fonte de %i até %i.\n\nInsira o novo valor na caixa abaixo:", E_GRAFFITY[grafid][gFontSize], MIN_GRAFFITY_FONTSIZE, MAX_GRAFFITY_FONTSIZE);
            Dialog_Show(playerid, Graffity_CE_FontSize, DIALOG_STYLE_INPUT, "{FFFFFF}Grafite - tamanho da fonte", graf_string, "Selecionar", "Voltar");
        }
        case 9:
        {
            Graffity_SetBold(grafid, !E_GRAFFITY[grafid][gBold]);
            Graffity_ShowPlayerEditDialog(playerid);
        }
    }
    return 1;
}

Dialog:Graffity_CE_Cancel(playerid, response, listitem, inputtext[])
{
    new grafid = Graffity_GetPlayerEdit(playerid);
    if (!response)   return Graffity_ShowPlayerEditDialog(playerid);

    Graffity_DestroyTemporary(grafid);
    Graffity_ResetPlayer(playerid);
    SendSyntaxMessage(playerid, "A criação do grafite foi cancelada.");
    return 1;
}

Dialog:Graffity_CE_Done(playerid, response, listitem, inputtext[])
{
    new grafid = Graffity_GetPlayerEdit(playerid);

    if (!response) return Graffity_ShowPlayerEditDialog(playerid);
    Graffity_DestroyTemporary(playerid);
    SetPVarInt(playerid, "Graffity:Spraying", 1);
    FreezePlayer(playerid);
    ApplyAnimation(playerid,"SPRAYCAN","spraycan_full",4.0,1,0,0,0,0);
    SetPVarInt(playerid, "Graffity:SprayingTime", strlen(E_GRAFFITY[grafid][gText]));
    SendSyntaxMessage(playerid, "Para parar de desenhar o grafite, digite /gr parar.");
    return 1;
}

Dialog:Graffity_CE_Text(playerid, response, listitem, inputtext[])
{
    new grafid = Graffity_GetPlayerEdit(playerid);
    if (response)
    {
        if (strlen(inputtext) > MAX_GRAFFITY_TEXT || strlen(inputtext) < MIN_GRAFFITY_TEXT)
        {
            SendErrorMessage(playerid, "Texto do grafite com no mínimo %i e no máximo %i caracteres.", MIN_GRAFFITY_TEXT, MAX_GRAFFITY_TEXT);
            
            new graf_string[256];
            
            format(graf_string, sizeof(graf_string), "{FFFFFF}Texto atual: %s\n\n* O texto deve conter no mínimo %i e no máximo %i caracteres.\n\nInsira o novo texto na caixa abaixo:", E_GRAFFITY[grafid][gText], MIN_GRAFFITY_TEXT, MAX_GRAFFITY_TEXT);
            Dialog_Show(playerid, Graffity_CE_Text, DIALOG_STYLE_INPUT, "{FFFFFF}Grafite - alterar texto", graf_string, ">>>", "Voltar");
            return 1;
        }

        Graffity_SetText(grafid, inputtext);
        Graffity_ShowPlayerEditDialog(playerid);
    }
    else 
    {
        Graffity_ShowPlayerEditDialog(playerid);
    }
    return 1;
}

Dialog:Graffity_CE_Color(playerid, response, listitem, inputtext[])
{
    new grafid = Graffity_GetPlayerEdit(playerid);
    if (response)
    {
        Graffity_SetColor(grafid, GRAFFITY_COLOR[listitem][color_data]);
        SetPVarInt(playerid, "Graffity:Color", listitem+1);
        Graffity_ShowPlayerEditDialog(playerid);
    }
    else 
    {
        Graffity_ShowPlayerEditDialog(playerid);
    }
    return 1;
}

Dialog:Graffity_CE_BackColor(playerid, response, listitem, inputtext[])
{
    new grafid = Graffity_GetPlayerEdit(playerid);
    if (response)
    {
        Graffity_SetBackColor(grafid, GRAFFITY_BACK[listitem][color_data]);
        SetPVarInt(playerid, "Graffity:Back", listitem+1);
        Graffity_ShowPlayerEditDialog(playerid);
    }
    else 
    {
        Graffity_ShowPlayerEditDialog(playerid);
    }
    return 1;
}

Dialog:Graffity_CE_Font(playerid, response, listitem, inputtext[])
{
    new grafid = Graffity_GetPlayerEdit(playerid);
    if (response)
    { 
        Graffity_SetFont(grafid, GRAFFITY_FONT[listitem][font_name]);
        SetPVarInt(playerid, "Graffity:Font", listitem+1);
        Graffity_ShowPlayerEditDialog(playerid);
    }
    else 
    {
        Graffity_ShowPlayerEditDialog(playerid);
    }
    return 1;
}

Dialog:Graffity_CE_FontSize(playerid, response, listitem, inputtext[])
{
    new grafid = Graffity_GetPlayerEdit(playerid), fsz = strval(inputtext);
    if (response)
    {
        if (fsz > MAX_GRAFFITY_FONTSIZE || fsz < MIN_GRAFFITY_FONTSIZE)
        {
            SendErrorMessage(playerid, "Tamanho da fonte de %i até %i.", MIN_GRAFFITY_FONTSIZE, MAX_GRAFFITY_FONTSIZE);

            new graf_string[196];

            format(graf_string, sizeof(graf_string), "{FFFFFF}Tamanho atual: %i\n\n* Tamanho da fonte de %i até %i.\n\nInsira o novo valor na caixa abaixo:", E_GRAFFITY[grafid][gFontSize], MIN_GRAFFITY_FONTSIZE, MAX_GRAFFITY_FONTSIZE);
            Dialog_Show(playerid, Graffity_CE_FontSize, DIALOG_STYLE_INPUT, "{FFFFFF}Grafite - tamanho da fonte", graf_string, "Selecionar", "Voltar");
            return 1;
        }

        Graffity_SetFontSize(grafid, fsz);
        Graffity_ShowPlayerEditDialog(playerid);
    }
    else 
    {
        Graffity_ShowPlayerEditDialog(playerid);
    }
    return 1;
}