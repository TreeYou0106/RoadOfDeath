// Fill out your copyright notice in the Description page of Project Settings.
#include "ROD_Projectile.h"
#include "Components/SphereComponent.h"
#include "ROD_Combat_Interface.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AROD_Projectile::AROD_Projectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("Collision");
	CollisionComponent->InitSphereRadius(15.0f);
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AROD_Projectile::OnOverlapBegin);
	RootComponent = CollisionComponent;

    // 使用此组件驱动发射物的移动。
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
    ProjectileMovementComponent->InitialSpeed = 3000.0f;
    ProjectileMovementComponent->MaxSpeed = 3000.0f;
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->bShouldBounce = true;
    ProjectileMovementComponent->Bounciness = 0.3f;
    ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

}

// Called when the game starts or when spawned
void AROD_Projectile::BeginPlay()
{
	Super::BeginPlay();
    GetWorld()->GetTimerManager().SetTimer(TH_Destory, this,&AROD_Projectile::DestorySelf, 1.5f);
}

void AROD_Projectile::FireInDirection(const FVector& ShootDirection)
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AROD_Projectile::DestorySelf()
{
    Destroy();
}

void AROD_Projectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if ((OtherActor != nullptr) && (OtherActor->GetClass() != this->GetClass()) && !(OtherActor->ActorHasTag("player")))
    {
        //GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, "Overlapped!");
        if (IROD_Combat_Interface* Interface = Cast<IROD_Combat_Interface>(OtherActor)) 
        {
            Interface->CauseDamage(this, damageVal);
            CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            DestorySelf();
        }
    }
}

// Called every frame
void AROD_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

