#!/usr/bin/tcl

proc ProcessConfigureFile {ConfigureFile} {
 
  global GrassPath
  global GrassConfiugreFile
  #global GrassConfPath
  global GrassCirPath
  #global GrassLibPath
  #global GrassNModels
  #global GrassModels
  #global GrassModelIndex
  #global GrassNViews
  #global GrassViews
  global GrassViewIndex

  if {[file exists $ConfigureFile]} {
    set fh [open $ConfigureFile r]
    while {[gets $fh line] >= 0} {
    
      if {[string is space $line]} {
	continue
      }
      if {[string match -nocase {* = *} $line]} {
        if {[scan $line {%s = %s} key value] == 2} {
	  switch $key {
            ConfPath	{set GrassConfPath	$value}
            CirPath	{set GrassCirPath	$value}
            LibPath	{set GrassLibPath	$value}
            NModels	{set GrassNModels	$value}
            Models	{set GrassModels	{[split $value ,]}}
            ModelIndex	{set GrassModelIndex	$value}
            NViews	{set GrassNViews	$value}
            Views	{set GrassViews		{[split $value ,]}}
            ViewIndex	{set GrassViewIndex	$value}
	    FreqFrom	{set GrassFreqFrom	$value}
	    FreqTo	{set GrassFreqTo	$value}
	    FreqN	{set GrassFreqN		$value}
	    P1Name	{set GrassP1Name	$value}
	    P1Alias	{set GrassP1Alias	$value}
	    P1ValFrom	{set GrassP1ValFrom	$value}
	    P1ValTo	{set GrassP1ValTo	$value}
	    P1ValN	{set GrassP1ValN	$value}
	    P1ValSigma	{set GrassP1ValSigma	$value}
	    P2Name	{set GrassP2Name	$value}
	    P2Alias	{set GrassP2Alias	$value}
	    P2ValFrom	{set GrassP2ValFrom	$value}
	    P2ValTo	{set GrassP2ValTo	$value}
	    P2ValN	{set GrassP2ValN	$value}
	    P2ValSigma	{set GrassP2ValSigma	$value}
	    Correlation	{set GrassCorrelation	$value}
	    default	{}
          }
        }
      }
    }
    close $fh

    if {[file isdirectory $GrassPath]} {
      set fh [open $GrassConfiugreFile w]
      puts $fh "$GrassP1Name $GrassP1Alias"
      puts $fh "$GrassP1ValFrom $GrassP1ValTo"
      puts $fh "$GrassP1ValN"
      puts $fh ""
      puts $fh "$GrassP2Name $GrassP2Alias"
      puts $fh "$GrassP2ValFrom $GrassP2ValTo"
      puts $fh "$GrassP2ValN"
      puts $fh ""
      puts $fh "-10\n-3\n"
      puts $fh "$GrassFreqFrom $GrassFreqTo"
      puts $fh "$GrassFreqN"
      puts $fh "$GrassP1ValSigma $GrassP2ValSigma $GrassCorrelation"
      close $fh
    } else {
      #Grass Directory Not Exists
      exit 2
    }
    return 0
  } else {
    #Configure File Not Exists
    exit 1
  }
}

proc RunSpiceSimulation {} {
  global GrassCirPath
  global RptPath
  global Script
  set ValidCommandTcl "IsCommandValid.tcl"
  set ValidCommandTclPath $Script$ValidCommandTcl
  if {[file exists $ValidCommandTclPath] && ![catch {exec $ValidCommandTclPath hspice}]} {
    set SpiceName [file tail [file root $GrassCirPath]]
    set RptSpiceName $RptPath$SpiceName
    set ErrRpt "spiceerr.rpt"
    set SpiceErrRpt $RptPath$ErrRpt
    exec dos2unix $GrassCirPath >&/dev/null
    if {[catch {exec hspice $GrassCirPath -o $RptSpiceName 2>$SpiceErrRpt}]} {
      #hspice run errors
      set ErrContentRpt "spiceerrlis.rpt"
      set SpiceErrContentRpt $RptPath$ErrContentRpt
      set ListExtension ".lis"
      set SpiceListRpt $RptPath$SpiceName$ListExtension
      exec cp $SpiceListRpt $SpiceErrContentRpt
      exit 4
    }

    if {[file exists $SpiceErrRpt]} {
      set fh [open $SpiceErrRpt r]
      while {[gets $fh line] >= 0} {
        if {[string match {*hspice job conclude*} $line]} {
	  close $fh
          return 0
	} else {
	  # hspice error
	  close $fh
	  set ErrContentRpt "spiceerrlis.rpt"
	  set SpiceErrContentRpt $RptPath$ErrContentRpt
	  set ListExtension ".lis"
	  set SpiceListRpt $RptPath$SpiceName$ListExtension
	  exec cp $SpiceListRpt $SpiceErrContentRpt
          exit 4
	}
	close $fh
      }	
    } else {
      return 0
    }
  } else {
    #hspice Not Exists
    exit 3
  }
}

