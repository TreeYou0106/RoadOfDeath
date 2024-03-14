#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ROD_Combat_Interface.h"
#include "ROD_Enemy.generated.h"

class AROD_Character;
class UWidgetComponent;
class UROD_EnemyHealth_UserWidget;

UCLASS()
class ROAD_OF_DEATH_API AROD_Enemy : public ACharacter, public IROD_Combat_Interface
{
    GENERATED_BODY()

public:
    // 构造函数
    AROD_Enemy();

protected:
    // 游戏开始或实例化时被调用
    virtual void BeginPlay() override;

    // 受到玩家攻击
    virtual void CauseDamage(AActor* Causer, float BaseDamage);

    // 朝向玩家移动
    void MoveToPlayer(float DeltaTime);

public:
    // 每帧调用
    virtual void Tick(float DeltaTime) override;

    // 声明是否攻击的标志
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bIsAttack = false;
    // 声明敌人死亡的标志
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bIsDead = false;
    // 声明敌人死亡的标志

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float HealthPercentage;

    // 胶囊体碰撞器 用于检测玩家是否进入目标区域
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UCapsuleComponent* AttackCollision;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UWidgetComponent* WidgetComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UClass* WidgetClass;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UROD_EnemyHealth_UserWidget* HealthWidget;

    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    class USoundCue* SufferSound; // 击中音效

    // 玩家与检测范围重叠时
    UFUNCTION()
    virtual void OnAttackOverlapBegin(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );
    // 玩家离开检测范围时
    UFUNCTION()
    virtual void OnAttackOverlapEnd(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex
    );

    // 攻击函数
    void Attack();
    // 更新血量函数
    void UpdateHealthPercentage(float CurrentHealth, float MaxHealth);
    // 敌人死亡函数
    void EnemyDead();
    // 怪物增强
    void EnemyPower(float HealthRate,float DamageRate,float BaseSpeedRate);

private:
    
    // 声明敌人最大血量并赋初值
    float EnemyMaxHealth = 100;
    // 声明敌人当前血量
    float EnemyCurrentHealth = 100;
    // 声明敌人可生成范围速度变量
    float MoveSpeed;
    // 声明敌人基础速度变量
    float BaseSpeed;
    // 声明实例化后敌人的速度
    float DamagePower = 10;

    // 设定死亡时间
    FTimerHandle TH_Death;

    // 声明对角色类的引用
    class AROD_Character* PlayerRef;
};

