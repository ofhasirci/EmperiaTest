// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "AckermannsRouletteStyle.h"

class FAckermannsRouletteCommands : public TCommands<FAckermannsRouletteCommands>
{
public:

	FAckermannsRouletteCommands()
		: TCommands<FAckermannsRouletteCommands>(TEXT("AckermannsRoulette"), NSLOCTEXT("Contexts", "AckermannsRoulette", "AckermannsRoulette Plugin"), NAME_None, FAckermannsRouletteStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};