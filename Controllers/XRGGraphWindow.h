/* 
 * XRG (X Resource Graph):  A system resource grapher for Mac OS X.
 * Copyright (C) 2002-2012 Gaucho Software, LLC.
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
//  XRGGraphWindow.h
//

#import <Cocoa/Cocoa.h>
#import "XRGFramelessWindow.h"
#import "XRGCPUView.h"
#import "XRGNetView.h"
#import "XRGDiskView.h"
#import "XRGMemoryView.h"
#import "XRGWeatherView.h"
#import "XRGStockView.h"
#import "XRGBatteryView.h"
#import "XRGTemperatureView.h"
#import "XRGBackgroundView.h"
#import "XRGSettings.h"
#import "XRGModuleManager.h"

@interface XRGGraphWindow : XRGFramelessWindow
{
    // Timers
    NSTimer						*min30Timer;
    NSTimer                     *min5Timer;
    NSTimer						*graphTimer;
    NSTimer						*fastTimer;
    
    // Module references
    XRGCPUView					*cpuView;
    XRGNetView					*netView;
    XRGDiskView					*diskView;
    XRGMemoryView				*memoryView;
    XRGWeatherView				*weatherView;
    XRGStockView				*stockView;
    XRGBatteryView				*batteryView;
    XRGTemperatureView          *temperatureView;
    XRGTemperatureMiner         *temperatureMiner;
    NSWindow					*parentWindow;

    // The Module Manager
    XRGModuleManager			*moduleManager; 
    
    // Settings
    XRGSettings					*appSettings;
    NSFontManager               *fontManager;
    
    // Outlets
    IBOutlet id					preferenceWindow;
    IBOutlet id					backgroundView;
    IBOutlet id                 controller;
    
    XRGURL						*xrgCheckURL;
}

// Initialization
+ (void)initialize;
+ (NSMutableDictionary*) getDefaultPrefs;
- (void) setupSettingsFromDictionary:(NSDictionary *) defs;
- (bool)systemJustWokeUp;
- (void)setSystemJustWokeUp:(bool)yesNo;
- (void)checkServerForUpdates;
- (void)checkServerForUpdatesPostProcess;
- (bool)isVersion:(NSString *)latestVersion laterThanVersion:(NSString *)currentVersion;
- (XRGSettings *)appSettings;
- (XRGModuleManager *)moduleManager;
- (XRGAppDelegate *)controller;
- (void)setController:(XRGAppDelegate *)c;

// Timer methods
- (void)initTimers;
- (void)min30Update:(NSTimer *)aTimer;
- (void)min5Update:(NSTimer *)aTimer;
- (void)graphUpdate:(NSTimer *)aTimer;
- (void)fastUpdate:(NSTimer *)aTimer;

// Methods that set up module references
- (void)setCPUView:(XRGCPUView *)cpuO;
- (void)setMemoryView:(XRGMemoryView *)memoryO;
- (void)setBatteryView:(XRGBatteryView *)batteryO;
- (void)setTemperatureView:(XRGTemperatureView *)temperatureO;
- (void)setTemperatureMiner:(XRGTemperatureMiner *)temperatureM;
- (void)setNetView:(XRGNetView *)netO;
- (void)setDiskView:(XRGDiskView *)diskO;
- (void)setWeatherView:(XRGWeatherView *)weatherO;
- (void)setStockView:(XRGStockView *)stockO;
- (void)setBackgroundView:(XRGBackgroundView *)background0;

// Methods that return module references
- (XRGCPUView *)cpuView;
- (XRGMemoryView *)memoryView;
- (XRGBatteryView *)batteryView;
- (XRGTemperatureView *)temperatureView;
- (XRGTemperatureMiner *)temperatureMiner;
- (XRGNetView *)netView;
- (XRGDiskView *)diskView;
- (XRGWeatherView *)weatherView;
- (XRGStockView *)stockView;
- (XRGBackgroundView *)backgroundView;

// Actions
- (IBAction)setShowCPUGraph:(id)sender;
- (IBAction)setShowNetGraph:(id)sender;
- (IBAction)setShowDiskGraph:(id)sender;
- (IBAction)setShowMemoryGraph:(id)sender;
- (IBAction)setShowWeatherGraph:(id)sender;
- (IBAction)setShowStockGraph:(id)sender;
- (IBAction)setShowBatteryGraph:(id)sender;
- (IBAction)setShowTemperatureGraph:(id)sender;
- (IBAction)setBorderWidthAction:(id)sender;
- (IBAction)setGraphOrientation:(id)sender;
- (IBAction)setAntiAliasing:(id)sender;
- (IBAction)setGraphRefreshActionPart2:(id)sender;
- (IBAction)setWindowLevel:(id)sender;
- (IBAction)setStickyWindow:(id)sender;
- (IBAction)setCheckForUpdates:(id)sender;
- (IBAction)setDropShadow:(id)sender;
- (IBAction)setShowTotalBandwidthSinceBoot:(id)sender;
- (IBAction)setShowTotalBandwidthSinceLoad:(id)sender;
- (IBAction)setWindowTitle:(id)sender;
- (IBAction)setAutoExpandGraph:(id)sender;
- (IBAction)setForegroundWhenExpanding:(id)sender;
- (IBAction)setShowSummary:(id)sender;
- (IBAction)setMinimizeUpDown:(id)sender;
- (IBAction)setAntialiasText:(id)sender;

- (IBAction)setObjectsToColor:(id)sender;
- (IBAction)setObjectsToTransparency:(id)sender;

- (IBAction)setFastCPUUsageCheckbox:(id)sender;
- (IBAction)setSeparateCPUColor:(id)sender;
- (IBAction)setShowLoadAverage:(id)sender;
- (IBAction)setShowCPUTemperature:(id)sender;
- (IBAction)setCPUTemperatureUnits:(id)sender;
- (IBAction)setCPUShowAverageUsage:(id)sender;
- (IBAction)setCPUShowUptime:(id)sender;

- (IBAction)setMemoryCheckbox:(id)sender;

- (IBAction)setTempUnits:(id)sender;
- (IBAction)setTempFG1Location:(id)sender;
- (IBAction)setTempFG2Location:(id)sender;
- (IBAction)setTempFG3Location:(id)sender;

- (IBAction)setNetGraphMode:(id)sender;
- (IBAction)setNetworkInterface:(id)sender;

- (IBAction)setDiskGraphMode:(id)sender;

- (IBAction)setICAO:(id)sender;
- (IBAction)setSecondaryWeatherGraph:(id)sender;
- (IBAction)setTemperatureUnits:(id)sender;
- (IBAction)setDistanceUnits:(id)sender;
- (IBAction)setPressureUnits:(id)sender;

- (IBAction)setStockSymbols:(id)sender;
- (IBAction)setStockGraphTimeFrame:(id)sender;
- (IBAction)setStockShowChange:(id)sender;
- (IBAction)setShowDJIA:(id)sender;

// Action helpers
- (void)setWindowLevelHelper:(int)index;
- (NSColor *)colorForTag:(int)aTag;
- (float)transparencyForTag:(int)aTag;
- (void)setWindowSize:(NSSize)newSize;
- (void)checkWindowSize;

- (void)cleanupBeforeExiting;

@end
