// Fill out your copyright notice in the Description page of Project Settings.


#include "GatherableActor/GatherableActorBase.h"

// sound
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGatherableActorBase::AGatherableActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AGatherableActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGatherableActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGatherableActorBase::ReceiveDamage_Implementation(float DamageAmount)
{

	CurrentHealth -= DamageAmount;
	if (0 <= CurrentHealth)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	}
	else
	{
		UGameplayStatics::PlaySoundAtLocation(this, DieSound, GetActorLocation());
		Execute_Die(this);
	}
}

void AGatherableActorBase::Die_Implementation()
{
	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);		// 캡슐 콜리전 비활성화
	//GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);					// 메쉬 콜리전 활성화
	//GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);	// 메쉬 콜리전 무시

	//// 행동 종료
	//DetachFromControllerPendingDestroy();
	SetLifeSpan(0.01f); // 5초 뒤 제거
}
