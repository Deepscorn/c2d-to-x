# C2d-to-x: cocos2d objective c to cocos2d-x C++ converter (ver. 0.75 alpha). A free converter.
# by Deepscorn, all questions or suggestions, please send to vnms11@gmail.com

# LICENSE
# Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
# You can read it here:
# http://creativecommons.org/licenses/by-nc-sa/3.0/

# HOWTO call:
# I used free version of ActiveTCL to run this script. It can be downloaded here:
# http://www.activestate.com/activetcl/downloads
# supported platforms:
# Windows, Mac, Linux, AIX, HP-UX & Solaris
#
# first par: path to file to convert
# second par: path to result (converted) file
# example call from tkcon (first param must be empty):
# exec {C:\Tcl\bin\tclsh} {C:\Dropbox\c2d-to-x\c2d-to-x.tcl} << {} {C:\temp\Recipe.h} {C:\temp\RecipeConverted.h}
# or using *.bat file:
# cd <dir with source .h/.m files
# tclsh {d:\Dropbox\projects\c2d-to-x-master\c2d-to-x.tcl} HelloWorldScene.m ../
# if directory is specified as third parameter, than file with name from second parameter will be created there!

#SUPPORTED FORMATS:
# *.h file remains the same
# *.mm file will be *.cpp

# IMPLICIT LOGIC:
# 1. class must start from Capital letter: "CCMenuItemFont" for ex.
# 2. class instance must not start from capital letter: "prev" for ex.
# so "[CCMenuItemFont setFontSize:16];"
# will be: "CCMenuItemFont::setFontSize(16);
# and "[node stopAllActions];"
# will be "node->stopAllActions();"
# 3. Only line-by-line convertion is supported by the moment
# 4. File name of *.mm file must correspond to class name, because all methods, realized there will have it's prefix.
# E.g.: File Some.mm: int a() { -> int Some::a() { 

# FROM AUTHOR
# Recently, I was following "Cocos2d Cookbook Receipts" from http://cocos2dcookbook.com/. This looks like very useful book for learning cocos2d-x for me. It is divided into receipts, so functionality in one recept doesn't use another, so it is very good for game development. It may be read in any way. So, if I want to use some receipt I had to convert it to cocos2d-x (v2.0.1 I use). To get faster I wrote this small script. Maybe it will be useful for you too.

# V 0.75
# Added tabulation symbol support in some cases.
# USING_NS_CC automatically added after cocos2d.h include
# In cpp file class name is added to each method. Read Implicit Logic #4 carefully!
# NSLog is translated as CCLOG. It's a macro and does nothing, unless you define COCOS2D_DEBUG.
# converts multiline statements too
# fixed bug, now a[i] inside function calls remain the same
# Added replaces list and list of delimeters for you to suit your needs (see below)
# added ":" delimeter in some cases
#
# V 0.8
# fixed some @"text" issue
# new delimeters syntax for faster work and easy-look code: \t for tabulation, any special character must start with \\, for ex.: \\.
# added: some.a will be converted to some->getA()
# ->string() will be converted to ->getString() (remove 2 corresponding elements from replaces list below, if you don't want such behavior)

# MUST STILL DO BY HAND
# Include class methods in {}

# REPLACES LIST - modify on your own
set elements [list "@interface" "class" "@class" "class" "id" "bool" "self" "this" "super" "this" "nil" "NULL" "YES" "true" "NO" "false" "#import" "#include" "NSString" "CCString" "NSLog" "CCLOG" "CGSize" "CCSize" "NSString" "CCString" "NSUInteger" "CCInteger" "NSObject" "CCObject" "NSSet" "CCSet" "NSMutableArray" "CCArray" "CGRectMake" "CCRectMake" "unichar" "char" "NSMakeRange" "CCRangeMake" "NSArray" "CCArray" ">string\\(\\)" ">getString()" "UIColor" "ccColor4B" "CGFloat" "CCFloat"]
# only this delimeters may delimit elements
set delimeters {[ \n\t\(\);\-\\.\\*/+%!:\,]}
#if line starts with one of these elements - it will be deleted
# "@implementation" will be deleted too
set elements_del [list "@end"]
#
set dev_mode 0

