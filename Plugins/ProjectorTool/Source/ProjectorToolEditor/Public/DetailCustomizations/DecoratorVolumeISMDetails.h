#pragma once
#include "IDetailCustomization.h"
#include "Enums/VolumeActionsEnum.h"

class ADecoratorVolumeISM;
class UInstanceBakingComponent;
class FReply;

class FDecoratorVolumeISMDetails : public IDetailCustomization
{
public:
	// Makes a new instance of this detail layout class for a specific detail view requesting it
	static TSharedRef<IDetailCustomization> MakeInstance();
	
	// IDetailCustomization interface 
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
	virtual void CustomizeDetails(const TSharedPtr<IDetailLayoutBuilder>& DetailBuilder) override;

	FReply EditorFuncs(EVolumeActionsEnum Type);
	bool CheckIfBaked();
	bool CheckIfCleared();

private:
	TWeakPtr<IDetailLayoutBuilder> CachedDetailBuilder;
	TArray<TWeakObjectPtr<UObject>> Objects;
	TWeakObjectPtr<ADecoratorVolumeISM> ActorBeingCustomized;
	TWeakObjectPtr<UInstanceBakingComponent> ComponentBeingCustomized;
};
