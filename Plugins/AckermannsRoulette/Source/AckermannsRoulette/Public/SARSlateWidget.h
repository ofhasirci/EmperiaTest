// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

DECLARE_DELEGATE_OneParam(FOnDTSelected, FSoftObjectPath)

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
		SLATE_EVENT(FOnDTSelected, OnDTSelected)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	FReply OnButtonClicked();

private:
	FOnClicked OnGenerateButtonClicked;

	FOnDTSelected OnDTSelected;

	void OnDTAssetSelected(const FAssetData& AssetData);

	void OnGetAllowedClasses(TArray<const UClass*>& Classes);

	void OnAssetSelected(const FAssetData& AssetData);
};