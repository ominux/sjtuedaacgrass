#!/usr/bin/tcl
proc mos_extract {LisFile} {

	global m_inst_para 

	#open once to extract mos parameters
	set fh [open "$LisFile"]

        #get small signal parameters of mos in mos_para_list.txt
        while {[gets $fh line] >= 0} {
		if {[string match -nocase {* mosfets} $line]} {
				#puts "find mosfet line"
				break;
		} 
	}

	while {[gets $fh line] >= 0} {
		if {[string match -nocase {* BSIM3v3*} $line]} {	
				#puts "find BSIM line"
				break;
		}
	}	

	gets $fh line1

	#normally 1 iteration would handle 4 transistors
	set it_num 11

	for {set m 0} {$m < $it_num} {incr m} {

		#puts "start para extraction"
	        while {[gets $fh line1] == 0 || [string is space $line1] } {}
		#puts $line1
		if {[string match -nocase {1****** HSPICE*} $line1]} {
			break
		}

		set row_num [llength $line1]
		#puts [lindex $line1 2]
		#puts $row_num


		#gets instance num
		for {set i 2} {$i < $row_num} {incr i} {
			lappend m_inst_num "[string trimleft "[lindex $line1 $i]" "0:m"]"
		}
		#puts [lindex $m_inst_num 0]

		#read each line
		while {[gets $fh line] >= 0} {										
			set para_name [lindex $line 0]
			if {$para_name=="rd" || $para_name=="rs"} {
				for {set k 1} {$k < [expr $row_num-1]} {incr k} {
					set inst_num [lindex $m_inst_num [expr $k-1]]
					#puts $inst_num
					#new parameter name for each instance is like "gm_1"
					set para_name_inst [join "$para_name $inst_num" _]
					#puts $para_name_inst
					#puts [lindex $line [expr $k+1]]
					lappend m_inst_para($inst_num) $para_name_inst [lindex $line [expr $k+1]]
					#puts [lindex $m_inst_para_t($k) 1]
				}
			} elseif {$para_name=="capbd" || $para_name=="capbs"} {
				for {set k 1} {$k < [expr $row_num-1]} {incr k} {
					set inst_num [lindex $m_inst_num [expr $k-1]]
					#new parameter name for each instance is like "gm_1"
					set para_name_inst [join "$para_name $inst_num" _]
					lappend m_inst_para($inst_num) $para_name_inst [lindex $line $k]
				}
			} elseif {$para_name=="mulub"} {												 
				unset m_inst_num	
				break;
			}		
		}
	        #while {[gets $fh line1] >= 0} {}
		#gets $fh line1
	}

	while {[gets $fh line] >= 0} {
		if {[string match -nocase {* mosfets} $line]} {
				#puts "find mosfet line"
				break;
		} 
	}

	#puts "test jump here"
	#normally 1 iteration would handle 6 transistors
	set it_num 7
	set termin 0

	for {set m 0} {$m < $it_num} {incr m} {
	
		set j 0 
		while {[gets $fh line] >= 0} {
			incr j
			if {[string match -nocase {*subckt*} $line]} {	
				#puts "find subckt line"
				break
			} elseif {$j > 3} {	
				#puts "no more subckt"
				set termin 1
				break
			}
		}

		if {$termin==1} {
			break
		}

		#puts "start para extraction"
		gets $fh line1
		#puts $line1
		#puts [lindex $line1 1]
		set row_num [llength $line1]
		#puts $row_num

		#gets instance num
		for {set i 1} {$i < $row_num} {incr i} {
			lappend m_inst_num "[string trimleft "[lindex $line1 $i]" "0:m"]"
		}
		#puts [lindex $m_inst_num 5]

		gets $fh line2
		#puts $line2

		#read each line
		while {[gets $fh line] >= 0} {										
			set para_name [lindex $line 0]
			if {$para_name=="gm" || $para_name=="gds" || $para_name=="gmb" || $para_name=="cgs"} {
				for {set k 1} {$k < $row_num} {incr k} {
					set inst_num [lindex $m_inst_num [expr $k-1]]
					#new parameter name for each instance is like "gm_1"
					set para_name_inst [join "$para_name $inst_num" _]
					lappend m_inst_para($inst_num) $para_name_inst [lindex $line $k]
				}
			} elseif {$para_name=="cgd"} {
			
				for {set k 1} {$k < $row_num} {incr k} {
					set inst_num [lindex $m_inst_num [expr $k-1]]
					#new parameter name for each instance is like "gm_1"
					set para_name_inst [join "$para_name $inst_num" _]
					lappend m_inst_para($inst_num) $para_name_inst [lindex $line $k]
				}
				unset m_inst_num
				break;
			}
		}		
	}	


	#for {set k 1} {$k < 16} {incr k} {
		#puts [llength $m_inst_para($k)]
	#	for {set i 0} {$i < [llength $m_inst_para($k)]} {incr i} {
	#		puts [lindex $m_inst_para($k) $i]
	#	}
	#}

	#set channel [open "mos_para_list.txt" a]
	#foreach {para val} [array get m_inst_para] {
	#	puts $channel "$para $val"
		#puts "$para $val"
	#}
	#close $channel
	close $fh
	#mos parameter extraction over
	
	
}
	
	
	
	
	
	
proc build_cir {SPFile} {

	global m_inst_para 
	
	#open once to read SPFile informations
	#set fh1 [open "$LisFile"]
	set fh [open "$SPFile"]
	set CirFile [string map -nocase {.sp _ext.sp .cir _ext.sp} "$SPFile"]
	set channel [open $CirFile w]
	set inode_num 10000
	set l_eqnode 0

	gets $fh line
	puts $channel $line

	#read input and output format defined by user
	while {[gets $fh line] >= 0} {
		if {[string match -nocase {*.lib*} $line]} {
			#puts "find lib line"
			#skip library line
		} elseif {[string match {*IODEF:*} $line]} {
			#read IO definition
			#puts "find IO definition"
			set input [lindex $line 1]
			#puts $input
			set input_node [lindex $line 2]
			set input_ac_val [lindex $line 3]
			#puts $input_node
			set output [lindex $line 4]
			#puts $output
			set output_node [lindex $line 5]
			#puts $output_node

			#puts artificial input for grass handling
			puts $channel "$input $input_node 0 ac $input_ac_val"
	
		} elseif {[string match -nocase {i*} $line]} {
				#cut independet current source	
		
		} elseif {[string match -nocase {l*} $line]} {
				#cut independet current source	
		
		} elseif {[string match -nocase {v*} $line]} {
				#cut independet voltage source
				if {[string match -nocase {vin*} $line]!=1} {
					if {[lindex $line 2]=="0"} {
						lappend equi_node_list [lindex $line 1] [lindex $line 2]
						set l_eqnode 1
						#puts "[lindex $line 1] [lindex $line 2]"
					}	else {
						lappend equi_node_list [lindex $line 2] [lindex $line 1]
						set l_eqnode 1
						#puts "[lindex $line 1] [lindex $line 2]"
					}
				}	
		} elseif {[string match -nocase {m*} $line]} {

			#puts $line
			#puts "find mos"
			set inst_num [string trimleft "[string tolower [lindex $line 0]]" "m"]
			#puts $inst_num
			#set node for this instance, M d g s b
			set node_D [lindex $line 1]
			set node_G [lindex $line 2]
			set node_S [lindex $line 3]
			set node_B [lindex $line 4]
			#puts $l_eqnode
			if {$l_eqnode==1} {
				for {set i 0} {$i < [llength $equi_node_list]} {set i [expr $i+2]} {
					if {$node_D==[lindex $equi_node_list $i]} {
						set node_D [lindex $equi_node_list [expr $i+1]]
					} 
	
					if {$node_G==[lindex $equi_node_list $i]} {
						set node_G [lindex $equi_node_list [expr $i+1]]
					} 
				
					if {$node_S==[lindex $equi_node_list $i]} {
						set node_S [lindex $equi_node_list [expr $i+1]]
					} 

					if {$node_B==[lindex $equi_node_list $i]} {
						set node_B [lindex $equi_node_list [expr $i+1]]
					} 
				}
			}

			if {$node_D=="gnd"||$node_D=="vdd"||$node_D=="vdd!"} {
				#puts "find gnd or vdd"
				set node_D 0
			} elseif {$node_D=="vin"} {
					set node_D $input_node
			}	elseif {$node_D=="vout"} {
				set node_D $output_node
			}

			if {$node_G=="gnd"||$node_G=="vdd"||$node_G=="vdd!"} {
				#puts "find gnd or vdd"
				set node_G 0
			} elseif {$node_G=="vin"} {
					set node_G $input_node
			}	elseif {$node_G=="vout"} {
				set node_G $output_node
			} 

			if {$node_S=="gnd"||$node_S=="vdd"||$node_S=="vdd!"} {
				#puts "find gnd or vdd"
				set node_S 0
			} elseif {$node_S=="vin"} {
					set node_S $input_node
			}	elseif {$node_S=="vout"} {
				set node_S $output_node
			} 

			if {$node_B=="gnd"||$node_B=="vdd"||$node_B=="vdd!"} {
				#puts "find gnd or vdd"
				set node_B 0
			} elseif {$node_B=="vin"} {
					set node_B $input_node
			}	elseif {$node_B=="vout"} {
				set node_B $output_node
			} 
			
			set inode_1 $inode_num
			incr inode_num
			set inode_2 $inode_num
			incr inode_num

			#set inode_1 [join "ind1 $inst_num" _]
			#set inode_2 [join "ind2 $inst_num" _]

			#puts $node_D
			#puts $node_B
			#puts $inode_1

			#puts [llength $m_inst_para($inst_num)]
			#comment this line
			puts $channel [join "* $line" ]
			
			for {set i 0} {$i < [llength $m_inst_para($inst_num)]} {set i [expr $i+2]} { 
				set para_name [lindex $m_inst_para($inst_num) $i]
				set para_val [string trimright "[lindex $m_inst_para($inst_num) [expr $i+1]]" {.}]
				#puts $para_val
				if {[string first {a} $para_val 1]!=-1} {
					set para_val [string trimright "$para_val" {a}]
					set para_val [expr $para_val/1000000000000000000]
				}
		
				if {[string match -nocase {rd_*} [lindex $m_inst_para($inst_num) $i]]} {
						if {$para_val==0} {
							set inode_1 $node_D
							continue
						}
						lappend temp [lindex $m_inst_para($inst_num) $i] $node_D $inode_1 $para_val
				}	elseif {[string match -nocase {rs_*} [lindex $m_inst_para($inst_num) $i]]} {
						if {$para_val==0} {
							set inode_2 $node_S
							continue
						}
						lappend temp [lindex $m_inst_para($inst_num) $i] $node_S $inode_2 $para_val
				} elseif {[string match -nocase {gm_*} $para_name]} {
					#avoid control source selfloop
					if {$node_G==$node_S} {
						continue
					}
					#avoid 0 value self loop
					if {$para_val==0} {
						continue
					}

					lappend temp [lindex $m_inst_para($inst_num) $i] $inode_1 $inode_2 $node_G $node_S $para_val
				} elseif {[string match -nocase {gmb_*} [lindex $m_inst_para($inst_num) $i]]} {
						if {$node_B==$node_S} {
							continue
						}
						if {$para_val==0} {
							continue
						}

						lappend temp [lindex $m_inst_para($inst_num) $i] $inode_1 $inode_2 $node_B $node_S $para_val
				}	elseif {[string match -nocase {gds_*} [lindex $m_inst_para($inst_num) $i]]} {
						set gds_val [lindex $m_inst_para($inst_num) [expr $i+1]]
						#puts $gds_val
						if {[string first {m} $gds_val 1]!=-1} {
							set gds_val [string trimright "$gds_val" {m}]
							set gds_val [expr $gds_val/1000]
						} elseif {[string first {u} $gds_val 1]!=-1} {
							set gds_val [string trimright "$gds_val" {u}]
							set gds_val [expr $gds_val/1000000]
						} elseif {[string first {n} $gds_val 1]!=-1} {
							set gds_val [string trimright "$gds_val" {n}]
							set gds_val [expr $gds_val/1000000000]
						} elseif {[string first {p} $gds_val 1]!=-1} {
							set gds_val [string trimright "$gds_val" {p}]
							set gds_val [expr $gds_val/1000000000000]
						} elseif {[string first {f} $gds_val 1]!=-1} {
							set gds_val [string trimright "$gds_val" {f}]
							set gds_val [expr $gds_val/1000000000000000]
						} elseif {[string first {a} $gds_val 1]!=-1} {
							set gds_val [string trimright "$gds_val" {a}]
							set gds_val [expr $gds_val/1000000000000000000]
						} 
						lappend temp [join "rds $inst_num" _] $inode_1 $inode_2 [expr 1/$gds_val]
				} elseif {[string match -nocase {capbd_*} [lindex $m_inst_para($inst_num) $i]]} {
						if {$para_val==0} {
							continue
						}
						lappend temp [lindex $m_inst_para($inst_num) $i] $inode_1 $node_B $para_val
				}	elseif {[string match -nocase {capbs_*} [lindex $m_inst_para($inst_num) $i]]} {
						if {$para_val==0} {
							continue
						}
						lappend temp [lindex $m_inst_para($inst_num) $i] $inode_2 $node_B $para_val
				}	elseif {[string match -nocase {cgd_*} [lindex $m_inst_para($inst_num) $i]]} {
						if {$para_val==0} {
						continue
						}
						lappend temp [lindex $m_inst_para($inst_num) $i] $node_G $inode_1 $para_val
				}	elseif {[string match -nocase {cgs_*} [lindex $m_inst_para($inst_num) $i]]} {
						if {$para_val==0} {
							continue
						}
						lappend temp [lindex $m_inst_para($inst_num) $i] $node_G $inode_2 $para_val
				}		

				puts $channel $temp
				unset temp
					
			#break
			}		
		} elseif {[string match -nocase {*.print*} $line]} {	
			set temp [string map -nocase "$output $output_node" $line]
			puts $channel $temp
			unset temp
		} else {
				set temp $line
				for {set i 0} {$i < [llength $line]} {incr i} {
					set indiv [lindex $line $i]
					if {$indiv=="vdd"||$indiv=="gnd"||$indiv=="vdd!"} {
						#puts $i
						#puts "find vdd or gnd node"
						set temp [lreplace $temp $i $i 0]
					} elseif {$indiv=="vout"} {
						#puts $i
						#puts "find out node"
						set temp [lreplace $temp $i $i $output_node]
					}
				}	 
			puts $channel $temp
			unset temp
		}
		puts $channel " "

#	break
	}
	close $fh
	close $channel
}

