CMD:saldo(playerid, params[])
{
	if (!Bank_Nearest(playerid)) return SendErrorMessage (playerid, "Voc� n�o est� em um banco.");

	SendClientMessageEx(playerid, COLOR_FADE1, "[Banco]: Na sua conta banc�ria: $%i.", PlayerInfo[playerid][pBank]);
    SendClientMessageEx(playerid, COLOR_FADE1, "[Banco]: Conta poupan�a: $%i. Contracheque: $%i", PlayerInfo[playerid][pSavings], PlayerInfo[playerid][pPayCheckMoney]);
	return 1;
}

CMD:transferir(playerid, params[])
{
	new userid, amount;

	if (!Bank_Nearest(playerid)) return SendErrorMessage(playerid, "Voc� n�o est� em um banco.");
	if (sscanf(params, "ud", userid, amount))
	{
		SendHelpMessage(playerid, "/transferir [ID/Parte do Nome] [dinheiro]");
		SendSyntaxMessage(playerid,"[Banco]: Voc� tem $%i na sua conta banc�ria.", PlayerInfo[playerid][pBank]);
		return 1;
	}

	if (userid == playerid)								return SendErrorMessage (playerid, "Voc� n�o pode transferir dinheiro para si mesmo.");
	if (!IsPlayerLogged(userid))						return SendNotConnected(playerid);
	if (GetBankCash(playerid) < amount || amount < 1)	return SendErrorMessage (playerid, "Voc� n�o tem essa quantia na sua conta.");

	BankTransfer(playerid, userid, amount);
	return 1;
}

CMD:depositar(playerid, params[])
{
	new amount;

	if (!Bank_Nearest(playerid)) return SendErrorMessage (playerid, "Voc� n�o est� em um banco.");

	if (sscanf(params, "d", amount))
	{
		SendHelpMessage(playerid, "/depositar [dinheiro]");
		SendSyntaxMessage(playerid, "[Banco]: Voc� tem $%i na conta banc�ria.", PlayerInfo[playerid][pBank]);
		return 1;
	}

    if (amount > GetCash(playerid) || amount < 1) return SendErrorMessage(playerid, "Voc� n�o tem essa quantia em m�os.");
		
	BankDeposit(playerid, amount);
	return 1;
}


CMD:sacar(playerid, params[])
{
	new amount;

	if (!Bank_Nearest(playerid)) return SendErrorMessage(playerid, "Voc� n�o est� em um banco.");

	if (sscanf(params, "d", amount))
	{
		SendHelpMessage(playerid, "/sacar [dinheiro]");
		SendClientMessageEx(playerid, COLOR_FADE1, "[Banco]: Voc� tem $%i em uma conta banc�ria.", PlayerInfo[playerid][pBank]);
		return 1;
	}

    if (amount > GetBankCash(playerid) || amount < 1) return SendErrorMessage(playerid, "Voc� n�o tem essa quantia em sua conta.");
    
	BankWithdraw(playerid, amount);
	return 1;
}

CMD:poupanca(playerid, params[])
{
	new amount;
	if (!Bank_Nearest(playerid)) return SendErrorMessage(playerid, "Voc� n�o est� em um banco.");

	if (PlayerInfo[playerid][pSavings] == 0)
	{
		if (sscanf(params, "d", amount))		return SendHelpMessage(playerid, "/poupanca [quantidade]");
		if (amount < 35000 || amount > 100000)	return SendErrorMessage(playerid, "Deposite uma quantia entre 35000 e 100000.");
		if (GetCash(playerid) < amount) 		return SendNotCash(playerid);

		SendSyntaxMessage(playerid, "Voc� depositou $%d em uma conta poupan�a com juros de 1%%.");
		GiveCash(playerid, -amount);
		PlayerInfo[playerid][pSavings] = amount;
		return 1;
	}
	else
	{
		SendSyntaxMessage(playerid, "Voc� retirou $%i de sua conta poupan�a.", PlayerInfo[playerid][pSavings]);
		GiveCash(playerid, PlayerInfo[playerid][pSavings]);
		PlayerInfo[playerid][pSavings] = 0;
	}
	return 1;
}


CMD:pagamento(playerid, params[])
{
	if (!Bank_Nearest(playerid))					return SendErrorMessage(playerid, "Voc� n�o est� em um banco.");
	if (PlayerInfo[playerid][pPayCheckMoney] < 1)	return SendErrorMessage(playerid, "N�o h� fundos dispon�veis para saque.");

	SendSyntaxMessage(playerid, "Voc� sacou $%i de seu cheque.", PlayerInfo[playerid][pPayCheckMoney]);
	GiveCash(playerid, PlayerInfo[playerid][pPayCheckMoney]);
	PlayerInfo[playerid][pPayCheckMoney] = 0;
	return 1;
}

CMD:atm(playerid, params[])
{
    if (ATM_Nearest(playerid) == -1) return 1;
	
	Dialog_Show(playerid, ATMMenu, DIALOG_STYLE_LIST, "{FFFFFF}Menu", "- Saldo\n- Sacar dinheiro\n- Depositar dinheiro\n- Transferir dinheiro", "Selecionar", "Cancelar");
	return 1;
}