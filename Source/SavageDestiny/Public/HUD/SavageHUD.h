// Copyright: Ader Einstein 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SavageHUD.generated.h"

class USavageOverlay;

UCLASS()
class SAVAGEDESTINY_API ASavageHUD : public AHUD
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = Savage)
	TSubclassOf<USavageOverlay> SavageOverlayClass;
	UPROPERTY()
	USavageOverlay* SavageOverlay;

public:
	FORCEINLINE USavageOverlay* GetSavageOverlay() const { return SavageOverlay; }
};
