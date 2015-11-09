
if { $argc != 2 } {
    puts "The TCP type and case number must be entered"
    puts "For example, Vegas 2"
    puts "Please try again."
	exit 0
} else {
 set tcptype [lindex $argv 0]
 set casenum [lindex $argv 1]
}

set filename "ratios_${tcptype}_${casenum}.txt"
puts "Opening $filename"
set fileId [open $filename w]
puts $fileId "Time\tThroughput1\tThroughput2\tRatio"
set sum1 0
set sum2 0

#Create a simulator object
set ns [new Simulator]

#Open the nam trace file
set nf [open out.nam w]
$ns namtrace-all $nf

#Define a 'finish' procedure
proc finish {} {
        global ns nf fileId
        $ns flush-trace
	#Close the trace file
        close $nf
	#Close output file 
		close $fileId
	#Execute nam on the trace file
        exec nam out.nam &
        exit 0
}

proc record {} {
	global tcpsink0 tcpsink1 fileId sum1 sum2
	set ns [Simulator instance]
	set time .5
	set now [$ns now]
	set bw0 [$tcpsink0 set bytes_]
	set bw1 [$tcpsink1 set bytes_]
	set sum1 [expr $sum1+$bw0/$time*8/1000000]
	set sum2 [expr $sum2+$bw1/$time*8/1000000]
	if { $sum2 != 0 } {
		set ratio [expr double($sum1)/$sum2]
		} else {
			set ratio 0
		}
	
	puts $fileId "$now\t[expr $sum1]\t[expr $sum2]\t[expr $ratio]"
	$ns at [expr $now+$time] "record"
}

proc case1 {} {
	puts "Selected Case 1"
	global ns src1 src2 rcv1 rcv2 R1 R2
	$ns duplex-link $src1 $R1 10Mb 5ms SFQ
	$ns duplex-link $src2 $R1 10Mb 12.5ms SFQ 
	$ns duplex-link $rcv1 $R2 10Mb 5ms SFQ
	$ns duplex-link $rcv2 $R2 10Mb 12.5ms SFQ
}
proc case2 {} {
	puts "Selected Case 2"
	global ns src1 src2 rcv1 rcv2 R1 R2
	$ns duplex-link $src1 $R1 10Mb 5ms SFQ
	$ns duplex-link $src2 $R1 10Mb 20ms SFQ 
	$ns duplex-link $rcv1 $R2 10Mb 5ms SFQ
	$ns duplex-link $rcv2 $R2 10Mb 20ms SFQ
}
proc case3 {} {
	puts "Selected Case 3"
	global ns src1 src2 rcv1 rcv2 R1 R2
	$ns duplex-link $src1 $R1 10Mb 5ms SFQ
	$ns duplex-link $src2 $R1 10Mb 27.5ms SFQ 
	$ns duplex-link $rcv1 $R2 10Mb 5ms SFQ
	$ns duplex-link $rcv2 $R2 10Mb 27.5ms SFQ
}

#Create two nodes
set R1 [$ns node]
set R2 [$ns node]
set src1 [$ns node]
set src2 [$ns node]
set rcv1 [$ns node]
set rcv2 [$ns node]

$R1 label "R1"
$R2 label "R2"
$src1 label "src1"
$src2 label "src2"
$rcv1 label "rcv1"
$rcv2 label "rcv2"
$R2 shape box
$R1 shape box

switch -exact $casenum {
    1      {case1}
    2      {case2}
    3 	   {case3}
}

#Create a duplex link between the Routers
$ns duplex-link $R1 $R2 1Mb 5ms SFQ

$ns duplex-link-op $src1 $R1 orient right-down
$ns duplex-link-op $src2 $R1 orient right-up
$ns duplex-link-op $rcv1 $R2 orient left-down
$ns duplex-link-op $rcv2 $R2 orient left-up
$ns duplex-link-op $R1 $R2 orient right

set tcp0 [new Agent/TCP/$tcptype] 
set tcp1 [new Agent/TCP/$tcptype]
set tcpsink0 [new Agent/TCPSink]
set tcpsink1 [new Agent/TCPSink]

$tcp0 set class_ 1
$tcp1 set class_ 2
$ns color 1 Blue
$ns color 2 Red

$ns attach-agent $src1 $tcp0
$ns attach-agent $src2 $tcp1
$ns attach-agent $rcv1 $tcpsink0
$ns attach-agent $rcv2 $tcpsink1


	
set ftp0 [new Application/FTP]
set ftp1 [new Application/FTP]



$ftp0 attach-agent $tcp0
$ftp1 attach-agent $tcp1
$ns attach-agent $rcv1 $tcpsink0
$ns attach-agent $rcv2 $tcpsink1



$ns connect $tcp0 $tcpsink0
$ns connect $tcp1 $tcpsink1

#$R1 node-config -wiredRouting ON
#$R2 node-config -wiredRouting ON

$ns at 100.0 "record"
$ns at 0.0 "$ftp0 start"
$ns at 400.0 "$ftp0 stop"
$ns at 0.0 "$ftp1 start"
$ns at 400.0 "$ftp1 stop"
#Call the finish procedure after 5 seconds of simulation time
$ns at 400.0 "finish"

#Run the simulation
$ns run
