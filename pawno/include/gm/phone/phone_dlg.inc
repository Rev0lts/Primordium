Dialog:SMSMenu(playerid, response, listitem, inputtext[])
{
	if (!response)
	{
		DeletePVar(playerid, #sms_number);
		DeletePVar(playerid, #select_sms);
	    return 1;
	}
	new mes[MAX_PHONE_STR],
		count = 0,
		msg[256];

	GetPVarString(playerid, #sms_number, mes, MAX_PHONE_STR);
			
	for(new i = 0; i < GetMaxPlayerContacts(playerid); i++)
	{
		if (strlen(PlayerContacts[playerid][i][contactName]) > 0)
		{
			if (CompareStrings(PlayerContacts[playerid][i][contactName], mes)) 
			{
				count = 1;
				break;
			}
		}
	}

	if (!count)
		format(msg, sizeof(msg), "Responder\nLigar\nEncaminhar\nSalvar número\nAdicionar ao arquivo\nApagar mensagem\nApagar {ff0303}TODAS AS MENSAGENS {FFFFFF}do número");
	else
		format(msg, sizeof(msg), "Responder\nLigar\nEncaminhar\nVer contato\nAdicionar ao arquivo\nApagar mensagem\nApagar {ff0303}TODAS AS MENSAGENS {FFFFFF}do contato");
		
	Dialog_Show(playerid, SMSMenu2, DIALOG_STYLE_LIST, "Opções", msg, "Selecionar", "Cancelar");
	return 1;
}

Dialog:SMSMenu2(playerid, response, listitem, inputtext[])
{
	if (!response)
	    return 1;
	
	switch(listitem)
	{
		case 0: // Responder
		{
			new mes[MAX_PHONE_STR+62];
			GetPVarString(playerid, #sms_number, mes, MAX_PHONE_STR);

			format(mes, sizeof(mes), "Mensagem >>> {33AA33}%s{a9c4e4}\n\n\tDigite sua mensagem:", mes);
			Dialog_Show(playerid, SendSMS2, DIALOG_STYLE_INPUT, "Enviar Mensagem", mes, "Enviar", "Cancelar");
			
			DeletePVar(playerid, #select_sms);
			return 1;
		}
		case 1: // Ligar
		{
			new mes[MAX_PHONE_STR];
			GetPVarString(playerid, #sms_number, mes, MAX_PHONE_STR);

			format(mes, sizeof(mes), "%s", mes);
			cmd::call(playerid, mes);

			DeletePVar(playerid, #sms_number);
			DeletePVar(playerid, #select_sms);
			return 1;
		}
		case 2: // Encaminhar
		{
			Dialog_Show(playerid, ForwardSMS, DIALOG_STYLE_INPUT, "Encaminhar mensagem", "Digite o número/nome do contato:","Enviar", "Cancelar");
			return 1;
		}
		case 3: // Salvar número || Ver contato
		{
			new msg[MAX_PHONE_STR], count = 0;
			GetPVarString(playerid, #sms_number, msg, MAX_PHONE_STR);
			
			for(new i = 0; i < GetMaxPlayerContacts(playerid); i++)
			{
				if (strlen(PlayerContacts[playerid][i][contactName]) > 0)
				{
					if (CompareStrings(PlayerContacts[playerid][i][contactName], msg)) 
					{
						count = 1;
						break;
					}
				}
			}

			if (CompareStrings(msg, "Red County Bank"))	return SendCellMessage(playerid,"Este número não pode ser salvo!");
			else if (CompareStrings(msg, "Sheriff's Departament")) return SendCellMessage(playerid, "Este número não pode ser salvo! ");
			//else if (CompareStrings(msg, "")) 

			if (!count)
			{
				SetPVarInt(playerid, #contact_number, strval(msg));
				
				new mes[128];
				format(mes, sizeof(mes), "Celular: {33AA33}%i{a9c4e4}\n\n\tDigite o nome do contato:", GetPVarInt(playerid, #contact_number));
				Dialog_Show(playerid, AddContact2, DIALOG_STYLE_INPUT, "Adicionar contato", mes, ">>>", "Cancelar");

				DeletePVar(playerid, #sms_number);
				DeletePVar(playerid, #select_sms);
			}
			else 
			{
				ph_showpage(playerid, PAGE_CONTACT_INFO);
				DeletePVar(playerid, #sms_number);
			}
			return 1;
		}
		case 4: ConfirmDialog(playerid, "Confirmação", "Tem certeza de que deseja arquivar esta mensagem?", "PhoneArchAdd");
		case 5: ConfirmDialog(playerid, "Confirmação", "Tem certeza de que deseja deletar esta mensagem?", "DeleteSMS");
		case 6: ConfirmDialog(playerid, "Confirmação", "{ffffff}Tem certeza de que deseja deletar {ff0303}TODAS AS MENSAGENS {ffffff} do contato/números?", "DeleteAllSMS");
	}
	return 1;
}

Dialog:ForwardSMS(playerid, response, listitem, inputtext[])
{
	if (!response)
	{
		new mes[MAX_PHONE_STR],
			count = 0,
			msg[256];
			
		GetPVarString(playerid, #sms_number, mes, MAX_PHONE_STR);
		
		for(new i = 0; i < GetMaxPlayerContacts(playerid); i++)
		{
			if (strlen(PlayerContacts[playerid][i][contactName]) > 0)
			{
				if (CompareStrings(PlayerContacts[playerid][i][contactName], mes)) 
				{
					count = 1;
					break;
				}
			}
		}

		if (!count)
			format(msg, sizeof(msg), "Responder\nLigar\nEncaminhar\nSalvar número\nAdicionar ao arquivo\nApagar mensagem\nApagar {ff0303}TODAS AS MENSAGENS {FFFFFF}do número");
		else
			format(msg, sizeof(msg), "Responder\nLigar\nEncaminhar\nVer contato\nAdicionar ao arquivo\nApagar mensagem\nApagar {ff0303}TODAS AS MENSAGENS {FFFFFF}do contato");
			
		Dialog_Show(playerid, SMSMenu2, DIALOG_STYLE_LIST, "Opções", msg, "Selecionar", "Cancelar");
		return 1;
	}

	if (strlen(inputtext) < 1 || strlen(inputtext) > 24)
 		return Dialog_Show(playerid, ForwardSMS, DIALOG_STYLE_INPUT, "Encaminhar mensagem", "Digite o número/nome do contato:","Enviar", "Cancelar");

	new number = strval(inputtext);
	if (!number)
	{
		for(new i = 0; i < GetMaxPlayerContacts(playerid); i++)
		{
			if (strlen(PlayerContacts[playerid][i][contactName]) > 0)
			{
				if (CompareStrings(PlayerContacts[playerid][i][contactName], inputtext)) 
				{
					number = PlayerContacts[playerid][i][contactNumber];
				}
			}
		}
	}
	
	new query[128], string[16];
	valstr(string, number);

	mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `sms` WHERE `id`=%i", GetPVarInt(playerid, #select_sms));
	mysql_tquery(dbHandle, query, "ForwardMessage", "is", playerid, string);

	DeletePVar(playerid, #sms_number);
	DeletePVar(playerid, #select_sms);

	return 1;
}

Dialog:SendSMS(playerid, response, listitem, inputtext[])
{
	if (!response)
	    return 1;

	if (strlen(inputtext) < 1 || strlen(inputtext) > 24)
 		return Dialog_Show(playerid, SendSMS, DIALOG_STYLE_INPUT, "Enviar Mensagem", "Nova mensagem\n\n\tDigite o número/nome do contato:", ">>>", "Cancelar");

    SetPVarString(playerid, #sms_number, inputtext);
    new mes[62+23];
	format(mes, sizeof(mes), "Mensagem >>> {33AA33}%s{a9c4e4}\n\n\tDigite sua mensagem:", inputtext);
	Dialog_Show(playerid, SendSMS2, DIALOG_STYLE_INPUT, "Enviar Mensagem", mes, "Enviar", "Cancelar");
	return 1;
}

Dialog:SendSMS2(playerid, response, listitem, inputtext[])
{
	if (!response)
	    return 1;

	if (strlen(inputtext) < 1 || strlen(inputtext) > 127)
	{
	    new mes[32];
	    GetPVarString(playerid, #sms_number, mes, 32);
	    Dialog_Show(playerid, SendSMS2, DIALOG_STYLE_INPUT, "Enviar Mensagem", "Digite sua mensagem:", "Enviar", "Cancelar");
	    return 1;
	}

	new mes[128 + 32],
	    name[32];

    GetPVarString(playerid, #sms_number, name, 32);

	format(mes, sizeof(mes), "%s %s", name, inputtext);
	cmd::sms(playerid, mes);
	return 1;
}

Dialog:SendSMS3(playerid, response, listitem, inputtext[])
{
	if (!response)
	    return 1;

	new contact = GetPVarInt(playerid, #select_contact);

	if (contact < 0 || contact >= MAX_PLAYER_CONTACTS || PlayerContacts[playerid][contact][contactOn] == 0)
	    return 1;

	if (strlen(inputtext) < 1 || strlen(inputtext) > 127)
	{
	    new mes[62+MAX_PHONE_STR];
	    format(mes, sizeof(mes), "Mensagem >>> {33AA33}%s{a9c4e4}\n\n\tDigite sua mensagem:", PlayerContacts[playerid][contact][contactName]);
	    Dialog_Show(playerid, SendSMS3, DIALOG_STYLE_INPUT, "Enviar Mensagem", mes, "Enviar", "Cancelar");
	    return 1;
	}

	new mes[128 + 12];
	format(mes, sizeof(mes), "%i %s", PlayerContacts[playerid][contact][contactNumber], inputtext);
	cmd::sms(playerid, mes);
	return 1;
}


Dialog:DeleteContact(playerid, response, listitem, inputtext[])
{
	if (!response)
	    return 1;

	new contact = GetPVarInt(playerid, #select_contact);

	if (contact < 0 || contact >= MAX_PLAYER_CONTACTS || PlayerContacts[playerid][contact][contactOn] == 0)
	    return 1;

	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `player_contacts` WHERE `id` = %i", PlayerContacts[playerid][contact][contactID]);
	mysql_tquery(dbHandle, query);

	mysql_format(dbHandle, query, sizeof(query), "UPDATE `sms` SET `from_name` = `from_number` WHERE `to_number` = %i AND `from_number`=%i", PlayerInfo[playerid][pNumber], PlayerContacts[playerid][contact][contactNumber]);
	mysql_tquery(dbHandle, query);

    mysql_format(dbHandle, query, sizeof(query), "UPDATE `history_calls` SET `from_name` = `from_number` WHERE `to_number` = %i AND `from_number`=%i", PlayerInfo[playerid][pNumber], PlayerContacts[playerid][contact][contactNumber]);
	mysql_tquery(dbHandle, query);

	SM2(playerid, "Contato {33AA33}%s (%i){FFFFFF} excluído com sucesso.", PlayerContacts[playerid][contact][contactName], PlayerContacts[playerid][contact][contactNumber]);

	
	PlayerContacts[playerid][contact][contactName] = EOS;
	PlayerContacts[playerid][contact][contactNumber] = 0;
	PlayerContacts[playerid][contact][contactOn] = 0;
	PlayerContacts[playerid][contact][contactID] = 0;

	PhoneInfo[playerid][current_page] = PAGE_MENU_CONTACTS;
	ph_showpage(playerid, PAGE_LIST_CONTACTS);

	DeletePVar(playerid, #select_contact);
	return 1;
}

Dialog:AddContact(playerid, response, listitem, inputtext[])
{
	if (!response)
	    return 1;

	new
		number = strval(inputtext);

	if (number < 1 || number > 99999999)
	    return SendErrorMessage(playerid, "Máximo de caracteres de 1 a 8.");

	for(new i = 0; i < GetMaxPlayerContacts(playerid); i++)
		{
			if (PlayerContacts[playerid][i][contactNumber] == number)
			{
				Dialog_Show(playerid, AddContact, DIALOG_STYLE_INPUT, "Adicionar contato", "Novo contato\n\n\tDigite o número do contato:", ">>>", "Cancelar");
				return SendErrorMessage(playerid, "Um contato com este número já existe: {33AA33}%s{FFFFFF}!", PlayerContacts[playerid][i][contactName]);
			}
		}

	SetPVarInt(playerid, #contact_number, number);

	new mes[128];
	format(mes, sizeof(mes), "Celular: {33AA33}%i{a9c4e4}\n\n\tDigite o nome do contato:", number);
	Dialog_Show(playerid, AddContact2, DIALOG_STYLE_INPUT, "Adicionar contato", mes, ">>>", "Cancelar");
	return 1;
}

Dialog:AddContact2(playerid, response, listitem, inputtext[])
{
	if (!response)
	    return 1;

	if (strlen(inputtext) < 1 || strlen(inputtext) > 16 || isnull(inputtext))
	{
	    new mes[128];
		format(mes, sizeof(mes), "Celular: {33AA33}%i{a9c4e4}\n\n\tDigite o nome do contato:", GetPVarInt(playerid, #contact_number));
		Dialog_Show(playerid, AddContact2, DIALOG_STYLE_INPUT, "Adicionar contato", mes, ">>>", "Cancelar");
		return SendErrorMessage(playerid, "Máximo de caracteres (1-16) no nome do contato, e o nome deve conter pelo menos uma letra.");
	}

	new contact = -1,
	    count;
	    
	for(new i = 0; i < GetMaxPlayerContacts(playerid); i++)
    {
		if (strlen(PlayerContacts[playerid][i][contactName]) > 0)
		{
			if (CompareStrings(PlayerContacts[playerid][i][contactName], inputtext)) 
			{
				new mes[128];
				format(mes, sizeof(mes), "Celular: {33AA33}%i{a9c4e4}\n\n\tDigite o nome do contato:", GetPVarInt(playerid, #contact_number));
				Dialog_Show(playerid, AddContact2, DIALOG_STYLE_INPUT, "Adicionar contato", mes, ">>>", "Cancelar");
				return SendErrorMessage(playerid, "Um contato com o mesmo nome já existe: {33AA33}%s (%d){FFFFFF}!", inputtext, PlayerContacts[playerid][i][contactNumber]);
			}
		}

		if (PlayerContacts[playerid][i][contactOn] == 0)
		{
			contact = i;
			break;
		}
		else count++;            
    }

	if (count == GetMaxPlayerContacts(playerid)-1 || contact == -1)
	    return SendErrorMessage(playerid, "A memória do Celular está cheia. ");

	PlayerContacts[playerid][contact][contactNumber] = GetPVarInt(playerid, #contact_number);
	format(PlayerContacts[playerid][contact][contactName], 32, "%s", inputtext);
	PlayerContacts[playerid][contact][contactOn] = 1;

	// As consultas ao banco de dados mantêm as variáveis e strings de consulta originais

	PlacePlayerContact(playerid, contact, inputtext, GetPVarInt(playerid, #contact_number));

	ph_showpage(playerid, PAGE_LIST_CONTACTS);
	return 1;
}


Dialog:DialogCall(playerid, response, listitem, inputtext[])
{
	if (!response)
	    return 1;

	cmd::call(playerid, inputtext);
	return 1;
}