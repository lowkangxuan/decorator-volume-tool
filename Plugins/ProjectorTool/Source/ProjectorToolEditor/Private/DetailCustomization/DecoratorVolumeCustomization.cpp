#include "DetailCustomization/DecoratorVolumeCustomization.h"

#include "DecoratorVolumeISM.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Components/InstanceBakingComponent.h"
#include "DetailCustomization/BaseDecoratorVolumeCustomization.h"

TSharedRef<IDetailCustomization> FDecoratorVolumeCustomization::MakeInstance()
{
	return MakeShareable(new FDecoratorVolumeCustomization);
}

void FDecoratorVolumeCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
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
			.OnClicked_Raw(this, &FDecoratorVolumeCustomization::EditorFuncs, EBtnType::Bake)
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
			.OnClicked_Raw(this, &FDecoratorVolumeCustomization::EditorFuncs, EBtnType::Unbake)
		]
	];
}

void FDecoratorVolumeCustomization::CustomizeDetails(const TSharedPtr<IDetailLayoutBuilder>& DetailBuilder)
{
	CachedDetailBuilder = DetailBuilder;
	CustomizeDetails(*DetailBuilder);
}

FReply FDecoratorVolumeCustomization::EditorFuncs(EBtnType Type)
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
			
			case EBtnType::Bake:
			{
				VolumeActor->BakeInstances();
				break;
			}

			case EBtnType::Unbake:
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

bool FDecoratorVolumeCustomization::CheckIfBaked()
{
	return ComponentBeingCustomized->bIsBaked;
}

bool FDecoratorVolumeCustomization::CheckIfCleared()
{
	return ActorBeingCustomized->bIsCleared;
}

