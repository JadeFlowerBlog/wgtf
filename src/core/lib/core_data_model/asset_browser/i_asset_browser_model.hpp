#ifndef I_ASSET_BROWSER_MODEL_HPP
#define I_ASSET_BROWSER_MODEL_HPP

#include "core_reflection/reflected_object.hpp"
#include "core_reflection/object_handle.hpp"

class IAssetObjectItem;
class IActiveFiltersModel;
class IComponentContext;
class IItem;
class IListModel;
class ITreeModel;
class IValueChangeNotifier;

typedef std::vector<std::string> AssetPaths;

//------------------------------------------------------------------------------
// IAssetBrowserModel
//
// Represents the data model for the WGAssetBrowser control. Developers must
// implement their own versions depending on the type of system their tools
// use to access assets be it a loose file or resource pack file system.
//------------------------------------------------------------------------------
class IAssetBrowserModel
{
	DECLARE_REFLECTED

public:

	//-------------------------------------
	// Lifecycle
	//-------------------------------------
	IAssetBrowserModel() : tempInt_( -1 ) {}
	virtual ~IAssetBrowserModel() {}
		
	//-------------------------------------
	// Public Methods
	//-------------------------------------

	// Initializes the data model.
	virtual void initialise( IComponentContext& contextManager, IDefinitionManager& definitionManager ) {}

	// Populate the folderContents list given the specified item
	virtual void populateFolderContents( const IItem* item ) {}
	
	// Asset path accessor/mutator
	virtual const AssetPaths& assetPaths() const
	{
		assert(!"must override IAssetBrowserModel::assetPaths() method");
		return tempStrVector_;
	}

	// Retrieves the data at the specified index
	virtual IAssetObjectItem* getFolderContentsAtIndex( const int & index ) const { return nullptr; }

	// Retrieves the contents of the selected folder
	// Expected: IListModel
	virtual IListModel * getFolderContents() const { return nullptr; }

	// Retrieves the model for the folder tree view
	// Expected: ITreeModel
	virtual ITreeModel * getFolderTreeModel() const { return nullptr; }

	// Retrieves the model for custom content filters
	// Expected: IListModel
	// Note: Feature likely to be removed once active filters and more robust filter handling is introduced
	virtual IListModel * getCustomContentFilters() const { return nullptr; }
	virtual IValueChangeNotifier * customContentFilterIndexNotifier() const { return nullptr; }
	virtual const int & currentCustomContentFilter() const { return tempInt_; }
	virtual void currentCustomContentFilter( const int & index ) {}

	// Retrieve the active filters model
	// Expected: IActiveFiltersModel
	virtual IActiveFiltersModel * getActiveFiltersModel() const { return nullptr; }

	virtual void setFolderContentsFilter( const std::string filter ) {}

private:

	// These are temporary variables. Type/definition registration does not 
	// allow for the registration of abstract classes. We need temporary
	// return values for the default implementation.
	std::vector<std::string> tempStrVector_;
	int tempInt_;
};

#endif // I_ASSET_BROWSER_MODEL_HPP