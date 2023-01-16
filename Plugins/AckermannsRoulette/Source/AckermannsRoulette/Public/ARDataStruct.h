#pragma once

#include "CoreMinimal.h"
#include "ARDataStruct.generated.h"

USTRUCT(BlueprintType)
struct FARDataStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UStaticMesh> StaticMesh;
};