// Made by Chen aka Herosama

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Http.h"
#include "MinecraftSkinSubsystem.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FMinecraftPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString PlayerName = "EmptyName";

	UPROPERTY(BlueprintReadOnly)
	FString MinecraftPlayerUUID = "853c80ef3c3749fdaa49938b674adae6";

	UPROPERTY(BlueprintReadOnly)
	FString SkinURL = "https://www.minecraft.net/en-us/profile/skin/steve.png";

	UPROPERTY(BlueprintReadOnly)
	UTexture* SkinTexture = nullptr;
};

UCLASS()
class UNMINECRAFT_API UMinecraftSkinSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	

public:
	// Fucntions
	UFUNCTION(BlueprintCallable, Category = "MinecraftSkinSubsystem")
	FString GetPlayerUUID(const FString MinecraftPlayerName);

	void OnGetPlayerUUIDComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);
	// UUID part


	// Properties

	UPROPERTY(BlueprintReadOnly, Category = "MinecraftSkinSubsystem")
	FMinecraftPlayerInfo LocalMinecraftPlayerInfo;
};



