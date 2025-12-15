// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Mob/Boss/DropRock.h"

// Sets default values
ADropRock::ADropRock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADropRock::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADropRock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

