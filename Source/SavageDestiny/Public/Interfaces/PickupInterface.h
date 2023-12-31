// Copyright: Ader Einstein 2024

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickupInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickupInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SAVAGEDESTINY_API IPickupInterface
{
	GENERATED_BODY()

public:
	virtual void SetOverlappingItem(class AItem* Item);
};
