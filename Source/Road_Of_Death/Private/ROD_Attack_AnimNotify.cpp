// Fill out your copyright notice in the Description page of Project Settings.


#include "ROD_Attack_AnimNotify.h"
#include "ROD_Enemy.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

void UROD_Attack_AnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::Notify(MeshComp, Animation);

    // ��õ�����
    AROD_Enemy* Character = Cast<AROD_Enemy>(MeshComp->GetOwner());
    if (Character)
    {
        // ��ɥʬ����������ʱ��Ѫ
        Character->Attack();
    }
}
