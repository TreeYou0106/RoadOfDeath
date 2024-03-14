
#include "ROD_Enemy.h"
#include "ROD_Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "ROD_EnemyHealth_UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ROD_Combat_Interface.h"
#include "Sound/SoundCue.h"

// Sets default values
AROD_Enemy::AROD_Enemy()
{
 	// 是否开启每帧调用
	PrimaryActorTick.bCanEverTick = true;

	// 初始化胶囊体组件
	AttackCollision = CreateDefaultSubobject<UCapsuleComponent>("AttackCollision");
	AttackCollision->SetupAttachment(RootComponent);
	AttackCollision->OnComponentBeginOverlap.AddDynamic(this, &AROD_Enemy::OnAttackOverlapBegin); // 绑定重叠事件
	AttackCollision->OnComponentEndOverlap.AddDynamic(this,&AROD_Enemy::OnAttackOverlapEnd);      // 绑定离开事件

	// 初始化控件组件
	WidgetClass = LoadObject<UClass>(nullptr, TEXT("/Game/Blueprints/UMG_EnemyHealth.UMG_EnemyHealth_C"));
	WidgetComp = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	WidgetComp->SetWidgetClass(WidgetClass);
	WidgetComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "head");
}

// 游戏开始时或实例化时被调用
void AROD_Enemy::BeginPlay()
{
	Super::BeginPlay();
	// 获取角色类
	PlayerRef = Cast<AROD_Character>(GetWorld()->GetFirstPlayerController()->GetPawn());

	// 设置基础速度
	BaseSpeed = 20;
	// 速度大小随机
	MoveSpeed = FMath::RandRange(BaseSpeed / 1.5f, BaseSpeed * 1.5f);
	// 更新血量
	// HealthPercentage = EnemyCurrentHealth / EnemyMaxHealth;
	HealthWidget = Cast<UROD_EnemyHealth_UserWidget>(WidgetComp->GetWidget());
	EnemyPower(2.f * (PlayerRef->EnemyWave - 1), 2.f * (PlayerRef->EnemyWave - 1), 0);
	if (PlayerRef->EnemyWave > 5)
	{
		EnemyPower(10.f * (PlayerRef->EnemyWave - 1), 10.f * (PlayerRef->EnemyWave - 1), 5.f * (PlayerRef->EnemyWave - 1));
	}
	if (PlayerRef->EnemyWave > 10)
	{
		PlayerRef->SpawnCD /= FMath::Pow(1.1f, PlayerRef->EnemyWave);
	}
	UpdateHealthPercentage(EnemyCurrentHealth, EnemyMaxHealth);
}

// -> 造成伤害函数：
// -> Causer：伤害的发起者
// -> BaseDamage：玩家的伤害
void AROD_Enemy::CauseDamage(AActor* Causer, float BaseDamage)
{
	// 造成伤害
	EnemyCurrentHealth -= BaseDamage;
	// 更新血量
	UpdateHealthPercentage(EnemyCurrentHealth, EnemyMaxHealth);
	// 播放受击打音效
	UGameplayStatics::PlaySoundAtLocation(this, SufferSound, GetActorLocation());
}

// -> 移动至玩家函数：
// -> DeltaTime：每帧间隔时间
void AROD_Enemy::MoveToPlayer(float DeltaTime)
{
	FVector OrientRotation = PlayerRef->GetActorLocation() - this->GetActorLocation();
	OrientRotation.Normalize();
	AddMovementInput(OrientRotation, MoveSpeed*DeltaTime);
	SetActorRotation(OrientRotation.Rotation());
}

// -> 攻击函数
void AROD_Enemy::Attack()
{
	if (bIsAttack)
	{
		if (IROD_Combat_Interface* Interface = Cast<IROD_Combat_Interface>(PlayerRef))
		{
			Interface->CauseDamage(this, DamagePower);
		}
	}
}

// -> 更新血条函数
void AROD_Enemy::UpdateHealthPercentage(float CurrentHealth, float MaxHealth)
{
	// 更新血条
	HealthPercentage = CurrentHealth / MaxHealth;
	// 更新UI
	HealthWidget->SetCurrentHealth(CurrentHealth);
	HealthWidget->SetMaxHealth(MaxHealth);
	HealthWidget->SetPrograssBarPercent(HealthPercentage);
	// 当敌人死亡时
	if (EnemyCurrentHealth <= 0)
	{
		// 将敌人标记为死亡
		bIsDead = true;
		// 开启角色死亡计时器
		GetWorld()->GetTimerManager().SetTimer(TH_Death, this, &AROD_Enemy::EnemyDead, 2.5f);

		// 关闭敌人的碰撞
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WidgetComp->SetVisibility(false);
		GetMesh()->SetEnableGravity(false);

		PlayerRef->KillNum += 1;
		PlayerRef->EnemyKillNumInWave += 1;
		PlayerRef->CurrentExp += (int)(MaxHealth / 6 + FMath::RandRange(-PlayerRef->EnemyWave, PlayerRef->EnemyWave));
		if (PlayerRef->EnemyKillNumInWave >= PlayerRef->MaxEnemyInWave)
		{
			PlayerRef->EnemyKillNumInWave = 0;
			PlayerRef->MaxEnemyInWave *= 1.2f;
			PlayerRef->CurrentEnemy = 0;
			PlayerRef->EnemyWave += 1;
		}
		if (PlayerRef->CurrentExp >= PlayerRef->MaxExp)
		{
			PlayerRef->Level += 1;
			PlayerRef->CurrentExp -= PlayerRef->MaxExp;
			PlayerRef->MaxExp = (int32)(PlayerRef->MaxExp * 1.1);
		}
	}
}

// 敌人死亡处理函数
void AROD_Enemy::EnemyDead()
{
	Destroy();
	// 当玩家升级时，标记为可以升级技能
	if (PlayerRef->Level != PlayerRef->CurrentLevel)
	{
		PlayerRef->CurrentLevel = PlayerRef->Level;
		PlayerRef->bCanAbilityUp = true;
	}
}

void AROD_Enemy::EnemyPower(float HealthRate, float DamageRate,float BaseSpeedRate)
{
	EnemyMaxHealth *= (1 + HealthRate/100);
	EnemyCurrentHealth *= (1 + HealthRate/100);
	DamagePower *= (1 + DamageRate/100);
	MoveSpeed *= (1 + BaseSpeedRate/100);
}

// 每帧调用
void AROD_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerRef && EnemyCurrentHealth > 0 and !bIsAttack)
	{
		MoveToPlayer(DeltaTime);
	}
}

// 进入攻击范围时调用
void AROD_Enemy::OnAttackOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 如果进入攻击范围的是玩家，设置为可以攻击状态
	if (OtherActor->ActorHasTag("player"))
	{
		bIsAttack = true;
	}
}

// 离开攻击范围时调用
void AROD_Enemy::OnAttackOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 如果离开攻击范围的是玩家，设置为不可攻击状态
	if (OtherActor->ActorHasTag("player"))
	{
		bIsAttack = false;
	}
}
