CMD:gps(playerid, params[])
{
	Dialog_Show(playerid, DIALOG_GPS, DIALOG_STYLE_LIST, "GPS > Início", "1. Locais principais\n2. Empresas\n3. Empregos", "Selecionar", "Cancelar");
	return 1;
}

ALTX:removecp("/rcp","/removercp","/removercheckpoint");
CMD:removecp(playerid, params[])
{
	SendSyntaxMessage(playerid, "Todos os checkpoints foram removidos do mapa.");
	DisableWaypoint(playerid);
	return 1;
}