// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BaseTreeView.h"

#include "Blueprint/IUserListEntry.h"
#include "Slate/SObjectTableRow.h"
#include "Blueprint/UserWidget.h"



void UBaseTreeView::OnItemClickedInternal(UObject* ListItem)
{
	// If the clicked item has children, expand it now as part of the click
	if (ensure(MyTreeView.IsValid()))
	{
		if (bCollapseItemsOnClick)
		{
			// The item was clicked, implying that there should certainly be a widget representing this item right now
			TSharedPtr<ITableRow> RowWidget = MyTreeView->WidgetFromItem(ListItem);
			if (ensure(RowWidget.IsValid()) && RowWidget->DoesItemHaveChildren() > 0)
			{
				const bool bNewExpansionState = !MyTreeView->IsItemExpanded(ListItem);
				MyTreeView->SetItemExpansion(ListItem, bNewExpansionState);
			}
		}
	}

	UListView::OnItemClickedInternal(ListItem);
}

bool UBaseTreeView::GetItemExpansion(UObject* Item)
{
	UUserWidget* Widget = this->GetEntryWidgetFromItem<UUserWidget>(Item);
	TSharedPtr<const IObjectTableRow> SlateRow = IObjectTableRow::ObjectRowFromUserWidget(Widget);
	if (SlateRow.IsValid())
	{
		return SlateRow->IsItemExpanded();
	}
	return false;
}

void UBaseTreeView::ToggleItemExpansion(UObject* Item)
{
	bool bIsExpanded = GetItemExpansion(Item);
	this->SetItemExpansion(Item, !bIsExpanded);
}
