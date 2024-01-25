// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tasks/Pipe.h"
#include "TaskActor.generated.h"

UCLASS()
class TASKSYSTEMSAMPLE_API ATaskActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATaskActor();

protected:
	virtual void BeginPlay() override;

public:	

    UFUNCTION(BlueprintCallable)
	void TaskLaunch();
	
	UFUNCTION(BlueprintCallable)
	void TaskWait();
	
	UFUNCTION(BlueprintCallable)
	void TaskBusyWait();
	
	UFUNCTION(BlueprintCallable)
	void TaskGetResult();
	
	UFUNCTION(BlueprintCallable)
	void TaskPrerequisites();
	
	UFUNCTION(BlueprintCallable)
	void TaskNested();
	
	UFUNCTION(BlueprintCallable)
	void TaskPipe();
	
	UFUNCTION(BlueprintCallable)
	void TaskEvent();

	UE::Tasks::FPipe Pipe{ TEXT("Pipe") };
};
