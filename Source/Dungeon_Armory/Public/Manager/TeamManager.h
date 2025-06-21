// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Manager/ManagerBase.h"
#include "GenericTeamAgentInterface.h"
#include "Characters/Core/AI/Team/TeamInfo.h"

#include "TeamManager.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEON_ARMORY_API UTeamManager : public UManagerBase
{
	GENERATED_BODY()
	
/***** Variables *****/
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team", meta = (AllowPrivateAccess = "true"))
	TMap<ETeamType, FTeamRelation> TeamRelationMap;

/***** Functions (Unreal) *****/
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

/***** Functions (ManagerBase) *****/
public:
	static UTeamManager* GetInstance();

/***** Functions (Team) *****/
public:
	UFUNCTION(BlueprintCallable, Category = "Team")
	// 팀의 모든 관계 맵을 가져오는 함수
	TMap<ETeamType, ERelationType> GetTeamRelation(ETeamType TeamA) const;

	UFUNCTION(BlueprintCallable, Category = "Team")
	// 팀의 모든 관계 맵을 설정하는 함수
	void SetTeamRelation(ETeamType TeamA, ETeamType TeamB, ERelationType Releation);

	UFUNCTION(BlueprintCallable, Category = "Team")
	// 팀간의 관계를 가져오는 함수
	ERelationType GetRelation(ETeamType TeamA, ETeamType TeamB) const;

protected:
	UFUNCTION(BlueprintCallable, Category = "Team")
	void InitializeTeamRelationMap();

};
