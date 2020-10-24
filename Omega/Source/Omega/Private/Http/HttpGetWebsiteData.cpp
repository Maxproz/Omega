// Fill out your copyright notice in the Description page of Project Settings.


#include "Omega/Public/Http/HttpGetWebsiteData.h"

#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Engine/Engine.h"

FHttpGetWebsiteData::FHttpGetWebsiteData(const FString& InVerb,const FString& InPayload,const FString& InUrl,int32 InIterations)
	: Verb(InVerb)
	,Payload(InPayload)
	,Url(InUrl)
	,TestsToRun(InIterations)
{

}

void FHttpGetWebsiteData::Run(void)
{
	UE_LOG(LogHttp,Log,TEXT("Starting test [%s] Url=[%s]"),
		*Verb,*Url);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindRaw(this, &FHttpGetWebsiteData::RequestComplete);
	//This is the url on which to process the request
	Request->SetURL("https://www.itsknowledgecenter.net/2020/09/Refurbished-meaning.html");
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"),"X-UnrealEngine-Agent");
	Request->ProcessRequest();
}

void FHttpGetWebsiteData::RequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{

	//Create a pointer to hold the json serialized data
	TSharedPtr<FJsonObject> JsonObject;

	//Create a reader pointer to read the json data
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(HttpResponse->GetContentAsString());

	//Deserialize the json data given Reader and the actual object to deserialize
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		//Get the value of the json object by field name
		int32 recievedInt = JsonObject->GetIntegerField("1");

		//Output it to the engine
		GEngine->AddOnScreenDebugMessage(1,2.0f,FColor::Green,FString::FromInt(recievedInt));
	}

}