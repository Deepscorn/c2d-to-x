# Convert all the *.h / *.m / *.mm files in directory and save to another one
# example call (for windows): tclsh dir-to-x.tcl {C:\TheGame\Classes} {C:\MyProj\Classes}
# Please, read c2d-to-x script for more info.

set src_dir [lindex $argv 0]
set dst_dir [lindex $argv 1]

set src_files [glob -type f -directory $src_dir *.{\[hm\],mm}]

set cur_dir [file dirname [info script]]
set conv_script [file join $cur_dir c2d-to-x.tcl]

foreach src_file $src_files {
  puts -nonewline "Parsing [file tail $src_file]"
  set argv [lreplace $argv 0 0 $src_file]
  set argv [lreplace $argv 1 1 $dst_dir]
  puts "  ok"
  source $conv_script
}
