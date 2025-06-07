// Made by Chen aka Herosama

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Http.h"	// HTTP
#include "MinecraftSkinGetNodes.generated.h"

/**
 * 
 */


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUUIDHttpResponseDelegate, FString, MinecraftPlayerUUID, FString, Result);

UCLASS(meta = (HideThen=true))
class UNMINECRAFT_API UMinecraftSkinGetNodes : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	// Functions

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UMinecraftSkinGetNodes* GetMinecraftPlayerUUID(const FString& MinecraftPlayerName, UObject* InWorldContextObject);

	void SendUUIDRequest(const FString& MinecraftPlayerName, UObject* InWorldContextObject);

	void OnUUIDRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	// Delegates (Out pins)

	UPROPERTY(BlueprintAssignable)
	FUUIDHttpResponseDelegate OnUUIDSuccess;
	
	UPROPERTY(BlueprintAssignable)
	FUUIDHttpResponseDelegate OnUUIDFailure;

	/* Better be an actor from current world */
	UObject* WorldContextObject = nullptr; // Use to acquire the current world context

};
