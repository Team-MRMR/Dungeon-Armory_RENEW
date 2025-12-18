// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Core/AI/Team/TeamComponent.h"
#include "Manager/TeamManager.h"

// Sets default values for this component's properties
UTeamComponent::UTeamComponent()
{
    TeamType = ETeamType::None;
	TeamId = FGenericTeamId::NoTeam;
}

void UTeamComponent::BeginPlay()
{
    Super::BeginPlay();
}

ETeamAttitude::Type UTeamComponent::GetTeamAttitudeTowards(const AActor& Other) const
{
    const UTeamComponent* OtherTeamComponent = Other.GetComponentByClass<const UTeamComponent>();
    
    if (OtherTeamComponent)
    {
        ETeamType OwnTeamType = GetTeamType();
        ETeamType OtherTeamType = OtherTeamComponent->GetTeamType();

        switch (UTeamManager::GetInstance()->GetRelation(OwnTeamType, OtherTeamType))
        {
        case ERelationType::Friendly:
            return ETeamAttitude::Friendly;
        case ERelationType::Hostile:
            return ETeamAttitude::Hostile;
        case ERelationType::Neutral:
			UE_LOG(LogTemp, Error, TEXT("%d(%s) and %d(%s) => Neutral"),
                static_cast<int32>(OwnTeamType), *GetOwner()->GetName(),
                static_cast<int32>(OtherTeamType), *Other.GetName());
            return ETeamAttitude::Neutral;
        default:
            UE_LOG(LogTemp, Error, TEXT("%d(%s) and %d(%s) => Undefined"),
                static_cast<int32>(OwnTeamType), *GetOwner()->GetName(),
                static_cast<int32>(OtherTeamType), *Other.GetName());
			return ETeamAttitude::Neutral;
        }
    }

    return ETeamAttitude::Neutral;
}

ETeamType UTeamComponent::GetTeamType() const
{
	return static_cast<ETeamType>(TeamId.GetId());
}

void UTeamComponent::SetTeamType(const ETeamType NewTeamType)
{
    TeamType = NewTeamType;
	SetGenericTeamId(FGenericTeamId(static_cast<uint8>(NewTeamType)));
}

FGenericTeamId UTeamComponent::GetGenericTeamId() const
{
    return TeamId;
}

void UTeamComponent::SetGenericTeamId(const FGenericTeamId& NewTeamId)
{
    TeamId = NewTeamId;
}

