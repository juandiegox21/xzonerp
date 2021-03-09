#include <izcmd>

CMD:facciones(playerid, params[])
{
    new string[42];
    for (new i = 0; i < MAX_FACTIONS; i++) {
        format(string, sizeof string, "%s", Factions[i][fName]);
        SendClientMessage(playerid, COLOR_WHITE, string);
    }

    return 1;
}