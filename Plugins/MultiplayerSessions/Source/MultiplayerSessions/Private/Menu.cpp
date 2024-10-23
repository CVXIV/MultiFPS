// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu.h"

#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Components/Button.h"
#include "Engine/GameInstance.h"

void UMenu::MenuSetup(int PublicConnections, const FString& MatchType, const FString& LobbyPath) {
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	SetIsFocusable(true);
	SwitchInputMode(true);

	if (UGameInstance* GameInstance = GetGameInstance()) {
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		if (MultiplayerSessionsSubsystem) {
			MultiplayerSessionsSubsystem->MultiplayerOnCreateSessionComplete.AddUObject(this, &ThisClass::OnSessionCreateComplete);
			MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessionComplete);
			MultiplayerSessionsSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSessionComplete);
			MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddUObject(this, &ThisClass::OnDestroySessionComplete);
			MultiplayerSessionsSubsystem->MultiplayerOnStartSessionComplete.AddUObject(this, &ThisClass::OnStartSessionComplete);
		}
	}

	this->M_PublicConnections = PublicConnections;
	this->M_MatchType = MatchType;
	this->PathToLobby = FString::Printf(TEXT("%s?listen"), *LobbyPath);
}

bool UMenu::Initialize() {
	if (!Super::Initialize()) {
		return false;
	}
	if (HostButton) {
		HostButton->OnClicked.AddUniqueDynamic(this, &UMenu::OnHostButtonClicked);
	}
	if (JoinButton) {
		JoinButton->OnClicked.AddUniqueDynamic(this, &UMenu::OnJoinButtonClicked);
	}
	return true;
}

void UMenu::NativeDestruct() {
	SwitchInputMode(false);
	Super::NativeDestruct();
}

void UMenu::SwitchInputMode(bool bUIOnly) {
	if (UWorld* World = GetWorld()) {
		if (bUIOnly) {
			if (APlayerController* PlayerController = World->GetFirstPlayerController()) {
				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(TakeWidget());
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				PlayerController->SetInputMode(InputMode);
				PlayerController->SetShowMouseCursor(true);
			}
		} else {
			RemoveFromParent();
			if (APlayerController* PlayerController = World->GetFirstPlayerController()) {
				FInputModeGameOnly InputMode;
				PlayerController->SetInputMode(InputMode);
				PlayerController->SetShowMouseCursor(false);
			}
		}
	}
}

void UMenu::OnHostButtonClicked() {
	OnProcessing(true);
	if (MultiplayerSessionsSubsystem) {
		MultiplayerSessionsSubsystem->CreateSession(M_PublicConnections, M_MatchType);
	}
}

void UMenu::OnJoinButtonClicked() {
	OnProcessing(true);
	if (MultiplayerSessionsSubsystem) {
		MultiplayerSessionsSubsystem->FindSessions(10000);
	}
}

void UMenu::OnSessionCreateComplete(bool bSuccessful) {
	if (bSuccessful) {
		if (UWorld* World = GetWorld()) {
			World->ServerTravel(PathToLobby);
		}
	} else {
		OnProcessing(false);
	}
}

void UMenu::OnJoinSessionComplete(EOnJoinSessionCompleteResult::Type Result) {
	if (MultiplayerSessionsSubsystem && Result == EOnJoinSessionCompleteResult::Success) {
		FString Address;
		if (MultiplayerSessionsSubsystem->GetResolvedConnectString(NAME_GameSession, Address)) {
			if (APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController()) {
				PlayerController->ClientTravel(Address, TRAVEL_Absolute);
				return;
			}
		}
	}
	OnProcessing(false);
}

void UMenu::OnFindSessionComplete(const TArray<FOnlineSessionSearchResult>& SearchResults, bool bResult) {
	if (MultiplayerSessionsSubsystem) {
		for (const FOnlineSessionSearchResult& Result : SearchResults) {
			FString MatchType;
			Result.Session.SessionSettings.Get(FName("MatchType"), MatchType);
			if (MatchType.Equals(M_MatchType)) {
				MultiplayerSessionsSubsystem->JoinSession(Result);
				return;
			}
		}
	}
	OnProcessing(false);
}

void UMenu::OnDestroySessionComplete(bool bSuccessful) {
}

void UMenu::OnStartSessionComplete(bool bSuccessful) {
}

void UMenu::OnProcessing(bool bProcessing) const {
	HostButton->SetIsEnabled(!bProcessing);
	JoinButton->SetIsEnabled(!bProcessing);
}
