// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ROD_PlayerController.generated.h"

/**
 * 
 */
class UInputMappingContext;
class UInputAction;
class AROD_Character;
struct FInputActionValue;

UCLASS()
class ROAD_OF_DEATH_API AROD_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AROD_PlayerController();

	// ��������µı�־
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Anim, meta = (AllowPrivateAccess = "true"))
	bool bIsFiringPressed = false;

	// IMC����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	// ǰ����InputAction����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_MoveForward;

	// ������InputAction����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_MoveRight;

	// ���InputAction����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Shoot;
	
protected:
	// ��Ϸ��ʼ��ʵ����ʱ����
	virtual void BeginPlay() override;
	// ǰ����
	void MoveForward(const FInputActionValue& Value);
	// ������
	void MoveRight(const FInputActionValue& Value);
	// ת�����
	void TurnToCursor(float DeltaTime);
	// ���������
	void FirePressed();
	// ������ͷ�
	void FireReleased();

	// ������Դ����
	
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundCue* WalkSound;  // ������Ч

public:
	// ÿ֡����
	virtual void Tick(float DeltaTime) override;
	// �����������
	virtual void SetupInputComponent() override;

private:

	bool bHitSuccessful = false; // �����Ƿ�����

	FVector CachedDestination; // ����λ��
	FHitResult Hit; // ����������н��
	AROD_Character* ROD_Character; // �Խ�ɫ�������
	
};
