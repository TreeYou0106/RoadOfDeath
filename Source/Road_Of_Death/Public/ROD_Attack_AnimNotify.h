// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ROD_Attack_AnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class ROAD_OF_DEATH_API UROD_Attack_AnimNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
	// ¶¯»­Í¨Öª
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
