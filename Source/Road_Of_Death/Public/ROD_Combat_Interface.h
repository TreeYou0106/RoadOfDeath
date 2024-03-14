// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ROD_Combat_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UROD_Combat_Interface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ROAD_OF_DEATH_API IROD_Combat_Interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void CauseDamage(AActor* Causer, float BaseDamage) = 0;
};
