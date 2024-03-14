// ��д��Ȩ��Ϣ

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

// ���캯��
AROD_Character::AROD_Character()
{
    // ����Tick����
    PrimaryActorTick.bCanEverTick = true;

    // ��������ڱ���������ó�ʼֵ
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->SetUsingAbsoluteRotation(true); // �������ɫ��ת
    CameraBoom->TargetArmLength = 800.f;
    CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
    CameraBoom->bDoCollisionTest = false; // ����������ڵ�

    // �������������������ó�ʼֵ
    TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
    TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    TopDownCameraComponent->bUsePawnControlRotation = false; // ���������ڱ���ת

    // ��������㳡�����
    SpawnPoint = CreateDefaultSubobject<USceneComponent>("SpawnPoint");
    SpawnPoint->SetupAttachment(RootComponent);
}

// ��Ϸ��ʼ��ʵ����ʱ����
void AROD_Character::BeginPlay()
{
    Super::BeginPlay();

    // ���㵱ǰѪ���ٷֱ�
    HealthPercentage = PlayerCurrentHealth / PlayerMaxHealth;

    // �������ɵ��˵Ķ�ʱ��
    GetWorld()->GetTimerManager().SetTimer(TH_Spawn, this, &AROD_Character::EnemySpawner, SpawnCD, true);
}

// ÿ֡����
void AROD_Character::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// �����빦��
void AROD_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// ���յ����˺��ĺ���
void AROD_Character::CauseDamage(AActor* Causer, float EnemyDamage)
{
    // ���ٽ�ɫѪ��
    PlayerCurrentHealth -= EnemyDamage;

    // ����Ѫ���ٷֱ�
    HealthPercentage = PlayerCurrentHealth / PlayerMaxHealth;

    // �жϽ�ɫ�Ƿ�����
    if (PlayerCurrentHealth <= 0)
    {
        PlayerCurrentHealth = 0;
        bIsDead = true;

        // ����������ʱ��
        GetWorld()->GetTimerManager().SetTimer(TH_Dead, this, &AROD_Character::GameOver, 2.f);

        // ������ײ
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

        // ��ȡ��ҿ���������������
        PCRef = Cast<AROD_PlayerController>(GetController());
        if (PCRef)
        {
            DisableInput(PCRef);
        }
    }
}

// ������
void AROD_Character::Fire()
{
    UWorld* World = GetWorld();
    PCRef = Cast<AROD_PlayerController>(GetController());

    // ����Ƿ���Կ���������Ƿ��¿���ť
    if (bCanFire && PCRef->bIsFiringPressed)
    {
        // ���ÿ�����ȴ��ʱ��
        World->GetTimerManager().SetTimer(TH_FireExpired, this, &AROD_Character::FireTimerExpired, ProjectileCD);
        bCanFire = false;

        // ���������Ч
        UGameplayStatics::PlaySoundAtLocation(this, ShootSound, GetActorLocation());

        // �ڿ�������ɷ�����
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = GetInstigator();
        AROD_Projectile* Projectile = World->SpawnActor<AROD_Projectile>(ProjectileClass, SpawnPoint->GetComponentLocation(),
            SpawnPoint->GetComponentRotation(), SpawnParams);

        // �����˺����Ӵ��������������˺�
        for (int i = 0; i < DamageClickNum; i++)
        {
            Projectile->damageVal *= 1.1;
        }

        if (Projectile)
        {
            // ���÷������ʼ�ٶ�
            FVector LaunchDirection = GetActorForwardVector();
            Projectile->FireInDirection(LaunchDirection);
        }
    }
}

// ������ȴ��������
void AROD_Character::FireTimerExpired()
{
    bCanFire = true;
}

// ���ɵ��˺���
void AROD_Character::EnemySpawner()
{
    // ��鵱ǰ���������Ƿ�С����󲨴ε�������
    if (CurrentEnemy < MaxEnemyInWave)
    {
        FNavLocation RandomPoint;
        UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

        // ��ȡ����ɴ��
        if (NavSystem)
        {
            bool bFoundPoint = NavSystem->GetRandomReachablePointInRadius(GetActorLocation(), 1000.f, RandomPoint);
            if (bFoundPoint)
            {
                // �������λ�����ɵ���
                FActorSpawnParameters SpawnParams;
                AROD_Enemy* Enemy = GetWorld()->SpawnActor<AROD_Enemy>(EnemyClass, RandomPoint.Location, FRotator::ZeroRotator, SpawnParams);
                CurrentEnemy += 1;
            }
        }
    }
}

// ��Ϸ����������
void AROD_Character::GameOver()
{
    // ��ͣ��Ϸ
    UGameplayStatics::SetGamePaused(GetWorld(), true);
    bIsGameOver = true;
}

