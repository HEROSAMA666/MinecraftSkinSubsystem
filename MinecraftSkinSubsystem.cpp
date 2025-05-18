// Made by Chen aka Herosama


#include "MinecraftSkinSubsystem.h"



FString UMinecraftSkinSubsystem::GetPlayerUUID(const FString MinecraftPlayerName)
{
	TWeakPtr<IHttpRequest> PlayerUUIDRequest = FHttpModule::Get().CreateRequest();
	FString URL_GetPlayerUUID = FString::Printf(TEXT("https://api.mojang.com/users/profiles/minecraft/%s"), *MinecraftPlayerName);


	return URL_GetPlayerUUID;
}

void UMinecraftSkinSubsystem::OnGetPlayerUUIDComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
	//TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonData);
}
