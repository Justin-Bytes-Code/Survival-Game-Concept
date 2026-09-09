// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerChar.h"

// Sets default values
APlayerChar::APlayerChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Camera Component blueprint
	PlayerCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Cam"));

	PlayerCamComp->SetupAttachment(GetMesh(), "head");

	PlayerCamComp->bUsePawnControlRotation = true;

	// Default Total Number Of Building Options Inside Array
	BuildingArray.SetNum(4);

	// Default Resources
	ResourcesArray.SetNum(3);
	ResourcesNameArray.Add(TEXT("Wood"));
	ResourcesNameArray.Add(TEXT("Stone"));
	ResourcesNameArray.Add(TEXT("Berry"));

	// Default Sounds (Set inside character Blue print) 
	ClickSound = CreateDefaultSubobject<USoundBase>(TEXT("Click Sound"));
	BuildSound = CreateDefaultSubobject<USoundBase>(TEXT("Build Sound"));
}

// Called when the game starts or when spawned
void APlayerChar::BeginPlay()
{
	Super::BeginPlay();
	
	// Decreases Player Stats overtime
	FTimerHandle StatsTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(StatsTimerHandle, this, &APlayerChar::DecreaseStats, 2.0f, true);
	
	// Updates Objective Widget 
	if (objWidget)
	{
		objWidget->UpdatebuildObj(0.0f);
		objWidget->UpdatematOBJ(0.0f);
	}
}

// Called every frame
void APlayerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Updates Player UI
	playerUI->UpdateBars(Health, Hunger, Stamina);


	if (isBuilding)
	{
		if (spawnedPart)
		{
			FVector StartLocation = PlayerCamComp->GetComponentLocation();
			FVector Direction = PlayerCamComp->GetForwardVector() * 400.0f;
			FVector EndLocation = StartLocation + Direction;
			spawnedPart->SetActorLocation(EndLocation);

		}
	}

}

// Called to bind functionality to input
void APlayerChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Inputs Player Can Make
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerChar::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerChar::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerChar::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerChar::AddControllerYawInput);
	PlayerInputComponent->BindAction("LoseMenu", IE_Pressed, this, &APlayerChar::LoseMenu);
	PlayerInputComponent->BindAction("JumpEvent", IE_Pressed, this, &APlayerChar::StartJump);
	PlayerInputComponent->BindAction("JumpEvent", IE_Released, this, &APlayerChar::StopJump);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerChar::FindObject);
	PlayerInputComponent->BindAction("RotPart", IE_Pressed, this, &APlayerChar::RotateBuilding); 
}

// Player Can Walk Forward
void APlayerChar::MoveForward(float axisValue)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, axisValue);
}
// Player Can Move Right
void APlayerChar::MoveRight(float axisValue)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, axisValue);
}
// Player can Jump 
void APlayerChar::StartJump()
{
	bPressedJump = true;
}
// Player Can't Jump
void APlayerChar::StopJump()
{
	bPressedJump = false; 
}

void APlayerChar::LoseMenu()
{

}

// Player Mouse Click Interaction
void APlayerChar::FindObject()
{
	FHitResult HitResult;
	FVector StartLocation = PlayerCamComp->GetComponentLocation();
	FVector Direction = PlayerCamComp->GetForwardVector() * 800.0f;
	FVector EndLocation = StartLocation + Direction;

	FCollisionQueryParams QuaryParams;
	QuaryParams.AddIgnoredActor(this);
	QuaryParams.bTraceComplex = true;
	QuaryParams.bReturnFaceIndex = true;

	if (!isBuilding)
	{
		// Allows Player to Collect Resource
		if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QuaryParams))
		{
			AResource_M* HitResource = Cast<AResource_M>(HitResult.GetActor());

			// Allows Player to collect resource if stamina is above certain threshold
			if (Stamina > 5.0f)
			{
				// If the player does connect with a item harvest the item
				if (HitResource)
				{
					float RandomPitch = FMath::FRandRange(0.90f, 1.00f);

					UGameplayStatics::PlaySound2D(this, ClickSound, RandomPitch);
					FString hitName = HitResource->resourceName;
					int resourceValue = HitResource->resourceAmount;

					HitResource->totalResource = HitResource->totalResource - resourceValue;

					// Spawns Building Material
					if (HitResource->totalResource > resourceValue)
					{
						GiveResource(resourceValue, hitName);

						matsCollected = matsCollected + resourceValue;

						objWidget->UpdatematOBJ(matsCollected);

						check(GEngine != nullptr);

						UGameplayStatics::SpawnDecalAtLocation(GetWorld(), hitDecal, FVector(10.0f, 10.0f, 10.0f), HitResult.Location, FRotator(-90, 0, 0), 2.0f);

						SetStamina(-5.0f);

					}
					// Error Control & resource depletion
					else
					{
						HitResource->Destroy();
						check(GEngine != nullptr);
						UGameplayStatics::PlaySound2D(this, DepletedSound, RandomPitch);

					}
				}
			}
		}

	}

	else
	{
		isBuilding = false;
		objectsBuilt = objectsBuilt + 1.0f;

		objWidget->UpdatebuildObj(objectsBuilt);
	}


}
// Allows Player to Gain Health
void APlayerChar::SetHealth(float amount)
{
	if (Health + amount <= 100)
	{
		Health = Health + amount;
	}
	else
	{
		Health = 100;
	}
}

