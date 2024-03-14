// 填写版权信息

#include "ROD_Character.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "NavigationSystem.h"
#include "ROD_Enemy.h"
#include "ROD_Projectile.h"
#include "Sound/SoundCue.h"
#include "ROD_PlayerController.h"

// 构造函数
AROD_Character::AROD_Character()
{
    // 允许Tick函数
    PrimaryActorTick.bCanEverTick = true;

    // 创建相机摆臂组件并设置初始值
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->SetUsingAbsoluteRotation(true); // 不跟随角色旋转
    CameraBoom->TargetArmLength = 800.f;
    CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
    CameraBoom->bDoCollisionTest = false; // 避免相机被遮挡

    // 创建顶部相机组件并设置初始值
    TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
    TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    TopDownCameraComponent->bUsePawnControlRotation = false; // 相机不跟随摆臂旋转

    // 创建开火点场景组件
    SpawnPoint = CreateDefaultSubobject<USceneComponent>("SpawnPoint");
    SpawnPoint->SetupAttachment(RootComponent);
}

// 游戏开始或实例化时调用
void AROD_Character::BeginPlay()
{
    Super::BeginPlay();

    // 计算当前血量百分比
    HealthPercentage = PlayerCurrentHealth / PlayerMaxHealth;

    // 设置生成敌人的定时器
    GetWorld()->GetTimerManager().SetTimer(TH_Spawn, this, &AROD_Character::EnemySpawner, SpawnCD, true);
}

// 每帧调用
void AROD_Character::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// 绑定输入功能
void AROD_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// 接收敌人伤害的函数
void AROD_Character::CauseDamage(AActor* Causer, float EnemyDamage)
{
    // 减少角色血量
    PlayerCurrentHealth -= EnemyDamage;

    // 更新血量百分比
    HealthPercentage = PlayerCurrentHealth / PlayerMaxHealth;

    // 判断角色是否死亡
    if (PlayerCurrentHealth <= 0)
    {
        PlayerCurrentHealth = 0;
        bIsDead = true;

        // 设置死亡计时器
        GetWorld()->GetTimerManager().SetTimer(TH_Dead, this, &AROD_Character::GameOver, 2.f);

        // 禁用碰撞
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

        // 获取玩家控制器并禁用输入
        PCRef = Cast<AROD_PlayerController>(GetController());
        if (PCRef)
        {
            DisableInput(PCRef);
        }
    }
}

// 开火函数
void AROD_Character::Fire()
{
    UWorld* World = GetWorld();
    PCRef = Cast<AROD_PlayerController>(GetController());

    // 检查是否可以开火并且玩家是否按下开火按钮
    if (bCanFire && PCRef->bIsFiringPressed)
    {
        // 设置开火冷却定时器
        World->GetTimerManager().SetTimer(TH_FireExpired, this, &AROD_Character::FireTimerExpired, ProjectileCD);
        bCanFire = false;

        // 播放射击音效
        UGameplayStatics::PlaySoundAtLocation(this, ShootSound, GetActorLocation());

        // 在开火点生成发射物
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = GetInstigator();
        AROD_Projectile* Projectile = World->SpawnActor<AROD_Projectile>(ProjectileClass, SpawnPoint->GetComponentLocation(),
            SpawnPoint->GetComponentRotation(), SpawnParams);

        // 根据伤害增加次数调整发射物伤害
        for (int i = 0; i < DamageClickNum; i++)
        {
            Projectile->damageVal *= 1.1;
        }

        if (Projectile)
        {
            // 设置发射物初始速度
            FVector LaunchDirection = GetActorForwardVector();
            Projectile->FireInDirection(LaunchDirection);
        }
    }
}

// 开火冷却结束函数
void AROD_Character::FireTimerExpired()
{
    bCanFire = true;
}

// 生成敌人函数
void AROD_Character::EnemySpawner()
{
    // 检查当前敌人数量是否小于最大波次敌人数量
    if (CurrentEnemy < MaxEnemyInWave)
    {
        FNavLocation RandomPoint;
        UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

        // 获取随机可达点
        if (NavSystem)
        {
            bool bFoundPoint = NavSystem->GetRandomReachablePointInRadius(GetActorLocation(), 1000.f, RandomPoint);
            if (bFoundPoint)
            {
                // 在随机点位置生成敌人
                FActorSpawnParameters SpawnParams;
                AROD_Enemy* Enemy = GetWorld()->SpawnActor<AROD_Enemy>(EnemyClass, RandomPoint.Location, FRotator::ZeroRotator, SpawnParams);
                CurrentEnemy += 1;
            }
        }
    }
}

// 游戏结束处理函数
void AROD_Character::GameOver()
{
    // 暂停游戏
    UGameplayStatics::SetGamePaused(GetWorld(), true);
    bIsGameOver = true;
}

