CMD:trabajo(playerid, params[])
{
    new 
        string[120],
        nearestJobId = Job_GetNearestJobIdToPlayer(playerid);
    
    if (nearestJobId < 0)
        return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: No estas cerca a un trabajo");
        
    if (Account_GetJobId(playerid) == nearestJobId)
        return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: Ya perteneces a este trabajo");

    // Player already has a job
    if (Account_GetJobId(playerid) > 0) {
        inline DialogJobQuit(id, dialogid, response, listitem, string: inputtext[]) {
            #pragma unused id, dialogid, listitem, inputtext
            if (!response) {
                return 1;
            }

            format(string, sizeof string, "[ATENCION] "COLOR_WHITE_INLINE" Has renunciado a tu trabajo de "COLOR_ORANGE_INLINE"%s"COLOR_WHITE_INLINE", ahora puedes pertenecer a otro trabajo", Job_GetName(Account_GetJobId(playerid)));
            SendClientMessage(playerid, COLOR_ORANGE, string);

            Account_SetJobId(playerid, 0);

            return cmd_trabajo(playerid, params);
        }

        format(string, sizeof string, ""COLOR_WHITE_INLINE"Para unirte a otro trabajo debes renunciar a tu actual trabajo de "COLOR_ORANGE_INLINE"%s"COLOR_WHITE_INLINE", deseas renunciar?", Job_GetName(nearestJobId));
        Dialog_ShowCallback(playerid, using inline DialogJobQuit, DIALOG_STYLE_MSGBOX, "Renunciar a tu trabajo", string, "Renunciar", "Cancelar");
        return 1;
    }

    inline DialogJobContract(id, dialogid, response, listitem, string: inputtext[]) {
        #pragma unused id, dialogid, listitem, inputtext
        if (!response) {
            return 1;
        }
        
        Account_SetJobId(playerid, nearestJobId);

        format(string, sizeof string, "[TRABAJOS] "COLOR_WHITE_INLINE" Bienvenido! ahora haces parte del trabajo de "COLOR_ORANGE_INLINE"%s", Job_GetName(nearestJobId));
        SendClientMessage(playerid, COLOR_ORANGE, string);
        return 1;
    }
    
    format(string, sizeof string, ""COLOR_WHITE_INLINE"Deseas aceptar este contrato para trabajar como "COLOR_ORANGE_INLINE"%s"COLOR_WHITE_INLINE"?", Job_GetName(nearestJobId));
    Dialog_ShowCallback(playerid, using inline DialogJobContract, DIALOG_STYLE_MSGBOX, "Contrato de trabajo", string, "Aceptar", "Cancelar");
    return 1;
}

CMD:trabajorenunciar(playerid, params[])
{
    new string[120];

    format(string, sizeof string, "[ATENCION] "COLOR_WHITE_INLINE" Has renunciado a tu trabajo de "COLOR_ORANGE_INLINE"%s"COLOR_WHITE_INLINE", ahora puedes pertenecer a otro trabajo", Job_GetName(Account_GetJobId(playerid)));
    SendClientMessage(playerid, COLOR_ORANGE, string);

    Account_SetJobId(playerid, 0);
    return 1;
}