void APlayerChar::HurtHealth(float amount)
{
	if (Health - amount > 0)
	{
		Health = Health - amount;
	}
	else if (Health - amount < 0)
	{
		Health = 0;
	}
	else if (Health == amount)
	{
		Health = 0;
	}
}


// Allows Player to Gain Hunger
void APlayerChar::SetHunger(float amount)
{
	if (Hunger + amount < 100)
	{
		Hunger = Hunger += amount;
	}
	else {
		Hunger = 100;
	}
}

// Allows Player to Gain Stamina
void APlayerChar::SetStamina(float amount)
{
	if (Stamina + amount < 100)
	{
		Stamina = Stamina + amount;
	}
	else if (Stamina + amount > 100)
	{
		Stamina = 100;
	}
}

// Allows Player to Gain Stamina (Currently Unused) 
void APlayerChar::GainStamina(float amount)
{

}
	
// Decreases Player Health, Hunger, and Stamina if certain certain variables hit 0.
void APlayerChar::DecreaseStats()
{
	if (Hunger > 0)
	{
		SetHunger(-1.0f);
	}
	
	SetStamina(10.0f);

	if (Hunger <= 0)
	{
		SetHealth(-3.0f);
	}
}

// Allows Add resources to player inventory when interacting.
// The amount is based on the resource they are mining
void APlayerChar::GiveResource(float amount, FString resourceType)
{

	if (resourceType == "Wood")
	{
		ResourcesArray[0] += amount;
	}
	if (resourceType == "Stone")
	{
		ResourcesArray[1] += amount;
	}
	if (resourceType == "Berry")
	{
		ResourcesArray[2] +=  amount;
	}
	
}

// Updates Players Inventory 
void APlayerChar::UpdateResources(float woodAmount, float stoneAmount, FString buildingObject)
{
	if (woodAmount <= ResourcesArray[0])
	{
		if (stoneAmount <= ResourcesArray[1])
		{
			ResourcesArray[0] = ResourcesArray[0] - woodAmount;
			ResourcesArray[1] = ResourcesArray[1] - stoneAmount;
			
			//All Building types. If your adding more buildings add them here. 
			if (buildingObject == "Wall")
			{
				BuildingArray[0] = BuildingArray[0] + 1;
			}
			if (buildingObject == "Floor")
			{
				BuildingArray[1] = BuildingArray[1] + 1;
			}
			if (buildingObject == "Ceiling")
			{
				BuildingArray[2] = BuildingArray[2] + 1;
			}
			if (buildingObject == "Stairs")
			{
				BuildingArray[3] = BuildingArray[3] + 1;
			}
		}
	}
}

// Spawning Building Function
void APlayerChar::SpawnBuilding(int buildingID, bool& isSuccess)
{
	if (!isBuilding)
	{
		if (BuildingArray[buildingID] >= 1)
		{
			isBuilding = true;
			FActorSpawnParameters SpawnParams;
			FVector StartLocation = PlayerCamComp->GetComponentLocation();
			FVector Direction = PlayerCamComp->GetForwardVector() * 400.0f;
			FVector EndLocation = StartLocation + Direction; 
			FRotator myRot(0, 0, 0);

			BuildingArray[buildingID] = BuildingArray[buildingID] - 1;

			spawnedPart = GetWorld()->SpawnActor<ABuildingPart>(BuildPartClass, EndLocation, myRot, SpawnParams);
				
			isSuccess = true;


		}

		isSuccess = false;
	}
}

// Player Presses E to Rotate Building
void APlayerChar::RotateBuilding()
{
	if (isBuilding)
	{
		spawnedPart->AddActorWorldRotation(FRotator(0, 90, 0));
	}
}

