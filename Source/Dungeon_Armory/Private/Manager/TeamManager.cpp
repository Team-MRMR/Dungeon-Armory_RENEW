// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/TeamManager.h"

void UTeamManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	TManagerBase<UTeamManager>::InitializeInstance(this);

	InitializeTeamRelationMap();
}

UTeamManager* UTeamManager::GetInstance()
{
	auto Instance = TManagerBase<UTeamManager>::GetInstance();
	if (Instance)
	{
		return Instance;
	}

    return nullptr;
}

TMap<ETeamType, ERelationType> UTeamManager::GetTeamRelation(ETeamType OneTeam) const
{
    return TeamRelationMap.FindRef(OneTeam).RelationMap;
}

void UTeamManager::SetTeamRelation(ETeamType TeamA, ETeamType TeamB, ERelationType Releation)
{
    // 대칭 관계 설정
	TeamRelationMap.FindOrAdd(TeamA).RelationMap.FindOrAdd(TeamB) = Releation;
	TeamRelationMap.FindOrAdd(TeamB).RelationMap.FindOrAdd(TeamA) = Releation;
}

ERelationType UTeamManager::GetRelation(ETeamType TeamA, ETeamType TeamB) const
{
    if (const auto TeamRelation = TeamRelationMap.Find(TeamA))
    {
        if (const auto Relation = TeamRelation->RelationMap.Find(TeamB))
        {
			return *Relation;
        }

        return ERelationType::None;
    }

    return ERelationType::None;
}

void UTeamManager::InitializeTeamRelationMap()
{
	TeamRelationMap.Empty();

	SetTeamRelation(ETeamType::Player, ETeamType::Player, ERelationType::Friendly);
	SetTeamRelation(ETeamType::Player, ETeamType::NPC, ERelationType::Friendly);
	SetTeamRelation(ETeamType::Player, ETeamType::Mob, ERelationType::Hostile);

	SetTeamRelation(ETeamType::NPC, ETeamType::NPC, ERelationType::Friendly);
	SetTeamRelation(ETeamType::NPC, ETeamType::Mob, ERelationType::Neutral);

	SetTeamRelation(ETeamType::Mob, ETeamType::Mob, ERelationType::Friendly);
}

