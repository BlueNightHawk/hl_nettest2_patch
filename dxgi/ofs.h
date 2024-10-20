#pragma once

struct offset_s {
	unsigned long CdCheck = 0;
	unsigned long WonComm = 0;
	unsigned long Resolution = 0;
	unsigned long ChangeDisplayModeByIndex = 0;

	unsigned long WidthPtr = 0;
	unsigned long HeightPtr = 0;
	unsigned long CountPtr = 0;

	unsigned long Bomb_Month;
	unsigned long Bomb_Year;
};

offset_s GetOffsets();