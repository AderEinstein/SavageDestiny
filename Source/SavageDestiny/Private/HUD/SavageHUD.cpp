// Copyright: Ader Einstein 2024


#include "HUD/SavageHUD.h"
#include "HUD/SavageOverlay.h"

void ASavageHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (GetWorld())
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && SavageOverlayClass)
		{
			SavageOverlay = CreateWidget<USavageOverlay>(Controller, SavageOverlayClass);
			SavageOverlay->AddToViewport();
		}
	}
}