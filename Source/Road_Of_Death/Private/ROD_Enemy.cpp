
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
 	// �Ƿ���ÿ֡����
	PrimaryActorTick.bCanEverTick = true;

	// ��ʼ�����������
	AttackCollision = CreateDefaultSubobject<UCapsuleComponent>("AttackCollision");
	AttackCollision->SetupAttachment(RootComponent);
	AttackCollision->OnComponentBeginOverlap.AddDynamic(this, &AROD_Enemy::OnAttackOverlapBegin); // ���ص��¼�
	AttackCollision->OnComponentEndOverlap.AddDynamic(this,&AROD_Enemy::OnAttackOverlapEnd);      // ���뿪�¼�

	// ��ʼ���ؼ����
	WidgetClass = LoadObject<UClass>(nullptr, TEXT("/Game/Blueprints/UMG_EnemyHealth.UMG_EnemyHealth_C"));
	WidgetComp = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	WidgetComp->SetWidgetClass(WidgetClass);
	WidgetComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "head");
}

// ��Ϸ��ʼʱ��ʵ����ʱ������
void AROD_Enemy::BeginPlay()
{
	Super::BeginPlay();
	// ��ȡ��ɫ��
	PlayerRef = Cast<AROD_Character>(GetWorld()->GetFirstPlayerController()->GetPawn());

	// ���û����ٶ�
	BaseSpeed = 20;
	// �ٶȴ�С���
	MoveSpeed = FMath::RandRange(BaseSpeed / 1.5f, BaseSpeed * 1.5f);
	// ����Ѫ��
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

// -> ����˺�������
// -> Causer���˺��ķ�����
// -> BaseDamage����ҵ��˺�
void AROD_Enemy::CauseDamage(AActor* Causer, float BaseDamage)
{
	// ����˺�
	EnemyCurrentHealth -= BaseDamage;
	// ����Ѫ��
	UpdateHealthPercentage(EnemyCurrentHealth, EnemyMaxHealth);
	// �����ܻ�����Ч
	UGameplayStatics::PlaySoundAtLocation(this, SufferSound, GetActorLocation());
}

// -> �ƶ�����Һ�����
// -> DeltaTime��ÿ֡���ʱ��
void AROD_Enemy::MoveToPlayer(float DeltaTime)
{
	FVector OrientRotation = PlayerRef->GetActorLocation() - this->GetActorLocation();
	OrientRotation.Normalize();
	AddMovementInput(OrientRotation, MoveSpeed*DeltaTime);
	SetActorRotation(OrientRotation.Rotation());
}

// -> ��������
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

// -> ����Ѫ������
void AROD_Enemy::UpdateHealthPercentage(float CurrentHealth, float MaxHealth)
{
	// ����Ѫ��
	HealthPercentage = CurrentHealth / MaxHealth;
	// ����UI
	HealthWidget->SetCurrentHealth(CurrentHealth);
	HealthWidget->SetMaxHealth(MaxHealth);
	HealthWidget->SetPrograssBarPercent(HealthPercentage);
	// ����������ʱ
	if (EnemyCurrentHealth <= 0)
	{
		// �����˱��Ϊ����
		bIsDead = true;
		// ������ɫ������ʱ��
		GetWorld()->GetTimerManager().SetTimer(TH_Death, this, &AROD_Enemy::EnemyDead, 2.5f);

		// �رյ��˵���ײ
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

// ��������������
void AROD_Enemy::EnemyDead()
{
	Destroy();
	// ���������ʱ�����Ϊ������������
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

// ÿ֡����
void AROD_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerRef && EnemyCurrentHealth > 0 and !bIsAttack)
	{
		MoveToPlayer(DeltaTime);
	}
}

// ���빥����Χʱ����
void AROD_Enemy::OnAttackOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// ������빥����Χ������ң�����Ϊ���Թ���״̬
	if (OtherActor->ActorHasTag("player"))
	{
		bIsAttack = true;
	}
}

// �뿪������Χʱ����
void AROD_Enemy::OnAttackOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// ����뿪������Χ������ң�����Ϊ���ɹ���״̬
	if (OtherActor->ActorHasTag("player"))
	{
		bIsAttack = false;
	}
}
