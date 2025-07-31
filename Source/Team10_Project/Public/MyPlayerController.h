#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class TEAM10_PROJECT_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMyPlayerController();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* CrouchAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* ToggleCrouchAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* ShootAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* ZoomAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* SprintAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* ReloadAction;
	
protected:
	virtual void BeginPlay() override;
	
	
};
