// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Characters/Core/AI/Team/TeamInfo.h"
#include "GenericTeamAgentInterface.h"

#include "TeamComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEON_ARMORY_API UTeamComponent : public UActorComponent, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

/***** Variables ******/
private:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Team", meta = (AllowPrivateAccess = "true"))
    ETeamType TeamType;
	FGenericTeamId TeamId;

/***** Functions *****/
public:	
	// Sets default values for this component's properties
	UTeamComponent();

    /** Retrieved owner attitude toward given Other object */
    virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

    /** Retrieve team identifier in form of FGenericTeamId */
    virtual FGenericTeamId GetGenericTeamId() const override;

    /** Assigns Team Agent to given TeamID */
    virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;

public:
    UFUNCTION(BlueprintCallable, Category = "Team")
    ETeamType GetTeamType() const;

    UFUNCTION(BlueprintCallable, Category = "Team")
    void SetTeamType(const ETeamType NewTeamType);
};
