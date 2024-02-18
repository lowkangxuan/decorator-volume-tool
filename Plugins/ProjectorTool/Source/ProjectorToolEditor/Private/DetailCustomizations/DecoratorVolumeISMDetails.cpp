#include "DetailCustomizations/DecoratorVolumeISMDetails.h"

#include "DecoratorVolumeISM.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Components/InstanceBakingComponent.h"
#include "DetailCustomizations/BaseDecoratorVolumeDetails.h"

TSharedRef<IDetailCustomization> FDecoratorVolumeISMDetails::MakeInstance()
{
	return MakeShareable(new FDecoratorVolumeISMDetails);
}

void FDecoratorVolumeISMDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	DetailBuilder.GetObjectsBeingCustomized(Objects);
	ActorBeingCustomized = Cast<ADecoratorVolumeISM>(Objects[0].Get());
	ComponentBeingCustomized = ActorBeingCustomized->GetComponentByClass<UInstanceBakingComponent>();
	const bool bShouldDisplay = !Objects[0]->HasAnyFlags(RF_ArchetypeObject | RF_ClassDefaultObject); // Checking if these flags exist
	
	IDetailCategoryBuilder& EditorCategory = DetailBuilder.EditCategory("Editor", FText::GetEmpty(), ECategoryPriority::Important);
	IDetailCategoryBuilder& DecoratorVolumeCategory = DetailBuilder.EditCategory("DecoratorVolumeISM", FText::GetEmpty(), ECategoryPriority::Important);
	
	// Don't draw buttons when actor is viewed in BP Editor
	if (!bShouldDisplay) return;
	
	EditorCategory.AddCustomRow(FText::FromString("Editor"))
	.WholeRowContent()
	.VAlign(VAlign_Center)
	.HAlign(HAlign_Fill)
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.FillWidth(1.0f)
		.HAlign(BUTTON_ALIGNMENT)
		.Padding(BUTTON_PADDING_FIRST)
		[
			SNew(SButton)
			.Text(FText::FromString("Bake"))
			.VAlign(VAlign_Center)
			.ContentPadding(FMargin(0, 5))
			.IsEnabled(!CheckIfBaked() && !CheckIfCleared())
			.OnClicked_Raw(this, &FDecoratorVolumeISMDetails::EditorFuncs, EVolumeActionsEnum::Bake)
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.0f)
		.HAlign(BUTTON_ALIGNMENT)
		.Padding(BUTTON_PADDING_FIRST)
		[
			SNew(SButton)
			.Text(FText::FromString("Unbake"))
			.VAlign(VAlign_Center)
			.ContentPadding(FMargin(0, 5))
			.IsEnabled(CheckIfBaked() && !CheckIfCleared())
			.OnClicked_Raw(this, &FDecoratorVolumeISMDetails::EditorFuncs, EVolumeActionsEnum::Unbake)
		]
	];
}

void FDecoratorVolumeISMDetails::CustomizeDetails(const TSharedPtr<IDetailLayoutBuilder>& DetailBuilder)
{
	CachedDetailBuilder = DetailBuilder;
	CustomizeDetails(*DetailBuilder);
}

FReply FDecoratorVolumeISMDetails::EditorFuncs(EVolumeActionsEnum Type)
{
	for (TWeakObjectPtr<UObject> Object : Objects)
	{
		if (!Object.IsValid()) continue;

		TWeakObjectPtr<ADecoratorVolumeISM> VolumeActor = Cast<ADecoratorVolumeISM>(Object.Get());

		if (!VolumeActor.IsValid()) continue;

		switch (Type)
		{
			default:
			{
				break;	
			}
			
			case EVolumeActionsEnum::Bake:
			{
				VolumeActor->BakeInstances();
				break;
			}

			case EVolumeActionsEnum::Unbake:
			{
				VolumeActor->UnbakeInstances();
				break;
			}
		}
	}

	if (IDetailLayoutBuilder* DetailBuilder = CachedDetailBuilder.Pin().Get())
	{
		DetailBuilder->ForceRefreshDetails();
	}
	
	return FReply::Handled();
}

bool FDecoratorVolumeISMDetails::CheckIfBaked()
{
	return ComponentBeingCustomized->bIsBaked;
}

bool FDecoratorVolumeISMDetails::CheckIfCleared()
{
	return ActorBeingCustomized->bIsCleared;
}

