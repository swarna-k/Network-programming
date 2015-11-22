
if { $argc != 2 } {
    puts "The Scenario number and case number must be entered"
    puts "For example, 1 2"
    puts "Please try again."
	exit 0
} else {
 set case [lindex $argv 0]
 set scenario [lindex $argv 1]
}

set filename "ratios_${case}_${scenario}.txt"
puts "Opening $filename"
set fileId [open $filename w]

if {$scenario == 1} {
	puts $fileId "Time\tThroughput1\tThroughput2"
} else {
	puts $fileId "Time\tThroughput1\tThroughput2\tThroughput3"
}

set sum1 0
set sum2 0
set sum3 0

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
	global tcpsink0 tcpsink1 udpsink2 fileId sum1 sum2 sum3
	set ns [Simulator instance]
	set time .5
	set now [$ns now]
	set bw0 [$tcpsink0 set bytes_]
	set bw1 [$tcpsink1 set bytes_]
	if {$scenario == 2} {
		set bw2 [$udpsink2 set bytes_]
	}
	set sum1 [expr $bw0/$time*8/1000]
	set sum2 [expr $bw1/$time*8/1000]

	puts $fileId "$now\t[expr $sum1]\t[expr $sum2]"

	if {$scenario == 2} {
		set sum3 [expr $bw2/$time*8/1000]
		puts $fileId "$now\t[expr $sum1]\t[expr $sum2]\t[expr $sum3]"
	}
	
	
	zerobytes
	$ns at [expr $now+$time] "record"
}

proc zerobytes {} {
	global tcpsink0 tcpsink1 udpsink2
	$tcpsink0 set bytes_ 0 
	$tcpsink1 set bytes_ 0
	$udpsink2 set bytes_ 0
}

proc case1a {} {
	puts "Selected Case 1a"
	
	global ns src1 src2 rcv1 rcv2 R1 R2
	$ns duplex-link $src1 $R1 10Mb 1ms DropTail
	$ns duplex-link $src2 $R1 10Mb 1ms DropTail 
	$ns duplex-link $rcv1 $R2 10Mb 1ms DropTail
	$ns duplex-link $rcv2 $R2 10Mb 1ms DropTail

	#Create a duplex link between the Routers
	$ns duplex-link $R1 $R2 1Mb 10ms DropTail
}
proc case1b {} {
	puts "Selected Case 1b"
	
	global ns src1 src2 rcv1 rcv2 R1 R2
	$ns duplex-link $src1 $R1 10Mb 1ms Red
	$ns duplex-link $src2 $R1 10Mb 1ms Red
	$ns duplex-link $rcv1 $R2 10Mb 1ms Red
	$ns duplex-link $rcv2 $R2 10Mb 1ms Red

	#Create a duplex link between the Routers
	$ns duplex-link $R1 $R2 1Mb 10ms Red
}
proc case2a {} {
	puts "Selected Case 2a"
	
	global ns src1 src2 src3 rcv1 rcv2 rcv3 R1 R2
	$ns duplex-link $src1 $R1 10Mb 1ms DropTail
	$ns duplex-link $src2 $R1 10Mb 1ms DropTail
	$ns duplex-link $src3 $R1 10Mb 1ms DropTail
	$ns duplex-link $rcv1 $R2 10Mb 1ms DropTail
	$ns duplex-link $rcv2 $R2 10Mb 1ms DropTail
	$ns duplex-link $rcv3 $R2 10Mb 1ms DropTail

	#Create a duplex link between the Routers
	$ns duplex-link $R1 $R2 1Mb 10ms DropTail
}
proc case2b {} {
	puts "Selected Case 2b"
	
	global ns src1 src2 src3 rcv1 rcv2 rcv3 R1 R2
	$ns duplex-link $src1 $R1 10Mb 1ms Red
	$ns duplex-link $src2 $R1 10Mb 1ms Red 
	$ns duplex-link $src3 $R1 10Mb 1ms Red 
	$ns duplex-link $rcv1 $R2 10Mb 1ms Red
	$ns duplex-link $rcv2 $R2 10Mb 1ms Red
	$ns duplex-link $rcv3 $R2 10Mb 1ms Red

	#Create a duplex link between the Routers
	$ns duplex-link $R1 $R2 1Mb 10ms Red
}

