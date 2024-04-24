/* Phone */
new 
    Text:PhNote[2][MAX_PLAYERS],

    Text:PhNetwork[MAX_PLAYERS],
    Text:PhTime[MAX_PLAYERS],
	
	Text:PhNewMSG[MAX_PLAYERS],
	Text:PhBtn[2][MAX_PLAYERS],
    Text:PhBtnStr[2][MAX_PLAYERS],
	Text:PhArrow[4][MAX_PLAYERS],
	Text:PhOFF[MAX_PLAYERS],
	Text:PhoneTDdisplay[2][MAX_PLAYERS],
    Text:PhNewMSGDisplay[2][MAX_PLAYERS],
    Text:PhoneTDbox[5][MAX_PLAYERS],
	Text:Ph_selfie[MAX_PLAYERS],
	Text:NewSMS[MAX_PLAYERS],

	Text:PhDisplay[MAX_PLAYERS],
	Text:CallBox[MAX_PLAYERS],

	PlayerText:PhoneMenu[4][MAX_PLAYERS],
	Text:PhoneMenuBg[4][MAX_PLAYERS]
;	


LoadPhoneTextDraw(playerid)
{
    // ================TEXTDRAWS================//

	PhoneMenu[0][playerid] = CreatePlayerTextDraw(playerid, 455.683410, 331.736022, "Contatos");
	PlayerTextDrawLetterSize(playerid, PhoneMenu[0][playerid], 0.197916, 0.868889);
	PlayerTextDrawTextSize(playerid, PhoneMenu[0][playerid], 534.850036, 0.000000);
	PlayerTextDrawAlignment(playerid, PhoneMenu[0][playerid], 1);
	PlayerTextDrawColor(playerid, PhoneMenu[0][playerid], 255);
	PlayerTextDrawSetShadow(playerid, PhoneMenu[0][playerid], 0);
	PlayerTextDrawSetOutline(playerid, PhoneMenu[0][playerid], 0);
	PlayerTextDrawBackgroundColor(playerid, PhoneMenu[0][playerid], -1378294017);
	PlayerTextDrawFont(playerid, PhoneMenu[0][playerid], 1);
	PlayerTextDrawSetProportional(playerid, PhoneMenu[0][playerid], 1);

	PhoneMenu[1][playerid] = CreatePlayerTextDraw(playerid, 455.683410, 344.132995, "Chamadas");
	PlayerTextDrawLetterSize(playerid, PhoneMenu[1][playerid], 0.197916, 0.868889);
	PlayerTextDrawTextSize(playerid, PhoneMenu[1][playerid], 534.850036, 0.000000);
	PlayerTextDrawAlignment(playerid, PhoneMenu[1][playerid], 1);
	PlayerTextDrawColor(playerid, PhoneMenu[1][playerid], 255);
	PlayerTextDrawSetShadow(playerid, PhoneMenu[1][playerid], 0);
	PlayerTextDrawSetOutline(playerid, PhoneMenu[1][playerid], 0);
	PlayerTextDrawBackgroundColor(playerid, PhoneMenu[1][playerid], -1378294017);
	PlayerTextDrawFont(playerid, PhoneMenu[1][playerid], 1);
	PlayerTextDrawSetProportional(playerid, PhoneMenu[1][playerid], 1);

	PhoneMenu[2][playerid] = CreatePlayerTextDraw(playerid, 455.683410, 356.629943, "Mensagens	");
	PlayerTextDrawLetterSize(playerid, PhoneMenu[2][playerid], 0.197916, 0.868889);
	PlayerTextDrawTextSize(playerid, PhoneMenu[2][playerid], 534.850036, 0.000000);
	PlayerTextDrawAlignment(playerid, PhoneMenu[2][playerid], 1);
	PlayerTextDrawColor(playerid, PhoneMenu[2][playerid], 255);
	PlayerTextDrawSetShadow(playerid, PhoneMenu[2][playerid], 0);
	PlayerTextDrawSetOutline(playerid, PhoneMenu[2][playerid], 0);
	PlayerTextDrawBackgroundColor(playerid, PhoneMenu[2][playerid], -1378294017);
	PlayerTextDrawFont(playerid, PhoneMenu[2][playerid], 1);
	PlayerTextDrawSetProportional(playerid, PhoneMenu[2][playerid], 1);
	PlayerTextDrawSetShadow(playerid, PhoneMenu[2][playerid], 0);

	PhoneMenu[3][playerid] = CreatePlayerTextDraw(playerid, 455.683410, 368.976928, "Configuracoes");
	PlayerTextDrawLetterSize(playerid, PhoneMenu[3][playerid], 0.197916, 0.868889);
	PlayerTextDrawTextSize(playerid, PhoneMenu[3][playerid], 534.850036, 0.000000);
	PlayerTextDrawAlignment(playerid, PhoneMenu[3][playerid], 1);
	PlayerTextDrawColor(playerid, PhoneMenu[3][playerid], 255);
	PlayerTextDrawSetShadow(playerid, PhoneMenu[3][playerid], 0);
	PlayerTextDrawSetOutline(playerid, PhoneMenu[3][playerid], 0);
	PlayerTextDrawBackgroundColor(playerid, PhoneMenu[3][playerid], -1378294017);
	PlayerTextDrawFont(playerid, PhoneMenu[3][playerid], 1);
	PlayerTextDrawSetProportional(playerid, PhoneMenu[3][playerid], 1);
	PlayerTextDrawSetShadow(playerid, PhoneMenu[3][playerid], 0);


	PhoneTDbox[0][playerid] = TextDrawCreate(445.666656, 283.333221, "Temas");
	TextDrawLetterSize(PhoneTDbox[0][playerid], 0.000000, 21.124006);
	TextDrawTextSize(PhoneTDbox[0][playerid], 546.000244, 0.000000);
	TextDrawAlignment(PhoneTDbox[0][playerid], 1);
	TextDrawColor(PhoneTDbox[0][playerid], -1);
	TextDrawUseBox(PhoneTDbox[0][playerid], 1);
	TextDrawBoxColor(PhoneTDbox[0][playerid], phone_display_color[PlayerInfo[playerid][pPhoneModel]]);
	TextDrawSetShadow(PhoneTDbox[0][playerid], 0);
	TextDrawSetOutline(PhoneTDbox[0][playerid], 0);
	TextDrawBackgroundColor(PhoneTDbox[0][playerid], 255);
	TextDrawFont(PhoneTDbox[0][playerid], 1);
	TextDrawSetProportional(PhoneTDbox[0][playerid], 1);
	TextDrawSetShadow(PhoneTDbox[0][playerid], 0);

	PhoneTDbox[1][playerid] = TextDrawCreate(442.916961, 274.777862, "LD_DRV:tvcorn");
	TextDrawLetterSize(PhoneTDbox[1][playerid], 0.000000, 0.000000);
	TextDrawTextSize(PhoneTDbox[1][playerid], 54.000000, 176.000000);
	TextDrawAlignment(PhoneTDbox[1][playerid], 1);
	TextDrawColor(PhoneTDbox[1][playerid], phone_model_color[PlayerInfo[playerid][pPhoneModel]]);
	TextDrawSetShadow(PhoneTDbox[1][playerid], 0);
	TextDrawSetOutline(PhoneTDbox[1][playerid], 0);
	TextDrawBackgroundColor(PhoneTDbox[1][playerid], phone_model_color[PlayerInfo[playerid][pPhoneModel]]);
	TextDrawFont(PhoneTDbox[1][playerid], 4);
	TextDrawSetProportional(PhoneTDbox[1][playerid], 0);
	TextDrawSetShadow(PhoneTDbox[1][playerid], 0);

	PhoneTDbox[2][playerid] = TextDrawCreate(548.632690, 274.777893, "LD_DRV:tvcorn");
	TextDrawLetterSize(PhoneTDbox[2][playerid], 0.000000, 0.000000);
	TextDrawTextSize(PhoneTDbox[2][playerid], -54.000000, 176.000000);
	TextDrawAlignment(PhoneTDbox[2][playerid], 1);
	TextDrawColor(PhoneTDbox[2][playerid], phone_model_color[PlayerInfo[playerid][pPhoneModel]]);
	TextDrawSetShadow(PhoneTDbox[2][playerid], 0);
	TextDrawSetOutline(PhoneTDbox[2][playerid], 0);
	TextDrawBackgroundColor(PhoneTDbox[2][playerid], phone_model_color[PlayerInfo[playerid][pPhoneModel]]);
	TextDrawFont(PhoneTDbox[2][playerid], 4);
	TextDrawSetProportional(PhoneTDbox[2][playerid], 0);
	TextDrawSetShadow(PhoneTDbox[2][playerid], 0);

	PhoneTDbox[3][playerid] = TextDrawCreate(467.366607, 308.844573, "LD_DRV:tvbase");
	TextDrawLetterSize(PhoneTDbox[3][playerid], 0.000000, 0.000000);
	TextDrawTextSize(PhoneTDbox[3][playerid], 54.000000, -2.000000);
	TextDrawAlignment(PhoneTDbox[3][playerid], 1);
	TextDrawColor(PhoneTDbox[3][playerid], phone_model_color[PlayerInfo[playerid][pPhoneModel]]);
	TextDrawSetShadow(PhoneTDbox[3][playerid], 0);
	TextDrawSetOutline(PhoneTDbox[3][playerid], 0);
	TextDrawBackgroundColor(PhoneTDbox[3][playerid], phone_model_color[PlayerInfo[playerid][pPhoneModel]]);
	TextDrawFont(PhoneTDbox[3][playerid], 4);
	TextDrawSetProportional(PhoneTDbox[3][playerid], 0);
	TextDrawSetShadow(PhoneTDbox[3][playerid], 0);

	PhoneTDbox[4][playerid] = TextDrawCreate(476.666717, 294.048461, "PHONE");
	TextDrawLetterSize(PhoneTDbox[4][playerid], 0.274166, 1.071112);
	TextDrawAlignment(PhoneTDbox[4][playerid], 1);
	TextDrawColor(PhoneTDbox[4][playerid], -1);
	TextDrawSetShadow(PhoneTDbox[4][playerid], 0);
	TextDrawSetOutline(PhoneTDbox[4][playerid], 0);
	TextDrawBackgroundColor(PhoneTDbox[4][playerid], 255);
	TextDrawFont(PhoneTDbox[4][playerid], 2);
	TextDrawSetProportional(PhoneTDbox[4][playerid], 1);
	TextDrawSetShadow(PhoneTDbox[4][playerid], 0);

	PhNewMSG[playerid] = TextDrawCreate(518.350402, 291.061187, "LD_BEAT:chit");
	TextDrawLetterSize(PhNewMSG[playerid], 0.000000, 0.000000);
	TextDrawTextSize(PhNewMSG[playerid], 7.000000, 9.000000);
	TextDrawAlignment(PhNewMSG[playerid], 1);
	TextDrawColor(PhNewMSG[playerid], 0xFFFFFFFF); //new msg - 0x007F14FF
	TextDrawSetShadow(PhNewMSG[playerid], 0);
	TextDrawSetOutline(PhNewMSG[playerid], 0);
	TextDrawBackgroundColor(PhNewMSG[playerid], 0xFFFFFFFF);
	TextDrawFont(PhNewMSG[playerid], 4);
	TextDrawSetProportional(PhNewMSG[playerid], 0);
	TextDrawSetShadow(PhNewMSG[playerid], 0);
	TextDrawSetSelectable(PhNewMSG[playerid], true);
	
	PhOFF[playerid] = TextDrawCreate(524.599914, 287.431396, "LD_BEAT:circle");
	TextDrawLetterSize(PhOFF[playerid], 0.000000, 0.000000);
	TextDrawTextSize(PhOFF[playerid], 10.000000, 10.000000);
	TextDrawAlignment(PhOFF[playerid], 1);
	TextDrawColor(PhOFF[playerid], -1);
	TextDrawSetShadow(PhOFF[playerid], 0);
	TextDrawSetOutline(PhOFF[playerid], 0);
	TextDrawBackgroundColor(PhOFF[playerid], 255);
	TextDrawFont(PhOFF[playerid], 4);
	TextDrawSetProportional(PhOFF[playerid], 0);
	TextDrawSetShadow(PhOFF[playerid], 0);
	TextDrawSetSelectable(PhOFF[playerid], true);

	Ph_selfie[playerid] = TextDrawCreate(543.550964, 340.610107, "LD_SPAC:white");
	TextDrawLetterSize(Ph_selfie[playerid], 0.000000, 0.000000);
	TextDrawTextSize(Ph_selfie[playerid],5.000000, 22.000000);
	TextDrawAlignment(Ph_selfie[playerid], 1);
	TextDrawColor(Ph_selfie[playerid], -2139062017);
	TextDrawSetShadow(Ph_selfie[playerid], 0);
	TextDrawSetOutline(Ph_selfie[playerid], 0);
	TextDrawBackgroundColor(Ph_selfie[playerid], 255);
	TextDrawFont(Ph_selfie[playerid], 4);
	TextDrawSetProportional(Ph_selfie[playerid], 0);
	TextDrawSetShadow(Ph_selfie[playerid], 0);
	TextDrawSetSelectable(Ph_selfie[playerid], true);

	NewSMS[playerid] = TextDrawCreate(440.416595, 249.370346, "LD_CHAT:goodcha");
	TextDrawLetterSize(NewSMS[playerid], 0.000000, 0.000000);
	TextDrawTextSize(NewSMS[playerid], -19.000000, 24.000000);
	TextDrawAlignment(NewSMS[playerid], 1);
	TextDrawColor(NewSMS[playerid], -1);
	TextDrawSetShadow(NewSMS[playerid], 0);
	TextDrawSetOutline(NewSMS[playerid], 0);
	TextDrawBackgroundColor(NewSMS[playerid], 255);
	TextDrawFont(NewSMS[playerid], 4);
	TextDrawSetProportional(NewSMS[playerid], 0);
	TextDrawSetShadow(NewSMS[playerid], 0);
	
	PhNetwork[playerid] = TextDrawCreate(536.083435, 320.148101, "|~r~||||"); // СВЯЗЬ (ФОН)
	TextDrawLetterSize(PhNetwork[playerid], 0.197916, 0.868888);
	TextDrawAlignment(PhNetwork[playerid], 3);
	TextDrawColor(PhNetwork[playerid], 255);
	TextDrawSetShadow(PhNetwork[playerid], 0);
	TextDrawSetOutline(PhNetwork[playerid], 0);
	TextDrawBackgroundColor(PhNetwork[playerid], 255);
	TextDrawFont(PhNetwork[playerid], 1);
	TextDrawSetProportional(PhNetwork[playerid], 1);
	TextDrawSetShadow(PhNetwork[playerid], 0);

	PhTime[playerid] = TextDrawCreate(455.683410, 320.148101, "16:01");
	TextDrawLetterSize(PhTime[playerid], 0.197916, 0.868888);
	TextDrawAlignment(PhTime[playerid], 1);
	TextDrawColor(PhTime[playerid], 255);
	TextDrawBackgroundColor(PhTime[playerid], 255);
	TextDrawFont(PhTime[playerid], 1);
	TextDrawSetProportional(PhTime[playerid], 1);
	TextDrawSetShadow(PhTime[playerid], 0);
	TextDrawSetOutline(PhTime[playerid], 0);

	PhArrow[0][playerid] = TextDrawCreate(479.633392, 413.795959, "LD_BEAT:left");
	TextDrawLetterSize(PhArrow[0][playerid], 0.000000, 0.000000);
	TextDrawTextSize(PhArrow[0][playerid], 15.000000, 15.000000);
	TextDrawAlignment(PhArrow[0][playerid], 1);
	TextDrawColor(PhArrow[0][playerid], -1);
	TextDrawSetShadow(PhArrow[0][playerid], 0);
	TextDrawSetOutline(PhArrow[0][playerid], 0);
	TextDrawBackgroundColor(PhArrow[0][playerid], 255);
	TextDrawFont(PhArrow[0][playerid], 4);
	TextDrawSetProportional(PhArrow[0][playerid], 0);
	TextDrawSetShadow(PhArrow[0][playerid], 0);
	TextDrawSetSelectable(PhArrow[0][playerid], true);

	PhArrow[1][playerid] = TextDrawCreate(498.800018, 413.795959, "LD_BEAT:right");
	TextDrawLetterSize(PhArrow[1][playerid], 0.000000, 0.000000);
	TextDrawTextSize(PhArrow[1][playerid], 15.000000, 15.000000);
	TextDrawAlignment(PhArrow[1][playerid], 1);
	TextDrawColor(PhArrow[1][playerid], -1);
	TextDrawSetShadow(PhArrow[1][playerid], 0);
	TextDrawSetOutline(PhArrow[1][playerid], 0);
	TextDrawBackgroundColor(PhArrow[1][playerid], 255);
	TextDrawFont(PhArrow[1][playerid], 4);
	TextDrawSetProportional(PhArrow[1][playerid], 0);
	TextDrawSetShadow(PhArrow[1][playerid], 0);
	TextDrawSetSelectable(PhArrow[1][playerid], true);

	PhArrow[2][playerid] = TextDrawCreate(489.316772, 399.551422, "LD_BEAT:up");
	TextDrawLetterSize(PhArrow[2][playerid], 0.000000, 0.000000);
	TextDrawTextSize(PhArrow[2][playerid], 15.000000, 15.000000);
	TextDrawAlignment(PhArrow[2][playerid], 1);
	TextDrawColor(PhArrow[2][playerid], -1);
	TextDrawSetShadow(PhArrow[2][playerid], 0);
	TextDrawSetOutline(PhArrow[2][playerid], 0);
	TextDrawBackgroundColor(PhArrow[2][playerid], 255);
	TextDrawFont(PhArrow[2][playerid], 4);
	TextDrawSetProportional(PhArrow[2][playerid], 0);
	TextDrawSetShadow(PhArrow[2][playerid], 0);
	TextDrawSetSelectable(PhArrow[2][playerid], true);

	PhArrow[3][playerid] = TextDrawCreate(489.333404, 427.514404, "LD_BEAT:down");
	TextDrawLetterSize(PhArrow[3][playerid], 0.000000, 0.000000);
	TextDrawTextSize(PhArrow[3][playerid], 15.000000, 15.000000);
	TextDrawAlignment(PhArrow[3][playerid], 1);
	TextDrawColor(PhArrow[3][playerid], -1);
	TextDrawSetShadow(PhArrow[3][playerid], 0);
	TextDrawSetOutline(PhArrow[3][playerid], 0);
	TextDrawBackgroundColor(PhArrow[3][playerid], 255);
	TextDrawFont(PhArrow[3][playerid], 4);
	TextDrawSetProportional(PhArrow[3][playerid], 0);
	TextDrawSetShadow(PhArrow[3][playerid], 0);
	TextDrawSetSelectable(PhArrow[3][playerid], true);

	PhBtnStr[0][playerid] = TextDrawCreate(455.251892, 381.733459, "ОК");  
	TextDrawLetterSize(PhBtnStr[0][playerid], 0.209166, 0.988148);
	TextDrawAlignment(PhBtnStr[0][playerid], 1);
	TextDrawColor(PhBtnStr[0][playerid], 255);
	TextDrawSetShadow(PhBtnStr[0][playerid], 0);
	TextDrawSetOutline(PhBtnStr[0][playerid], 0);
	TextDrawBackgroundColor(PhBtnStr[0][playerid], 255);
	TextDrawFont(PhBtnStr[0][playerid], 1);
	TextDrawSetProportional(PhBtnStr[0][playerid], 1);
	TextDrawSetShadow(PhBtnStr[0][playerid], 0);

	PhBtnStr[1][playerid] = TextDrawCreate(534.566833, 381.733459, "Voltar");
	TextDrawLetterSize(PhBtnStr[1][playerid], 0.209166, 0.988148);
	TextDrawAlignment(PhBtnStr[1][playerid], 3);
	TextDrawColor(PhBtnStr[1][playerid], 255);
	TextDrawSetShadow(PhBtnStr[1][playerid], 0);
	TextDrawSetOutline(PhBtnStr[1][playerid], 0);
	TextDrawBackgroundColor(PhBtnStr[1][playerid], 255);
	TextDrawFont(PhBtnStr[1][playerid], 1);
	TextDrawSetProportional(PhBtnStr[1][playerid], 1);
	TextDrawSetShadow(PhBtnStr[1][playerid], 0);

	PhBtn[0][playerid] = TextDrawCreate(454.999450, 397.526245, "box"); //MENU BUTTON
	TextDrawLetterSize(PhBtn[0][playerid], 0.000000, 0.000000);
	TextDrawTextSize(PhBtn[0][playerid], 19.000000, 10.000000);
	TextDrawAlignment(PhBtn[0][playerid], 1);
	TextDrawColor(PhBtn[0][playerid], phone_btns_color[PlayerInfo[playerid][pPhoneModel]]);
	TextDrawSetOutline(PhBtn[0][playerid], 0);
	TextDrawBackgroundColor(PhBtn[0][playerid], phone_btns_color[PlayerInfo[playerid][pPhoneModel]]);
	TextDrawFont(PhBtn[0][playerid], 5);
	TextDrawSetProportional(PhBtn[0][playerid], 0);
	TextDrawSetSelectable(PhBtn[0][playerid], true);
	TextDrawSetPreviewModel(PhBtn[0][playerid], 0);
	TextDrawSetPreviewRot(PhBtn[0][playerid], 0.000000, 0.000000, 0.000000, -1.000000);

	PhBtn[1][playerid] = TextDrawCreate(515.984130, 397.525848, "box"); //BACK BUTTON
	TextDrawLetterSize(PhBtn[1][playerid], 0.000000, 0.000000);
	TextDrawTextSize(PhBtn[1][playerid], 19.000000, 10.000000);
	TextDrawAlignment(PhBtn[1][playerid], 1);
	TextDrawColor(PhBtn[1][playerid], phone_btns_color[PlayerInfo[playerid][pPhoneModel]]);
	TextDrawSetOutline(PhBtn[1][playerid], 0);
	TextDrawBackgroundColor(PhBtn[1][playerid], phone_btns_color[PlayerInfo[playerid][pPhoneModel]]);
	TextDrawFont(PhBtn[1][playerid], 5);
	TextDrawSetProportional(PhBtn[1][playerid], 0);
	TextDrawSetSelectable(PhBtn[1][playerid], true);
	TextDrawSetPreviewModel(PhBtn[1][playerid], 0);
	TextDrawSetPreviewRot(PhBtn[1][playerid], 0.000000, 0.000000, 0.000000, -1.000000);
	
	PhNote[0][playerid] = TextDrawCreate(470.000030, 348.007324, "LD_CHAT:thumbup"); //Emoji
	TextDrawLetterSize(PhNote[0][playerid], 0.000000, 0.000000);
	TextDrawTextSize(PhNote[0][playerid], -10.000000, 13.000000);
	TextDrawAlignment(PhNote[0][playerid], 1);
	TextDrawColor(PhNote[0][playerid], -1);
	TextDrawSetShadow(PhNote[0][playerid], 0);
	TextDrawSetOutline(PhNote[0][playerid], 0);
	TextDrawBackgroundColor(PhNote[0][playerid], 255);
	TextDrawFont(PhNote[0][playerid], 4);
	TextDrawSetProportional(PhNote[0][playerid], 0);
	TextDrawSetShadow(PhNote[0][playerid], 0);

	PhNote[1][playerid] = TextDrawCreate(495.333129, 350.217681, "Mensagem enviada!"); //Text  or  "Calling...~n~5552255")
	TextDrawLetterSize(PhNote[1][playerid], 0.197916, 0.868888);
	TextDrawAlignment(PhNote[1][playerid], 2);
	TextDrawColor(PhNote[1][playerid], 255);
	TextDrawSetShadow(PhNote[1][playerid], 0);
	TextDrawBackgroundColor(PhNote[1][playerid], -1378294017);
	TextDrawFont(PhNote[1][playerid], 1);
	TextDrawSetProportional(PhNote[1][playerid], 1);
	TextDrawSetOutline(PhNote[1][playerid], 0);
	
	// Игрок в телефоне во время звонка
	/*CallBox[0][playerid] = TextDrawCreate(454.999908, 320.148101, "CallBox"); // CallBox
	TextDrawLetterSize(CallBox[0][playerid], 0.000000, 0.791667);
	TextDrawTextSize(CallBox[0][playerid], 536.000000, 0.000000);
	TextDrawAlignment(CallBox[0][playerid], 1);
	TextDrawColor(CallBox[0][playerid], -1);
	
	TextDrawSetShadow(CallBox[0][playerid], 0);
	TextDrawSetOutline(CallBox[0][playerid], 0);
	TextDrawBackgroundColor(CallBox[0][playerid], 255);
	TextDrawFont(CallBox[0][playerid], 1);
	TextDrawSetProportional(CallBox[0][playerid], 1);
	TextDrawSetShadow(CallBox[0][playerid], 0);
	
	CallBox[playerid] = TextDrawCreate(487.083129, 320.148101, "00:00"); // CALL TIME
	TextDrawLetterSize(CallBox[playerid], 0.197916, 0.868888);// 0.196666, 0.946667);
	TextDrawAlignment(CallBox[playerid], 1);
	TextDrawColor(CallBox[playerid], 255);
	TextDrawUseBox(CallBox[playerid], 1);
	TextDrawBoxColor(CallBox[playerid], COLOR_GRAD2);
	TextDrawSetShadow(CallBox[playerid], 0);
	TextDrawSetOutline(CallBox[playerid], 0);
	TextDrawBackgroundColor(CallBox[playerid], 255);
	TextDrawFont(CallBox[playerid], 1);
	TextDrawSetProportional(CallBox[playerid], 1);
	TextDrawSetShadow(CallBox[playerid], 0);

	CallBox[playerid] = TextDrawCreate(495.366668, 315.963104, "01:02");
	TextDrawLetterSize(CallBox[playerid], 0.197916, 0.868888); // 0.182500, 0.899999);
	TextDrawTextSize(CallBox[playerid], 0.000000, 77.930198);
	TextDrawAlignment(CallBox[playerid], 2);
	TextDrawColor(CallBox[playerid], -1);
	TextDrawUseBox(CallBox[playerid], 1);
	TextDrawBoxColor(CallBox[playerid], 0xFF6347AA);
	TextDrawSetShadow(CallBox[playerid], 0);
	TextDrawSetOutline(CallBox[playerid], 0);
	TextDrawBackgroundColor(CallBox[playerid], 255);
	TextDrawFont(CallBox[playerid], 1);
	TextDrawSetProportional(CallBox[playerid], 1);
	TextDrawSetShadow(CallBox[playerid], 0);
	
	CallBox[playerid] = TextDrawCreate(495.416656, 319.948181, "01:10");
	TextDrawLetterSize(CallBox[playerid], 0.197916, 0.868888);
	TextDrawTextSize(CallBox[playerid], 81.000000, 81.000000);
	TextDrawAlignment(CallBox[playerid], 2);
	TextDrawColor(CallBox[playerid], -1);
	TextDrawUseBox(CallBox[playerid], 1);
	TextDrawBoxColor(CallBox[playerid], 0xFF6347AA);//-2147483393);
	TextDrawSetShadow(CallBox[playerid], 0);
	TextDrawSetOutline(CallBox[playerid], 0);
	TextDrawBackgroundColor(CallBox[playerid], 255);
	TextDrawFont(CallBox[playerid], 1);
	TextDrawSetProportional(CallBox[playerid], 1);
	TextDrawSetShadow(CallBox[playerid], 0);
	TextDrawSetSelectable(CallBox[playerid], true);*/
		
	// Дисплей телефона
	PhoneTDdisplay[0][playerid] = TextDrawCreate(478.116699, 328.599792, "22:22"); //TIME
	TextDrawLetterSize(PhoneTDdisplay[0][playerid], 0.415416, 2.885926);
	TextDrawAlignment(PhoneTDdisplay[0][playerid], 1);
	TextDrawColor(PhoneTDdisplay[0][playerid], 255);
	TextDrawSetShadow(PhoneTDdisplay[0][playerid], 0);
	TextDrawSetOutline(PhoneTDdisplay[0][playerid], 0);
	TextDrawBackgroundColor(PhoneTDdisplay[0][playerid], 255);
	TextDrawFont(PhoneTDdisplay[0][playerid], 1);
	TextDrawSetProportional(PhoneTDdisplay[0][playerid], 1);
	TextDrawSetShadow(PhoneTDdisplay[0][playerid], 0);

	PhoneTDdisplay[1][playerid] = TextDrawCreate(495.233459, 348.848205, "1"); // DATE
	TextDrawLetterSize(PhoneTDdisplay[1][playerid], 0.221250, 1.428888);
	TextDrawAlignment(PhoneTDdisplay[1][playerid], 2);
	TextDrawColor(PhoneTDdisplay[1][playerid], 255);
	TextDrawSetShadow(PhoneTDdisplay[1][playerid], 0);
	TextDrawSetOutline(PhoneTDdisplay[1][playerid], 0);
	TextDrawBackgroundColor(PhoneTDdisplay[1][playerid], 255);
	TextDrawFont(PhoneTDdisplay[1][playerid], 1);
	TextDrawSetProportional(PhoneTDdisplay[1][playerid], 1);
	TextDrawSetShadow(PhoneTDdisplay[1][playerid], 0);


	PhNewMSGDisplay[0][playerid] = TextDrawCreate(477.649810, 365.562957, "LD_CHAT:badchat");
	TextDrawLetterSize(PhNewMSGDisplay[0][playerid], 0.000000, 0.000000);
	TextDrawTextSize(PhNewMSGDisplay[0][playerid], -8.000000, 9.000000);
	TextDrawAlignment(PhNewMSGDisplay[0][playerid], 1);
	TextDrawColor(PhNewMSGDisplay[0][playerid], -1);
	TextDrawSetShadow(PhNewMSGDisplay[0][playerid], 0);
	TextDrawSetOutline(PhNewMSGDisplay[0][playerid], 0);
	TextDrawBackgroundColor(PhNewMSGDisplay[0][playerid], 255);
	TextDrawFont(PhNewMSGDisplay[0][playerid], 4);
	TextDrawSetProportional(PhNewMSGDisplay[0][playerid], 0);
	TextDrawSetShadow(PhNewMSGDisplay[0][playerid], 0);

	PhNewMSGDisplay[1][playerid] = TextDrawCreate(472.649810, 365.562957, "- novas mensagens"); //479.317626, 365.303802,
	TextDrawLetterSize(PhNewMSGDisplay[1][playerid], 0.144166, 1.019258);
	TextDrawAlignment(PhNewMSGDisplay[1][playerid], 1);
	TextDrawColor(PhNewMSGDisplay[1][playerid], -1523963137);
	TextDrawSetShadow(PhNewMSGDisplay[1][playerid], 0);
	TextDrawSetOutline(PhNewMSGDisplay[1][playerid], 0);
	TextDrawBackgroundColor(PhNewMSGDisplay[1][playerid], 255);
	TextDrawFont(PhNewMSGDisplay[1][playerid], 1);
	TextDrawSetProportional(PhNewMSGDisplay[1][playerid], 1);
	TextDrawSetShadow(PhNewMSGDisplay[1][playerid], 0);

	PhDisplay[playerid] = TextDrawCreate(454.999816, 320.148162, "display");
	TextDrawLetterSize(PhDisplay[playerid], 0.000000, 7.791664);
	TextDrawTextSize(PhDisplay[playerid], 536.000000, 0.000000);
	TextDrawAlignment(PhDisplay[playerid], 1);
	TextDrawColor(PhDisplay[playerid], -1);
	TextDrawUseBox(PhDisplay[playerid], 1);
	TextDrawBoxColor(PhDisplay[playerid], phone_window[PlayerInfo[playerid][pPhoneStyle]]);
	TextDrawSetShadow(PhDisplay[playerid], 0);
	TextDrawSetOutline(PhDisplay[playerid], 0);
	TextDrawBackgroundColor(PhDisplay[playerid], phone_window[PlayerInfo[playerid][pPhoneStyle]]);
	TextDrawFont(PhDisplay[playerid], 1);
	TextDrawSetProportional(PhDisplay[playerid], 1);
	TextDrawSetShadow(PhDisplay[playerid], 0);

	PhoneMenuBg[0][playerid] = TextDrawCreate(454.683410, 329.736022, "Contatos");//фон
	TextDrawLetterSize(PhoneMenuBg[0][playerid], 0.197916, 0.868889);
	TextDrawTextSize(PhoneMenuBg[0][playerid], 81.000000, 12.000000);
	TextDrawAlignment(PhoneMenuBg[0][playerid], 1);
	TextDrawColor(PhoneMenuBg[0][playerid], phone_panel[PlayerInfo[playerid][pPhoneStyle]]);	
	TextDrawSetShadow(PhoneMenuBg[0][playerid], 0);
	TextDrawSetOutline(PhoneMenuBg[0][playerid], 0);
	TextDrawBackgroundColor(PhoneMenuBg[0][playerid], phone_panel[PlayerInfo[playerid][pPhoneStyle]]);
	TextDrawFont(PhoneMenuBg[0][playerid], 5);
	TextDrawSetProportional(PhoneMenuBg[0][playerid], 0);
	TextDrawSetShadow(PhoneMenuBg[0][playerid], 0);
	TextDrawSetSelectable(PhoneMenuBg[0][playerid], true);
	TextDrawSetPreviewModel(PhoneMenuBg[0][playerid], 0);
	TextDrawSetPreviewRot(PhoneMenuBg[0][playerid], 0.000000, 0.000000, 0.000000, -1.000000);

	PhoneMenuBg[1][playerid] = TextDrawCreate(454.683410, 342.132995, "Ligue para um nъmero"); //фон
	TextDrawLetterSize(PhoneMenuBg[1][playerid], 0.197916, 0.868889);
	TextDrawTextSize(PhoneMenuBg[1][playerid], 81.000000, 12.000000);
	TextDrawAlignment(PhoneMenuBg[1][playerid], 1);
	TextDrawColor(PhoneMenuBg[1][playerid], phone_panel[PlayerInfo[playerid][pPhoneStyle]]);
	TextDrawSetShadow(PhoneMenuBg[1][playerid], 0);
	TextDrawSetOutline(PhoneMenuBg[1][playerid], 0);
	TextDrawBackgroundColor(PhoneMenuBg[1][playerid], phone_panel[PlayerInfo[playerid][pPhoneStyle]]);
	TextDrawFont(PhoneMenuBg[1][playerid], 5);
	TextDrawSetProportional(PhoneMenuBg[1][playerid], 0);
	TextDrawSetShadow(PhoneMenuBg[1][playerid], 0);
	TextDrawSetSelectable(PhoneMenuBg[1][playerid], true);
	TextDrawSetPreviewModel(PhoneMenuBg[1][playerid], 0);
	TextDrawSetPreviewRot(PhoneMenuBg[1][playerid], 0.000000, 0.000000, 0.000000, -1.000000);
	
	PhoneMenuBg[2][playerid] = TextDrawCreate(454.683410, 354.629943, "SMS"); //фон
	TextDrawLetterSize(PhoneMenuBg[2][playerid], 0.197916, 0.868889);
	TextDrawTextSize(PhoneMenuBg[2][playerid], 81.000000, 12.000000);
	TextDrawAlignment(PhoneMenuBg[2][playerid], 1);
	TextDrawColor(PhoneMenuBg[2][playerid], phone_panel[PlayerInfo[playerid][pPhoneStyle]]);
	TextDrawSetShadow(PhoneMenuBg[2][playerid], 0);
	TextDrawSetOutline(PhoneMenuBg[2][playerid], 0);
	TextDrawBackgroundColor(PhoneMenuBg[2][playerid], phone_panel[PlayerInfo[playerid][pPhoneStyle]]);
	TextDrawFont(PhoneMenuBg[2][playerid], 5);
	TextDrawSetProportional(PhoneMenuBg[2][playerid], 0);
	TextDrawSetShadow(PhoneMenuBg[2][playerid], 0);
	TextDrawSetSelectable(PhoneMenuBg[2][playerid], true);
	TextDrawSetPreviewModel(PhoneMenuBg[2][playerid], 0);
	TextDrawSetPreviewRot(PhoneMenuBg[2][playerid], 0.000000, 0.000000, 0.000000, -1.000000);

	PhoneMenuBg[3][playerid] = TextDrawCreate(454.683410, 366.976928, "Opзхes"); //фон
	TextDrawLetterSize(PhoneMenuBg[3][playerid], 0.197916, 0.868889);
	TextDrawTextSize(PhoneMenuBg[3][playerid], 81.000000, 12.000000);
	TextDrawAlignment(PhoneMenuBg[3][playerid], 1);
	TextDrawColor(PhoneMenuBg[3][playerid], phone_panel[PlayerInfo[playerid][pPhoneStyle]]);
	TextDrawSetShadow(PhoneMenuBg[3][playerid], 0);
	TextDrawSetOutline(PhoneMenuBg[3][playerid], 0);
	TextDrawBackgroundColor(PhoneMenuBg[3][playerid], phone_panel[PlayerInfo[playerid][pPhoneStyle]]);
	TextDrawFont(PhoneMenuBg[3][playerid], 5);
	TextDrawSetProportional(PhoneMenuBg[3][playerid], 0);
	TextDrawSetShadow(PhoneMenuBg[3][playerid], 0);
	TextDrawSetSelectable(PhoneMenuBg[3][playerid], true);
	TextDrawSetPreviewModel(PhoneMenuBg[3][playerid], 0);
	TextDrawSetPreviewRot(PhoneMenuBg[3][playerid], 0.000000, 0.000000, 0.000000, -1.000000);

	CallBox[playerid] = TextDrawCreate(495.416778, 320.147979, "00:30");
	TextDrawLetterSize(CallBox[playerid], 0.154166, 0.713333);
	TextDrawTextSize(CallBox[playerid], 10.000000, 81.000000);
	TextDrawAlignment(CallBox[playerid], 2);
	TextDrawColor(CallBox[playerid], 255);
	TextDrawUseBox(CallBox[playerid], 1);
	TextDrawBoxColor(CallBox[playerid], COLOR_LIGHTRED);
	TextDrawSetShadow(CallBox[playerid], 0);
	TextDrawSetOutline(CallBox[playerid], 0);
	TextDrawBackgroundColor(CallBox[playerid], 255);
	TextDrawFont(CallBox[playerid], 1);
	TextDrawSetProportional(CallBox[playerid], 1);
	TextDrawSetShadow(CallBox[playerid], 0);
	TextDrawSetSelectable(CallBox[playerid], true);

    return 1;
}