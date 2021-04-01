// Get DateTime in format: YYYY-MM-DD HH:MM:SS
stock getDateTime()
{
	new year, month, day;
	getdate(year, month, day);

	new hour, minute, second;
	gettime(hour, minute, second);

	new string[32];
	format(string, sizeof(string), "%02d-%02d-%02d %02d:%02d:%02d", year, month, day, hour, minute, second);

	return string;
}