// Made by Chen aka Herosama

#include "MinecraftSkinSubsystem.h"
#include "MinecraftSkinGetNodes.h"


UMinecraftSkinGetNodes* UMinecraftSkinGetNodes::GetMinecraftPlayerUUID(const FString& MinecraftPlayerName)
{
	UMinecraftSkinGetNodes* MinecraftSkinGetNodes = NewObject<UMinecraftSkinGetNodes>();

	MinecraftSkinGetNodes->SendUUIDRequest(MinecraftPlayerName);

	return MinecraftSkinGetNodes;
}

UMinecraftSkinGetNodes* UMinecraftSkinGetNodes::GetMinecraftPlayerInfo(const FString& MinecraftPlayerUUID)
{
	UMinecraftSkinGetNodes* MinecraftSkinGetNodes = NewObject<UMinecraftSkinGetNodes>();

	MinecraftSkinGetNodes->SendPlayerInfoRequest(MinecraftPlayerUUID);

	return MinecraftSkinGetNodes;
}

void UMinecraftSkinGetNodes::SendUUIDRequest(const FString& MinecraftPlayerName)
{
	AddToRoot(); // In case not got GC

	TSharedPtr<IHttpRequest> HttpRequest_UUID = FHttpModule::Get().CreateRequest();// Create a new HTTP request

	HttpRequest_UUID->SetURL(FString::Printf(TEXT("https://api.mojang.com/users/profiles/minecraft/%s"),*MinecraftPlayerName));
	HttpRequest_UUID->SetVerb("GET");
	HttpRequest_UUID->OnProcessRequestComplete().BindUObject(this, &UMinecraftSkinGetNodes::OnUUIDRequestComplete);// Call back function
	// Set request parameters

	HttpRequest_UUID->ProcessRequest();
}

void UMinecraftSkinGetNodes::SendPlayerInfoRequest(const FString& MinecraftPlayerUUID)
{
	
}

void UMinecraftSkinGetNodes::OnUUIDRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if(bWasSuccessful && Response)
	{
		FString JsonBody = Response->GetContentAsString(); // Get the response body as a string

		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		TSharedRef< TJsonReader<> > JsonReader = TJsonReaderFactory<>::Create(JsonBody);

		if(FJsonSerializer::Deserialize(JsonReader,JsonObject))
		{
			//UMinecraftSkinSubsystem* MinecraftSkinSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UMinecraftSkinSubsystem>();
			//if(MinecraftSkinSubsystem)
			//{
			//	MinecraftSkinSubsystem->LocalMinecraftPlayerInfo.PlayerName = JsonObject->GetStringField("name"); // Save name to local player name form subsystem
			//	MinecraftSkinSubsystem->LocalMinecraftPlayerInfo.MinecraftPlayerUUID = JsonObject->GetStringField("id"); // Save UUID to local player UUID form subsystem

			//	OnUUIDSuccess.Broadcast(MinecraftSkinSubsystem->LocalMinecraftPlayerInfo.MinecraftPlayerUUID);
			//}
			OnUUIDSuccess.Broadcast(JsonObject->GetStringField("id")); // Get UUID from JsonObject and broadcast it
		}
	}
	else
	{
		OnUUIDFailure.Broadcast(TEXT("Error or empty"));
	}
	//RemoveFromRoot();
}

void UMinecraftSkinGetNodes::OnPlayerInfoRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
}