# path to file to convert
set fname_src [lindex $argv 0]
# path to result file
set fname_dst [lindex $argv 1]
set classname ""

# replace functions - start
#FUNCTIONS, WHICH DOES NOT REQUIRE ";"
proc handle_del { line } {
# line passed by value
    global classname
	global elements_del
	global delimeters
	
	if { [regexp {(.+)(@implementation) +?(.+?)\n(.+)} $line _ s0 _ classname s1 ] } {
	  set classname $classname\:\:
	  set line $s0$s1
	}
	foreach subel $elements_del {
		regsub -all $subel$delimeters $line "" line
	}
	return $line
}
# handles one-word replacement. Checks if it is single word, 
# e.g. it has delimeters from the left and right
proc handle_replace_one { line } {
# line passed by value
	global elements
	global delimeters
	##import "cocos2d.h" -> #include "cocos2d.h"\nUSING_NS_CC;
	set line [regsub {\#import +("cocos2d.h")} $line "\#include \\1\nUSING_NS_CC;"]
	for {set i 0} {$i<[llength $elements]} {incr i 2} {
		#our template must end with delimeter
		set templ "[lindex $elements $i]($delimeters)"
		# templ may be at the line beginning
		set templ2 "^$templ"
		set line [regsub -all $templ2 $line "[lindex $elements $i+1]\\1"]
		# or templ may start with delimeter
		set templ3 "($delimeters)$templ"
		set line [regsub -all $templ3 $line "\\1[lindex $elements $i+1]\\2"]
	}
	return $line
}
proc handle_replace_at_last { line } {
	#must be THE LAST!
	#@interface ParticleWaterfall : CCParticleRain {}
	#@interface ParticleWaterfall : public CCParticleRain {}
	set line [regsub {class +([A-Za-z]+) *[:] *([A-Za-z]+)} $line "class \\1: public \\2"]
	return $line
}
#end - FUNCTIONS, WHICH DOES NOT REQUIRE ";"
#must be called after super position replacement!
proc handle_replace_one_per_line { line } {
# line passed by value
	proc fromCapital str { 
		set res ""
		append res [string toupper [string index $str 0]]
		for {set i 1} {$i<[string length $str]} {incr i} {
			append res [string index $str $i]
		}
		return $res
	}
	#self.speed = 100;
	#self->setSpeed(100);
	set temp [regexp {(.+)[.]([A-Za-z0-9]+) *[=] *([^;]*)} "$line" match sub1 sub2 sub3 ]
	if {$temp} {
		set line "$sub1->set[fromCapital $sub2]($sub3);"
	}
	#sharedDirector().winSize
	#sharedDirector()->getWinSize()
	while {[set temp [regexp {(.+\(\))[.](\w+)([^=]$)} "$line" match sub1 sub2 sub3]]} {
		set line "$sub1->get[fromCapital $sub2]()$sub3"
	}
	return $line
}
#must be called by super power algorithm!
proc handle_replace_many_per_line { line cname} {
# line passed by value
	#-(CCLayer*) runRecipe;
	#CCLayer* runRecipe();
	set line [regsub {[-+] *[(]([A-Za-z]+[*]*)[)] *([A-Za-z]+)([\n ]* *[;\{])} "$line" "\\1 $cname\\2()\\3" ]
	#-(id)initWithTotalParticles:(int)p;
	set line [regsub {[-+] *[(](\w+[*]*)[)] *(\w+): *[(]([^\)]+)[)] *(\w+)} "$line" "\\1 $cname\\2(\\3 \\4)" ]
	# ) didFinishAllTextWithPageCount:(int)pc
	# , int pc )
	while { [regsub -all {[)][ \n\t]*\w+: *[(]([^\)]+)[)] *(\w+)} $line ", \\1 \\2 )" line]} {}
	
	#@"ParticleExplosion"
	#"ParticleExplosion"
	#while {[regsub -all {@(".*")} $line "\\1" line]} {}
	regsub -all {@(".*?")} $line "\\1" line
	return $line
}
#recursive replacement (for super position replacement)
proc brackets_replacer { line } {
  # ::, ->
	regsub -all {\[([A-Z]\w*) +(\w+)(:([\w, :()]+)){0,1}\]} $line "\\1::\\2(\\4)" line
	regsub -all {\[([^ ]+) +(\w+)(:([^\]]+)){0,1}\]} $line "\\1->\\2(\\4)" line
  # attributes in ( )
	regsub -all {\w+:([^:])} $line ", \\1" line
	return $line
}
proc handle_super_position { line } {
	#super-power algorithm!
	#FROM: CCLabelTTF* name = [CCLabelTTF labelWithString:[NSString stringWithFormat:@"%@ %@", actionMethods[currentActionX], actionMethods[currentActionY]] fontName:@"Arial" fontSize:14];
	#TO: CCLabelTTF* name = CCLabelTTF::labelWithString(CCString::stringWithFormat("%@ %@",actionMethods[currentActionX],actionMethods[currentActionY]),"Arial",14);
	#puts "line:$line"
	
	if {[regexp {.*?[\[].+ *[\]] *(.+$)} $line match ending] == 1} {
	  #@selector(gameLoop:) specific
	  regsub -all {\((\w+):\)} $line "(\\1)" line
		#[word] -> {<}word{>} because of brackets_replacer's specific
		regsub -all {\[(\w+)\]} $line "\x0\\1\x1" line
		set line [brackets_replacer $line]
		#remove all changes, we made because of brackets_replacer's specific
		regsub -all {\x0} $line "\[" line
		regsub -all {\x1} $line "\]" line
		#puts "result:$line"
	}
	return $line
}
proc handle_dev_replaces { line } {
  set to_add "\n#include \"cocos2d.h\"\nUSING_NS_CC;\n\n"
  regsub {((#include[^\n]+[\n]+)+)} $line "/*\\1*/$to_add" line
  return $line
}
# replace functions - end

# main
# fool-checks
if {[file isdirectory $fname_src]} {
  puts "Error: arg(1), source can't be folder."
  foreach t_arg $argv {
    puts "arg $t_arg"
  }
}

#if have only directory as destination - use source file name
set only_fname_src [file tail $fname_src]
#get file name without ext for many purposes
regexp {(.+)\.(mm|m|h)$} $only_fname_src match fname_no_ext f_ext
if {[file isdirectory $fname_dst] == 1} {
	set f_ext [regsub {(mm|m)} $f_ext {cpp}]
	set newname $fname_no_ext.$f_ext
	set fname_dst $fname_dst/$newname
}

set filefrom [open  $fname_src r]

#check if we have *.h or *.mm file
set fileHas_H_Extension [regexp {.h$} $only_fname_src]
#open destination file
set fileto [open $fname_dst w]
#add to start of *.h file
if {$fileHas_H_Extension} {
	set fnamecomb [regsub {\.} $only_fname_src "_"]
	set fnamecomb [string toupper $fnamecomb]
	puts $fileto "#ifndef $fnamecomb"
	puts $fileto "#define $fnamecomb"
	flush $fileto
}

# main replace-loop

set classname ""
set in_str [read $filefrom]\;
# added separator for our algorithm

while { [regexp {(.+?[;\{\}])(.*$)} $in_str _ line in_str] == 1} {

	#DELETE
	set line [handle_del $line]
	#REPLACE UNTIL ";"
	#puts "\nline:$line"
	set line [handle_replace_many_per_line $line $classname]
	#puts "res:$line"
	#puts $line
	set line [handle_super_position $line]
	set line [handle_replace_one_per_line $line]
	#REPLACE - no matter where is ";"
	set line [handle_replace_one $line]
	set line [handle_replace_at_last $line]
	if { $dev_mode == 1 } {
	  set line [handle_dev_replaces $line]
	}
	
	puts -nonewline $fileto $line
}
#end - for each ";"

#remove separator
seek $fileto -1 current
puts $fileto "" 

# add at the end of *.h file
if {$fileHas_H_Extension} {
	puts -nonewline $fileto "\n\n#endif //$fnamecomb"
}
flush $fileto
close $fileto
