// Copyright: Ader Einstein 2024

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SavageOverlay.generated.h"

/**
 * 
 */
UCLASS()
class SAVAGEDESTINY_API USavageOverlay : public UUserWidget
{
	GENERATED_BODY()
		
public: 
	void SetHealthBarPercent(const float Percent);
	void SetStaminaBarPercent(const float Percent);
	void SetGold(int32 Gold);
	void SetSouls(int32 Souls);

private:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthProgressBar;
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* StaminaProgressBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GoldText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SoulsText;
};