set Script "../script/"
set RptPath "../../rpt/"
set GrassPath "../grass/"
set GRASS3D "../grass/src/grass3D"
set GRASS2D "../grass/src/grass2D"
set GrassConfigureFileName "GrassConfigureFile"
set GrassConfiugreFile "$GrassPath$GrassConfigureFileName"

set ret [ProcessConfigureFile [lindex $argv 0]]
if {$ret == 0} {
  set ret [RunSpiceSimulation]
  set TKYTcl "mos_extract4.tcl"
  set TKYTclPath $Script$TKYTcl
  set GrassCir [file tail [file root $GrassCirPath]]
  if {[file exists $TKYTclPath]} {
    #set ErrRpt "tkyerr.rpt"
    #set TKYErrRpt $RptPath$ErrRpt
    set ListRpt "$GrassCir.lis"
    set TKYListRpt $RptPath$ListRpt
    exec cp $GrassCirPath $RptPath
    if {[catch {exec $TKYTclPath $TKYListRpt}]} {
      #TKY's Script Run Error
      exit 7
    }

    if {$ret == 0} {
      set GrassCirExtension "_ext.sp"
      set GrassCirPathNew $RptPath$GrassCir$GrassCirExtension
      set Grass3DErrRpt "grass3derr.rpt"
      set Grass2DErrRpt "grass2derr.rpt"
      set Grass3DErrRptPath $RptPath$Grass3DErrRpt
      set Grass2DErrRptPath $RptPath$Grass2DErrRpt
      set Grass3DRunRpt "grass3drun.rpt"
      set Grass2DRunRpt "grass2drun.rpt"
      set Grass3DRunRptPath $RptPath$Grass3DRunRpt
      set Grass2DRunRptPath $RptPath$Grass2DRunRpt
      if {[string match -nocase {0} $GrassViewIndex]} {       
        if {[file exists $GRASS3D]} {
          if {[catch {exec $GRASS3D $GrassCirPathNew -c $GrassConfiugreFile >&$Grass3DRunRptPath 2>$Grass3DErrRptPath &}]} {
	    #puts "Running To Error"
            #Grass Running 3D Err
	    exit 8
	  }
	  if {[file size $Grass3DErrRptPath]} {
            #Generates 3D Error
	    exit 9
	  }
        }
        #puts "Finish Running 3DGrass"
        return 0
      } elseif {[string match -nocase {1} $GrassViewIndex]} {       
        if {[file exists $GRASS2D]} {
          if {[catch {exec $GRASS2D $GrassCirPathNew >&$Grass2DRunRptPath 2>$Grass2DErrRptPath &}]} {
            #Grass Running 2D Err
	    exit 10
	  }
	  if {[file size $Grass2DErrRptPath]} {
            #Generates 2D Error
	    exit 11
	  }
        }
        #puts "Finish Running 2DGrass"
        return 0
      } elseif {[string match -nocase {2} $GrassViewIndex]} {       
	set ERR3D 0
	set ERR2D 0
        if {[file exists $GRASS3D]} {
          if {[catch {exec $GRASS3D $GrassCirPathNew -c $GrassConfiugreFile >&$Grass3DRunRptPath 2>$Grass3DErrRptPath &}]} {
            #Grass Running 3D Err
	    set ERR3D 1
	  }
	  if {[file size $Grass3DErrRptPath]} {
            #Generates Error
	    set ERR3D 2
	  }
        }
        #puts "Finish Running 3DGrass"
        if {[file exists $GRASS2D]} {
          if {[catch {exec $GRASS2D $GrassCirPathNew >&$Grass2DRunRptPath 2>$Grass2DErrRptPath &}]} {
            #Grass Running 2D Err
	    set ERR2D 1
	  }
	  if {[file size $Grass2DErrRptPath]} {
            #Generates 2D Error
	    set ERR2D 2
	  }
        }
	#puts "\$ERR3D = $ERR3D"
	#puts "\$ERR2D = $ERR2D"
	if {1 == $ERR3D} {
	  #puts "Hit 3D1"
          exit 8
	} elseif {2 == $ERR3D} {
	  #puts "Hit 3D2"
          exit 9
	}
	if {1 == $ERR2D} {
	  #puts "Hit 2D1"
          exit 10
	} elseif {2 == $ERR2D} {
	  #puts "Hit 2D2"
          exit 11
	}
	#puts "No Hits"
        #puts "Finish Running 2DGrass"
        return 0
      } else {
        #No Support View Found
        exit 5
      }
    } else {
      return $ret
    }
    return 0
  } else {
    #TKY's Script Not Exists
    exit 6
  }
} else {
  return $ret
}
