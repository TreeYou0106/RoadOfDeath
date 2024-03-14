// ����Ŀ���õ�����ҳ����д��Ȩ��Ϣ

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
    // ���캯��
    AROD_Character();

protected:
    // ��Ϸ��ʼ��ʵ����ʱ����
    virtual void BeginPlay() override;

public:
    // ÿ֡����
    virtual void Tick(float DeltaTime) override;

    // �����빦��
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // ����˺��Ľӿں���
    virtual void CauseDamage(AActor* Causer, float BaseDamage) override;

    // ������
    void Fire();

    // ������ȴ��������
    void FireTimerExpired();

    // ���ɵ��˺���
    void EnemySpawner();

    // ��Ϸ����������
    void GameOver();

public:
    // ս���������
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
    bool bIsDead = false; // ��ɫ�Ƿ�����

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
    bool bIsGameOver = false; // ��Ϸ�Ƿ����

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool bCanAbilityUp = false; // �Ƿ������������

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
    float HealthPercentage; // Ѫ���ٷֱ�

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
    float PlayerCurrentHealth = 100; // ��ǰѪ��

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
    float PlayerMaxHealth = 100; // ���Ѫ��

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
    float CurrentExp = 0; // ��ǰ����

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
    float MaxExp = 100; // �����

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
    int32 Level = 1; // ��ɫ�ȼ�

    int32 CurrentLevel = 1; // ��ǰ��ɫ�ȼ�

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
    float MoveSpeed = 200; // �ƶ��ٶ�

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
    float SpawnCD = 2.f; // ��������CD

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
    float ProjectileCD = 0.3; // ��������ȴʱ��

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
    int32 DamageClickNum = 0; // �˺����Ӵ���

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
    int32 KillNum = 0; // ��ɱ��

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
    int32 CurrentEnemy = 0; // ��ǰ������

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
    int32 MaxEnemyInWave = 5; // ������������

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
    int32 EnemyKillNumInWave = 0; // ���λ�ɱ��

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
    int32 EnemyWave = 1; // ��ǰ����

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* TopDownCameraComponent; // ����������

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom; // ����ڱ����

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
    USceneComponent* SpawnPoint; // ����㳡�����

    UPROPERTY(EditDefaultsOnly, Category = Combat)
    TSubclassOf<class AROD_Enemy> EnemyClass; // ������

    UPROPERTY(EditDefaultsOnly, Category = Projectile)
    TSubclassOf<class AROD_Projectile> ProjectileClass; // ��������

    UPROPERTY(EditAnywhere, Category = Combat)
    class ANavigationData* NavData; // ��������

    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    class USoundCue* ShootSound; // �����Ч

private:
    bool bCanFire = true; // �Ƿ��ܿ���

    FTimerHandle TH_Dead; // ���������ʱ��

    FTimerHandle TH_Spawn; // ���ɵ��˼�ʱ��

    FTimerHandle TH_FireExpired; // ������ȴ��ʱ��

    class AROD_PlayerController* PCRef; // ��ҿ���������
};

