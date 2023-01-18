// Fill out your copyright notice in the Description page of Project Settings.


#include "SARSlateWidget.h"

#include "ARDataStruct.h"
#include "DetailLayoutBuilder.h"
#include "SlateOptMacros.h"
#include "PropertyCustomizationHelpers.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SARSlateWidget::Construct(const FArguments& InArgs)
{
	OnGenerateButtonClicked = InArgs._OnGenerateButtonClicked;
	OnDTSelected = InArgs._OnDTSelected;

	TArray<const UClass*> AllowedClasses;
	AllowedClasses.Add(UDataTable::StaticClass());

	ChildSlot
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.Padding(2.0f, 2.0f, 2.0f, 10.0f)
			.AutoHeight()
			.HAlign(EHorizontalAlignment::HAlign_Center)
			.VAlign(EVerticalAlignment::VAlign_Center)
			[
				PropertyCustomizationHelpers::MakeAssetPickerWithMenu(
					FAssetData(), 
					false, 
					false, 
					AllowedClasses, 
					TArray<UFactory*>(),
					FOnShouldFilterAsset(),
					FOnAssetSelected::CreateRaw(this, &SARSlateWidget::OnDTAssetSelected),
					FSimpleDelegate())
				//PropertyCustomizationHelpers::MakeAssetPickerAnchorButton(FOnGetAllowedClasses::CreateRaw(this, &SARSlateWidget::OnGetAllowedClasses), FOnAssetSelected::CreateRaw(this, &SARSlateWidget::OnAssetSelected))
			]
			+ SVerticalBox::Slot()
			.Padding(2.0f)
			.AutoHeight()
			[
				SNew(STextBlock)
				.Text(FText::FromString("Generate Random Shapes"))
				.Justification(ETextJustify::Center)
			]
			+ SVerticalBox::Slot()
			.Padding(2.0f)
			.AutoHeight()
			[
				SNew(SButton)
				[
					SNew(STextBlock)
					.Text(FText::FromString("GENERATE"))
					.Justification(ETextJustify::Center)
				]
				.OnClicked(FOnClicked::CreateSP(this, &SARSlateWidget::OnButtonClicked))
			]
		];
}

FReply SARSlateWidget::OnButtonClicked()
{
	if (OnGenerateButtonClicked.IsBound())
	{
		OnGenerateButtonClicked.Execute();
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

void SARSlateWidget::OnDTAssetSelected(const FAssetData& AssetData)
{
	UE_LOG(LogTemp, Warning, TEXT("DT Asset: %s"), *AssetData.GetFullName());

	OnDTSelected.Execute(AssetData.GetSoftObjectPath());
	
}

void SARSlateWidget::OnGetAllowedClasses(TArray<const UClass*>& Classes)
{
}

void SARSlateWidget::OnAssetSelected(const FAssetData& AssetData)
{
	UE_LOG(LogTemp, Warning, TEXT("Anchor Asset: %s"), *AssetData.GetObjectPathString());

}



END_SLATE_FUNCTION_BUILD_OPTIMIZATION
