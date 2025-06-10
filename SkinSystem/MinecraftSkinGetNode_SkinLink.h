// Made by Chen aka Herosama

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Http.h"	// HTTP
#include "MinecraftSkinGetNode_SkinLink.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayerSkinLinkHttpResponseDelegate, FString, MinecraftPlayerSkinLink, FString ,Result);

UCLASS(meta = (HideThen = true))
class UNMINECRAFT_API UMinecraftSkinGetNode_SkinLink : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UMinecraftSkinGetNode_SkinLink* GetMinecraftPlayerSkinLink(const FString& MinecraftPlayerUUID, UObject* InWorldContextObject);

	void SendPlayerSkinLinkRequest(const FString& MinecraftPlayerUUID, UObject* InWorldContextObject);

	void OnPlayerSkinLinkRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	FString base64_Decode(const FString& base64String);

	FString GetBase64DataFromJson(const FString& RawJsonData);

	FString GetSkinLinkFromJson(const FString& RawJsonData);

	UPROPERTY(BlueprintAssignable)
	FPlayerSkinLinkHttpResponseDelegate OnPlayerSkinLinkSuccess;

	UPROPERTY(BlueprintAssignable)
	FPlayerSkinLinkHttpResponseDelegate OnPlayerSkinLinkFailure;

	/* Better be an actor from current world */
	UObject* WorldContextObject = nullptr; // Use to acquire the current world context
};
