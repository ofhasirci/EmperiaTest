// Fill out your copyright notice in the Description page of Project Settings.


#include "SARSlateWidget.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SARSlateWidget::Construct(const FArguments& InArgs)
{
	OnGenerateButtonClicked = InArgs._OnGenerateButtonClicked;

	ChildSlot
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
				[
					SNew(STextBlock)
					.Text(FText::FromString("Generate Random Shapes"))
					.Justification(ETextJustify::Center)
				]
			+ SVerticalBox::Slot()
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
	UE_LOG(LogTemp, Warning, TEXT("SLATE BUTTON IS CLICKED"));

	if (OnGenerateButtonClicked.IsBound())
	{
		OnGenerateButtonClicked.Execute();
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