#Create two nodes
set R1 [$ns node]
set R2 [$ns node]
set src1 [$ns node]
set src2 [$ns node]
set src3 [$ns node]
set rcv1 [$ns node]
set rcv2 [$ns node]
set rcv3 [$ns node]

$R1 label "R1"
$R2 label "R2"


if {$scenario == 1} {
	
	$src1 label "H1"
	$src2 label "H2"
	$rcv1 label "H3"
	$rcv2 label "H4"
	$src1 shape box
	$src2 shape box
	$rcv1 shape box
	$rcv2 shape box
	
	

	switch -exact $case {
		"DROPTAIL"      {case1a}
		"RED"      {case1b}
	}

	$ns duplex-link-op $src2 $R1 orient right-up
	$ns duplex-link-op $rcv2 $R2 orient left-up

} else {
	
	$src1 label "H1"
	$src2 label "H2"
	$src3 label "H3"
	$rcv1 label "H6"
	$rcv2 label "H4"
	$rcv3 label "H5"
	$src1 shape box
	$src2 shape box
	$src3 shape box
	$rcv1 shape box
	$rcv2 shape box
	$rcv3 shape box

	switch -exact $case {
		"DROPTAIL"      {case2a}
		"RED"      {case2b}
	}

	$ns duplex-link-op $src2 $R1 orient right
	$ns duplex-link-op $src3 $R1 orient right-up
	$ns duplex-link-op $rcv2 $R2 orient left
	$ns duplex-link-op $rcv3 $R2 orient left-up
}





$ns duplex-link-op $src1 $R1 orient right-down

$ns duplex-link-op $rcv1 $R2 orient left-down

$ns duplex-link-op $R1 $R2 orient right
$ns queue-limit $R1 $R2 20

set tcp0 [new Agent/TCP/Reno] 
set tcp1 [new Agent/TCP/Reno]
set udp2 [new Agent/UDP]
set tcpsink0 [new Agent/TCPSink]
set tcpsink1 [new Agent/TCPSink]
set udpsink2 [new Agent/Null]

$tcp0 set class_ 1
$tcp1 set class_ 2
$ns color 1 Blue
$ns color 2 Red

$ns attach-agent $src1 $tcp0
$ns attach-agent $src2 $tcp1
$ns attach-agent $src3 $udp2
$ns attach-agent $rcv1 $tcpsink0
$ns attach-agent $rcv2 $tcpsink1
$ns attach-agent $rcv3 $udpsink2

	
set ftp0 [new Application/FTP]
set ftp1 [new Application/FTP]
set cbr0 [new Application/Traffic/CBR]



$ftp0 attach-agent $tcp0
$ftp1 attach-agent $tcp1
$cbr0 attach-agent $udp2
$cbr0 set packet_size_ 100
$cbr0 set rate_ 1mb





$ns connect $tcp0 $tcpsink0
$ns connect $tcp1 $tcpsink1

if {$scenario == 2} {
$ns connect $udp2 $udpsink2
}
#$R1 node-config -wiredRouting ON
#$R2 node-config -wiredRouting ON
$ns at 29.5 "zerobytes"
$ns at 30.0 "record"
$ns at 0.0 "$ftp0 start"
$ns at 180.0 "$ftp0 stop"
$ns at 0.0 "$ftp1 start"
$ns at 180.0 "$ftp1 stop"
if {$scenario == 2} {
	$ns at 0.0 "$cbr0 start"
	$ns at 180.0 "$cbr0 stop"
}
#Call the finish procedure after 5 seconds of simulation time
$ns at 180.0 "finish"

#Run the simulation
$ns run
