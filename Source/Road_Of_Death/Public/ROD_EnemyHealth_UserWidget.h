// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ROD_EnemyHealth_UserWidget.generated.h"
/**
 * 
 */
UCLASS()
class ROAD_OF_DEATH_API UROD_EnemyHealth_UserWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TXT_CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TXT_MaxHealth;
	
	UFUNCTION(BlueprintCallable)
	void SetPrograssBarPercent(float percentage);
	UFUNCTION(BlueprintCallable)
	void SetCurrentHealth(float CurrentHealth);
	UFUNCTION(BlueprintCallable)
	void SetMaxHealth(float MaxHealth);
};
