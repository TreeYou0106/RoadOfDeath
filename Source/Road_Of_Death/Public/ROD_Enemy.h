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
    // ���캯��
    AROD_Enemy();

protected:
    // ��Ϸ��ʼ��ʵ����ʱ������
    virtual void BeginPlay() override;

    // �ܵ���ҹ���
    virtual void CauseDamage(AActor* Causer, float BaseDamage);

    // ��������ƶ�
    void MoveToPlayer(float DeltaTime);

public:
    // ÿ֡����
    virtual void Tick(float DeltaTime) override;

    // �����Ƿ񹥻��ı�־
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bIsAttack = false;
    // �������������ı�־
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bIsDead = false;
    // �������������ı�־

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float HealthPercentage;

    // ��������ײ�� ���ڼ������Ƿ����Ŀ������
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UCapsuleComponent* AttackCollision;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UWidgetComponent* WidgetComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UClass* WidgetClass;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UROD_EnemyHealth_UserWidget* HealthWidget;

    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    class USoundCue* SufferSound; // ������Ч

    // ������ⷶΧ�ص�ʱ
    UFUNCTION()
    virtual void OnAttackOverlapBegin(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );
    // ����뿪��ⷶΧʱ
    UFUNCTION()
    virtual void OnAttackOverlapEnd(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex
    );

    // ��������
    void Attack();
    // ����Ѫ������
    void UpdateHealthPercentage(float CurrentHealth, float MaxHealth);
    // ������������
    void EnemyDead();
    // ������ǿ
    void EnemyPower(float HealthRate,float DamageRate,float BaseSpeedRate);

private:
    
    // �����������Ѫ��������ֵ
    float EnemyMaxHealth = 100;
    // �������˵�ǰѪ��
    float EnemyCurrentHealth = 100;
    // �������˿����ɷ�Χ�ٶȱ���
    float MoveSpeed;
    // �������˻����ٶȱ���
    float BaseSpeed;
    // ����ʵ��������˵��ٶ�
    float DamagePower = 10;

    // �趨����ʱ��
    FTimerHandle TH_Death;

    // �����Խ�ɫ�������
    class AROD_Character* PlayerRef;
};

