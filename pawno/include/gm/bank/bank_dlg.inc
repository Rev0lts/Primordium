Dialog:ATMMenu(playerid, response, listitem, inputtext[])
{
    if (!response) return 1;

    switch(listitem)
    {
        case 0:
        {
            static const str_size[] = "Conta bancária: {"COL_GREEN"}$%i.\nConta poupança: {"COL_GREEN"}$%i\nCheque: {"COL_GREEN"}$%i";
            new mes[sizeof(str_size) - 4 + 24];
            
            format(mes, sizeof(mes), str_size, PlayerInfo[playerid][pBank], PlayerInfo[playerid][pSavings], PlayerInfo[playerid][pPayCheckMoney]);
            Dialog_Show(playerid, ATMBack, DIALOG_STYLE_LIST, "{FFFFFF}Saldo", mes, "Voltar", "");
        }
        case 1: Dialog_Show(playerid, ATMWithdraw, DIALOG_STYLE_INPUT, "{FFFFFF}Sacar dinheiro", "{FFFFFF}Digite o valor que deseja sacar:", "Sacar", "Voltar");
        case 2: Dialog_Show(playerid, ATMDeposit, DIALOG_STYLE_INPUT, "{FFFFFF}Depositar dinheiro", "{FFFFFF}Digite o valor que deseja depositar:", "Depositar", "Voltar");
        case 3: Dialog_Show(playerid, ATMTransfer, DIALOG_STYLE_INPUT, "{FFFFFF}Transferência de dinheiro", "{FFFFFF}Digite o Nome / ID do destinatário abaixo:", ">>>", "Voltar");
    }
    return 1;
}


Dialog:ATMBack(playerid, response, listitem, inputtext[])
{
	if (response)	cmd::atm(playerid, "");
	
	return 1;
}

Dialog:ATMTransfer(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::atm(playerid, "");

	new userid;

	sscanf(inputtext, "u", userid);

	if (!IsPlayerLogged(userid) || userid == playerid)
	{
	    Dialog_Show(playerid, ATMTransfer, DIALOG_STYLE_INPUT, "{FFFFFF}Transferência de dinheiro", "{FFFFFF}Digite o Nome / ID do destinatário abaixo:", ">>>", "Voltar");
	    return SendErrorMessage(playerid, "Destinatário não encontrado.");
	}

	static const str_size[] = "{FFFFFF}Transferência de dinheiro - {FFFF00}%s";
	new mes[sizeof(str_size) - 2 + MAX_PLAYER_NAME + 1];

	format(mes, sizeof(mes), str_size, GetNameEx(userid));
	SetPVarInt(playerid, #transfer_player, userid);
	Dialog_Show(playerid, ATMTransfer2, DIALOG_STYLE_INPUT, mes, "{FFFFFF}Digite o valor que deseja transferir:", "Transferir", "Cancelar");
	return 1;
}


Dialog:ATMTransfer2(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::atm(playerid, "");

	new userid = GetPVarInt(playerid, #transfer_player);
	new amount = strval(inputtext);

	DeletePVar(playerid, #transfer_player);

	if (!IsPlayerLogged(userid) || userid == playerid)
	{
	    Dialog_Show(playerid, ATMTransfer, DIALOG_STYLE_INPUT, "{FFFFFF}Transferência de dinheiro", "{FFFFFF}Digite o Nome / ID do destinatário abaixo:", ">>>", "Voltar");
	    return SendErrorMessage(playerid, "Destinatário não encontrado.");
	}
	if (GetBankCash(playerid) < amount || amount < 1)
	{
	    static const str_size[] = "{FFFFFF}Transferência de dinheiro - {FFFF00}%s";
		new mes[sizeof(str_size) - 2 + MAX_PLAYER_NAME + 1];

		format(mes, sizeof(mes), str_size, GetNameEx(userid));
		SetPVarInt(playerid, #transfer_player, userid);
		Dialog_Show(playerid, ATMTransfer2, DIALOG_STYLE_INPUT, mes, "{FFFFFF}Digite o valor que deseja transferir:", "Transferir", "Cancelar");
 		return SendErrorMessage(playerid, "Você não tem essa quantia em sua conta bancária.");
	}
	
	BankTransfer(playerid, userid, amount);
	cmd::atm(playerid, "");
	return 1;
}


Dialog:ATMWithdraw(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::atm(playerid, "");
	    
	new amount = strval(inputtext);

    if (amount > GetBankCash(playerid) || amount < 1)
	{
        Dialog_Show(playerid, ATMWithdraw, DIALOG_STYLE_INPUT, "{FFFFFF}Sacar dinheiro", "{FFFFFF}Digite o valor que deseja sacar:", "Sacar", "Voltar");
		return SendErrorMessage(playerid, "Você não tem essa quantia em sua conta bancária.");
	}
	
	BankWithdraw(playerid, amount);
	cmd::atm(playerid, "");
	return 1;
}


Dialog:ATMDeposit(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::atm(playerid, "");

	new amount = strval(inputtext);

    if (amount > GetCash(playerid) || amount < 1)
	{
        Dialog_Show(playerid, ATMDeposit, DIALOG_STYLE_INPUT, "{FFFFFFF}Depositar dinheiro", "{FFFFFF}Digite o valor que deseja depositar:", "Depositar", "Voltar");
		return SendErrorMessage(playerid, "Você não tem essa quantia em mãos.");
	}

	BankDeposit(playerid, amount);
	cmd::atm(playerid, "");
	return 1;
}
