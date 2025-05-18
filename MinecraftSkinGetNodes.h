// Made by Chen aka Herosama

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Http.h"	// HTTP
#include "MinecraftSkinGetNodes.generated.h"

/**
 * 
 */


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUUIDHttpResponseDelegate ,FString ,MinecraftPlayerUUID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerInfoHttpResponseDelegate, FString, MinecraftPlayerInfo_Json);

UCLASS(meta = (HideThen=true))
class UNMINECRAFT_API UMinecraftSkinGetNodes : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	// Functions

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UMinecraftSkinGetNodes* GetMinecraftPlayerUUID(const FString& MinecraftPlayerName);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UMinecraftSkinGetNodes* GetMinecraftPlayerInfo(const FString& MinecraftPlayerUUID);

	void SendUUIDRequest(const FString& MinecraftPlayerName);

	void SendPlayerInfoRequest(const FString& MinecraftPlayerUUID);

	void OnUUIDRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void OnPlayerInfoRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);


	// Delegates

	UPROPERTY(BlueprintAssignable)
	FUUIDHttpResponseDelegate OnUUIDSuccess;
	
	UPROPERTY(BlueprintAssignable)
	FUUIDHttpResponseDelegate OnUUIDFailure;

	UPROPERTY(BlueprintAssignable)
	FPlayerInfoHttpResponseDelegate OnPlayerInfoSuccess;

	UPROPERTY(BlueprintAssignable)
	FPlayerInfoHttpResponseDelegate OnPlayerInfoFailure;
};
