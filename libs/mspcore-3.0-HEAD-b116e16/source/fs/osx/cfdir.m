#import <Foundation/NSPathUtilities.h>

unsigned get_home_dir(char *buf, unsigned size)
{
	NSString *path = NSHomeDirectory();
	if(![path getCString:buf maxLength:size encoding:NSUTF8StringEncoding])
		return 0;

	return [path lengthOfBytesUsingEncoding:NSUTF8StringEncoding];
}

unsigned get_application_support_dir(char *buf, unsigned size)
{
	NSArray *dirs = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
	if(![dirs count])
		return 0;

	NSString *path = [dirs objectAtIndex:0];
	if(![path getCString:buf maxLength:size encoding:NSUTF8StringEncoding])
		return 0;

	return [path lengthOfBytesUsingEncoding:NSUTF8StringEncoding];
}
