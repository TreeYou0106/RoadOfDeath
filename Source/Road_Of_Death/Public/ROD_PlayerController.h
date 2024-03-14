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

	// 开火键按下的标志
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Anim, meta = (AllowPrivateAccess = "true"))
	bool bIsFiringPressed = false;

	// IMC声明
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	// 前后走InputAction声明
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_MoveForward;

	// 左右走InputAction声明
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_MoveRight;

	// 射击InputAction声明
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Shoot;
	
protected:
	// 游戏开始或实例化时调用
	virtual void BeginPlay() override;
	// 前后走
	void MoveForward(const FInputActionValue& Value);
	// 左右走
	void MoveRight(const FInputActionValue& Value);
	// 转向鼠标
	void TurnToCursor(float DeltaTime);
	// 开火键按下
	void FirePressed();
	// 开火键释放
	void FireReleased();

	// 声音资源加载
	
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundCue* WalkSound;  // 行走音效

public:
	// 每帧调用
	virtual void Tick(float DeltaTime) override;
	// 设置输入组件
	virtual void SetupInputComponent() override;

private:

	bool bHitSuccessful = false; // 射线是否被命中

	FVector CachedDestination; // 鼠标的位置
	FHitResult Hit; // 鼠标射线命中结果
	AROD_Character* ROD_Character; // 对角色类的引用
	
};
