// Copyright Epic Games, Inc. All Rights Reserved.

#include "AckermannsRouletteCommands.h"

#define LOCTEXT_NAMESPACE "FAckermannsRouletteModule"

void FAckermannsRouletteCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "AckermannsRoulette", "Bring up AckermannsRoulette window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
