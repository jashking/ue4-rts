#include "RTSPluginPCH.h"
#include "RTSResourceDrainComponent.h"

#include "Net/UnrealNetwork.h"

#include "RTSPlayerResourcesComponent.h"


URTSResourceDrainComponent::URTSResourceDrainComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
    SetIsReplicated(true);

	// Set reasonable default values.
	GathererCapacity = 1;
}

float URTSResourceDrainComponent::ReturnResources(AActor* Gatherer, TSubclassOf<URTSResourceType> ResourceType, float ResourceAmount)
{
	// Notify player.
	auto Owner = GetOwner()->GetOwner();

	if (!Owner)
	{
		return 0.0f;
	}

    auto PlayerResourcesComponent = Owner->FindComponentByClass<URTSPlayerResourcesComponent>();

    if (!PlayerResourcesComponent)
    {
        return 0.0f;
    }

	float ReturnedResources = PlayerResourcesComponent->AddResources(ResourceType, ResourceAmount);

	if (ReturnedResources <= 0.0f)
	{
		return 0.0f;
	}

	// Notify listeners.
    NotifyOnResourcesReturned(Gatherer, ResourceType, ReturnedResources);
	return ReturnedResources;
}

void URTSResourceDrainComponent::NotifyOnResourcesReturned_Implementation(AActor* Gatherer, TSubclassOf<URTSResourceType> ResourceType, float ResourceAmount)
{
    UE_LOG(LogRTS, Log, TEXT("Actor %s has returned %f resources of type %s to %s."),
        *Gatherer->GetName(),
        ResourceAmount,
        *ResourceType->GetName(),
        *GetOwner()->GetName());

    OnResourcesReturned.Broadcast(Gatherer, ResourceType, ResourceAmount);
}
