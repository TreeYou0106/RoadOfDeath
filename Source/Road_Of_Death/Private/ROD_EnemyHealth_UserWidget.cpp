// Fill out your copyright notice in the Description page of Project Settings.


#include "ROD_EnemyHealth_UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UROD_EnemyHealth_UserWidget::SetPrograssBarPercent(float percentage)
{
	HealthBar->SetPercent(percentage);
}

void UROD_EnemyHealth_UserWidget::SetCurrentHealth(float CurrentHealth)
{
	TXT_CurrentHealth->SetText(FText::FromString(FString::SanitizeFloat((int)(CurrentHealth*100)/100)));
}

void UROD_EnemyHealth_UserWidget::SetMaxHealth(float MaxHealth)
{
	TXT_MaxHealth->SetText(FText::FromString(FString::SanitizeFloat((int)(MaxHealth*100)/100)));
}
