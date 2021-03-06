/*
 * XRG (X Resource Graph):  A system resource grapher for Mac OS X.
 * Copyright (C) 2002-2016 Gaucho Software, LLC.
 * You can view the complete license in the LICENSE file in the root
 * of the source tree.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */

//
//  XRGGraphicsView.m
//

#import "XRGGPUView.h"
#import "XRGGraphWindow.h"

@implementation XRGGPUView

- (void)awakeFromNib {
	graphicsMiner = [[XRGGPUMiner alloc] init];
	
	parentWindow = (XRGGraphWindow *)[self window];
	[parentWindow setGpuView:self];
	[parentWindow initTimers];
	appSettings = [parentWindow appSettings];
	moduleManager = [parentWindow moduleManager];
	
	textRectHeight = [appSettings textRectHeight];
	
	NSUserDefaults *defs = [NSUserDefaults standardUserDefaults];
	m = [[XRGModule alloc] initWithName:@"GPU" andReference:self];
	m.doesFastUpdate = NO;
	m.doesGraphUpdate = YES;
	m.doesMin5Update = NO;
	m.doesMin30Update = NO;
	m.displayOrder = 1;
	[self updateMinSize];
	[m setIsDisplayed: (bool)[defs boolForKey:XRG_showGPUGraph]];
	
	[[parentWindow moduleManager] addModule:m];
	[self setGraphSize:[m currentSize]];
}

- (void)setGraphSize:(NSSize)newSize {
	NSSize tmpSize;
	tmpSize.width = newSize.width;
	tmpSize.height = newSize.height;
	if (tmpSize.width < 23) tmpSize.width = 23;
	if (tmpSize.width > 2000) tmpSize.width = 2000;
	[self setWidth:tmpSize.width];
	graphSize = tmpSize;
}

- (void)setWidth:(int)newWidth {
	int newNumSamples = newWidth;
	if (newNumSamples < 0) return;
	
	numSamples = newNumSamples;
	
	[graphicsMiner setDataSize:newNumSamples];
}

- (void)updateMinSize {
	float width = [@"GPU 9: 9999M" sizeWithAttributes:[appSettings alignRightAttributes]].width + 6;
	
	[m setMinWidth: width];
    [m setMinHeight: XRG_MINI_HEIGHT];
}

- (void)graphUpdate:(NSTimer *)aTimer {
	[graphicsMiner getLatestGraphicsInfo];
	[self setNeedsDisplay:YES];
}

- (void)drawRect:(NSRect)rect {
	if ([self isHidden]) return;
	
	NSGraphicsContext *gc = [NSGraphicsContext currentContext];
	
#ifdef XRG_DEBUG
	NSLog(@"In Graphics DrawRect.");
#endif
	
	NSArray *cpuWaitValues = [graphicsMiner cpuWaitDataSets];
	NSArray *totalValues = [graphicsMiner totalVRAMDataSets];
	NSArray *freeValues = [graphicsMiner freeVRAMDataSets];
	if ((totalValues.count != freeValues.count) || (totalValues.count == 0)) {
		[@"GPU n/a" drawInRect:self.bounds withAttributes:[appSettings alignCenterAttributes]];
		return;
	}
	
	CGFloat graphHeight = graphSize.height / totalValues.count;
	for (NSInteger i = 0; i < totalValues.count; i++) {
		NSRect graphRect = NSMakeRect(0, (totalValues.count - i - 1) * floor(graphHeight), numSamples, floor(graphHeight) - 1);
		[[appSettings graphBGColor] set];
		NSRectFill(graphRect);

		// Draw the graph.
		[gc setShouldAntialias:[appSettings antiAliasing]];

		// VRAM Usage
		XRGDataSet *usedDataSet = [[XRGDataSet alloc] initWithContentsOfOtherDataSet:totalValues[i]];
		[usedDataSet subtractOtherDataSetValues:freeValues[i]];
		if ([usedDataSet max] > 0) {
			[self drawGraphWithDataFromDataSet:usedDataSet maxValue:[totalValues[i] max] inRect:graphRect flipped:NO filled:YES color:[appSettings graphFG1Color]];
		}
		else {
			[[appSettings backgroundColor] set];
			NSRectFill(NSMakeRect(graphRect.origin.x, graphRect.origin.y, graphRect.size.width, 1.));
		}
		
		// CPU Wait
		[self drawGraphWithDataFromDataSet:cpuWaitValues[i] maxValue:MAX([cpuWaitValues[i] max], 100) inRect:graphRect flipped:NO filled:NO color:[appSettings graphFG2Color]];
		
		// Draw the text
		[gc setShouldAntialias:[appSettings antialiasText]];
		NSRect textRect = graphRect;
		textRect.origin.x += 3;
		textRect.size.width -= 6;
		CGFloat t = [(XRGDataSet *)totalValues[i] currentValue];
		CGFloat f = [(XRGDataSet *)freeValues[i] currentValue];
		
		// CPU Wait Time Text
		NSString *waitText = nil;
		CGFloat w = [(XRGDataSet *)cpuWaitValues[i] currentValue];
		if (w < 0) {
			// Don't print anything.
			waitText = @"";
		}
		else if (w < 1000.) {
			waitText = [NSString stringWithFormat:@"%d ns", (int)w];
		}
		else if (w < 1000000.) {
			waitText = [NSString stringWithFormat:@"%d µs", (int)(w / 1000)];
		}
		else if (w < 1000000000.) {
			waitText = [NSString stringWithFormat:@"%d ms", (int)(w / 1000000)];
		}
		else {
			waitText = [NSString stringWithFormat:@"%d s", (int)(w / 1000000000)];
		}
		
		NSString *vendorString = graphicsMiner.vendorNames[i];
		if (textRect.size.width < 90) {
			[[NSString stringWithFormat:@"GPU %d\n%@", (int)i, vendorString] drawInRect:textRect withAttributes:[appSettings alignLeftAttributes]];
			if (t > 0) {
				[[NSString stringWithFormat:@"%dM\n%@", (int)(ceil((CGFloat)(t - f) / 1024. / 1024.)), waitText] drawInRect:textRect withAttributes:[appSettings alignRightAttributes]];
			}
		}
		else {
			[[NSString stringWithFormat:@"GPU %d\n%@", (int)i, vendorString] drawInRect:textRect withAttributes:[appSettings alignCenterAttributes]];

			if (t > 0) {
				[[NSString stringWithFormat:@"%dM\n%@", (int)(ceil((CGFloat)(t - f) / 1024. / 1024.)), waitText] drawInRect:textRect withAttributes:[appSettings alignLeftAttributes]];
				[[NSString stringWithFormat:@"%d%%", (int)((t - f) / t * 100.)] drawInRect:textRect withAttributes:[appSettings alignRightAttributes]];
			}
		}
	}

	[gc setShouldAntialias:YES];
}

- (BOOL)acceptsFirstMouse:(NSEvent *)theEvent {
	return YES;
}

- (void)mouseDown:(NSEvent *)theEvent {
	[parentWindow mouseDown:theEvent];
}

- (void)mouseDragged:(NSEvent *)theEvent {
	[parentWindow mouseDragged:theEvent];
}

- (void)mouseUp:(NSEvent *)theEvent {
	[parentWindow mouseUp:theEvent];
}

@end
