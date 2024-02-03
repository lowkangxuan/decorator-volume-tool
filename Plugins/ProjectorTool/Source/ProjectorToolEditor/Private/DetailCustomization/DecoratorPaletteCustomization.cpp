#include "DetailCustomization/DecoratorPaletteCustomization.h"

#include "DecoratorPalette.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"

TSharedRef<IDetailCustomization> FDecoratorPaletteCustomization::MakeInstance()
{
	return MakeShareable(new FDecoratorPaletteCustomization);
}

void FDecoratorPaletteCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	DetailBuilder.GetObjectsBeingCustomized(Objects);
	

}

