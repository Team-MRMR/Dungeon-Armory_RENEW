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

void AGatherableActorBase::ReceiveDamage(float DamageAmount)
{

	CurrentHealth -= DamageAmount;
	if (0 <= CurrentHealth)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	}
	else
	{
		UGameplayStatics::PlaySoundAtLocation(this, DieSound, GetActorLocation());
		Die();
	}
}

