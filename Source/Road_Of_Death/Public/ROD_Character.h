// 在项目设置的描述页中填写版权信息

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ROD_Combat_Interface.h"
#include "ROD_Character.generated.h"

UCLASS()
class ROAD_OF_DEATH_API AROD_Character : public ACharacter, public IROD_Combat_Interface
{
    GENERATED_BODY()

public:
    // 构造函数
    AROD_Character();

protected:
    // 游戏开始或实例化时调用
    virtual void BeginPlay() override;

public:
    // 每帧调用
    virtual void Tick(float DeltaTime) override;

    // 绑定输入功能
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // 造成伤害的接口函数
    virtual void CauseDamage(AActor* Causer, float BaseDamage) override;

    // 开火函数
    void Fire();

    // 开火冷却结束函数
    void FireTimerExpired();

    // 生成敌人函数
    void EnemySpawner();

    // 游戏结束处理函数
    void GameOver();

public:
    // 战斗相关属性
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
    bool bIsDead = false; // 角色是否死亡

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
    bool bIsGameOver = false; // 游戏是否结束

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool bCanAbilityUp = false; // 是否可以提升技能

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
    float HealthPercentage; // 血量百分比

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
    float PlayerCurrentHealth = 100; // 当前血量

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
    float PlayerMaxHealth = 100; // 最大血量

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
    float CurrentExp = 0; // 当前经验

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
    float MaxExp = 100; // 最大经验

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
    int32 Level = 1; // 角色等级

    int32 CurrentLevel = 1; // 当前角色等级

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
    float MoveSpeed = 200; // 移动速度

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
    float SpawnCD = 2.f; // 敌人生成CD

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
    float ProjectileCD = 0.3; // 发射物冷却时间

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
    int32 DamageClickNum = 0; // 伤害增加次数

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
    int32 KillNum = 0; // 击杀数

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
    int32 CurrentEnemy = 0; // 当前敌人数

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
    int32 MaxEnemyInWave = 5; // 波次最大敌人数

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
    int32 EnemyKillNumInWave = 0; // 波次击杀数

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
    int32 EnemyWave = 1; // 当前波次

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* TopDownCameraComponent; // 顶部相机组件

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom; // 相机摆臂组件

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
    USceneComponent* SpawnPoint; // 开火点场景组件

    UPROPERTY(EditDefaultsOnly, Category = Combat)
    TSubclassOf<class AROD_Enemy> EnemyClass; // 敌人类

    UPROPERTY(EditDefaultsOnly, Category = Projectile)
    TSubclassOf<class AROD_Projectile> ProjectileClass; // 发射物类

    UPROPERTY(EditAnywhere, Category = Combat)
    class ANavigationData* NavData; // 导航数据

    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    class USoundCue* ShootSound; // 射击音效

private:
    bool bCanFire = true; // 是否能开火

    FTimerHandle TH_Dead; // 玩家死亡计时器

    FTimerHandle TH_Spawn; // 生成敌人计时器

    FTimerHandle TH_FireExpired; // 开火冷却计时器

    class AROD_PlayerController* PCRef; // 玩家控制器引用
};

