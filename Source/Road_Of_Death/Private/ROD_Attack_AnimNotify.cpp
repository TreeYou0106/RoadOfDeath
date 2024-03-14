// Fill out your copyright notice in the Description page of Project Settings.


#include "ROD_Attack_AnimNotify.h"
#include "ROD_Enemy.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

void UROD_Attack_AnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::Notify(MeshComp, Animation);

    // 获得敌人类
    AROD_Enemy* Character = Cast<AROD_Enemy>(MeshComp->GetOwner());
    if (Character)
    {
        // 在丧尸攻击到我们时扣血
        Character->Attack();
    }
}
