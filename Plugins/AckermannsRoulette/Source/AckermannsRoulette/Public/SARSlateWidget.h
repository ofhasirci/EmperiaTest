// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"


/**
 * 
 */
class ACKERMANNSROULETTE_API SARSlateWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SARSlateWidget)
	{
	}
		SLATE_EVENT(FOnClicked, OnGenerateButtonClicked)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	FReply OnButtonClicked();

private:
	FOnClicked OnGenerateButtonClicked;
};
