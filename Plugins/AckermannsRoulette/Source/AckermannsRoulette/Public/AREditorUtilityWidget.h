// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "AREditorUtilityWidget.generated.h"

class USinglePropertyView;
class UButton;
/**
 * 
 */
UCLASS()
class ACKERMANNSROULETTE_API UAREditorUtilityWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	USinglePropertyView* PropertyView;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* GenerateButton;
	
};
