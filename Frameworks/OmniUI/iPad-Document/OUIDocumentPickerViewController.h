// Copyright 2010-2017 Omni Development, Inc. All rights reserved.
//
// This software may only be used and reproduced according to the
// terms in the file OmniSourceLicense.html, which should be
// distributed with this project and can also be found at
// <http://www.omnigroup.com/developer/sourcecode/sourcelicense/>.
//
// $Id$

#import <OmniDocumentStore/ODSStore.h>
#import <OmniUIDocument/OUIDocumentPickerScrollView.h>
#import <OmniUIDocument/OUIExportOptionsType.h>
#import <OmniUIDocument/OUIDocumentExporter.h>

@class NSFileWrapper;
@class ODSScope, ODSItem, ODSFileItem, ODSFolderItem, OUIDocumentPicker, OUIDocumentPickerScrollView, OUIDocumentPickerFilter, ODSFilter, OFXServerAccount, OUINewDocumentCreationContext;
@class OUIEmptyOverlayView;

@protocol OUIDocumentPickerDelegate;

@interface OUIDocumentPickerViewController : UIViewController <UIGestureRecognizerDelegate, OUIDocumentPickerScrollViewDelegate, UIDocumentInteractionControllerDelegate, OUIDocumentExporterHost>

- (instancetype)initWithDocumentPicker:(OUIDocumentPicker *)picker scope:(ODSScope *)scope;
- (instancetype)initWithDocumentPicker:(OUIDocumentPicker *)picker folderItem:(ODSFolderItem *)folderItem;

- (ODSFilter *)newDocumentStoreFilter;

@property (nonatomic, readonly) OUIDocumentPicker *documentPicker;
@property (nonatomic, retain) IBOutlet UIImageView *backgroundView;
@property(nonatomic,retain) IBOutlet OUIDocumentPickerScrollView *mainScrollView;
@property (nonatomic, strong) NSString *displayedTitleString;

@property(nonatomic,readonly) ODSStore *documentStore;

@property(nonatomic,readonly) ODSFilter *documentStoreFilter;
@property(nonatomic,readonly) ODSScope *selectedScope;
@property(nonatomic,readonly) ODSFolderItem *folderItem;
@property(nonatomic,readonly) NSSet *filteredItems;

@property(nonatomic,readonly) BOOL canAddDocuments;
@property(nonatomic,assign) BOOL isReadOnly;

@property(nonatomic,retain) IBOutlet UIToolbar *toolbar;

- (OUIEmptyOverlayView *)newEmptyOverlayView;

- (void)rescanDocuments;
- (void)rescanDocumentsScrollingToURL:(NSURL *)targetURL;
- (void)rescanDocumentsScrollingToURL:(NSURL *)targetURL animated:(BOOL)animated completionHandler:(void (^)(void))completionHandler;

@property(nonatomic,readonly) NSSet *selectedItems;
@property(nonatomic,readonly) NSSet *selectedFolders;

- (void)clearSelectionAndEndEditing;
- (void)scrollTopControlsToVisibleWithCompletion:(void (^)(BOOL))completion;

- (void)addDocumentFromURL:(NSURL *)url completionHandler:(void (^)(void))completionHandler;
- (void)addDocumentFromURL:(NSURL *)url;
- (void)addSampleDocumentFromURL:(NSURL *)url;
- (void)exportedDocumentToURL:(NSURL *)url;
    // For exports to iTunes, it's possible that we'll want to show the result of the export in our document picker, e.g., Outliner can export to OPML or plain text, but can also work with those document types. This method is called after a successful export to give the picker a chance to update if necessary.
- (void)addDocumentToSelectedScopeFromURL:(NSURL *)fromURL withOption:(ODSStoreAddOption)option openNewDocumentWhenDone:(BOOL)openWhenDone completion:(void (^)(void))completion;

- (NSArray *)availableFilters;
- (void)animateFilterChangeTo:(NSString *)filterIdentifier withCompletion:(void (^)(void))completion;

- (void)scrollToTopAnimated:(BOOL)animated;
- (void)scrollItemToVisible:(ODSItem *)item animated:(BOOL)animated;
- (void)scrollItemsToVisible:(id <NSFastEnumeration>)items animated:(BOOL)animated completion:(void (^)(void))completion;

// This will present a template picker if the delegate calls for it, otherwise it will create a new untitled document.
// There are two flavors of template pickers.  If the documentPicker has an internalTemplateDelegate set it will use the new template picker which supports displaying of internal templates to the app wrapper, without the need to copy them out.
// Othrwise, if the documentPicker's delegate has implemented, documentPickerTemplateDocumentFilter:, you will end up with the old style template picker.
- (IBAction)newDocument:(id)sender;

// The new prefered way to create a new document is to use the templateContext.  The other newDocumentWithTemplateFileItem: functions will create a templateContext and then call newDocumentWithTemplateContext:
- (void)newDocumentWithContext:(OUINewDocumentCreationContext *)context completion:(void (^)(void))completion;
- (void)newDocumentWithTemplateFileItem:(ODSFileItem *)templateFileItem documentType:(ODSDocumentType)type preserveDocumentName:(BOOL)preserveDocumentName completion:(void (^)(void))completion;
- (void)newDocumentWithTemplateFileItem:(ODSFileItem *)templateFileItem documentType:(ODSDocumentType)type completion:(void (^)(void))completion;
- (void)newDocumentWithTemplateFileItem:(ODSFileItem *)templateFileItem;

- (IBAction)duplicateDocument:(id)sender;
- (IBAction)deleteDocument:(id)sender;
- (IBAction)sortSegmentChanged:(id)sender;

+ (OFPreference *)scopePreference;
+ (OFPreference *)folderPreference;
+ (OFPreference *)filterPreference; // value is the identifier of an OUIDocumentPickerFilter returned by the delegate's -documentPickerAvailableFilters:
+ (OUIDocumentPickerFilter *)documentFilterForPicker:(OUIDocumentPicker *)picker scope:(ODSScope *)scope;
+ (OUIDocumentPickerFilter *)selectedFilterForPicker:(OUIDocumentPicker *)picker;
+ (OFPreference *)sortPreference;
+ (NSArray *)sortDescriptors;
+ (NSArray *)sortDescriptorsForSortType:(OUIDocumentPickerItemSort)sortPreference;
- (BOOL)supportsUpdatingSorting;

- (void)ensureSelectedFilterMatchesFileURL:(NSURL *)fileURL;
- (void)ensureSelectedFilterMatchesFileItem:(ODSFileItem *)fileItem;

- (void)selectedFilterChanged;
- (void)selectedSortChanged;

- (void)addDocumentStoreInitializationAction:(void (^)(OUIDocumentPickerViewController *blockSelf))action; // Note: performed immediately once the document store is initialized

- (void)updateTitle;
- (void)updateToolbarItemsEnabledness;

- (NSString *)nameLabelForItem:(ODSItem *)item;
+ (ODSDocumentType)documentTypeForCurrentFilterWith:(OUIDocumentPicker *)documentPicker;
- (ODSDocumentType)documentTypeForCurrentFilter;

@property(nonatomic,readonly) NSError *selectedScopeError;

- (void)setupTopControls;

@end
