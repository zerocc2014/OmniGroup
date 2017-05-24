// Copyright 2010-2016 Omni Development, Inc. All rights reserved.
//
// This software may only be used and reproduced according to the
// terms in the file OmniSourceLicense.html, which should be
// distributed with this project and can also be found at
// <http://www.omnigroup.com/developer/sourcecode/sourcelicense/>.

#import <OmniUI/OUIAbstractTableViewInspectorSlice.h>

#import <OmniUI/OUIInspector.h>
#import <OmniUI/UITableView-OUIExtensions.h>

RCS_ID("$Id$");

#define UPPERCASE_LABELS    (1)

@implementation OUIAbstractTableViewInspectorSlice
{
    UITableView *_tableView;
}

+ (UIView *)sectionHeaderViewWithLabelText:(NSString *)labelString forTableView:(UITableView *)tableView;
{
    UILabel *label = [[UILabel alloc] initWithFrame:CGRectZero];
    UIFont *textFont = [UIFont systemFontOfSize:[UIFont systemFontSize]];
    label.font = textFont;
    label.textColor = [UIColor colorWithWhite:0.42f alpha:1];
#if UPPERCASE_LABELS
    labelString = [labelString uppercaseStringWithLocale:[NSLocale currentLocale]];
#endif
    label.text = labelString;
    [label sizeToFit];
    UIEdgeInsets separatorInset = tableView.separatorInset;
    CGRect labelFrame = label.frame;
    labelFrame.origin.x = separatorInset.left;
    labelFrame.origin.y = -8 - labelFrame.size.height;
    labelFrame.size.width = 0;
    label.frame = labelFrame;
    label.autoresizingMask = UIViewAutoresizingFlexibleTopMargin | UIViewAutoresizingFlexibleWidth;
    
    CGRect headerFrame = CGRectMake(0, 0, separatorInset.left + separatorInset.right, 0);
    UIView *headerView = [[UIView alloc] initWithFrame:headerFrame];
    headerView.autoresizingMask = headerView.autoresizingMask & ~UIViewAutoresizingFlexibleHeight;
    [headerView addSubview:label];
    
    return headerView;
}

- (void)dealloc;
{
    _tableView.delegate = nil;
    _tableView.dataSource = nil;
}

- (UITableView *)tableView;
{
    if (!_tableView)
        (void)[self view];
    OBASSERT(_tableView);
    return _tableView;
}

- (void)_resizeTable;
{
    OUITableViewAdjustHeightToFitContents(_tableView);
    CGFloat currentHeight = self.tableView.contentSize.height;
    if (currentHeight == 0.0)
        return;
    
    if (self.heightConstraint == nil) {
        self.heightConstraint = [self.tableView.heightAnchor constraintEqualToConstant:currentHeight];
        self.heightConstraint.active = YES;
    } else {
        self.heightConstraint.constant = currentHeight;
    }
}

- (void)reloadTableAndResize;
{
    [_tableView reloadData];
    [self _resizeTable];
}

#pragma mark - OUIInspectorSlice subclass

- (void)updateInterfaceFromInspectedObjects:(OUIInspectorUpdateReason)reason;
{
    [super updateInterfaceFromInspectedObjects:reason];
    [self reloadTableAndResize];
}

#pragma mark - UIViewController subclass

- (UITableViewStyle)tableViewStyle;
{
    return UITableViewStyleGrouped;
}

- (void)loadView;
{
    OBPRECONDITION(_tableView == nil);
    
    _tableView = [[UITableView alloc] initWithFrame:CGRectMake(0, 0, [OUIInspector defaultInspectorContentWidth], 420) style:[self tableViewStyle]];
    
    // Subclasses must implement these protocols -- this class just does the UIViewController and OUIInspectorSlice glue code dealing with the view property being a UITableView.
    OBASSERT([self conformsToProtocol:@protocol(UITableViewDataSource)]);
    OBASSERT([self conformsToProtocol:@protocol(UITableViewDelegate)]);
    
    _tableView.delegate = (id <UITableViewDelegate>)self;
    _tableView.dataSource = (id <UITableViewDataSource>)self;
    self.view = _tableView;
}

- (void)viewDidLoad;
{
    [super viewDidLoad];
    
    [self configureTableViewBackground:_tableView];
}

- (void)viewWillAppear:(BOOL)animated;
{
    [super viewWillAppear:animated];
    //If we are inspecting a new type of graphic for the first time (like tapping a graphic when we first open the document) the it is possible for the table view to have no content. We then try to resize an empty table view to fit its content size, but the table view has no contents and the resize function expects the table view to have contents. So, make sure we have contents first.
    [self updateInterfaceFromInspectedObjects:OUIInspectorUpdateReasonDefault];
    // Might be coming back from a detail pane that edited a displayed value
    [self reloadTableAndResize];
}

- (void)viewDidDisappear:(BOOL)animated;
{
    if ([_tableView isEditing])
        [self setEditing:NO animated:NO];

    [super viewDidDisappear:animated];
}

- (void)setEditing:(BOOL)editing animated:(BOOL)animated;
{
    [super setEditing:editing animated:animated]; // updates our editingButtonItem
    [_tableView setEditing:editing animated:animated];
}

-(UIColor *)sliceBackgroundColor;
{
    return [UIColor clearColor];
}

- (CGFloat)tableView:(UITableView *)tableView heightForFooterInSection:(NSInteger)section;
{
    return 12;
}

@end
