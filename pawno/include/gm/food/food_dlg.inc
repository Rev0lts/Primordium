Dialog:BuyStandFood(playerid, response, listitem, inputtext[]) //t�nt�
{
	if (!response)	return 1;

	new price_food[4] = {PRICE_BURGER, PRICE_PIZZA, PRICE_CHICKEN, PRICE_HOTDOG}, name_food[4][16] = {"hamb�rguer", "pizza", "frango", "cachorro-quente"};

	if (GetCash(playerid) < price_food[listitem])				return SendNotCash(playerid);
	if (GiveInvItem(playerid, INV_BURGER + listitem, 1) == -1)	return SendErrorMessage(playerid, "O invent�rio est� cheio.");

	SendSyntaxMessage(playerid, "Voc� comprou um(a) %s por $%i.", name_food[listitem], price_food[listitem]);
	GiveCash(playerid, -price_food[listitem]);
	return 1;
}