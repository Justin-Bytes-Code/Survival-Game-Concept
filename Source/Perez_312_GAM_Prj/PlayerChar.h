// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Resource_M.h"
#include "Kismet/GameplayStatics.h"
#include "BuildingPart.h"
#include "PlayerWidget.h"
#include "ObjectiveWidget.h"
#include "PlayerChar.generated.h"




UCLASS()
class PEREZ_312_GAM_PRJ_API APlayerChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerChar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void MoveForward(float axisValue);

	UFUNCTION()
		void MoveRight(float axisValue);

	UFUNCTION()
		void StartJump();

	UFUNCTION()
		void StopJump();

	UFUNCTION()
		void LoseMenu();

	UFUNCTION()
		void FindObject();

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* PlayerCamComp;


	//Establishing Player Variables. 
	// Edit Anywhere is so you can edit inside unreal. 
	// Blue Print read write is so we can customize it anywhere.  
	// Player stats sorts it and adds a new section.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float Health = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float Hunger = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float Stamina = 100.0f;

	// Initializing Resources
	//------------------------------------------------------------//

	UPROPERTY(EditAnywhere, Category = "Resources")
		int Wood;

	UPROPERTY(EditAnywhere, Category = "Resources")
		int Stone;

	UPROPERTY(EditAnywhere, Category = "Resources")
		int Berry;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
		TArray<int> ResourcesArray;

	UPROPERTY(EditAnywhere, Category = "Resources")
		TArray<FString> ResourcesNameArray;

	UPROPERTY(EditAnywhere, Category = "HitMarker")
		UMaterialInterface* hitDecal;

	// Initializing Sound Effects
	//------------------------------------------------------------//
	UPROPERTY(EditAnywhere)
		class USoundBase* ClickSound;
	UPROPERTY(EditAnywhere)
		class USoundBase* BuildSound;
	UPROPERTY(EditAnywhere)
		class USoundBase* DepletedSound;

	// Initializing Building Supplies
	//------------------------------------------------------------//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Supplies")
		TArray<int> BuildingArray;

	UPROPERTY()
		bool isBuilding;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<ABuildingPart> BuildPartClass;

	UPROPERTY()
		ABuildingPart* spawnedPart;

	// Initializing Player UI
	//------------------------------------------------------------//
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UPlayerWidget* playerUI;

	// Initializing Objectives
	//------------------------------------------------------------//
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UObjectiveWidget* objWidget;

	UPROPERTY()
		float objectsBuilt;

	UPROPERTY()
		float matsCollected;

	// Initializing Set Player Parameters 
	//------------------------------------------------------------//
	UFUNCTION (BlueprintCallable)
		void SetHealth(float amount);

	UFUNCTION(BlueprintCallable)
		void SetHunger(float amount);

	UFUNCTION(BlueprintCallable)
		void SetStamina(float amount);

	// Initalizing Decreasing Player Parameters
	//------------------------------------------------------------//
	UFUNCTION (BlueprintCallable)
		void HurtHealth(float amount);


	// Initializing Gain Functions (Currently Unused) 
	//------------------------------------------------------------//
	UFUNCTION(BlueprintCallable)
		void GainStamina(float amount);

	// Initializing Decrease Player Parameters  
	//------------------------------------------------------------//
	UFUNCTION()
		void DecreaseStats();

	// Initializing Player Inventory 
	//------------------------------------------------------------//
	UFUNCTION()
		void GiveResource(float amount, FString resourceType);

	// Initializing Inventory
	//------------------------------------------------------------//

	UFUNCTION(BlueprintCallable)
		void UpdateResources(float woodAmount, float stoneAmount, FString buildingObject);

	UFUNCTION(BlueprintCallable)
		void SpawnBuilding(int buildingID, bool& isSuccess);

	// Initializing Rotate Function For Players
	//------------------------------------------------------------//
	UFUNCTION()
		void RotateBuilding();
};
