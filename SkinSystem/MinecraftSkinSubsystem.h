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

	/*
	* Remember to add .format at the end of the filename, like _heroo.png
	*/
	UFUNCTION(BlueprintCallable, Category = "MinecraftSkinSubsystem")
	bool SaveSkinImage_RenderTarget(UTextureRenderTarget2D* RenderTargetToSave,const FString& FileDestination,const FString& FileName);

	/*
	* If you have the skin png file in path : YourProject/Saved ,this function will return the 
	* If there is no such file then it will return false
	*/
	UFUNCTION(BlueprintCallable, Category = "MinecraftSkinSubsystem")
	UTexture2D* TryGetMinecraftSkinFromName(const FString PlayerName);

	// UUID part


	// Properties

	UPROPERTY(BlueprintReadOnly, Category = "MinecraftSkinSubsystem")
	FMinecraftPlayerInfo LocalMinecraftPlayerInfo;

};



