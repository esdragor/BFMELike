#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameCameraInputDataConfig.generated.h"

class UInputAction;

UCLASS()
class HEROESOFLIBERTY_API UGameCameraInputDataConfig : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* Move;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* Zoom;
};