proc replace_node {fileName} {
        
	set fh [open "$fileName"]
	gets $fh line
	set nodeList {}
	while {[gets $fh line] >= 0} {
		if {[string is space $line] || [string match -nocase {.*} $line] || [string match -nocase {\**} $line]} {
			continue
		}
		set numNodes 0
		if {[string match -nocase {[rlcvi]*} $line]} {
			set numNodes 2
		}
		if {[string match -nocase {[eghf]*} $line]} {
			set numNodes 4
		}
		for {set i 1} {$i < [expr $numNodes+1]} {incr i} {
			set node [lindex $line $i]
			if {[string match {0} $node]} {
				continue
			}
			if {[lsearch -exact $nodeList $node] < 0} {
				lappend nodeList $node
			}
		}
	}
	close $fh

	set nodeIndex 20000
	foreach n $nodeList {
		incr nodeIndex
		if {[catch {exec sed -i "1!{/^\.\[^(\]*(\[^)\]\\+)\\|^\[rlcvighfeRLCVIGHFE\]/s/\\<$n\\>/$nodeIndex/g}" $fileName}]} {
			break;
		} 
	}
}


foreach arg $argv {
	mos_extract $arg
	set name [file root $arg]
	#puts $name
	set spName "$name.sp"
	#puts $spName
	build_cir $spName
	set spNewName [string map -nocase {.sp _ext.sp .cir _ext.sp} "$spName"]
        replace_node $spNewName
}
