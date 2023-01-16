#pragma once

#include "CoreMinimal.h"
#include "ResponseStruct.generated.h"

USTRUCT()
struct FResponseStruct
{
    GENERATED_BODY()

	UPROPERTY()
	TArray<int32> RandomArray;
};